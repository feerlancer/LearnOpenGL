#pragma once
class Shader;
class Skybox
{
public:
	Skybox(const char* a_FilePathes[]);
	Skybox() {};
	void Draw(Shader shader);
	unsigned int GetCubeTextureID() { return m_TextureID; }
private:
	unsigned int m_VAO;
	unsigned int m_TextureID;

	unsigned int loadCubeMapFromFile(const char* a_FilePathes[]);
};
