#include "Utility.h"

unsigned int Utility::loadTextureFromFile(const char* filePath, bool isSRGB)
{
	unsigned int texture;

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
	assert(data);
	if (data)
	{
		GLenum texture_data_format, file_data_format;
		if (nrChannels == 3)
		{
			file_data_format = GL_RGB;
			texture_data_format = isSRGB ? GL_SRGB : GL_RGB;
		}
		else if (nrChannels == 4)
		{
			file_data_format = GL_RGBA;
			texture_data_format = isSRGB ? GL_SRGB_ALPHA : GL_RGBA;
		}
		else
			assert(false);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, texture_data_format, width, height, 0, file_data_format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	return texture;
}
