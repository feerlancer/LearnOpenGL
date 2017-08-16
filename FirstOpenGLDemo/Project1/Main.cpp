#include"CommonHeader.h"
#include "main.h"
#include "Shader.h"
#include"Camera.h"
#include "Factory.h"
#include <vector>
#include<map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include"Singleton.h"
using namespace glm;
using namespace std;

float mixValue = 0.2;
#define screenWidth  800.f
#define screenHeight 600.f

float lastFrameTime = 0;
float deltaTime = 0;

Camera g_Camera;

#define VAO_NUMBER 3

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

unsigned int g_indices[6] =
{
	0,1,2, 0,2,3
};

void test()
{
	singleton::CreateInstance();
	singleton::GetInstance();
	singleton::DestroyInstance();
	vector<int> VA;
	VA.reserve(10 * sizeof(int));
	for (int i = 0; i < 10; ++i)
	{
		VA.push_back(i);
	}
	cout << "VA: "<<VA[3] << endl;
	vector<int>::iterator a = VA.begin();
	for (int i = 0; i < 1000; ++i)
	{
		//VA.push_back(53456);
	}

	for (; a != VA.end(); ++a)
	{

	}
	//cout << "a: " << *a << endl;


	map<const char*, int> amap;
	amap["nima"] = 3;
	amap["gsfd"] = 5;
	cout << amap["nima"] << endl;
	amap.insert(make_pair("guo", 8));
	auto it = amap.find("grte");
	if (it != amap.end()) {
		cout << "find " << it->second << endl;
	}
}

void processInput(GLFWwindow* window)
{
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

unsigned int loadTextureFromFile(const char* filePath, unsigned int textureUnit)
{
	unsigned int texture;

	int width, height, nrChannels;
	//unsigned char* data = stbi_load("../../Resources/Textures/woodContainer.jpg", &width, &height, &nrChannels, 0);
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
		glActiveTexture(GL_TEXTURE0+textureUnit);
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
	Shader LampProgram("ShaderCode/LocalToClipVS.glsl", "ShaderCode/LampPS.glsl");
	Shader ObjectProgram("ShaderCode/ObjectLightingVS.glsl", "ShaderCode/ObjectLightingPS.glsl");
	//Shader ObjectProgram("ShaderCode/ObjectLightingGourandVS.glsl", "ShaderCode/ObjectLightingGourandPS.glsl");
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

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	unsigned int VAO[VAO_NUMBER];
	prepareVAOs(VAO);


	mat4 model, view, projection;
	model = mat4(1);
	projection = perspective(radians(45.f), (screenWidth) / screenHeight, 0.1f, 100.f);

	vec3 cameraTarget(0.f, 0.f, 0.f);
	vec3 up(0.f, 1.f, 0.f);

	vec3 cameraDirection;

	float radius = 10;
	glEnable(GL_DEPTH_TEST);

	unsigned int ironWoodTexture = loadTextureFromFile("../../Resources/Textures/container2.png", 0);
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
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		view = g_Camera.GetWorldToViewMatrix();
		projection = g_Camera.GetProjectionMatrix();

#if 0
		//multi cubes scene
		shaderProgram.use();
		shaderProgram.setInt("outTexture", 0);
		shaderProgram.setInt("outTexture1", 1);
		shaderProgram.setMat4("projection", projection);
		shaderProgram.setMat4("view", view);

		//draw
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO[0]);
		int rCount = 0;
		for (int i = 0; i < (sizeof(cubePositions) / sizeof(vec3));++i)
		{
				
			model = glm::translate(mat4(1), cubePositions[i]);

			model = glm::rotate(model, radians(float(20*i)), glm::vec3(1.0f, 0.3f, 0.5f));

			shaderProgram.setMat4("model", model);

			shaderProgram.setFloat("mixValue", mixValue);

			glDrawArrays(GL_TRIANGLES, 0, 36);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
#else
		//light scene

		//Lamp_____________________________________________________________________________
		LampProgram.use();
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		LampProgram.setMat4("model", model);
		LampProgram.setMat4("projection", projection);
		LampProgram.setMat4("view", view);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//cube_____________________________________________________________________________
		ObjectProgram.use();
		model = glm::mat4(1);
		ObjectProgram.setMat4("model", model);
		ObjectProgram.setMat4("projection", projection);
		ObjectProgram.setMat4("view", view);

		ObjectProgram.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		ObjectProgram.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
		ObjectProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		ObjectProgram.setInt("material.diffuse", 0);
		ObjectProgram.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		ObjectProgram.setFloat("material.shininess", 32.0f);
		ObjectProgram.setVec3("lightWorldPos", lightPos);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ironWoodTexture);

		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
#endif

		//swap back buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}