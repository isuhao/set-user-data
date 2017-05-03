/**
 * Registry structure
 */

#ifndef DISPLAY_DISPATCH_REGISTRY_HPP
#define DISPLAY_DISPATCH_REGISTRY_HPP

#include "display.hpp"

struct Registry {

  Registry()
      : native(nullptr) {}

  ~Registry() {
    if (native) wl_registry_destroy(native);
  }

  void Setup(const Display &display) {
    Destroy();
    native = wl_display_get_registry(display.native);
  }

  void Destroy() {
    if (native) {
      wl_registry_destroy(native);
      native = nullptr;
    }
  }

  struct wl_registry *native;

};

#endif //DISPLAY_DISPATCH_REGISTRY_HPP
