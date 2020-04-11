#include "PBRMaterial.h"
#include "Shader.h"

void PBRMaterial::set(Shader &shader)
{
	//shader.Set3f("material.albedo", albedo);
	//shader.SetFloat("material.metallic", metallic);
	//shader.SetFloat("material.roughness", roughness);
	//shader.SetFloat("material.ao", ao);

	shader.Set3f("albedo", albedo);
	shader.SetFloat("metallic", metallic);
	shader.SetFloat("roughness", roughness);
	shader.SetFloat("ao", ao);
}