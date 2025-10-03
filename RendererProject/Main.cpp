
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
	Object box;
	Geometry swordGeometry = LoadGeometry("Models/obj/soulspear.obj");
	Texture swordTex = LoadTexture("Models/img/soulspear_diffuse.tga");
	Geometry boxObject = LoadGeometry("Models/obj/cube.obj");
	Texture boxTexture = LoadTexture("Models/img/white.png");
	box.Geo = &boxObject;
	box.Tex = &boxTexture;

	swordObject.Geo = &swordGeometry;
	swordObject.Tex = &swordTex;


	glm::mat4 cam_proj = glm::perspective(glm::radians(45.5f), 800.0f / 600.0f, 0.1f, 1000.0f);
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 1, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));
	glm::mat4 cameraTransfrom = glm::identity<glm::mat4>();

	
	Shader advancedShad = LoadShader("Res/Shaders/Camera.vert", "Res/Shaders/Camera.frag");
	swordObject.Shad = &advancedShad;
	box.Shad = &advancedShad;
	glm::vec3 ambient(1, 1, 1);
	glm::vec3 sunDirection(0, 1, 0);
	glm::vec3 lightColor(0,0,10);

	double lastFrameTime = glfwGetTime();

	while (!Window.ShouldClose())
	{
		double currentFrameTime = glfwGetTime();
		double deltaTime = currentFrameTime - lastFrameTime;
		Window.Tick();
		swordObject.Tick(deltaTime);
		box.Tick(deltaTime);

		Window.Clear();
		SetUniform(advancedShad, 0, cam_proj);
		SetUniform(advancedShad, 1, cam_view);
		
		SetUniform(advancedShad, 4, ambient);
		SetUniform(advancedShad, 5, sunDirection);
		SetUniform(advancedShad, 6, lightColor);

		SetUniform(advancedShad, 2, swordObject.OTransform.WorldMat());
		SetUniform(advancedShad, 3, *swordObject.Tex, 0);
		swordObject.Draw();
		SetUniform(advancedShad, 2, box.OTransform.WorldMat());
		SetUniform(advancedShad, 3, *box.Tex, 0);
		box.Draw();
		lastFrameTime = currentFrameTime;
	}
	
	aie::FreeShader(advancedShad);
	aie::FreeGeometry(*swordObject.Geo);
	aie::FreeTexture(swordTex);
	Window.Term();
	return 0;
}