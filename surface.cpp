//
// surface.cpp
//

#include "surface.hpp"

#include <assert.h>
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
}

void Surface::OnLeave(void *data, struct wl_surface *surface, struct wl_output *output) {
  fprintf(stdout, "%s\n", __func__);
  Surface *_this = static_cast<Surface *>(data);
  assert(_this->placeholder1 == 0);
  assert(_this->placeholder2 == 0);
}
