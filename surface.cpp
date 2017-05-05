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
  assert(_this->placeholder1 == 0);
  assert(_this->placeholder2 == 0);
  if (_this->enter_)
    _this->enter_(output);
}

void Surface::OnLeave(void *data, struct wl_surface *surface, struct wl_output *output) {
  fprintf(stdout, "%s\n", __func__);
  Surface *_this = static_cast<Surface *>(data);
  assert(_this->placeholder1 == 0);
  assert(_this->placeholder2 == 0);
  if (_this->leave_)
    _this->leave_(output);
}
