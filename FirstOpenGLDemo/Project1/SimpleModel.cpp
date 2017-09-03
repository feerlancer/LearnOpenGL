#include "SimpleModel.h"
#include"Utility.h"
SimpleModel::SimpleModel(vector<unsigned int> vertexDataFormat, void * vertexData, unsigned int vertexDataSize, const char * textureFilePath)
{
	m_Texture = 0;
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);
	int stride = 0;
	for (int i = 0; i < vertexDataFormat.size(); ++i)
	{
		stride += vertexDataFormat[i];
	}
	stride *= sizeof(float);
	m_VertexNum = vertexDataSize / stride;
	unsigned int offset = 0;
	for (int i = 0; i < vertexDataFormat.size(); ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, vertexDataFormat[i], GL_FLOAT, GL_FALSE, stride, (void*)(offset * sizeof(float)));
		offset += vertexDataFormat[i];
	}

	m_Texture = Utility::loadTextureFromFile(textureFilePath);
}

void SimpleModel::Draw(Shader & shader)
{
	shader.use();
	if (m_Texture > 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,m_Texture);
	}
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_VertexNum);
}
