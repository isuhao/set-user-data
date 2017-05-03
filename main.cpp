/**
 *
 */
#include "display.hpp"
#include "registry.hpp"
#include "compositor.hpp"
#include "shm.hpp"
#include "output.hpp"
#include "shell.hpp"
#include "shm-pool.hpp"
#include "buffer.hpp"
#include "shell-surface.hpp"
#include "surface.hpp"

#include <string.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <cerrno>

void *shm_data;

int WIDTH = 480;
int HEIGHT = 360;

static int
set_cloexec_or_close(int fd) {
  long flags;

  if (fd == -1)
    return -1;

  flags = fcntl(fd, F_GETFD);
  if (flags == -1)
    goto err;

  if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
    goto err;

  return fd;

  err:
  close(fd);
  return -1;
}

static int
create_tmpfile_cloexec(char *tmpname) {
  int fd;

#ifdef HAVE_MKOSTEMP
  fd = mkostemp(tmpname, O_CLOEXEC);
        if (fd >= 0)
                unlink(tmpname);
#else
  fd = mkstemp(tmpname);
  if (fd >= 0) {
    fd = set_cloexec_or_close(fd);
    unlink(tmpname);
  }
#endif

  return fd;
}

/*
 * Create a new, unique, anonymous file of the given size, and
 * return the file descriptor for it. The file descriptor is set
 * CLOEXEC. The file is immediately suitable for mmap()'ing
 * the given size at offset zero.
 *
 * The file should not have a permanent backing store like a disk,
 * but may have if XDG_RUNTIME_DIR is not properly implemented in OS.
 *
 * The file name is deleted from the file system.
 *
 * The file is suitable for buffer sharing between processes by
 * transmitting the file descriptor over Unix sockets using the
 * SCM_RIGHTS methods.
 */
int
os_create_anonymous_file(off_t size) {
  static const char anonymous_template[] = "/weston-shared-XXXXXX";
  const char *path;
  char *name;
  int fd;

  path = getenv("XDG_RUNTIME_DIR");
  if (!path) {
    errno = ENOENT;
    return -1;
  }

  name = (char *) malloc(strlen(path) + sizeof(anonymous_template));
  if (!name)
    return -1;
  strcpy(name, path);
  strcat(name, anonymous_template);

  fd = create_tmpfile_cloexec(name);

  free(name);

  if (fd < 0)
    return -1;

  if (ftruncate(fd, size) < 0) {
    close(fd);
    return -1;
  }

  return fd;
}

struct Application {

  Display display;
  Registry registry;
  Shm shm;
  ShmPool pool;
  Compositor compositor;
  Output output;
  Shell shell;
  ShellSurface shell_surface;
  Surface surface;
  Buffer buffer;

  int epoll_fd;
  int display_fd;

  static const wl_registry_listener kRegistryListener;
  static const wl_shm_listener kShmListener;
  static const wl_shell_surface_listener kShellSurfaceListener;
  static const wl_surface_listener kSurfaceListener;

  Application()
      : epoll_fd(0), display_fd(0) {}

  ~Application() {}

  void Init() {
    display.Connect(NULL);
    display_fd = display.GetFd();
    epoll_fd = CreateEpollFd();
    WatchEpollFd(epoll_fd, display_fd, EPOLLIN | EPOLLERR | EPOLLHUP, this);

    registry.Setup(display);
    wl_registry_add_listener(registry.native, &kRegistryListener, this);

    display.DispatchPending();
    display.Roundtrip();

    surface.Setup(compositor);
    wl_surface_add_listener(surface.native, &kSurfaceListener, this);

    shell_surface.Setup(shell, surface);
    wl_shell_surface_add_listener(shell_surface.native, &kShellSurfaceListener, this);

    // TODO: Create Window
    // TODO: Paint pixels
    CreateWindow();
    PaintPixels();
  }

  void Run() {
    static const int kMaxEpollEvents = 16;
    int ret = 0;
    struct epoll_event ep[kMaxEpollEvents];

    while (true) {
      ret = display.DispatchPending();
      if (ret == -1) break;

      ret = display.Flush();
      if (ret == -1) break;

      int count = 0;
      count = epoll_wait(epoll_fd, ep, kMaxEpollEvents, -1);
      for (int i = 0; i < count; i++) {
        HandleEpollTask(ep[i].events);
      }
    }
  }

  void Fini() {
    close(epoll_fd);
    epoll_fd = 0;

    buffer.Destroy();
    surface.Destroy();
    shell_surface.Destroy();
    shell.Destroy();
    output.Destroy();
    compositor.Destroy();
    pool.Destroy();
    shm.Destroy();
    registry.Destroy();
    display.Disconnect();
  }

  int CreateEpollFd() {
    int fd = 0;

#ifdef EPOLL_CLOEXEC
    fd = epoll_create1(EPOLL_CLOEXEC);
    if (fd >= 0)
      return fd;
    if (errno != EINVAL)
      return -1;
#endif

    fd = epoll_create(1);
    return SetCloexecOrClose(fd);
  }

  int SetCloexecOrClose(int fd) {
    if (SetCloexec(fd) != 0) {
      close(fd);
      return -1;
    }
    return fd;
  }

  int SetCloexec(int fd) {
    long flags;

    if (fd == -1)
      return -1;

    flags = fcntl(fd, F_GETFD);
    if (flags == -1)
      return -1;

    if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
      return -1;

    return 0;
  }

  void HandleEpollTask(uint32_t events) {
    if (events & EPOLLERR || events & EPOLLHUP) {
      exit(1);
    }
    if (events & EPOLLIN) {
      if (display.Dispatch() == -1) { // FIXME: this line cause delegate fault
        exit(1);
      }
    }
    if (events & EPOLLOUT) {
      struct epoll_event ep;
      int ret = display.Flush();
      if (ret == 0) {
        ep.events = EPOLLIN | EPOLLERR | EPOLLHUP;
        ep.data.ptr = this;
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, display_fd, &ep);
      } else if (ret == -1 && errno != EAGAIN) {
        exit(1);
      }
    }
  }

  void CreateWindow() {
    CreateBuffer();

    surface.Attach(buffer, 0, 0);
    surface.Commit();
  }

  void CreateBuffer() {
    int stride = WIDTH * 4; // 4 bytes per pixel
    int size = stride * HEIGHT;
    int fd;

    fd = os_create_anonymous_file(size);
    if (fd < 0) {
      fprintf(stderr, "creating a buffer file for %d B failed: %m\n",
              size);
      exit(1);
    }

    shm_data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm_data == MAP_FAILED) {
      fprintf(stderr, "mmap failed: %m\n");
      close(fd);
      exit(1);
    }

    pool.Setup(shm, fd, size);
    buffer.Setup(pool, 0, WIDTH, HEIGHT, stride, WL_SHM_FORMAT_ARGB8888);
  }

  void PaintPixels() {
    int n;
    uint32_t *pixel = (uint32_t *) shm_data;

    fprintf(stdout, "Painting pixels\n");
    for (n = 0; n < WIDTH * HEIGHT; n++) {
      *pixel++ = 0xffff;
    }
  }

  static void WatchEpollFd(int epoll_fd, int fd, uint32_t events, void *data) {
    struct epoll_event ep;
    ep.events = events;
    ep.data.ptr = data;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ep);
  }

  static void UnwatchEpollFd(int epoll_fd, int fd) {
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
  }

  static void OnGlobal(void *data, struct wl_registry *registry,
                       uint32_t name, const char *interface, uint32_t version) {
    fprintf(stdout, "name: %d, interface: %s, version: %d\n", name, interface, version);
    Application *_this = static_cast<Application *>(data);

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
      _this->compositor.Setup(_this->registry, name, version);
    } else if (strcmp(interface, wl_shm_interface.name) == 0) {
      _this->shm.Setup(_this->registry, name, version);
      wl_shm_add_listener(_this->shm.native, &kShmListener, _this);
    } else if (strcmp(interface, wl_shell_interface.name) == 0) {
      _this->shell.Setup(_this->registry, name, version);
    } else if (strcmp(interface, wl_output_interface.name) == 0) {
      _this->output.Setup(_this->registry, name, version);
    }
  }

  static void OnGlobalRemove(void *data, struct wl_registry *registry, uint32_t name) {
    fprintf(stdout, "name: %d\n", name);
  }

  static void OnShmFormat(void *data, struct wl_shm *shm, uint32_t format) {
    fprintf(stdout, "format: %d\n", format);
  }

  static void OnShellSurfaceConfigure(void *data,
                                      struct wl_shell_surface *shell_surface,
                                      uint32_t edges,
                                      int32_t width,
                                      int32_t height) {

  }

  static void OnShellSurfacePing(void *data, struct wl_shell_surface *shell_surface, uint32_t serial) {
    Application *_this = static_cast<Application *>(data);
    _this->shell_surface.Pong(serial);
  }

  static void OnShellSurfacePopupDone(void *data, struct wl_shell_surface *shell_surface) {

  }

  static void OnSurfaceEnter(void *data, struct wl_surface *surface, wl_output *output) {
    fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);
  }

  static void OnSurfaceLeave(void *data, struct wl_surface *surface, wl_output *output) {
    fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);
  }

};

const wl_registry_listener Application::kRegistryListener = {
    OnGlobal,
    OnGlobalRemove
};

const wl_shm_listener Application::kShmListener = {
    OnShmFormat
};

const wl_shell_surface_listener Application::kShellSurfaceListener = {
    OnShellSurfacePing,
    OnShellSurfaceConfigure,
    OnShellSurfacePopupDone
};

const wl_surface_listener Application::kSurfaceListener = {
    OnSurfaceEnter,
    OnSurfaceLeave
};

int main() {
  Application app;

  app.Init();
  app.Run();
  app.Fini();

  return 0;
}