#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
namespace aie
{
		struct Vertex
		{
			glm::vec4 Pos;
			glm::vec4 Normals;
			glm::vec2 UVs;
		};

		struct Geometry
		{
			GLuint Vao=0, Vbo=0, Ibo=0;
			GLuint Size=0;
		};

		struct Shader 
		{
			GLuint Program;
		};

		Geometry MakeGeometry(const Vertex* const Verts, GLsizei VertCount, const GLuint* const Indicies, GLsizei IndexCount);
		void FreeGeometry(Geometry& Geo);
		Shader MakeShader(const char* vertSource, const char* fragSource);
		void FreeShader(Shader& shad);
		void Draw(const Shader& shad, const Geometry& geo);
		void SetUniform(const Shader& shad, GLuint location, const glm::mat4& value);
		Geometry LoadGeometry(const char* filePath);

}
