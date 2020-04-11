#include "BasicMaterial.h"
#include "Shader.h"

void BASICMaterial::set(Shader &shader)
{
	shader.Set3f("material.ambient", ambient);
	shader.Set3f("material.diffuse", diffuse);
	shader.Set3f("material.specular", specular);
	shader.SetFloat("material.shininess", shininess);
}