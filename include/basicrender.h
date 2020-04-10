// basicrender.h
// Author: Bairuo

#ifndef BASICRENDER_H_
#define BASICRENDER_H_

#define GLEW_STATIC
#include <GL/glew.h>

GLuint getSquareVAO();
GLuint getCubeVAO();
//GLuint getSphereVAO(unsigned int longPart, unsigned int latPart, float radius);
GLuint getIcoSphereVAO(const int level, int &triNum);

struct TriangleIndices
{
	int v1, v2, v3;

	TriangleIndices(int v1, int v2, int v3) : v1(v1), v2(v2), v3(v3){}
};

#endif // BASICRENDER_H
