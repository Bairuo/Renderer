## Fun Renderer

[//]: # (Begin current test results)

Based on opengl programmable rendering pipeline

* Font rendering
* Multi light source
* Basic shadow mapping (PCF optimization)
* Frame animation of position, scale, rotate and color in 2D
* Rough contact detection based on BVH in dynamic scene

![Screenshot](https://github.com/Bairuo/renderer/raw/master/meta/move1.jpg)
![Screenshot](https://github.com/Bairuo/renderer/raw/master/meta/move2.jpg)
![Screenshot](https://github.com/Bairuo/renderer/raw/master/meta/contacts5.jpg)
![Screenshot](https://github.com/Bairuo/renderer/raw/master/meta/contacts8.jpg)

### Dependencies
opengl,  glew, glfw, freetype, boost

If you are on windows, there have been prepared most binary files(libs) of these dependencies for you.

note: your graphics driver have to support glsl core 330.

If not, upgrade driver first.

### Build
For windows, you can use cmake to generate a vs solution first(Visual Studio 2017 is suggested, which test pass).

Note you have to assign your boost path.

![Screenshot](https://github.com/Bairuo/renderer/raw/master/meta/cmake.jpg)

Or if you have a codeblock on your device, you can use build/renderer-dev.workspace directly.

For linux,  do just like this:

```
cd build
cmake .
make
```
