#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//定义相机的移动
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const GLfloat YAW = -90.0f; //左右转动角
const GLfloat PITCH = -30.0f;//上下转动角
const GLfloat SPEED = 8.0f;//移动速度
const GLfloat SENSITIVTY = 0.25f;//鼠标灵敏度
const GLfloat ZOOM = 45.0f;//缩放

class Camera
{
public:

	glm::vec3 Position;//摄像机位置
	glm::vec3 Front;//摄像机方向
	glm::vec3 Up; //上轴
	glm::vec3 Right; //右轴
	glm::vec3 WorldUp;//世界上轴

	GLfloat Yaw;//左右转动角
	GLfloat Pitch;//上下转动角

	GLfloat MovementSpeed;//移动速度
	GLfloat MouseSensitivity;//鼠标灵敏度
	GLfloat Zoom;//缩放

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

	//计算摄相机观察矩阵
	glm::mat4 GetViewMatrix();

	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	//计算鼠标滑轮的输入
	void ProcessMouseScroll(GLfloat yoffset);

	//计算相机的三个向量
	void updateCameraVectors();
};

#endif;
