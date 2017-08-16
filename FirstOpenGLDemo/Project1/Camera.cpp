#include "CommonHeader.h"
#include "Camera.h"

using namespace glm;

Camera::Camera():
	m_Fly(true),
	m_ScreenWidth(800.f),
	m_ScreenHeight(600.f),
	m_MovementSpeed(2.5f),
	m_MouseSensitivity(0.2f),
	m_CameraPos(vec3(0.f,0.f,3.f)),
	m_CameraTargetReverseDirection(vec3(0.0f, 0.0f, -4.0f)),
	m_WorldUp(vec3(0.0f, 1.0f, 0.0f)),
	m_FirstMouse(true),
	m_LastCursorPosX(0.f),
	m_LastCursorPosY(0.f),
	m_Pitch(0.f),
	m_Yaw(-90.f),
	m_Fov(45.f),
	m_FovMax(100.f),
	m_FovMin(1.f)
{
	updateCameraDirections();
}

Camera::~Camera()
{
}

void Camera::move(ENUM_MOVEDIRECTION direction, float deltaTime)
{
	vec3 targetDirectionVector;
	switch (direction)
	{
	case ENUM_MOVEDIRECTION::FORWARD:
	{
		targetDirectionVector = m_CameraTargetReverseDirection;
		break;
	}
	case ENUM_MOVEDIRECTION::BACKWARD:
	{
		targetDirectionVector = -m_CameraTargetReverseDirection;
		break;
	}
	case ENUM_MOVEDIRECTION::LEFT:
	{
		targetDirectionVector = -m_CameraRight;
		break;
	}
	case ENUM_MOVEDIRECTION::RIGHT:
	{
		targetDirectionVector = m_CameraRight;
		break;
	}
	case ENUM_MOVEDIRECTION::UP:
	{
		targetDirectionVector = m_CameraUp;
		break;
	}
	case ENUM_MOVEDIRECTION::DOWN:
	{
		targetDirectionVector = -m_CameraUp;
		break;
	}
	default:
		assert(false);

	}
	m_CameraPos += targetDirectionVector * m_MovementSpeed * deltaTime;
	if (!m_Fly)m_CameraPos.y = 0.f;
}

mat4 Camera::GetProjectionMatrix()
{
	return perspective(radians(m_Fov), m_ScreenWidth/m_ScreenHeight, 0.1f, 100.f);
}

void Camera::updateCameraDirections()
{
	m_CameraRight = normalize(cross(m_CameraTargetReverseDirection, m_WorldUp));
	m_CameraUp = normalize(cross(m_CameraRight, m_CameraTargetReverseDirection));
}

glm::mat4 CaculateLookAtMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
	glm::vec3 cameraDirection = glm::normalize(position - target);
	glm::vec3 cameraRight = glm::normalize(glm::cross(glm::normalize(worldUp), cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	// In glm we access elements as mat[col][row] due to column-major layout
	glm::mat4 rotate;
	rotate[0][0] = cameraRight.x;
	rotate[1][0] = cameraRight.y;
	rotate[2][0] = cameraRight.z;
	rotate[3][0] = 0.f;
	rotate[0][1] = cameraUp.x;
	rotate[1][1] = cameraUp.y;
	rotate[2][1] = cameraUp.z;
	rotate[3][1] = 0.f;
	rotate[0][2] = cameraDirection.x;
	rotate[1][2] = cameraDirection.y;
	rotate[2][2] = cameraDirection.z;
	rotate[3][2] = 0.f;
	rotate[0][3] = 0.f;
	rotate[1][3] = 0.f;
	rotate[2][3] = 0.f;
	rotate[3][3] = 1.f;

	glm::mat4 translate(1);	
	translate[3][0] = -position.x;
	translate[3][1] = -position.y;
	translate[3][2] = -position.z;

	//glm::mat4 rotate = mat4(
	//	glm::vec4(cameraRight, 0.f),
	//	glm::vec4(cameraUp, 0.f),
	//	glm::vec4(cameraDirection, 0.f),
	//	glm::vec4(0, 0, 0, 1)
	//);

	//view *= mat4(
	//	vec4(1, 0, 0, position.x),
	//	vec4(0, 1, 0, position.y),
	//	vec4(0, 0, 1, position.z),
	//	vec4(0, 0, 0, 1)
	//);
	return rotate * translate;
}

mat4 Camera::GetWorldToViewMatrix()
{
#if 1
	return lookAt(m_CameraPos, m_CameraPos + m_CameraTargetReverseDirection, m_WorldUp);

#else
	return CaculateLookAtMatrix(m_CameraPos, m_CameraPos + m_CameraTargetReverseDirection, m_WorldUp);
#endif
}


void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (m_FirstMouse)
	{
		m_LastCursorPosX = xpos;
		m_LastCursorPosY = ypos;
		m_FirstMouse = false;
	}
	float xoffset = xpos - m_LastCursorPosX;
	float yoffset = ypos - m_LastCursorPosY;
	m_LastCursorPosX = xpos;
	m_LastCursorPosY = ypos;

	float sensitivity = 0.2f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_Yaw += xoffset;
	m_Pitch -= yoffset;

	if (m_Pitch > 89.f)
	{
		m_Pitch = 89.f;
	}
	else if (m_Pitch<-89)
	{
		m_Pitch = -89;
	}
	vec3 front;
	front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
	front.y = sin(glm::radians(m_Pitch));
	front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
	m_CameraTargetReverseDirection = glm::normalize(front);

	updateCameraDirections();
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	m_Fov -= yoffset;
	if (m_Fov > m_FovMax)
	{
		m_Fov = m_FovMax;
	}
	else if (m_Fov < m_FovMin)
	{
		m_Fov = m_FovMin;
	}
}