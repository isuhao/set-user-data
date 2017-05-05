//
// shell.hpp
//

#ifndef DISPLAY_DISPATCH_SHELL_HPP
#define DISPLAY_DISPATCH_SHELL_HPP

#include "registry.hpp"

struct Shell {

  Shell()
      : native(nullptr) {}

  ~Shell() {
    if (native) wl_shell_destroy(native);
  }

  void Setup(const Registry& registry, uint32_t name, uint32_t version) {
    Destroy();
    native = static_cast<wl_shell*>(wl_registry_bind(registry.native, name, &wl_shell_interface, version));
  }

  void Destroy() {
    if (native) {
      wl_shell_destroy(native);
      native = nullptr;
    }
  }

  struct wl_shell* native;

};

#endif //DISPLAY_DISPATCH_SHELL_HPP
