
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
	
	Vertex triVerts[] =
	{
		{{-.5f,-.5f,0,1}},
		{{.5f,-.5f,0,1}},
		{{0,.5f,0,1}}
	};
	unsigned int triIndicies[] = { 0,1,2 };

	Geometry basicTriangleGeo = MakeGeometry(triVerts, 3, triIndicies, 3);
	const char* modelPath = "Models/obj/soulspear.obj";
	const char* swordTexture = "Models/img/soulspear_normal.tga";
	Texture swordTex = LoadTexture(swordTexture);

	Camera swordObject;

	Geometry sword = LoadGeometry(modelPath);
	swordObject.Tex = &swordTex;
	swordObject.Geo = &sword;
	const char* basicVert =
		"#version 430 core\n"
		"layout (location = 0) in vec4 position;\n"
		"void main() {gl_Position = position;}";

	const char* advancedVert =
		"#version 430 core\n"
		"layout (location = 0) in vec4 position;\n"
		"layout (location = 1) in vec4 normal;\n"
		"layout (location = 2) in vec2 uv;\n"
		"layout (location = 0) uniform mat4 proj;\n"
		"layout (location = 1) uniform mat4 view;\n"
		"layout (location = 2) uniform mat4 model;\n"
		"out vec2 vUV;"
		"out vec3 vNormal;"
		"out vec4 vPos;"
		"void main() {gl_Position = proj * view * model * position; vPos = position; vUV = uv; vNormal = mat3(transpose(inverse(model))) * normal.xyz;}";
		
	
	const char* texture =
		"#version 430 core\n"
		"layout (location = 3) uniform sampler2D albedo;"
		"layout (location = 4) uniform vec3 ambientLightColor;"
		"layout (location = 5) uniform vec3 lightDir;"
		"layout (location = 6) uniform vec3 lightColor;"
		"in vec4 vPos;"
		"in vec2 vUV;"
		"in vec3 vNormal;"
		"out vec4 outColor;"
		"void main() {float d = max(0,dot(vNormal, -lightDir)); vec3 diffuse = d * lightColor; vec4 base = texture(albedo,vUV); outColor.xyz = base.xyz * (abientLightColor + diffuse); outColor.a = base.a;}";


	const char* basicFrag =
		"#version 430 core\n"
		"out vec4 vertColor;\n"
		"void main(){vertColor = vec4(1.0,0.0,0.0,1.0);}";

	glm::mat4 cam_proj = glm::perspective(glm::radians(45.5f), 800.0f / 600.0f, 0.1f, 1000.0f);
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 1, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));
	glm::mat4 triangle_model = glm::identity<glm::mat4>();

	Shader basicShad = MakeShader(basicVert, basicFrag);
	Shader advancedShad = MakeShader(advancedVert, texture);
	swordObject.Shad = &advancedShad;

	while (!Window.ShouldClose())
	{
		Window.Tick();
		swordObject.Tick(0);
		if (glfwGetKey(Window.Window, GLFW_KEY_W) == GLFW_PRESS)
		{
			swordObject.OTransform.SetLocalPosition(swordObject.OTransform.GetLocalPosition() + glm::vec3(1,0,0));
		}
		if (glfwGetKey(Window.Window, GLFW_KEY_S) == GLFW_PRESS)
		{
			swordObject.OTransform.SetLocalPosition(swordObject.OTransform.GetLocalPosition() + glm::vec3(-1, 0, 0));
		}
		Window.Clear();
		swordObject.Draw();
		
	}
	
	aie::FreeGeometry(basicTriangleGeo);
	aie::FreeShader(basicShad);
	aie::FreeShader(advancedShad);
	aie::FreeTexture(swordTex);
	Window.Term();
	return 0;
}