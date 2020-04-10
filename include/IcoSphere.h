// Cuboid.h
// Author: Bairuo

#ifndef ICOSPHERE_H_
#define ICOSPHERE_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

#include "Color.h"
#include "Shader.h"
#include "Material.h"
#include "Renderer.h"

class IcoSphere : public Renderer
{
public:
	bool castShadow = true;

	virtual double getSphereBoundingRadius();

	virtual void render();

	virtual void render(Shader &shader);

	virtual ~IcoSphere();

	IcoSphere(const Material &material = kDefaultMaterial,
		const GLchar *vertexPath = kStandardVsPath, const GLchar *fragmentPath = kStandardFragPath);

	void setMaterial(Shader *shader);


private:
	GLuint VAO;

	Shader shader;
	Material material;

	bool active = true;

	static const GLchar kStandardVsPath[];
	static const GLchar kStandardFragPath[];
	static const Material kDefaultMaterial;
};


#endif // ICOSPHERE_H_
