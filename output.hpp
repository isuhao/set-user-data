//
// Created by zhanggyb on 17-5-3.
//

#ifndef DISPLAY_DISPATCH_OUTPUT_HPP
#define DISPLAY_DISPATCH_OUTPUT_HPP

#include "registry.hpp"

struct Output {

  Output()
      : native(nullptr) {}

  ~Output() {
    if (native) wl_output_destroy(native);
  }

  void Setup(const Registry &registry, uint32_t name, uint32_t version) {
    Destroy();
    native = static_cast<wl_output *>(wl_registry_bind(registry.native, name, &wl_output_interface, version));
  }

  void Destroy() {
    if (native) {
      wl_output_destroy(native);
      native = nullptr;
    }
  }

  struct wl_output *native;

};

#endif //DISPLAY_DISPATCH_OUTPUT_HPP
