#include "Transform.h"

#include "glm/gtc/matrix_inverse.hpp"
Transform::Transform()
{
	Parent = nullptr;
	LocalPos = glm::vec3(0, 0, 1);
	LocalRot = glm::identity<glm::quat>();
	LocalScl = glm::vec3(1, 1, 1);

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
	if (Parent == nullptr)
	{
		return glm::identity<glm::mat4>();
	}
	else
	{
		return Parent->WorldMat();
	}
}

glm::mat4 Transform::WorldToLocalMatrix() const
{
	glm::mat4 inverse = glm::affineInverse(LocalMat());
	return WorldMat() * inverse;
}


void Transform::SetParent(Transform* newParent)
{
	if (Parent != nullptr)
	{
		// returns an iterator to this child in its parent's vector of children
		auto findIt = std::find(Parent->Children.begin(), Parent->Children.end(), this);

		// if found, iterator will be something besides the end of the vector
		if (findIt != Parent->Children.end())
		{
			Parent->Children.erase(findIt);
			Parent = nullptr;
		}
	}

	// add to new parent if any
	if (newParent != nullptr)
	{
		Parent = newParent;
		Parent->Children.push_back(this);
	}
}

Transform* Transform::GetParent() const
{
	return Parent;
}

Transform* Transform::GetChildAtIndex(size_t index) const
{
	return Children[index];
}

size_t Transform::GetChildCount() const
{
	return Children.size();
}

glm::vec3 Transform::GetPosition() const
{
	glm::mat4 worldMat = WorldMat();
	return glm::vec3();
}

void Transform::SetPosition(glm::vec3 position)
{
	
}

glm::vec3 Transform::GetLocalPosition() const
{
	return LocalPos;
}

void Transform::SetLocalPosition(glm::vec3 position)
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
	auto mat = WorldMat();

	return glm::vec3(0,0,0);
}

void Transform::SetForward(glm::vec3 forward)
{

}


