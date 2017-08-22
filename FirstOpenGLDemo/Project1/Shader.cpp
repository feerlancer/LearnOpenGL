#include"CommonHeader.h"
#include "Shader.h"
using namespace std;
using namespace glm;
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//read shader code from file
	string vertexCode,fragmentCode;
	ifstream vShaderFile,fShaderFile;
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::Shader File Failed to Read!" << endl;
		assert(false);
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//compilation

	int success;
	char infoLog[512];
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	checkCompilationError(vertexShader, vertexPath);

	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	checkCompilationError(fragmentShader, fragmentPath);

	//Shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	checkLinkError();

	//delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const char* name, bool value)
{
	checkIsProgramBound();
	int location = getUniformLocation(name);
	glUniform1i(location, static_cast<int>(value));
}

void Shader::setInt(const char* name, int value)
{
	checkIsProgramBound();
	int location = getUniformLocation(name);
	glUniform1i(location, value);
}

void Shader::setFloat(const char* name, float value)
{
	checkIsProgramBound();
	int location = getUniformLocation(name);
	glUniform1f(location, value);
}

void Shader::setVec3(const char* name, float x, float y, float z )
{
	checkIsProgramBound();
	int location = getUniformLocation(name);
	glUniform3f(location,x,y,z);
}

void Shader::setVec3(const char* name, const glm::vec3 value)
{
	checkIsProgramBound();
	int location = getUniformLocation(name);
	glUniform3f(location, value.x, value.y, value.z);
}


void Shader::setMat4(const char * name, const glm::mat4& value)
{
	checkIsProgramBound();
	int location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

void Shader::checkCompilationError(unsigned int shader,const char* shaderFilePath)
{
	int success;
	char infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		cout << "ERROR:: shader "<< shaderFilePath <<" compilatin failed!\n" << infoLog << endl;
	}
	assert(success);
}

void Shader::checkLinkError()
{
	int success;
	char infoLog[1024];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 1024, NULL, infoLog);
		cout << "Error:: link shaders failed\n" << infoLog << endl;
	}
	assert(success);
}

void Shader::checkIsProgramBound()
{
	int curID;
	glGetIntegerv(GL_CURRENT_PROGRAM, &curID);
	assert(ID == curID);
}

int Shader::getUniformLocation(const char * name)
{
	int location = glGetUniformLocation(ID,name);
	assert(location != -1);
	return location;
}
