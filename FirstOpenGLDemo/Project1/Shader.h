#pragma once
class Shader
{
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);

	void use();
	void setBool(const char* name, bool value);
	void setInt(const char * name, int value);
	void setFloat(const char * name, float value);
	void setVec3(const char * name, float x, float y, float z);
	void setVec3(const char * name, const glm::vec3 value);
	void setMat4(const char * name, const glm::mat4 & value);
private:
	void checkCompilationError(unsigned int shader);
	void checkLinkError();
	void checkIsProgramBound();
	int getUniformLocation(const char * name);
};