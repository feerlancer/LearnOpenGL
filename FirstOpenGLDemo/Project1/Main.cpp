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
#define screenWidth  800.f
#define screenHeight 600.f

float lastFrameTime = 0;
float deltaTime = 0;

Camera g_Camera;

bool g_EnableFlashLight = false;
bool g_IsHoldFlashLightKey = 0;

#define VAO_NUMBER 3
#define NR_POINT_LIGHT 4

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

unsigned int g_indices[] =
{
	0,1,2, 0,2,3
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
		//glActiveTexture(GL_TEXTURE0+textureUnit);
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

unsigned int prerareSingleTextureVAO()
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

void resetRenderState()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glDisable(GL_STENCIL_TEST);
	glStencilMask(0xff);
	glDisable(GL_BLEND);
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
	Shader shaderProgram("ShaderCode/FirstDemoVS.glsl", "ShaderCode/FirstDemoPS.glsl");
	Shader SingleColorProgram("ShaderCode/SingleColorVS.glsl", "ShaderCode/SingleColorPS.glsl");
	Shader SingleTextureProgram("ShaderCode/SingleTextureVS.glsl", "ShaderCode/SingleTexturePS.glsl");
	Shader LampProgram("ShaderCode/LocalToClipVS.glsl", "ShaderCode/LampPS.glsl");
	Shader ObjectProgram("ShaderCode/MultiLightingVS.glsl", "ShaderCode/MultiLightingPS.glsl");
	glm::vec3 cubePositions[] = 
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
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	unsigned int VAO[VAO_NUMBER];
	prepareVAOs(VAO);
	unsigned int singleTextureVAO = prerareSingleTextureVAO();

	mat4 model, view, projection;
	model = mat4(1);
	projection = perspective(radians(45.f), (screenWidth) / screenHeight, 0.1f, 100.f);

	vec3 cameraTarget(0.f, 0.f, 0.f);
	vec3 up(0.f, 1.f, 0.f);

	vec3 cameraDirection;

	float radius = 10;

	unsigned int ironWoodTexture = loadTextureFromFile("../../Resources/Textures/container2.png");
	unsigned int ironWoodSpecualrTexture = loadTextureFromFile("../../Resources/Textures/container2_specular.png");
	unsigned int windowTexture = loadTextureFromFile("../../Resources/Textures/blending_transparent_window.png");

	Model ourModel("../../Resources/objects/nanosuit/nanosuit.obj");
	//renderLoop
	while (!glfwWindowShouldClose(window))
	{

		float curTime = glfwGetTime();
		deltaTime = curTime - lastFrameTime;
		lastFrameTime = curTime;

		//input
		processInput(window);

		//clearcolor buffer
		glClearColor(0.2f, 0.3f, .3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);

		model = glm::mat4(1);
		view = g_Camera.GetWorldToViewMatrix();
		projection = g_Camera.GetProjectionMatrix();

#if 1
		//light scene

		//Lamps_____________________________________________________________________________
		LampProgram.use();
		LampProgram.setMat4("projection", projection);
		glBindVertexArray(VAO[1]);
		for (int i = 0; i < NR_POINT_LIGHT; ++i)
		{
			LampProgram.setMat4("view", view);
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			LampProgram.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//cube_____________________________________________________________________________
		ObjectProgram.use();
		model = glm::mat4(1);
		ObjectProgram.setMat4("model", model);
		ObjectProgram.setMat4("projection", projection);
		ObjectProgram.setMat4("view", view);

#ifndef DISABLE_DIRECTIONAL_LIGHT
		//directional light param
		ObjectProgram.setVec3("directionalLight.direction", -0.2f, -1.0f, -0.3f);

		ObjectProgram.setVec3("directionalLight.ambient", 0.05f, 0.05f, 0.05f);
		ObjectProgram.setVec3("directionalLight.diffuse", 0.4f, 0.4f, 0.4f);
		ObjectProgram.setVec3("directionalLight.specular", 0.5f, 0.5f, 0.5f);
#endif // !DISABLE_DIRECTIONAL_LIGHT

#ifndef DISABLE_POINT_LIGHT
		// point light 1
		ObjectProgram.setVec3("pointLights[0].position", pointLightPositions[0]);
		ObjectProgram.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		ObjectProgram.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		ObjectProgram.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		ObjectProgram.setFloat("pointLights[0].constant", 1.0f);
		ObjectProgram.setFloat("pointLights[0].linear", 0.09f);
		ObjectProgram.setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		ObjectProgram.setVec3("pointLights[1].position", pointLightPositions[1]);
		ObjectProgram.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		ObjectProgram.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		ObjectProgram.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		ObjectProgram.setFloat("pointLights[1].constant", 1.0f);
		ObjectProgram.setFloat("pointLights[1].linear", 0.09f);
		ObjectProgram.setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		ObjectProgram.setVec3("pointLights[2].position", pointLightPositions[2]);
		ObjectProgram.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		ObjectProgram.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		ObjectProgram.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		ObjectProgram.setFloat("pointLights[2].constant", 1.0f);
		ObjectProgram.setFloat("pointLights[2].linear", 0.09f);
		ObjectProgram.setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		ObjectProgram.setVec3("pointLights[3].position", pointLightPositions[3]);
		ObjectProgram.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		ObjectProgram.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		ObjectProgram.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		ObjectProgram.setFloat("pointLights[3].constant", 1.0f);
		ObjectProgram.setFloat("pointLights[3].linear", 0.09f);
		ObjectProgram.setFloat("pointLights[3].quadratic", 0.032f);
#endif // !DISABLE_POINT_LIGHT

#ifndef DISABLE_SPOT_LIGHT
		//spot light param
		if (g_EnableFlashLight)
		{
			ObjectProgram.setVec3("spotLight.position", g_Camera.getPos());
			ObjectProgram.setVec3("spotLight.direction", g_Camera.getFront());
			ObjectProgram.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			ObjectProgram.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			ObjectProgram.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			ObjectProgram.setFloat("spotLight.constant", 1.0f);
			ObjectProgram.setFloat("spotLight.linear", 0.09);
			ObjectProgram.setFloat("spotLight.quadratic", 0.032);
			ObjectProgram.setFloat("spotLight.cutOffCos", glm::cos(glm::radians(12.5f)));
			ObjectProgram.setFloat("spotLight.outerCutOffCos", glm::cos(glm::radians(18.0f)));
		}
		else
		{
			ObjectProgram.setVec3("spotLight.position", g_Camera.getPos());
			ObjectProgram.setVec3("spotLight.direction", g_Camera.getFront());
			ObjectProgram.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			ObjectProgram.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
			ObjectProgram.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
			ObjectProgram.setFloat("spotLight.constant", 1.0f);
			ObjectProgram.setFloat("spotLight.linear", 0.09f);
			ObjectProgram.setFloat("spotLight.quadratic", 0.032f);
			ObjectProgram.setFloat("spotLight.cutOffCos", glm::cos(glm::radians(12.5f)));
			ObjectProgram.setFloat("spotLight.outerCutOffCos", glm::cos(glm::radians(18.0f)));
		}
#endif // !DISABLE_SPOT_LIGHT

		//material param
		//ObjectProgram.setInt("material.diffuse", 0);
		//ObjectProgram.setInt("material.specular", 1);
		ObjectProgram.setFloat("material.shininess", 32.0f);
		ObjectProgram.setVec3("worldCameraPos", g_Camera.getPos());

		ObjectProgram.use();
		model = glm::mat4(1);
		ourModel.Draw(ObjectProgram);



		//draw cubes
		shaderProgram.use();
		model = glm::mat4(1);
		shaderProgram.setMat4("model", model);
		shaderProgram.setMat4("view", view);
		shaderProgram.setMat4("projection", projection);
		shaderProgram.setFloat("outTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ironWoodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ironWoodSpecualrTexture);

		//draw cubes
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0x1, 0xff);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
		glStencilMask(0xff);
		glBindVertexArray(VAO[2]);
		for (int i = 0; i < (sizeof(cubePositions) / sizeof(vec3)); ++i)
		{
			model = mat4(1);
			model = glm::rotate(model, radians(float(20 * i)), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::translate(model, cubePositions[i]);
			shaderProgram.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//draw cubes outline
		glStencilFunc(GL_NOTEQUAL, 0x1, 0xff);
		glDisable(GL_DEPTH_TEST);
		SingleColorProgram.use();
		SingleColorProgram.setMat4("projection", projection);
		SingleColorProgram.setMat4("view", view);
		for (int i = 0; i < (sizeof(cubePositions) / sizeof(vec3)); ++i)
		{
			model = mat4(1);
			model = glm::rotate(model, radians(float(20 * i)), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::translate(model, cubePositions[i]);
			model = glm::scale(model, glm::vec3(1.1, 1.1, 1.1));
			SingleColorProgram.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
#endif

		//draw windows
		glDepthMask(GL_FALSE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		SingleTextureProgram.use();
		SingleTextureProgram.setMat4("projection", projection);
		SingleTextureProgram.setMat4("view", view);
		SingleTextureProgram.setFloat("texture1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, windowTexture);

		glBindVertexArray(singleTextureVAO);

		for (auto i = g_SortedWindowsPos.rbegin();i!= g_SortedWindowsPos.rend();++i)
		{
			model = glm::translate(glm::mat4(1), i->second);
			model = glm::scale(model, glm::vec3(3, 3, 3));
			SingleTextureProgram.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		


		resetRenderState();
		//swap back buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}