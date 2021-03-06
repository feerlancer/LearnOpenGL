#pragma once
#include "CommonHeader.h"
class Shader
{
public:
	unsigned int ID;
	Shader() {};
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

	void use();
	void setBool(const char* name, bool value);
	void setInt(const char * name, int value);
	void setFloat(const char * name, float value);
	void setVec3(const char * name, float x, float y, float z);
	void setVec3(const char * name, const glm::vec3 value);
	void setMat4(char * name, const glm::mat4 & value);
	void setMat4(const char * name, const glm::mat4 & value);
private:
	void checkCompilationError(unsigned int shader, const char* shaderFilePath);
	void checkLinkError();
	void checkIsProgramBound();
	int getUniformLocation(const char * name);
};