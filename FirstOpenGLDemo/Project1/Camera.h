#pragma once
class GLFWwindow;
class Camera
{
public:
	enum ENUM_MOVEDIRECTION
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

public:
	Camera();
	~Camera();

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xpos, double ypos);
	void move(ENUM_MOVEDIRECTION direction,float deltaTime);
	glm::mat4 GetWorldToViewMatrix();
	glm::mat4 GetProjectionMatrix();

	glm::vec3 getPos() {
		return m_CameraPos;
	}
private:
	bool m_Fly;
	float m_ScreenWidth;
	float m_ScreenHeight;
	float m_MovementSpeed;
	float m_MouseSensitivity;

	glm::vec3 m_CameraPos;
	glm::vec3 m_CameraTargetReverseDirection;
	glm::vec3 m_CameraRight;
	glm::vec3 m_CameraUp;
	glm::vec3 m_WorldUp;

	bool m_FirstMouse;
	double m_LastCursorPosX , m_LastCursorPosY;
	float m_Yaw, m_Pitch;
	float m_Fov,m_FovMax, m_FovMin;

	void updateCameraDirections();
};
