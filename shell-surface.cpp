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
  if (_this->ping_)
    _this->ping_(serial);
}

void ShellSurface::OnConfigure(void *data,
                               struct wl_shell_surface *shell_surface,
                               uint32_t edges,
                               int32_t width,
                               int32_t height) {
  ShellSurface *_this = static_cast<ShellSurface *>(data);
  if (_this->configure_)
    _this->configure_(edges, width, height);
}

void ShellSurface::OnPopupDone(void *data, struct wl_shell_surface *shell_surface) {
  ShellSurface *_this = static_cast<ShellSurface *>(data);
  if (_this->popup_done_)
    _this->popup_done_();
}
