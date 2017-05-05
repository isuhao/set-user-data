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
  wl_shell_surface_pong(shell_surface, serial);
}

void ShellSurface::OnConfigure(void *data,
                               struct wl_shell_surface *shell_surface,
                               uint32_t edges,
                               int32_t width,

                               int32_t height) {
}

void ShellSurface::OnPopupDone(void *data, struct wl_shell_surface *shell_surface) {

}
