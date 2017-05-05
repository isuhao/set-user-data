//
// buffer.hpp
//

#ifndef DISPLAY_DISPATCH_BUFFER_HPP
#define DISPLAY_DISPATCH_BUFFER_HPP

#include "shm-pool.hpp"

struct Buffer {

  Buffer()
      : native(nullptr) {}

  ~Buffer() {
    if (native) wl_buffer_destroy(native);
  }

  void Setup(const ShmPool &shm_pool, int32_t offset, int32_t width, int32_t height, int32_t stride, uint32_t format) {
    Destroy();
    native = wl_shm_pool_create_buffer(shm_pool.native, offset, width, height, stride, format);
  }

  void Destroy() {
    if (native) {
      wl_buffer_destroy(native);
      native = nullptr;
    }
  }

  struct wl_buffer *native;

};

#endif //DISPLAY_DISPATCH_BUFFER_HPP
