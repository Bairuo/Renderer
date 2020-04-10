#include "ConfigUtilities.h"
#include "Cuboid.h"
#include "IcoSphere.h"

GLFWwindow *window;
char WindowName[] = "Renderer";
extern const GLuint WindowWidth = 800;
extern const GLuint WindowHeight = 800;

extern const float c_cardRadius = 0.01f;
extern const float c_refreshTime = 0.3f;
extern const float c_mergeBornTime = 0.2f;

#if defined(_WIN32)

#if defined(SHADOWMAP)
const GLchar Cuboid::standardVsPath[] = ".\\shaders\\3D_Standard\\standard_s.vs";
const GLchar Cuboid::standardFragPath[] = ".\\shaders\\3D_Standard\\standard_s.frag";
const GLchar IcoSphere::standardVsPath[] = ".\\shaders\\3D_Standard\\standard_s.vs";
const GLchar IcoSphere::standardFragPath[] = ".\\shaders\\3D_Standard\\standard_s.frag";
#else
const GLchar Cuboid::standardVsPath[] = ".\\shaders\\3D_Standard\\standard.vs";
const GLchar Cuboid::standardFragPath[] = ".\\shaders\\3D_Standard\\standard.frag";
const GLchar IcoSphere::standardVsPath[] = ".\\shaders\\3D_Standard\\standard.vs";
const GLchar IcoSphere::standardFragPath[] = ".\\shaders\\3D_Standard\\standard.frag";
#endif

#else

#if defined(SHADOWMAP)
const GLchar Cuboid::standardVsPath[] = "./shaders/3D_Standard/standard_s.vs";
const GLchar Cuboid::standardFragPath[] = "./shaders/3D_Standard/standard_s.frag";
const GLchar IcoSphere::standardVsPath[] = "./shaders/3D_Standard/standard_s.vs";
const GLchar IcoSphere::standardFragPath[] = "./shaders/3D_Standard/standard_s.frag";
#else
const GLchar Cuboid::standardVsPath[] = "./shaders/3D_Standard/standard.vs";
const GLchar Cuboid::standardFragPath[] = "./shaders/3D_Standard/standard.frag";
const GLchar IcoSphere::standardVsPath[] = "./shaders/3D_Standard/standard.vs";
const GLchar IcoSphere::standardFragPath[] = "./shaders/3D_Standard/standard.frag";
#endif

#endif
