#pragma once
#include "Object.h"
class Camera : public Object
{
public:
	float VerticalFov;
	float NearPlane;
	float FarPlane;

	Camera();

	glm::mat4 ViewMat() const;
	glm::mat4 ProjMat() const;

	 void Tick(float deltaTime) override;
	 void Draw() const override;

};

