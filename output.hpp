//
// output.hpp
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
    wl_output_add_listener(native, &kListener, this);
  }

  void Destroy() {
    if (native) {
      wl_output_destroy(native);
      native = nullptr;
    }
  }

  void SetUserData(void *user_data) {
    wl_output_set_user_data(native, user_data);
  }

  const int placeholder1 = 0;

  struct wl_output *native;

  const int placeholder2 = 0;

  static void OnGeometry(void *data,
                         struct wl_output *wl_output,
                         int32_t x,
                         int32_t y,
                         int32_t physical_width,
                         int32_t physical_height,
                         int32_t subpixel,
                         const char *make,
                         const char *model,
                         int32_t transform);

  static void OnMode(void *data,
                     struct wl_output *wl_output,
                     uint32_t flags,
                     int32_t width,
                     int32_t height,
                     int32_t refresh);

  static void OnDone(void *data,
                     struct wl_output *wl_output);

  static void OnScale(void *data,
                      struct wl_output *wl_output,
                      int32_t factor);

  static const struct wl_output_listener kListener;

};

#endif //DISPLAY_DISPATCH_OUTPUT_HPP
