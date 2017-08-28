#include"CommonHeader.h"
#include "main.h"
#include "Shader.h"
#include"Camera.h"
#include "Factory.h"
#include <vector>
#include<map>
#include"Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include"Singleton.h"
using namespace glm;
using namespace std;

//#define DISABLE_DIRECTIONAL_LIGHT
//#define DISABLE_POINT_LIGHT
//#define DISABLE_SPOT_LIGHT




float mixValue = 0.2;
#define screenWidth  800
#define screenHeight 600

float lastFrameTime = 0;
float deltaTime = 0;

Camera g_Camera;

bool g_EnableFlashLight = false;
bool g_IsHoldFlashLightKey = 0;

#define VAO_NUMBER 3
#define NR_POINT_LIGHT 4

//VAOS
unsigned int g_VAO[VAO_NUMBER];
unsigned int g_SingleTextureVAO, g_ScreenVAO;

//shaders
Shader 
g_ShaderProgram,
g_SingleColorProgram,
g_SingleTextureProgram,
g_LampProgram,
g_ObjectProgram,
g_ScreenProgram;
mat4 g_Model, g_View, g_Projection;

//textures
unsigned int 
g_IronWoodTexture
, g_IronWoodSpecualrTexture
, g_WindowTexture
;

//Model
Model g_OurModel;


// transparent window locations
// --------------------------------
vector<glm::vec3> g_windows_pos
{
	glm::vec3(-1.5f, 0.0f, -0.48f),
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
	glm::vec3(-0.3f, 0.0f, -2.3f),
	glm::vec3(0.5f, 0.0f, -0.6f)
};

std::map<float,glm::vec3> g_SortedWindowsPos;

float g_vertices[36 * 5] =
{
	//position			//texCoord
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f

};

float g_verticesPosAndNormal[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float g_QuadVerticesPosAndNormal[] =
{
	-0.5f, -0.5f, 0.0f,  0,0,
	0.5f,-0.5f,0.0f,  1,0,
	0.5f,0.5f,0.0f,  1,1,
	-0.5f,0.5f,0.0f,  0,1
};

float g_QuadVerticesPosAndTexCoords[] = {
	// positions			// texCoords
	-1.0f,  -1.0f,	0.0f,  0.0f, 0.0f,
	1.0f,	-1.0f,	0.0f,  1.0f, 0.0f,
	1.0f,	1.0f,	0.0f,  1.0f, 1.0f,
	-1.0f,	1.0f,	0.0f,  0.0f, 1.0f
};

unsigned int g_indices[] =
{
	0,1,2, 0,2,3
};

glm::vec3 g_cubePositions[] =
{
	glm::vec3(0.0f,  0.0f,  0.f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 g_pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};
void test()
{

}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		if (!g_IsHoldFlashLightKey)
		{
			g_EnableFlashLight = !g_EnableFlashLight;
			g_IsHoldFlashLightKey = true;
		}
	}
	else
	{
		g_IsHoldFlashLightKey = false;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.01;
		if (mixValue > 1)mixValue = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.01;
		if (mixValue <0)mixValue = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		g_Camera.move(Camera::ENUM_MOVEDIRECTION::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		g_Camera.move(Camera::ENUM_MOVEDIRECTION::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		g_Camera.move(Camera::ENUM_MOVEDIRECTION::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		g_Camera.move(Camera::ENUM_MOVEDIRECTION::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		g_Camera.move(Camera::ENUM_MOVEDIRECTION::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		g_Camera.move(Camera::ENUM_MOVEDIRECTION::DOWN, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void prepareVAOs(unsigned int *out_VAO)
{
	glGenVertexArrays(VAO_NUMBER, out_VAO);

	//multi cubes VAO
	glBindVertexArray(out_VAO[0]);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	unsigned int PosAndNormalVBO;
	glGenBuffers(1, &PosAndNormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, PosAndNormalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_verticesPosAndNormal), g_verticesPosAndNormal, GL_STATIC_DRAW);

	//Light VAO
	glBindVertexArray(out_VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, PosAndNormalVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	//Cube VAO
	glBindVertexArray(out_VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, PosAndNormalVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

unsigned int loadTextureFromFile(const char* filePath)
{
	unsigned int texture;

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
	assert(data);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	return texture;
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	g_Camera.scroll_callback(window, xOffset, yOffset);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	g_Camera.mouse_callback(window, xPos, yPos);

	g_SortedWindowsPos.clear();
	for (int i = 0; i< g_windows_pos.size(); ++i)
	{
		float distance = glm::length(g_Camera.getPos() - g_windows_pos[i]);
		g_SortedWindowsPos[distance] = g_windows_pos[i];
	}
}
void drawScene()
{
#if 1
	//light scene
	glEnable(GL_DEPTH_TEST);
	//Lamps_____________________________________________________________________________
	g_LampProgram.use();
	g_LampProgram.setMat4("projection", g_Projection);
	glBindVertexArray(g_VAO[1]);
	for (int i = 0; i < NR_POINT_LIGHT; ++i)
	{
		g_LampProgram.setMat4("view", g_View);
		glm::mat4 model(1);
		model = glm::translate(model, g_pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		g_LampProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	//cube_____________________________________________________________________________
	g_ObjectProgram.use();
	g_ObjectProgram.setMat4("model", g_Model);
	g_ObjectProgram.setMat4("projection", g_Projection);
	g_ObjectProgram.setMat4("view", g_View);

#ifndef DISABLE_DIRECTIONAL_LIGHT
	//directional light param
	g_ObjectProgram.setVec3("directionalLight.direction", -0.2f, -1.0f, -0.3f);

	g_ObjectProgram.setVec3("directionalLight.ambient", 0.05f, 0.05f, 0.05f);
	g_ObjectProgram.setVec3("directionalLight.diffuse", 0.4f, 0.4f, 0.4f);
	g_ObjectProgram.setVec3("directionalLight.specular", 0.5f, 0.5f, 0.5f);
#endif // !DISABLE_DIRECTIONAL_LIGHT

#ifndef DISABLE_POINT_LIGHT
	// point light 1
	g_ObjectProgram.setVec3("pointLights[0].position", g_pointLightPositions[0]);
	g_ObjectProgram.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	g_ObjectProgram.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	g_ObjectProgram.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	g_ObjectProgram.setFloat("pointLights[0].constant", 1.0f);
	g_ObjectProgram.setFloat("pointLights[0].linear", 0.09f);
	g_ObjectProgram.setFloat("pointLights[0].quadratic", 0.032f);
	// point light 2
	g_ObjectProgram.setVec3("pointLights[1].position", g_pointLightPositions[1]);
	g_ObjectProgram.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	g_ObjectProgram.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	g_ObjectProgram.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	g_ObjectProgram.setFloat("pointLights[1].constant", 1.0f);
	g_ObjectProgram.setFloat("pointLights[1].linear", 0.09f);
	g_ObjectProgram.setFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	g_ObjectProgram.setVec3("pointLights[2].position", g_pointLightPositions[2]);
	g_ObjectProgram.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	g_ObjectProgram.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	g_ObjectProgram.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	g_ObjectProgram.setFloat("pointLights[2].constant", 1.0f);
	g_ObjectProgram.setFloat("pointLights[2].linear", 0.09f);
	g_ObjectProgram.setFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	g_ObjectProgram.setVec3("pointLights[3].position", g_pointLightPositions[3]);
	g_ObjectProgram.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	g_ObjectProgram.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	g_ObjectProgram.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	g_ObjectProgram.setFloat("pointLights[3].constant", 1.0f);
	g_ObjectProgram.setFloat("pointLights[3].linear", 0.09f);
	g_ObjectProgram.setFloat("pointLights[3].quadratic", 0.032f);
#endif // !DISABLE_POINT_LIGHT

#ifndef DISABLE_SPOT_LIGHT
	//spot light param
	if (g_EnableFlashLight)
	{
		g_ObjectProgram.setVec3("spotLight.position", g_Camera.getPos());
		g_ObjectProgram.setVec3("spotLight.direction", g_Camera.getFront());
		g_ObjectProgram.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		g_ObjectProgram.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		g_ObjectProgram.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		g_ObjectProgram.setFloat("spotLight.constant", 1.0f);
		g_ObjectProgram.setFloat("spotLight.linear", 0.09);
		g_ObjectProgram.setFloat("spotLight.quadratic", 0.032);
		g_ObjectProgram.setFloat("spotLight.cutOffCos", glm::cos(glm::radians(12.5f)));
		g_ObjectProgram.setFloat("spotLight.outerCutOffCos", glm::cos(glm::radians(18.0f)));
	}
	else
	{
		g_ObjectProgram.setVec3("spotLight.position", g_Camera.getPos());
		g_ObjectProgram.setVec3("spotLight.direction", g_Camera.getFront());
		g_ObjectProgram.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		g_ObjectProgram.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
		g_ObjectProgram.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
		g_ObjectProgram.setFloat("spotLight.constant", 1.0f);
		g_ObjectProgram.setFloat("spotLight.linear", 0.09f);
		g_ObjectProgram.setFloat("spotLight.quadratic", 0.032f);
		g_ObjectProgram.setFloat("spotLight.cutOffCos", glm::cos(glm::radians(12.5f)));
		g_ObjectProgram.setFloat("spotLight.outerCutOffCos", glm::cos(glm::radians(18.0f)));
	}
#endif // !DISABLE_SPOT_LIGHT

	//material param
	//g_ObjectProgram.setInt("material.diffuse", 0);
	//g_ObjectProgram.setInt("material.specular", 1);
	g_ObjectProgram.setFloat("material.shininess", 32.0f);
	g_ObjectProgram.setVec3("worldCameraPos", g_Camera.getPos());

	g_ObjectProgram.use();
	g_OurModel.Draw(g_ObjectProgram);


	//draw cubes
	g_ShaderProgram.use();
	g_ShaderProgram.setMat4("model", g_Model);
	g_ShaderProgram.setMat4("view", g_View);
	g_ShaderProgram.setMat4("projection", g_Projection);
	g_ShaderProgram.setFloat("outTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_IronWoodTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_IronWoodSpecualrTexture);

	//draw cubes
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 0x1, 0xff);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	glStencilMask(0xff);
	glBindVertexArray(g_VAO[2]);
	for (int i = 0; i < (sizeof(g_cubePositions) / sizeof(vec3)); ++i)
	{
		glm::mat4 model(1);
		model = glm::rotate(model, radians(float(20 * i)), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::translate(model, g_cubePositions[i]);
		g_ShaderProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	//draw cubes outline
	glStencilFunc(GL_NOTEQUAL, 0x1, 0xff);
	glDisable(GL_DEPTH_TEST);
	g_SingleColorProgram.use();
	g_SingleColorProgram.setMat4("projection", g_Projection);
	g_SingleColorProgram.setMat4("view", g_View);
	for (int i = 0; i < (sizeof(g_cubePositions) / sizeof(vec3)); ++i)
	{
		glm::mat4 model(1);
		model = glm::rotate(model, radians(float(20 * i)), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::translate(model, g_cubePositions[i]);
		model = glm::scale(model, glm::vec3(1.1, 1.1, 1.1));
		g_SingleColorProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
#endif
}

void drawWindows()
{
	//draw windows
	glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	g_SingleTextureProgram.use();
	g_SingleTextureProgram.setMat4("projection", g_Projection);
	g_SingleTextureProgram.setMat4("view", g_View);
	g_SingleTextureProgram.setFloat("texture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_WindowTexture);

	glBindVertexArray(g_SingleTextureVAO);

	for (auto i = g_SortedWindowsPos.rbegin(); i != g_SortedWindowsPos.rend(); ++i)
	{
		glm::mat4 model(1);
		model = glm::translate(model, i->second);
		model = glm::scale(model, glm::vec3(3, 3, 3));
		g_SingleTextureProgram.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

unsigned int createSingleTextureVAO()
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

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_QuadVerticesPosAndNormal), g_QuadVerticesPosAndNormal, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);

	//pos 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	//normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	return GrassVAO;
}

unsigned int createScreenVAO()
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

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_QuadVerticesPosAndTexCoords), g_QuadVerticesPosAndTexCoords, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);

	//pos 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	//texCoords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	return GrassVAO;
}
void createFBO(unsigned int &out_FBO, unsigned int &out_ColorTexture)
{
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	out_FBO = FBO;

	//generateTexture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	out_ColorTexture = textureColorbuffer;

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																								  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)assert(false);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void resetRenderState()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glDisable(GL_STENCIL_TEST);
	glStencilMask(0xff);

	glDisable(GL_BLEND);

	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
}
int main()
{
	test();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	if (NULL == window)
	{
		std::cout << "Failed to create glfw window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	//prepare render-----------------------------------------------------------------------------------------------------------------------
	g_ShaderProgram = Shader("ShaderCode/FirstDemoVS.glsl", "ShaderCode/FirstDemoPS.glsl");
	g_SingleColorProgram = Shader("ShaderCode/SingleColorVS.glsl", "ShaderCode/SingleColorPS.glsl");
	g_SingleTextureProgram = Shader("ShaderCode/SingleTextureVS.glsl", "ShaderCode/SingleTexturePS.glsl");
	g_LampProgram = Shader("ShaderCode/LocalToClipVS.glsl", "ShaderCode/LampPS.glsl");
	g_ObjectProgram = Shader("ShaderCode/MultiLightingVS.glsl", "ShaderCode/MultiLightingPS.glsl");
	//g_ScreenProgram = Shader("ShaderCode/PassThroughVS.glsl", "ShaderCode/SingleTexturePS.glsl");
	g_ScreenProgram = Shader("ShaderCode/PassThroughVS.glsl", "ShaderCode/BlurPS.glsl");

	prepareVAOs(g_VAO);
	g_SingleTextureVAO = createSingleTextureVAO();
	g_ScreenVAO = createScreenVAO();
	//g_Projection = perspective(radians(45.f), (screenWidth) / screenHeight, 0.1f, 100.f);

	vec3 cameraTarget(0.f, 0.f, 0.f);
	vec3 up(0.f, 1.f, 0.f);
	vec3 cameraDirection;
	float radius = 10;

	g_IronWoodTexture = loadTextureFromFile("../../Resources/Textures/container2.png");
	g_IronWoodSpecualrTexture = loadTextureFromFile("../../Resources/Textures/container2_specular.png");
	g_WindowTexture = loadTextureFromFile("../../Resources/Textures/blending_transparent_window.png");

	g_OurModel = Model("../../Resources/objects/nanosuit/nanosuit.obj");

	unsigned int FBO, textureColorBuffer;
	createFBO(FBO, textureColorBuffer);

	//renderLoop
	while (!glfwWindowShouldClose(window))
	{

		float curTime = glfwGetTime();
		deltaTime = curTime - lastFrameTime;
		lastFrameTime = curTime;

		//input
		processInput(window);

		//update matrixes
		g_Model = glm::mat4(1);
		g_View = g_Camera.GetWorldToViewMatrix();
		g_Projection = g_Camera.GetProjectionMatrix();

		//clearcolor buffer
		//glClearColor(0.2f, 0.3f, .3f, 1.f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		drawScene();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.3f, .3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glBindVertexArray(g_ScreenVAO);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		g_ScreenProgram.use();
		g_ScreenProgram.setInt("texture1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//reset
		resetRenderState();

		//swap back buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}