#pragma once
#include "Transform.h"

namespace aie
{
	struct Geometry;
	struct Shader;
	struct Texture;
}

class Object
{
public:
	Transform OTransform;

	aie::Geometry* Geo;
	aie::Shader* Shad;
	aie::Shader* TexShader;
	aie::Texture* Tex;

	Object();
	virtual void Tick(float deltaTime);
	virtual void Draw() const;
};

