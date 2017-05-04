//
// Created by zhanggyb on 17-5-4.
//

#include "surface.hpp"

#include <stdio.h>

const wl_surface_listener Surface::kListener = {
    OnEnter,
    OnLeave
};

void Surface::OnEnter(void *data, struct wl_surface *surface, struct wl_output *output) {
  fprintf(stdout, "%s\n", __func__);
  Surface *_this = static_cast<Surface *>(data);
  if (_this->enter)
    _this->enter(output);
}

void Surface::OnLeave(void *data, struct wl_surface *surface, struct wl_output *output) {
  fprintf(stdout, "%s\n", __func__);
  Surface *_this = static_cast<Surface *>(data);
  if (_this->leave)
    _this->leave(output);
}
