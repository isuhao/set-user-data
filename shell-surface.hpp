//
// Created by zhanggyb on 17-5-3.
//

#ifndef DISPLAY_DISPATCH_SHELL_SURFACE_HPP
#define DISPLAY_DISPATCH_SHELL_SURFACE_HPP

#include "shell.hpp"
#include "surface.hpp"

struct ShellSurface {

  ShellSurface()
      : native(nullptr) {}

  ~ShellSurface() {
    if (native) wl_shell_surface_destroy(native);
  }

  void Setup(const Shell &shell, const Surface &surface) {
    Destroy();
    native = wl_shell_get_shell_surface(shell.native, surface.native);
  }

  void Destroy() {
    if (native) {
      wl_shell_surface_destroy(native);
      native = nullptr;
    }
  }

  void Pong(uint32_t serial) {
    wl_shell_surface_pong(native, serial);
  }

  struct wl_shell_surface *native;

};

#endif //DISPLAY_DISPATCH_SHELL_SURFACE_HPP
