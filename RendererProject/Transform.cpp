#include "Transform.h"
#include <glm/glm.hpp>
Transform::Transform()
{

}

glm::mat4 Transform::LocalMat() const
{
	glm::mat4 trs = glm::translate(glm::mat4(1.0f), LocalPos); // translation
	trs *= glm::toMat4(LocalRot);        // rotation
	trs = glm::scale(trs, LocalScl);       // scale
	return trs;
}

glm::mat4 Transform::WorldMat() const
{
	if (Parent)
	{
		return Parent->WorldMat() * LocalMat();
	}
	else
	{
		return LocalMat();
	}
}

glm::mat4 Transform::LocalToWorldMatrix() const
{
	return glm::mat4();
}

glm::mat4 Transform::WorldToLocalMatrix() const
{
	return glm::mat4();
}

void Transform::SetParent(Transform* newParent)
{
	Parent = newParent;
}

Transform* Transform::GetParent() const
{
	return Parent;
}

glm::vec3 Transform::GetPosition() const
{
	return LocalPos;
}

void Transform::SetPosition(glm::vec3 position)
{
	LocalPos = position;
}

glm::quat Transform::GetRotation() const
{
	return LocalRot;
}

void Transform::SetRotation(glm::quat rotation)
{
	LocalRot = rotation;
}

glm::vec3 Transform::GetForward() const
{
	return glm::vec3();
}

void Transform::SetForward(glm::vec3 forward)
{
}

glm::vec3 Transform::GetLossyScale() const
{
	return glm::vec3();
}
