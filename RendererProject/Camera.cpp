#include "Camera.h"
#include "Render.h"
#include <GLFW/glfw3.h>
using namespace aie;
Camera::Camera()
    :NearPlane(0.5f),FarPlane(1000),VerticalFov(45.5)
{

    
    

}

glm::mat4 Camera::ViewMat() const
{
    return glm::lookAt(glm::vec3(0, 1, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::ProjMat() const
{
    return glm::perspective(glm::radians(VerticalFov), 800.0f / 600.0f, NearPlane, FarPlane);
}

void Camera::Tick(float deltaTime)
{
    OTransform.SetRotation(OTransform.GetRotation() * glm::quat(glm::highp_vec3(0, 0.1, 0)));
    


}

void Camera::Draw() const
{
    glm::vec3 ambient(1.5f, 1.5f, 1.5f);
    
    SetUniform(*Shad, 0, ProjMat());
    SetUniform(*Shad, 1, ViewMat());
    SetUniform(*Shad, 2, OTransform.LocalMat());
    SetUniform(*Shad, 3, *Tex, 0);
    SetUniform(*Shad, 4, ambient);
    aie::Draw(*Shad, *Geo);
}
