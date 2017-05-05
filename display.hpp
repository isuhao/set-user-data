//
// display.hpp
//

#ifndef DISPLAY_DISPATCH_DISPLAY_HPP
#define DISPLAY_DISPATCH_DISPLAY_HPP

#include <wayland-client.h>

struct Display {

  Display()
      : native(nullptr) {}

  ~Display() {
    if (native) wl_display_disconnect(native);
  }

  void Connect(const char *name = NULL) {
    Disconnect();
    native = wl_display_connect(name);
  }

  void Disconnect() {
    if (native) {
      wl_display_disconnect(native);
      native = nullptr;
    }
  }

  int GetFd() { return wl_display_get_fd(native); }

  int DispatchPending() { return wl_display_dispatch_pending(native); }

  int Roundtrip() { return wl_display_roundtrip(native); }

  int Dispatch() { return wl_display_dispatch(native); }

  int Flush() { return wl_display_flush(native); }

  struct wl_display *native;

};

#endif //DISPLAY_DISPATCH_DISPLAY_HPP
