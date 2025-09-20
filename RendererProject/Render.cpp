#include "Render.h"
#include <glm/ext.hpp>
#define TINYOBJLOADER_IMPLEMENTATION 1
#include <tiny_obj_loader.h>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION 1
#include <stb_image.h>
namespace aie
{

    Geometry MakeGeometry(const Vertex* const Verts, GLsizei VertCount, const GLuint* const Indicies, GLsizei IndexCount)
    {

        Geometry NewGeo = {};
        NewGeo.Size = IndexCount;

        glGenVertexArrays(1, &NewGeo.Vao);
        glGenBuffers(1, &NewGeo.Vbo);
        glGenBuffers(1, &NewGeo.Ibo);

        glBindVertexArray(NewGeo.Vao);
        glBindBuffer(GL_ARRAY_BUFFER, NewGeo.Vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NewGeo.Ibo);

        glBufferData(GL_ARRAY_BUFFER, VertCount * sizeof(Vertex), Verts, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(GLuint), Indicies, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return NewGeo;
    }

    void FreeGeometry(Geometry& Geo)
    {
        glDeleteBuffers(1, &Geo.Vbo);
        glDeleteBuffers(1, &Geo.Ibo);
        glDeleteVertexArrays(1, &Geo.Vao);

        Geo = {};
    }

    Shader MakeShader(const char* vertSource, const char* fragSource)
    {
        Shader NewShad = {};
        NewShad.Program = glCreateProgram();

        GLuint vert = glCreateShader(GL_VERTEX_SHADER);
        GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vert, 1, &vertSource, 0);
        glShaderSource(frag, 1, &fragSource, 0);
        glCompileShader(vert);
        glCompileShader(frag);
        glAttachShader(NewShad.Program, vert);
        glAttachShader(NewShad.Program, frag);

        glLinkProgram(NewShad.Program);

        glDeleteShader(vert);
        glDeleteShader(frag);

        return NewShad;
    }

    void FreeShader(Shader& shad)
    {
        glDeleteProgram(shad.Program);
        shad = {};
    }

    void Draw(const Shader& shad, const Geometry& geo)
    {
        glUseProgram(shad.Program);
        glBindVertexArray(geo.Vao);

        glDrawElements(GL_TRIANGLES, geo.Size, GL_UNSIGNED_INT, 0);

    }

    void SetUniform(const Shader& shad, GLuint location, const glm::mat4& value)
    {
        glProgramUniformMatrix4fv(shad.Program, location, 1, GL_FALSE, glm::value_ptr(value));
    }

    Geometry LoadGeometry(const char* filePath)
    {
        using namespace tinyobj;

        // contains all data for all vertex attributes loaded from the file
        attrib_t vertexAttributes;

        // enumeration of all shapes in obj file
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string error;
        std::string warn;

        bool success = LoadObj(&vertexAttributes, &shapes, &materials, &warn, &error, filePath);

        if (!error.empty())
        {
            fprintf(stderr, "%s", error.c_str());
        }
        if (!success || shapes.size() < 1)
        {
            return {};
        }

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        // Get mesh data
        size_t offSet = 0;
        for (GLsizei i = 0; i < shapes[0].mesh.num_face_vertices.size(); ++i)
        {
            // number of vertices for current face
            GLsizei faceVertices = shapes[0].mesh.num_face_vertices[i];

            assert(faceVertices == 3 && "Faces must be provided in triangles");

            // iterate over vertices used to form current face
            for (unsigned char j = 0; j < faceVertices; ++j)
            {
                tinyobj::index_t idx = shapes[0].mesh.indices[offSet + j];

                tinyobj::real_t vx = vertexAttributes.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = vertexAttributes.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = vertexAttributes.vertices[3 * idx.vertex_index + 2];

                tinyobj::real_t nx = vertexAttributes.normals[3 * idx.normal_index + 0];
                tinyobj::real_t ny = vertexAttributes.normals[3 * idx.normal_index + 1];
                tinyobj::real_t nz = vertexAttributes.normals[3 * idx.normal_index + 2];

                tinyobj::real_t tx = vertexAttributes.texcoords[2 * idx.texcoord_index + 0];
                tinyobj::real_t ty = vertexAttributes.texcoords[2 * idx.texcoord_index + 1];

                vertices.push_back(Vertex{ {vx, vy, vz, 1}, {nx, ny, nz, 1}, {tx, ty} });
                indices.push_back(faceVertices * i + j);
            }
            offSet += faceVertices;
        }

        assert(vertices.size() <= std::numeric_limits<GLsizei>::max());
        assert(indices.size() <= std::numeric_limits<GLsizei>::max());

        return MakeGeometry(&vertices[0], (GLsizei)vertices.size(), &indices[0], (GLsizei)shapes[0].mesh.indices.size());
    }
    Texture MakeTexture(unsigned width, unsigned height, unsigned channels, const unsigned char* pixels)
    {
        Texture retVal = { 0,width,height,channels };

        GLenum oglFormat = GL_RED;
        switch (channels)
        {
        case 1:
            oglFormat = GL_RED;
            break;
        case 2:
            oglFormat = GL_RG;
            break;
        case 3:
            oglFormat = GL_RGB;
            break;
        case 4:
            oglFormat = GL_RGBA;
            break;
        }
        glGenTextures(1, &retVal.Handle);
        glBindTexture(GL_TEXTURE_2D, retVal.Handle);

        glTexImage2D(GL_TEXTURE_2D, 0, oglFormat, width, height, 0, oglFormat, GL_UNSIGNED_BYTE, pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        return retVal;
    }
    void FreeTexture(Texture& tex)
    {
        glDeleteTextures(1, &tex.Handle);
        tex = {};
    }

    Texture LoadTexture(const char* imagePath)
    {
        int imageWidth, imageHeight, imageFormat;
        unsigned char* rawPixelData = nullptr;

        Texture newTexture = {};

        stbi_set_flip_vertically_on_load(true);

        rawPixelData = stbi_load(imagePath, &imageWidth, &imageHeight, &imageFormat, STBI_default);

        assert(rawPixelData != nullptr && "Image failed to load.");
        newTexture = MakeTexture(imageWidth, imageHeight, imageFormat, rawPixelData);
        stbi_image_free(rawPixelData);

        return newTexture;
    }

    void SetUniform(const Shader& shad, GLuint location, const Texture value, int textureSlot)
    {
        glActiveTexture(GL_TEXTURE0 + textureSlot);

        glBindTexture(GL_TEXTURE_2D, value.Handle);

        glProgramUniform1i(shad.Program, location, textureSlot);

    }

    void SetUniform(const Shader& shad, GLuint location, const glm::vec3& value)
    {
        glProgramUniform3fv(shad.Program, location, 1, glm::value_ptr(value));
    }

}