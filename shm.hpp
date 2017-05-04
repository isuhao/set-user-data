//
// Created by zhanggyb on 17-5-3.
//

#ifndef DISPLAY_DISPATCH_SHM_HPP
#define DISPLAY_DISPATCH_SHM_HPP

#include "registry.hpp"

struct Shm {

  Shm()
      : native(nullptr) {}

  ~Shm() {
    if (native) wl_shm_destroy(native);
  }

  void Setup(const Registry &registry, uint32_t name, uint32_t version) {
    Destroy();
    native = static_cast<wl_shm *>(wl_registry_bind(registry.native, name, &wl_shm_interface, version));
    wl_shm_add_listener(native, &kListener, this);
  }

  void Destroy() {
    if (native) {
      wl_shm_destroy(native);
      native = nullptr;
    }
  }

  struct wl_shm *native;

  Delegate<void(uint32_t)> format;

  static void OnFormat(void *data, struct wl_shm *shm, uint32_t format);

  static const wl_shm_listener kListener;

};

#endif //DISPLAY_DISPATCH_SHM_HPP
