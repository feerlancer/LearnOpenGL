#include"CommonHeader.h"
#include "main.h"
#include "Shader.h"
#include"Camera.h"
#include "Factory.h"
#include <vector>
#include<map>
#include"Model.h"
#include"SkyBox.h"
#include"RawData.h"
#include"Utility.h"
#include "SimpleModel.h"
#include"FBOCreater.h"
#include"VAOCreater.h"
#include"UniformBlockDataStruct.h"
//“∆Ñ”ŒÔÛw 1rotate 2 scale 3 translate
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include"Singleton.h"
using namespace glm;
using namespace std;
using namespace RenderRawData;

//#define DISABLE_DIRECTIONAL_LIGHT
//#define DISABLE_POINT_LIGHT
//#define DISABLE_SPOT_LIGHT

//#define RENDER_TO_TEXTURE

#define ENABLE_MSAA
#ifdef ENABLE_MSAA
#	define SELF_RESLOVE_MSAA
#endif

#define MSAA_SAMPLE_NUM 4


float mixValue = 0.2;
#define screenWidth  800
#define screenHeight 600

#define shadowMapWidth 1024
#define shadowMapHeight 1024

float lastFrameTime = 0;
float deltaTime = 0;

Camera g_Camera;

bool g_EnableFlashLight = false;
bool g_FlashLightKeyPressed = false;

bool g_BlingKeyPressed = false;
bool g_EnableBling = false;

bool g_GammaCorrectKeyPressed = false;
bool g_EnableGammaCorrect = false;

#define VAO_NUMBER 3
#define NR_POINT_LIGHT 4

const int g_AsteroidNum = 2000;
glm::mat4 g_AsteroidModel[g_AsteroidNum];

//VAOS
unsigned int g_LampVAO,g_CubeVAO,g_SingleTextureVAO, g_ScreenVAO;

//UBOS
unsigned int g_Matrix_UBO
,g_Common_USE_UBO
,g_Light_Info_UBO
, g_PBR_Light_Info_UBO
;

//shaders
Shader 
g_ShaderProgram,
g_SingleColorProgram,
g_SingleTextureProgram,
g_LampProgram,
g_MultiLightingProgram,
g_MultiLightingPointShadowProgram,
g_ScreenProgram,
g_OutputTextureProgram,
g_OutputTextureRRRProgram,
g_SkyboxProgram,
g_EnvironmentMappingProgram, 
g_ModelProgram,
g_ResloveMSAAProgram,
g_GenShadowMapProgram,
g_GenPointShadowMapProgram,
g_PBRProgram
;


mat4 g_Model, g_View, g_Projection;

//textures
unsigned int 
g_IronWoodTexture
, g_IronWoodSpecualrTexture
, g_WindowTexture
, g_IronWoodTextureGammaCorrected
;

//Model
Model g_OurModel,g_ModelAsteroid;
SimpleModel g_ModelFloor;
//SkyBox
Skybox g_SkyBox;

struct UniformBlockData_MVPMatries
{
	glm::mat4 projection;
	glm::mat4 view;
};

UniformBlockData_MVPMatries g_ubo_Matirx_data;
UBO_BLOCK_1 g_uboBlock1_data;
UBO_BLOCK_LIGHT_INFO g_ubo_block_light_info_data;
UBO_BLOCK_PBR_LIGHT_INFO g_ubo_block_PBR_light_info_data;
// transparent window locations
// --------------------------------

void resetRenderState();


const int aNum = 100;
int unsorted[aNum], unsorted2[aNum], unsorted3[aNum];
void test()
{
	std::srand(glfwGetTime());
	for (int i = 0; i < aNum; ++i)
	{
		//unsorted2[i] = unsorted[i] = aNum-i;
		unsorted3[i]=unsorted2[i] = unsorted[i] = rand();
	}

	//quickSortMid(unsorted2, 0, sizeof(unsorted2) / sizeof(int)-1);
	//cout << "Mid: " << countQuickSortMid << endl;

	//quickSort(unsorted, 0, sizeof(unsorted) / sizeof(int) - 1);
	//cout << "Normal: " << countQuickSort << endl;

	//quickSort(unsorted3, 0, sizeof(unsorted3) / sizeof(int) - 1);
	//cout << "Tail: " << countQuickSortTail << endl;
	//for (int i : unsorted3)
	//{
	//	cout << i << " ";
	//}

}

void processInput(GLFWwindow* window)
{
	//Flash light
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		if (!g_FlashLightKeyPressed)
		{
			g_EnableFlashLight = !g_EnableFlashLight;
			g_FlashLightKeyPressed = true;
		}
	}
	else
	{
		g_FlashLightKeyPressed = false;
	}
	//Gamma correction
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		if (!g_GammaCorrectKeyPressed)
		{
			g_EnableGammaCorrect = !g_EnableGammaCorrect;
			g_GammaCorrectKeyPressed = true;
		}
	}
	else
	{
		g_GammaCorrectKeyPressed = false;
	}

	//Bling-phone
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS )
	{
		if (!g_BlingKeyPressed)
		{
			g_EnableBling = !g_EnableBling;
			g_BlingKeyPressed = true;
		}
	}
	else
	{
		g_BlingKeyPressed = false;
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

void loadShaders()
{
	g_ShaderProgram = Shader("ShaderCode/FirstDemoVS.glsl", "ShaderCode/FirstDemoPS.glsl");
	g_SingleColorProgram = Shader("ShaderCode/SingleColorVS.glsl", "ShaderCode/SingleColorPS.glsl");
	g_SingleTextureProgram = Shader("ShaderCode/SingleTextureVS.glsl", "ShaderCode/SingleTexturePS.glsl");
	g_LampProgram = Shader("ShaderCode/LocalToClipVS.glsl", "ShaderCode/LampPS.glsl");
	//g_MultiLightingProgram = Shader("ShaderCode/MultiLightingVS.glsl", "ShaderCode/MultiLightingPS.glsl");
	g_MultiLightingPointShadowProgram = Shader("ShaderCode/MultiLightingPointShadowMapVS.glsl", "ShaderCode/MultiLightingPointShadowMapPS.glsl");
	//g_MultiLightingProgram = Shader("ShaderCode/MultiLightingVS.glsl", "ShaderCode/MultiLightingPS.glsl","ShaderCode/ExplodeGS.glsl");
	//g_MultiLightingProgram = Shader("ShaderCode/MultiLightingVS.glsl", "ShaderCode/MultiLightingPS.glsl", "ShaderCode/VisiualizeNormalGS.glsl");
	//g_ScreenProgram = Shader("ShaderCode/PassThroughVS.glsl", "ShaderCode/SingleTexturePS.glsl");
	g_ScreenProgram = Shader("ShaderCode/PassThroughVS.glsl", "ShaderCode/BlurPS.glsl");
	g_OutputTextureProgram = Shader("ShaderCode/PassThroughVS.glsl", "ShaderCode/OutputTexturePS.glsl");
	g_OutputTextureRRRProgram = Shader("ShaderCode/PassThroughVS.glsl", "ShaderCode/OutputTextureXPS.glsl");
	g_ResloveMSAAProgram = Shader("ShaderCode/PassThroughVS.glsl", "ShaderCode/ResloveMSAAPS.glsl");
	g_SkyboxProgram = Shader("ShaderCode/SkyboxVS.glsl", "ShaderCode/SkyboxPS.glsl");
	g_EnvironmentMappingProgram = Shader("ShaderCode/TransformPosAndNormalVS.glsl", "ShaderCode/EnvieromentMappingPS.glsl");
	g_ModelProgram = Shader("ShaderCode/ModelVS.glsl", "ShaderCode/ModelPS.glsl");
	g_GenShadowMapProgram = Shader("ShaderCode/GenShadowMapVS.glsl", "ShaderCode/GenShadowMapPS.glsl");
	g_GenPointShadowMapProgram = Shader("ShaderCode/GenPointShadowMapVS.glsl", "ShaderCode/GenPointShadowMapPS.glsl", "ShaderCode/GenPointShadowMapGS.glsl");
	g_PBRProgram = Shader("ShaderCode/PBRVS.glsl", "ShaderCode/PBRPS.glsl");
}

void updateShaderUniforms()
{
	//material param
	//g_MultiLightingProgram.use();
	//g_MultiLightingProgram.setFloat("material.shininess", 32.0f);
	//g_MultiLightingProgram.setInt("material.texture_diffuse0", 0);
	//g_MultiLightingProgram.setInt("material.texture_specular0", 1);

//g_MultiLightingPointShadowProgram----------------------------------------------------------------------------------------------------
	g_MultiLightingPointShadowProgram.use();
	g_MultiLightingPointShadowProgram.setFloat("material.shininess", 32.0f);
	g_MultiLightingPointShadowProgram.setInt("material.texture_diffuse0", 0);
	g_MultiLightingPointShadowProgram.setInt("material.texture_specular0", 1);


//GenPointShadowMapProgram -----------------------------------------------------------------------------------------------
	g_GenPointShadowMapProgram.use();
	const float FarPlane = 25.f;//TODO
	glm::mat4 LightProjection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, FarPlane);
	glm::mat4 LightView[6];
	LightView[0]= glm::lookAt(g_pointLightPositions[0], g_pointLightPositions[0] + glm::vec3(1,0,0), glm::vec3(0, -1, 0));
	LightView[1] = glm::lookAt(g_pointLightPositions[0], g_pointLightPositions[0] + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
	LightView[2] = glm::lookAt(g_pointLightPositions[0], g_pointLightPositions[0] + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
	LightView[3] = glm::lookAt(g_pointLightPositions[0], g_pointLightPositions[0] + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
	LightView[4] = glm::lookAt(g_pointLightPositions[0], g_pointLightPositions[0] + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0));
	LightView[5] = glm::lookAt(g_pointLightPositions[0], g_pointLightPositions[0] + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
	for (int i = 0; i < 6; ++i)
	{
		g_GenPointShadowMapProgram.setMat4(("shadowVPMatrices[" + std::to_string(i) + "]").c_str(), LightProjection*LightView[i]);
	}
	g_GenPointShadowMapProgram.setVec3("pointlightWorldPos", g_pointLightPositions[0]);
	g_GenPointShadowMapProgram.setFloat("pointlightViewFarPlane", FarPlane);

}

void prepareVAOs()
{
	glGenVertexArrays(1, &g_LampVAO);
	glGenVertexArrays(1, &g_CubeVAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);
	unsigned int PosAndNormalVBO;
	glGenBuffers(1, &PosAndNormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, PosAndNormalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_verticesPosAndNormal), g_verticesPosAndNormal, GL_STATIC_DRAW);

	//Light VAO
	glBindVertexArray(g_LampVAO);
	glBindBuffer(GL_ARRAY_BUFFER, PosAndNormalVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	//Cube VAO
	glBindVertexArray(g_CubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, PosAndNormalVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
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

void drawLamps()
{
	resetRenderState();
	//Lamps_____________________________________________________________________________
	g_LampProgram.use();
	glBindVertexArray(g_LampVAO);
	//TODO
	for (int i = 0; i < 1; ++i)
	{
		glm::vec3 iPos = g_pointLightPositions[i];
		glm::mat4 model(1);
		model = glm::translate(model, iPos);
		model = glm::scale(model, glm::vec3(0.2f));
		g_LampProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void drawSkyBox()
{
	resetRenderState();
	//skybox
	glDepthFunc(GL_LEQUAL);
	g_SkyboxProgram.use();
	g_SkyboxProgram.setMat4("projection", g_Projection);
	g_SkyboxProgram.setMat4("view", glm::mat4(glm::mat3(g_View)));

	g_SkyBox.Draw(g_SkyboxProgram);
}

void drawCubes(Shader& a_Shader)
{
	resetRenderState();
	//cube_____________________________________________________________________________
	a_Shader.use();
	//a_Shader.setFloat("deltaTime", glfwGetTime());
	//g_OurModel.Draw(g_MultiLightingProgram);

	//enviromentreflection shader
	//g_EnvironmentMappingProgram.use();
	//g_EnvironmentMappingProgram.setVec3("worldCameraPos", g_Camera.getPos());
	//g_EnvironmentMappingProgram.setInt("cubeTexture", 0);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, g_SkyBox.GetCubeTextureID());

	//g_ShaderProgram.use();
	//g_ShaderProgram.setFloat("outTexture", 0);
	glActiveTexture(GL_TEXTURE0);
	if (g_EnableGammaCorrect)
	{
		glBindTexture(GL_TEXTURE_2D, g_IronWoodTextureGammaCorrected);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, g_IronWoodTexture);
	}
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_IronWoodSpecualrTexture);
	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_ALWAYS, 0x1, 0xff);
	//glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	//glStencilMask(0xff);
	glBindVertexArray(g_CubeVAO);
	for (int i = 0; i < (sizeof(g_cubePositions) / sizeof(vec3)); ++i)
	{
		glm::mat4 model(1);
		model = glm::translate(model, g_cubePositions[i]);
		model = glm::rotate(model, radians(float(20 * i)), glm::vec3(1.0f, 0.3f, 0.5f));
		a_Shader.setMat4("model", model);
		//g_EnvironmentMappingProgram.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	//draw cubes outline
	//glDepthMask(GL_TRUE);
	//glStencilFunc(GL_NOTEQUAL, 0x1, 0xff);
	//g_SingleColorProgram.use();
	//for (int i = 0; i < (sizeof(g_cubePositions) / sizeof(vec3)); ++i)
	//{
	//	glm::mat4 model(1);
	//	model = glm::translate(model, g_cubePositions[i]);
	//	model = glm::scale(model, glm::vec3(1.1, 1.1, 1.1));
	//	model = glm::rotate(model, radians(float(20 * i)), glm::vec3(1.0f, 0.3f, 0.5f));
	//	g_SingleColorProgram.setMat4("model", model);
	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//}
}

void initAsteroidFieldPos()
{
	const float raduis = 150.f;
	const float offset = 25.f;
	glm::mat4 model;
	for (int i = 0; i < g_AsteroidNum; ++i)
	{

		model = mat4(1);
		//translate
		glm::vec3 translate;
		float angle = i / (float)g_AsteroidNum * 360;
		translate.x = raduis * sin(angle) + (rand() % (int)(2 * offset * 100)) / 100 - offset;
		translate.y = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		translate.y *= 0.4;
		translate.z = raduis * cos(angle) + (rand() % (int)(2 * offset * 100)) / 100 - offset;
		model = glm::translate(model, translate);

		//scale
		float scale = rand() % 20 / 100 + 0.5;
		model = glm::scale(model, glm::vec3(scale));

		//rotate
		model = glm::rotate(model, (float)(rand() % 360), glm::vec3(0.4, 0.7, 0.3));

		g_AsteroidModel[i] = model;
	}

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	int asteroidModelMatrixArraySize = g_AsteroidNum * sizeof(glm::mat4);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, asteroidModelMatrixArraySize, g_AsteroidModel, GL_STATIC_DRAW);

	GLsizei vec4Size = sizeof(glm::vec4);
	for (auto i = g_ModelAsteroid.meshes.begin(); i != g_ModelAsteroid.meshes.end(); ++i)
	{
		int sizeofVerticesArray = i->m_Vertices.size() * sizeof(Vertex);
		glBindVertexArray(i->GetVAO());

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}

void drawAsteroidField()
{
	resetRenderState();
	g_ModelProgram.use();
	//for (int i = 0; i < g_AsteroidNum; ++i)
	//{
	//	g_ModelProgram.setMat4("model", g_AsteroidModel[i]);
	//	g_ModelAsteroid.Draw(g_ModelProgram);
	//}
	g_ModelProgram.setInt("material.texture_diffuse0", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_ModelAsteroid.meshes[0].m_Textures[0].id);
	//for (unsigned int i = 0; i < g_ModelAsteroid.meshes.size(); i++)
	for (auto iter:g_ModelAsteroid.meshes)
	{
		glBindVertexArray(iter.GetVAO());
		glDrawElementsInstanced(GL_TRIANGLES, iter.m_Indices.size(), GL_UNSIGNED_INT, 0, g_AsteroidNum);
	}
}
void drawWindows()
{
	//draw windows
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glDisable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	g_SingleTextureProgram.use();
	g_SingleTextureProgram.setInt("texture1", 0);
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
void drawFloor(Shader& shader)
{
	resetRenderState();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_IronWoodTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_IronWoodSpecualrTexture);

	shader.use();
	mat4 model = glm::translate(mat4(1), glm::vec3(0,-4,0));
	shader.setMat4("model", model);
	g_ModelFloor.Draw(shader);

	model = glm::translate(mat4(1), glm::vec3(5, 0, 0));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
	shader.setMat4("model", model);
	g_ModelFloor.Draw(shader);
}
void drawScene()
{
	//draw opacity
	drawLamps();
	//drawCubes();
	drawSkyBox();
	drawAsteroidField();

	//draw semi-transparent
	//semi-transparent object must be render at last, as it enable z-test, but disable written z-buffer, 
	//drawWindows();
}

unsigned int createAndBindUBO(unsigned int bingdingPoint, unsigned int sizeInByte = 256)
{
	unsigned int UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeInByte, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, bingdingPoint, UBO);
	return UBO;
}

unsigned int updateUBO(unsigned int UBO, void * data, unsigned int sizeInByte, int offsetInByte=0)
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, offsetInByte, sizeInByte, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return UBO;
}
void drawTextureToScreen(unsigned int a_textureColorBuffer,Shader &a_Shader = g_OutputTextureProgram)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClearColor(0.2f, 0.3f, .3f, 1.f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindVertexArray(g_ScreenVAO);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	a_Shader.use();
	a_Shader.setInt("texture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, a_textureColorBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void drawSceneTextureToDefaultColorBufferWithReslove(unsigned int a_textureColorBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.3f, .3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindVertexArray(g_ScreenVAO);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	g_ResloveMSAAProgram.use();
	g_ResloveMSAAProgram.setInt("texture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, a_textureColorBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void updateAllUBOs()
{
	//ubdate UBOs
	g_ubo_Matirx_data.projection = g_Projection;
	g_ubo_Matirx_data.view = g_View;
	updateUBO(g_Matrix_UBO, &g_ubo_Matirx_data, sizeof(g_ubo_Matirx_data));

	g_uboBlock1_data.Bling = g_EnableBling;
	g_uboBlock1_data.GammaCorrect = g_EnableGammaCorrect;
	g_uboBlock1_data.worldCameraPos = g_Camera.getPos();
	updateUBO(g_Common_USE_UBO, &g_uboBlock1_data, sizeof(g_uboBlock1_data));


#ifndef DISABLE_DIRECTIONAL_LIGHT
	//directional light param
	g_ubo_block_light_info_data.dirlight.direction = g_directionLightDir;
	g_ubo_block_light_info_data.dirlight.ambient=glm::vec3(0.05f, 0.05f, 0.05f);
	g_ubo_block_light_info_data.dirlight.diffuse=glm::vec3(0.4f, 0.4f, 0.4f);
	g_ubo_block_light_info_data.dirlight.specular=glm::vec3(0.5f, 0.5f, 0.5f);

	//g_ubo_block_light_info_data.dirlight.ambient=glm::vec3(0.1f, 0.1f, 0.1f);
	//g_ubo_block_light_info_data.dirlight.diffuse=glm::vec3(1.f, 1.f, 1.f);
	//g_ubo_block_light_info_data.dirlight.specular=glm::vec3(1.f, 1.f, 1.f);
#endif // !DISABLE_DIRECTIONAL_LIGHT

#ifndef DISABLE_POINT_LIGHT
	// point light 1
	g_ubo_block_light_info_data.pointLights[0].position=glm::vec3(g_pointLightPositions[0]);
	g_ubo_block_light_info_data.pointLights[0].ambient=glm::vec3(0.05f, 0.05f, 0.05f);
	g_ubo_block_light_info_data.pointLights[0].diffuse=glm::vec3(0.8f, 0.8f, 0.8f);
	g_ubo_block_light_info_data.pointLights[0].specular=glm::vec3(1.0f, 1.0f, 1.0f);
	g_ubo_block_light_info_data.pointLights[0].constant= 1.0f;
	g_ubo_block_light_info_data.pointLights[0].linear= 0.09f;
	g_ubo_block_light_info_data.pointLights[0].quadratic= 0.032f;
	g_ubo_block_light_info_data.pointLights[0].far_plane = 25.f;

	g_ubo_block_light_info_data.pointLights[0].position = glm::vec3(g_pointLightPositions[0]);
	g_ubo_block_light_info_data.pointLights[0].ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	g_ubo_block_light_info_data.pointLights[0].diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	g_ubo_block_light_info_data.pointLights[0].specular = glm::vec3(1.0f, 1.0f, 1.0f);
	g_ubo_block_light_info_data.pointLights[0].constant = 1.0f;
	g_ubo_block_light_info_data.pointLights[0].linear = 0.0001f;
	g_ubo_block_light_info_data.pointLights[0].quadratic = 0.0001f;
	// point light 2
	g_ubo_block_light_info_data.pointLights[1].position=glm::vec3(g_pointLightPositions[1]);
	g_ubo_block_light_info_data.pointLights[1].ambient=glm::vec3(0.05f, 0.05f, 0.05f);
	g_ubo_block_light_info_data.pointLights[1].diffuse=glm::vec3(0.8f, 0.8f, 0.8f);
	g_ubo_block_light_info_data.pointLights[1].specular=glm::vec3(1.0f, 1.0f, 1.0f);
	g_ubo_block_light_info_data.pointLights[1].constant= 1.0f;
	g_ubo_block_light_info_data.pointLights[1].linear= 0.09f;
	g_ubo_block_light_info_data.pointLights[1].quadratic= 0.032f;
	// point light 3
	g_ubo_block_light_info_data.pointLights[2].position=g_pointLightPositions[2];
	g_ubo_block_light_info_data.pointLights[2].ambient=glm::vec3(0.05f, 0.05f, 0.05f);
	g_ubo_block_light_info_data.pointLights[2].diffuse=glm::vec3(0.8f, 0.8f, 0.8f);
	g_ubo_block_light_info_data.pointLights[2].specular=glm::vec3(1.0f, 1.0f, 1.0f);
	g_ubo_block_light_info_data.pointLights[2].constant= 1.0f;
	g_ubo_block_light_info_data.pointLights[2].linear= 0.09f;
	g_ubo_block_light_info_data.pointLights[2].quadratic= 0.032f;
	// point light 4
	g_ubo_block_light_info_data.pointLights[3].position=g_pointLightPositions[3];
	g_ubo_block_light_info_data.pointLights[3].ambient=glm::vec3(0.05f, 0.05f, 0.05f);
	g_ubo_block_light_info_data.pointLights[3].diffuse=glm::vec3(0.8f, 0.8f, 0.8f);
	g_ubo_block_light_info_data.pointLights[3].specular=glm::vec3(1.0f, 1.0f, 1.0f);
	g_ubo_block_light_info_data.pointLights[3].constant= 1.0f;
	g_ubo_block_light_info_data.pointLights[3].linear= 0.09f;
	g_ubo_block_light_info_data.pointLights[3].quadratic= 0.032f;
#endif // !DISABLE_POINT_LIGHT

#ifndef DISABLE_SPOT_LIGHT
	//spot light param
	if (g_EnableFlashLight)
	{
		g_ubo_block_light_info_data.spotLight.position=g_Camera.getPos();
		g_ubo_block_light_info_data.spotLight.direction=g_Camera.getFront();
		g_ubo_block_light_info_data.spotLight.ambient=glm::vec3(0.0f, 0.0f, 0.0f);
		g_ubo_block_light_info_data.spotLight.diffuse=glm::vec3(1.0f, 1.0f, 1.0f);
		g_ubo_block_light_info_data.spotLight.specular=glm::vec3(1.0f, 1.0f, 1.0f);
		g_ubo_block_light_info_data.spotLight.constant= 1.0f;
		g_ubo_block_light_info_data.spotLight.linear= 0.09;
		g_ubo_block_light_info_data.spotLight.quadratic= 0.032;
		g_ubo_block_light_info_data.spotLight.cutOffCos= glm::cos(glm::radians(12.5f));
		g_ubo_block_light_info_data.spotLight.outerCutOffCos= glm::cos(glm::radians(18.0f));
	}
	else
	{
		g_ubo_block_light_info_data.spotLight.position=g_Camera.getPos();
		g_ubo_block_light_info_data.spotLight.direction=g_Camera.getFront();
		g_ubo_block_light_info_data.spotLight.ambient=glm::vec3(0.0f, 0.0f, 0.0f);
		g_ubo_block_light_info_data.spotLight.diffuse=glm::vec3(0.0f, 0.0f, 0.0f);
		g_ubo_block_light_info_data.spotLight.specular=glm::vec3(0.0f, 0.0f, 0.0f);
		g_ubo_block_light_info_data.spotLight.constant= 1.0f;
		g_ubo_block_light_info_data.spotLight.linear= 0.09f;
		g_ubo_block_light_info_data.spotLight.quadratic= 0.032f;
		g_ubo_block_light_info_data.spotLight.cutOffCos= glm::cos(glm::radians(12.5f));
		g_ubo_block_light_info_data.spotLight.outerCutOffCos= glm::cos(glm::radians(18.0f));
	}
#endif // !DISABLE_SPOT_LIGHT

	float near_plane = 1.0f, far_plane = 50.f;
	g_ubo_block_light_info_data.dirlight.projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	g_ubo_block_light_info_data.dirlight.view = glm::lookAt(-g_directionLightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	updateUBO(g_Light_Info_UBO, &g_ubo_block_light_info_data, sizeof(g_ubo_block_light_info_data));

	//PBR light info
	for (int i = 0; i < 4; ++i)
	{
		g_ubo_block_PBR_light_info_data.pbr_pointLights[i].color = g_PBR_light_colors[i];
		g_ubo_block_PBR_light_info_data.pbr_pointLights[i].position = g_PBR_light_positions[i];
	}
	updateUBO(g_PBR_Light_Info_UBO, &g_ubo_block_PBR_light_info_data, sizeof(g_ubo_block_PBR_light_info_data));

}

unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
	if (sphereVAO == 0)
	{
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();

		std::vector<float> data;
		for (int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

void drawSpheres(Shader & a_shader)
{
	float nrRows = 7;
	float nrColumns = 7;
	float spacing = 2.5;

	glm::mat4 model;
	a_shader.use();
	a_shader.setVec3("albedo", 0.5f, 0.0f, 0.0f);
	for (unsigned int row = 0; row < nrRows; ++row)
	{
		a_shader.setFloat("metallic", ((float)row +1)/ (float)nrRows);
		for (unsigned int col = 0; col < nrColumns; ++col)
		{
			// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
			// on direct lighting.
			a_shader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(
				(float)(col - (nrColumns / 2)) * spacing,
				(float)(row - (nrRows / 2)) * spacing,
				0.0f
			));
			a_shader.setMat4("model", model);
			renderSphere();
		}
	}
}
void resetRenderState()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

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
#ifdef ENABLE_MSAA
	glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLE_NUM);
#endif
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
#ifdef ENABLE_MSAA
	glEnable(GL_MULTISAMPLE);
#endif
	glViewport(0, 0, screenWidth, screenHeight);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	loadShaders();

	prepareVAOs();
	g_SingleTextureVAO = VAOCreater::createSingleTextureVAO(g_QuadVerticesPosAndNormal,sizeof(g_QuadVerticesPosAndNormal), g_indices, sizeof(g_indices));
	g_ScreenVAO = VAOCreater::createScreenVAO(g_QuadVerticesPosAndTexCoords,sizeof(g_QuadVerticesPosAndTexCoords), g_indices, sizeof(g_indices));

	g_Matrix_UBO = createAndBindUBO(0);
	g_Common_USE_UBO = createAndBindUBO(1);
	g_Light_Info_UBO = createAndBindUBO(2,2048);
	g_PBR_Light_Info_UBO = createAndBindUBO(3, 2048);

	vec3 cameraTarget(0.f, 0.f, 0.f);
	vec3 up(0.f, 1.f, 0.f);
	vec3 cameraDirection;
	float radius = 10;

	g_IronWoodTexture = Utility::loadTextureFromFile("../../Resources/Textures/container2.png",false);
	g_IronWoodTextureGammaCorrected = Utility::loadTextureFromFile("../../Resources/Textures/container2.png",true);
	g_IronWoodSpecualrTexture = Utility::loadTextureFromFile("../../Resources/Textures/container2_specular.png");
	g_WindowTexture = Utility::loadTextureFromFile("../../Resources/Textures/blending_transparent_window.png");

	//g_OurModel = Model("../../Resources/objects/nanosuit/nanosuit.obj");
	g_ModelAsteroid = Model("../../Resources/objects/rock/rock.obj");
	//SimpleModel(vector<unsigned int> vertexDataFormat, void* vertexData, unsigned int vertexDataSize, const char* textureFilePath);
	vector<unsigned int> a({ 3,3,2 });
	g_ModelFloor = SimpleModel(a, g_FloorPosNormalTexcoords,sizeof(g_FloorPosNormalTexcoords), "../../Resources/Textures/container2.png");

	initAsteroidFieldPos();
	const char* cubeMapFaces[] =
	{
		"../../Resources/Textures/skybox/right.jpg",
		"../../Resources/Textures/skybox/left.jpg",
		"../../Resources/Textures/skybox/top.jpg",
		"../../Resources/Textures/skybox/bottom.jpg",
		"../../Resources/Textures/skybox/back.jpg",
		"../../Resources/Textures/skybox/front.jpg",
	};
	g_SkyBox = Skybox(cubeMapFaces);
	unsigned int FBO, textureColorBuffer;
	FBOCreater::createFBO(FBO, textureColorBuffer);
	unsigned int multisampleFBO, multisampleTextureColorBuffer;
	FBOCreater::createMultisampleFBO(multisampleFBO, multisampleTextureColorBuffer);
	unsigned int shadowMapFBO, shadowMapTexture;
	FBOCreater::createShadowMapFBO(shadowMapFBO, shadowMapTexture, shadowMapWidth, shadowMapHeight);
	unsigned int pointShadowMapFBO, pointShadowMapCubeTexture;
	FBOCreater::createPointShadowMapFBO(pointShadowMapFBO, pointShadowMapCubeTexture, shadowMapWidth, shadowMapHeight);

	//bindtexture
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_CUBE_MAP, pointShadowMapCubeTexture);


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

		updateAllUBOs();//must after g_Projection updated

		updateShaderUniforms();
		//render to texture
#ifdef RENDER_TO_TEXTUR
#	ifdef ENABLE_MSAA
		glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);
#	else
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
#	endif
#else

#endif
#ifdef DIR_SHADOW_MAP
		//draw shadow map
		glViewport(0, 0, shadowMapWidth, shadowMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		drawCubes(g_GenShadowMapProgram);
		drawFloor(g_GenShadowMapProgram);

		//normal draw
		glViewport(0, 0, screenWidth, screenHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.5f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		drawCubes(g_MultiLightingProgram);
		drawFloor(g_MultiLightingProgram);
		//debug shadow map
		glViewport(0, 0, screenWidth / 2, screenHeight / 2);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		drawTextureToScreen(shadowMapTexture, g_OutputTextureRRRProgram);
#endif

#ifdef draw_shadow
		//draw point shadow map
		glViewport(0, 0, shadowMapWidth, shadowMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, pointShadowMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		drawCubes(g_GenPointShadowMapProgram);
		drawFloor(g_GenPointShadowMapProgram);

		//normal draw
		glViewport(0, 0, screenWidth, screenHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pointShadowMapCubeTexture);
		drawCubes(g_MultiLightingPointShadowProgram);
		drawFloor(g_MultiLightingPointShadowProgram);
		drawLamps();
#endif
		////debug shadow map
		//glViewport(0, 0, screenWidth / 2, screenHeight / 2);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//drawTextureToScreen(shadowMapTexture, g_OutputTextureRRRProgram);


		glClearColor(0.2f, 0.5f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		drawSpheres(g_PBRProgram);



#ifdef RENDER_TO_TEXTURE		//draw the texture
#	ifdef ENABLE_MSAA
#		ifdef SELF_RESLOVE_MSAA
		drawSceneTextureToDefaultColorBufferWithReslove(multisampleTextureColorBuffer);
#		else
		glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
		//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		drawTextureToScreen(textureColorBuffer);
#		endif

#	else
		drawTextureToScreen(textureColorBuffer);
#	endif

#endif


		//reset
		resetRenderState();

		//swap back buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}