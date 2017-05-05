//
// Created by zhanggyb on 17-5-3.
//

#ifndef DISPLAY_DISPATCH_SURFACE_HPP
#define DISPLAY_DISPATCH_SURFACE_HPP

#include "compositor.hpp"
#include "buffer.hpp"
#include "delegate.hpp"

struct Surface {

  Surface()
      : native(nullptr) {}

  ~Surface() {
    if (native) wl_surface_destroy(native);
  }

  void Setup(const Compositor &compositor) {
    Destroy();
    native = wl_compositor_create_surface(compositor.native);
    wl_surface_add_listener(native, &kListener, this);
  }

  void Destroy() {
    if (native) {
      wl_surface_destroy(native);
      native = nullptr;
    }
  }

  void Attach(const Buffer &buffer, int32_t x, int32_t y) {
    wl_surface_attach(native, buffer.native, x, y);
  }

  void Commit() {
    wl_surface_commit(native);
  }

  void SetUserData(void *user_data) {
    wl_surface_set_user_data(native, user_data);
  }

  const int placeholder1 = 0;
  struct wl_surface *native;
  const int placeholder2 = 0;

  DelegateRef<void(struct wl_output *)> enter() {
    return enter_;
  }

  DelegateRef<void(struct wl_output *)> leave() {
    return leave_;
  }

 private:

  Delegate<void(struct wl_output *)> enter_;

  Delegate<void(struct wl_output *)> leave_;

  static void OnEnter(void *data, struct wl_surface *surface, struct wl_output *output);

  static void OnLeave(void *data, struct wl_surface *surface, struct wl_output *output);

  static const wl_surface_listener kListener;

};

#endif //DISPLAY_DISPATCH_SURFACE_HPP
