#pragma once
#include "CommonHeader.h"
class VAOCreater
{
public:
	static unsigned int createSingleTextureVAO(float* a_Vertices, unsigned int sizeOfVertices, unsigned int* a_indices, unsigned int a_sizeOfIndices)
	{
		unsigned int GrassVAO;
		glGenVertexArrays(1, &GrassVAO);
		unsigned int GrassVBO;
		glGenBuffers(1, &GrassVBO);
		unsigned int GrassEBO;
		glGenBuffers(1, &GrassEBO);

		glBindVertexArray(GrassVAO);
		glBindBuffer(GL_ARRAY_BUFFER, GrassVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GrassEBO);

		glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, a_Vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_sizeOfIndices, a_indices, GL_STATIC_DRAW);

		//pos 
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		//normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		return GrassVAO;
	}
	static unsigned int createScreenVAO(float* a_Vertices, unsigned int sizeOfVertices,unsigned int* a_indices, unsigned int a_sizeOfIndices)
	{
		unsigned int GrassVAO;
		glGenVertexArrays(1, &GrassVAO);
		unsigned int GrassVBO;
		glGenBuffers(1, &GrassVBO);
		unsigned int GrassEBO;
		glGenBuffers(1, &GrassEBO);

		glBindVertexArray(GrassVAO);
		glBindBuffer(GL_ARRAY_BUFFER, GrassVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GrassEBO);

		glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, a_Vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_sizeOfIndices, a_indices, GL_STATIC_DRAW);

		//pos 
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		//texCoords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		return GrassVAO;
	}
};