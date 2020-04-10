#include "IcoSphere.h"
#include "ConfigUtilities.h"
#include "basicrender.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Animation.h"
#include "Object.h"
#include "Deferred.h"
#include <cmath>

const Material IcoSphere::defaultMaterial(
	glm::vec3(1.0f, 0.5f, 0.31f),
	glm::vec3(1.0f, 0.5f, 0.31f),
	glm::vec3(0.5f, 0.5f, 0.5f),
	32.0f
);

static int triNums;

void IcoSphere::render(Shader &shader)
{
	if (!active)
	{
		return;
	}

	shader.Use();

	shader.SetMat4("model", obj->transform->getMatrix());
	Camera::setMainCamera(&shader);
	Light::setLight(&shader);

	setMaterial(&shader);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, triNums, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader.Stop();
}

void IcoSphere::render()
{
	render(shader);
}

IcoSphere::~IcoSphere()
{
	//    glDeleteVertexArrays(1, &VAO);
	//    glDeleteBuffers(1, &VBO);
}


void IcoSphere::setMaterial(Shader *shader)
{
	shader->Set3f("material.ambient", material.ambient);
	shader->Set3f("material.diffuse", material.diffuse);
	shader->Set3f("material.specular", material.specular);
	shader->SetFloat("material.shininess", material.shininess);
}

IcoSphere::IcoSphere(const Material &material,
	const GLchar *vertexPath, const GLchar *fragmentPath)
	: material(material)
{
	shader = Shader(vertexPath, fragmentPath);

	shader.Use();
	setMaterial(&shader);
	shader.Stop();

	VAO = getIcoSphereVAO(2, triNums);
}

double IcoSphere::getSphereBoundingRadius()
{
	double x = obj->transform->getPosX() * obj->transform->getScaleX();
	double y = obj->transform->getPosY() * obj->transform->getScaleY();
	double z = obj->transform->getPosZ() * obj->transform->getScaleZ();

	return sqrt(x * x + y * y + z * z);
}
