## Fun Renderer

[//]: # (Begin current test results)

Based on opengl programmable rendering pipeline

#### Rendering
* Font rendering
* Multi light source
* Basic shadow mapping (PCF optimization)
* MSAA
* Forward Rendering and Deferred Rendering
* Scene Graph

#### Physics
* Rough contact detection based on BVH in dynamic scene

#### Animation
* Frame animation of position, scale, rotate and color in 2D

![Screenshot](https://github.com/Bairuo/renderer/raw/master/meta/move.jpg)
![Screenshot](https://github.com/Bairuo/renderer/raw/master/meta/contacts.jpg)
![Screenshot](https://github.com/Bairuo/renderer/raw/master/meta/gBuffer.jpg)

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
