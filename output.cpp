//
// Created by zhanggyb on 17-5-4.
//

#include "output.hpp"

#include <stdio.h>

const struct wl_output_listener Output::kListener = {
    OnGeometry,
    OnMode,
    OnDone,
    OnScale
};

void Output::OnGeometry(void *data,
                        struct wl_output *wl_output,
                        int32_t x,
                        int32_t y,
                        int32_t physical_width,
                        int32_t physical_height,
                        int32_t subpixel,
                        const char *make,
                        const char *model,
                        int32_t transform) {
  Output *_this = static_cast<Output *>(data);
  assert(_this->placeholder1 == 0);
  assert(_this->placeholder2 == 0);
  if (_this->geometry_)
    _this->geometry_(x, y, physical_width, physical_height, subpixel, make, model, transform);
}

void Output::OnMode(void *data,
                    struct wl_output *wl_output,
                    uint32_t flags,
                    int32_t width,
                    int32_t height,
                    int32_t refresh) {
  Output *_this = static_cast<Output *>(data);
  assert(_this->placeholder1 == 0);
  assert(_this->placeholder2 == 0);
  if (_this->mode_)
    _this->mode_(flags, width, height, refresh);
}

void Output::OnDone(void *data, struct wl_output *wl_output) {
  Output *_this = static_cast<Output *>(data);
  assert(_this->placeholder1 == 0);
  assert(_this->placeholder2 == 0);
  if (_this->done_)
    _this->done_();
}

void Output::OnScale(void *data, struct wl_output *wl_output, int32_t factor) {
  Output *_this = static_cast<Output *>(data);
  assert(_this->placeholder1 == 0);
  assert(_this->placeholder2 == 0);
  if (_this->scale_)
    _this->scale_(factor);
}