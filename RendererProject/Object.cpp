#include "Object.h"
#include "Render.h"



Object::Object()
{

}

void Object::Tick(float deltaTime)
{
	OTransform.SetRotation(OTransform.GetRotation() * glm::quat(glm::highp_vec3(0, 5, 0) * deltaTime));
}

void Object::Draw() const
{
	aie::Draw(*Shad, *Geo);
}
