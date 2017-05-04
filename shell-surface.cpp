//
// Created by zhanggyb on 17-5-4.
//

#include "shell-surface.hpp"

const wl_shell_surface_listener ShellSurface::kListener = {
    OnPing,
    OnConfigure,
    OnPopupDone
};

void ShellSurface::OnPing(void *data, struct wl_shell_surface *shell_surface, uint32_t serial) {
  ShellSurface *_this = static_cast<ShellSurface *>(data);
  if (_this->ping)
    _this->ping(serial);
}

void ShellSurface::OnConfigure(void *data,
                               struct wl_shell_surface *shell_surface,
                               uint32_t edges,
                               int32_t width,
                               int32_t height) {
  ShellSurface *_this = static_cast<ShellSurface *>(data);
  if (_this->configure)
    _this->configure(edges, width, height);
}

void ShellSurface::OnPopupDone(void *data, struct wl_shell_surface *shell_surface) {
  ShellSurface *_this = static_cast<ShellSurface *>(data);
  if (_this->popup_done)
    _this->popup_done();
}
