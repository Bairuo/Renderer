#include "IcoSphere.h"
#include "BasicConfig.h"
#include "basicrender.h"
#include "Camera.h"
#include "Light.h"
#include "Transform.h"
#include "Animation.h"
#include "Object.h"
#include "Deferred.h"
#include "BasicMaterial.h"
#include "PBRMaterial.h"
#include <cmath>

//const boost::shared_ptr<Material> IcoSphere::kDefaultMaterial(
//	new BASICMaterial(
//	glm::vec3(1.0f, 0.5f, 0.31f),
//	glm::vec3(1.0f, 0.5f, 0.31f),
//	glm::vec3(0.5f, 0.5f, 0.5f),
//	32.0f)
//);

const boost::shared_ptr<Material> IcoSphere::kDefaultMaterial(
	new PBRMaterial(
		glm::vec3(1.0f, 0.5f, 0.31f),	// albedo
		0.03f,	// metallic
		0.2f,	// roughness
		0.1f	// ao
	)	
);

static unsigned int triNums;
static const unsigned int kRecursionLevel = 3;

#if defined(_WIN32)

#if defined(SHADOWMAP)
// pbr shader in shadowmap need to write
const GLchar IcoSphere::kStandardVsPath[] = ".\\shaders\\3D_Standard\\standard_shadow.vs";
const GLchar IcoSphere::kStandardFragPath[] = ".\\shaders\\3D_Standard\\standard_shadow.frag";
#else
const GLchar IcoSphere::kStandardVsPath[] = ".\\shaders\\3D_Standard\\pbr.vs";
const GLchar IcoSphere::kStandardFragPath[] = ".\\shaders\\3D_Standard\\pbr.frag";
#endif

#else

#if defined(SHADOWMAP)
// pbr shader in shadowmap need to write
const GLchar IcoSphere::kStandardVsPath[] = "./shaders/3D_Standard/standard_shadow.vs";
const GLchar IcoSphere::kStandardFragPath[] = "./shaders/3D_Standard/standard_shadow.frag";
#else
const GLchar IcoSphere::kStandardVsPath[] = "./shaders/3D_Standard/pbr.vs";
const GLchar IcoSphere::kStandardFragPath[] = "./shaders/3D_Standard/pbr.frag";
#endif

#endif

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

	material->set(shader);

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

IcoSphere::IcoSphere(const boost::shared_ptr<Material> &material,
	const GLchar *vertexPath, const GLchar *fragmentPath)
	: material(material)
{
	shader = Shader(vertexPath, fragmentPath);

	shader.Use();
	material->set(shader);
	shader.Stop();

	VAO = getIcoSphereVAO(kRecursionLevel, triNums);
}

double IcoSphere::getSphereBoundingRadius()
{
	// Temporary code, to be modified
	double x = obj->transform->getPosX() * obj->transform->getScaleX();
	double y = obj->transform->getPosY() * obj->transform->getScaleY();
	double z = obj->transform->getPosZ() * obj->transform->getScaleZ();

	return sqrt(x * x + y * y + z * z);
}
