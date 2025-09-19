
#include "Render.h"
#include "Context.h"
#include <glm/ext.hpp>
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

	const char* basicVert =
		"#version 430 core\n"
		"layout (location = 0) in vec4 position;\n"
		"void main() {gl_Position = position;}";

	const char* basicFrag =
		"#version 430 core\n"
		"out vec4 vertColor;\n"
		"void main(){vertColor = vec4(1.0,0.0,0.0,1.0);}";

	glm::mat4 cam_proj = glm::perspective(glm::radians(45.5f), 800.0f / 600.0f, 0.1f, 1000.0f);
	glm::mat4 cam_view = glm::lookAt(glm::vec3(0, 1, 20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));
	glm::mat4 triangle_model = glm::identity<glm::mat4>();

	Shader basicShad = MakeShader(basicVert, basicFrag);

	while (!Window.ShouldClose())
	{
		Window.Tick();
		Window.Clear();

		SetUniform(basicShad, 0, cam_proj);
		SetUniform(basicShad, 1, cam_view);
		SetUniform(basicShad, 2, triangle_model);

		Draw(basicShad, basicTriangleGeo);
	}

	FreeGeometry(basicTriangleGeo);
	FreeShader(basicShad);
	Window.Term();
	return 0;
}