//
// Created by zhanggyb on 17-5-3.
//

#ifndef DISPLAY_DISPATCH_SHM_POOL_HPP
#define DISPLAY_DISPATCH_SHM_POOL_HPP

#include "shm.hpp"

struct ShmPool {

  ShmPool()
      : native(nullptr) {}

  ~ShmPool() {
    if (native) wl_shm_pool_destroy(native);
  }

  void Setup(const Shm &shm, int32_t fd, int32_t size) {
    Destroy();
    native = wl_shm_create_pool(shm.native, fd, size);
  }

  void Destroy() {
    if (native) {
      wl_shm_pool_destroy(native);
      native = nullptr;
    }
  }

  struct wl_shm_pool *native;

};

#endif //DISPLAY_DISPATCH_SHM_POOL_HPP
