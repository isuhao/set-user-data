/**
 * @file Compositor structure
 */

#ifndef DISPLAY_DISPATCH_COMPOSITOR_HPP
#define DISPLAY_DISPATCH_COMPOSITOR_HPP

#include "registry.hpp"

struct Compositor {

  Compositor()
      : native(nullptr) {}

  ~Compositor() {
    if (native) wl_compositor_destroy(native);
  }

  void Setup(const Registry &registry, uint32_t name, uint32_t version) {
    Destroy();
    native = static_cast<wl_compositor *>(wl_registry_bind(registry.native, name, &wl_compositor_interface, version));
  }

  void Destroy() {
    if (native) {
      wl_compositor_destroy(native);
      native = nullptr;
    }
  }

  struct wl_compositor *native;

};

#endif //DISPLAY_DISPATCH_COMPOSITOR_HPP
