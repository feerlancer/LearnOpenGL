#pragma once
class FBOCreater
{
public:
	FBOCreater();
	~FBOCreater();
	static void createFBO(unsigned int &out_FBO, unsigned int &out_ColorTexture, int a_screenWidth = 800, int a_screenHeight=600);
	static void createMultisampleFBO(unsigned int &out_FBO, unsigned int &out_ColorTexture,int multi = 4,int a_screenWidth = 800, int a_screenHeight = 600);
	static void createShadowMapFBO(unsigned int &out_FBO, unsigned int &out_DepthTexture, int a_screenWidth = 800, int a_screenHeight = 600);
	static void createPointShadowMapFBO(unsigned int & out_FBO, unsigned int & out_DepthTexture, int shadowMapWidth, int shadowMapHeight);

};

