//
// Created by zhanggyb on 17-5-4.
//

#include "registry.hpp"

const struct wl_registry_listener Registry::kListener = {
    OnGlobal,
    OnGlobalRemove
};

void Registry::OnGlobal(void *data,
                        struct wl_registry *registry,
                        uint32_t id,
                        const char *interface,
                        uint32_t version) {
  Registry *_this = static_cast<Registry *>(data);
  if (_this->global)
    _this->global(id, interface, version);
}

void Registry::OnGlobalRemove(void *data,
                              struct wl_registry *registry,
                              uint32_t name) {
  Registry *_this = static_cast<Registry *>(data);
  if (_this->global_remove)
    _this->global_remove(name);
}
