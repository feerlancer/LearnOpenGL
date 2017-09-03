#pragma once
#include<vector>
#include"Shader.h"
using namespace std;
class SimpleModel
{
public:
	SimpleModel() {};
	SimpleModel(vector<unsigned int> vertexDataFormat, void* vertexData, unsigned int vertexDataSize, const char* textureFilePath);
	void Draw(Shader &shader);
private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_VertexNum;
	unsigned int m_Texture;
};