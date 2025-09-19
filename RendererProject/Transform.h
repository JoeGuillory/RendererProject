#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

class Transform
{
public:
	glm::vec3 LocalPos;
	glm::quat LocalRot;
	glm::vec3 LocalScl;

	Transform();

	Transform* Parent;

	std::vector<Transform*> Children;


public:
	glm::mat4 LocalMat() const;
	glm::mat4 WorldMat() const;

	glm::mat4 LocalToWorldMatrix() const;
	glm::mat4 WorldToLocalMatrix() const;

	void SetParent(Transform* newParent);
	Transform* GetParent() const;

	Transform* GetChildAtIndex(size_t index) const;
	size_t GetChildCount() const;

	glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 position);

	glm::vec3 GetLocalPosition() const;
	void SetLocalPosition(glm::vec3 position);

	glm::quat GetRotation() const;
	void SetRotation(glm::quat rotation);

	glm::vec3 GetForward() const;

	void SetForward(glm::vec3 forward);

	

};

