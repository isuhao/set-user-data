/**
 * Registry structure
 */

#ifndef DISPLAY_DISPATCH_REGISTRY_HPP
#define DISPLAY_DISPATCH_REGISTRY_HPP

#include "display.hpp"
#include "delegate.hpp"

struct Registry {

  Registry()
      : native(nullptr) {}

  ~Registry() {
    if (native) wl_registry_destroy(native);
  }

  void Setup(const Display &display) {
    Destroy();
    native = wl_display_get_registry(display.native);
    wl_registry_add_listener(native, &kListener, this);
  }

  void Destroy() {
    if (native) {
      wl_registry_destroy(native);
      native = nullptr;
    }
  }

  struct wl_registry *native;

  Delegate<void(uint32_t, const char *, uint32_t)> global;
  Delegate<void(uint32_t)> global_remove;

  static void OnGlobal(void *data,
                       struct wl_registry *registry,
                       uint32_t id,
                       const char *interface,
                       uint32_t version);

  static void OnGlobalRemove(void *data,
                             struct wl_registry *registry,
                             uint32_t name);

  static const struct wl_registry_listener kListener;

};

#endif //DISPLAY_DISPATCH_REGISTRY_HPP
