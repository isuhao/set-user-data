//
// Created by zhanggyb on 17-5-4.
//

#include "shm.hpp"

#include <stdio.h>

const struct wl_shm_listener Shm::kListener = {
    OnFormat
};

void Shm::OnFormat(void *data, struct wl_shm *shm, uint32_t format) {
  Shm *_this = static_cast<Shm *>(data);
  if (_this->format)
    _this->format(format);
}