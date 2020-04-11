// Cuboid.h
// Author: Bairuo

#ifndef ICOSPHERE_H_
#define ICOSPHERE_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

#include "Color.h"
#include "Shader.h"
#include "Renderer.h"

class Material;

class IcoSphere : public Renderer
{
public:
	bool castShadow = true;

	virtual double getSphereBoundingRadius();

	virtual void render();

	virtual void render(Shader &shader);

	virtual ~IcoSphere();

	IcoSphere(const boost::shared_ptr<Material> &material = kDefaultMaterial,
		const GLchar *vertexPath = kStandardVsPath, const GLchar *fragmentPath = kStandardFragPath);


private:
	GLuint VAO;

	Shader shader;
	boost::shared_ptr<Material> material;

	bool active = true;

	static const GLchar kStandardVsPath[];
	static const GLchar kStandardFragPath[];
	static const boost::shared_ptr<Material> kDefaultMaterial;
};


#endif // ICOSPHERE_H_
