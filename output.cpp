//
// Created by zhanggyb on 17-5-4.
//

#include "output.hpp"

#include <stdio.h>
#include <assert.h>

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
  fprintf(stdout, "%s\n", __func__);
  Output *_this = static_cast<Output *>(data);
  assert(_this->placeholder1 == 0);
  assert(_this->placeholder2 == 0);

  if (_this->geometry)
    _this->geometry(x, y, physical_width, physical_height, subpixel, make, model, transform);
}

void Output::OnMode(void *data,
                    struct wl_output *wl_output,
                    uint32_t flags,
                    int32_t width,
                    int32_t height,
                    int32_t refresh) {
  fprintf(stdout, "%s\n", __func__);
  Output *_this = static_cast<Output *>(data);
  assert(_this->placeholder1 == 0);
  assert(_this->placeholder2 == 0);

  if (_this->mode)
    _this->mode(flags, width, height, refresh);
}

void Output::OnDone(void *data, struct wl_output *wl_output) {
  fprintf(stdout, "%s\n", __func__);
  Output *_this = static_cast<Output *>(data);
  assert(_this->placeholder1 == 0);
  assert(_this->placeholder2 == 0);

  if (_this->done)
    _this->done();
}

void Output::OnScale(void *data, struct wl_output *wl_output, int32_t factor) {
  fprintf(stdout, "%s\n", __func__);
  Output *_this = static_cast<Output *>(data);
  assert(_this->placeholder1 == 0);
  assert(_this->placeholder2 == 0);

  if (_this->scale)
    _this->scale(factor);
}