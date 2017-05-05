# Sample code for my 'set user data' issue

Minimal sample code to demonstrate the issue to use wl_output_set_user_data() and wl_surface_set_user_data.

Test environment:
* Fedora 25
* Gnome 3.22.2
* wayland-devel-1.12.0-1.fc25.x86_64
* libwayland-client-1.12.0-1.fc25.x86_64

Checkout, build and run:

```shell
$ git clone https://github.com/zhanggyb/set-user-data
$ mkdir build
$ cd build/
$ cmake ..
$ ./demo
```

When run the executable `demo`, assert error will be raised, and the const member variables around wayland object will 
be changed in the `wl_output` and `wl_surface` listener callbacks.

```shell
output.cpp:27: static void Output::OnGeometry(void*, wl_output*, int32_t, int32_t, int32_t, int32_t, int32_t, const char*, const char*, int32_t): Assertion `_this->placeholder1 == 0' failed.
```

```shell
surface.cpp:18: static void Surface::OnEnter(void*, wl_surface*, wl_output*): Assertion `_this->placeholder1 == 0' failed.
```

Uncomment these 2 lines can avoid assert error:

```c++
main.cpp:163: output.SetUserData(this);
main.cpp:164: surface.SetUserData(this);
```
