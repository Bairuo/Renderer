#ifndef MATERIAL_H_
#define MATERIAL_H_

class Shader;

class Material
{
public:
	virtual void set(Shader &shader) = 0;

	virtual ~Material() = default;
};

#endif
