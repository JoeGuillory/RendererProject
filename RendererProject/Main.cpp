
#include "Render.h"
#include "Context.h"
#include <glm/ext.hpp>
#include "Camera.h"
#include "Object.h"
#include "GLFW/glfw3.h"

using namespace aie;


int main()
{
	Context Window;
	Window.Init(640, 480, "Hello Window");
	
	Object swordObject;

	Geometry swordGeometry = LoadGeometry("Models/obj/soulspear.obj");
	Texture swordTex = LoadTexture("Models/img/soulspear_diffuse.tga");
	swordObject.Geo = &swordGeometry;
	swordObject.Tex = &swordTex;


	glm::mat4 cam_proj = glm::perspective(glm::radians(45.5f), 800.0f / 600.0f, 0.1f, 1000.0f);
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 1, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));
	glm::mat4 triangle_model = glm::identity<glm::mat4>();

	
	Shader advancedShad = LoadShader("Res/Shaders/Camera.vert", "Res/Shaders/Camera.frag");
	swordObject.Shad = &advancedShad;

	glm::vec3 ambient(1, 1, 1);
	glm::vec3 sunDirection(10, 10, 10);
	glm::vec3 lightColor(1,0, 0);

	while (!Window.ShouldClose())
	{
		Window.Tick();
		swordObject.Tick(glfwGetTime());
		SetUniform(advancedShad, 0, cam_proj);
		SetUniform(advancedShad, 1, cam_view);
		SetUniform(advancedShad, 2, swordObject.OTransform.WorldMat());
		SetUniform(advancedShad, 3, swordTex, 0);
		SetUniform(advancedShad, 4, ambient);
		SetUniform(advancedShad, 5, sunDirection);
		SetUniform(advancedShad, 6, lightColor);

		Window.Clear();
		swordObject.Draw();
		
	}
	
	aie::FreeShader(advancedShad);
	aie::FreeGeometry(*swordObject.Geo);
	aie::FreeTexture(swordTex);
	Window.Term();
	return 0;
}