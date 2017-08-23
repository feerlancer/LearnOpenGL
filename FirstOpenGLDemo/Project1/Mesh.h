#pragma once
#include"CommonHeader.h"
#include <vector>
#include <assimp/Importer.hpp>

using namespace std;

class Shader;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Texture
{
	unsigned int id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Draw(Shader shader);
private:
	vector<Vertex> m_Vertices;
	vector<unsigned int> m_Indices;
	vector<Texture> m_Textures;

	unsigned int m_VBO; //vertex buffer object
	unsigned int m_EBO;// element buffer object
	unsigned int m_VAO;// vertex array object
	void setupMesh();
};

