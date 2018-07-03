
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint screenWidth = 800, screenHeight = 600;

// 回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLuint makeSkybox(GLuint *skyboxVAO);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
// Light attributes
glm::vec3 lightPos(1.0f, 8.0f, 12.0f);
GLfloat scale = 0.2;
/******** 小车主体 ********/
//body位置
glm::vec3 bodyPos = glm::vec3(0.0f, 0.0f, 0.0f);
GLfloat speed = 1.0f;
glm::vec3 dir = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//角度
GLfloat lastAngle = 0.0f;
GLfloat curAngle = lastAngle;
GLfloat rSpeed = 30;//转弯角速度

/*轮子*/
GLfloat wSpeed = 120;//轮子转动角速度
GLfloat wAngle = 0.0f;
GLfloat fAngle = 0.0;
GLfloat fSpeed = 0.5;

// 摄像机
Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
glm::mat4 view;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//加载声音
	//mciSendString(TEXT("open ./rec/下子.WAV alias mysong2"), NULL, 0, NULL);
	//mciSendString(TEXT("open ./rec/bgm.mp3 alias mysong1"), NULL, 0, NULL);
	//创建窗口
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	// 设置回调函数
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// 初始化glew
	glewExperimental = GL_TRUE;
	glewInit();
	// 视口
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// 着色器
	Shader shader("./src/texture_vertex.vs", "./src/directional_light.fs");
	Shader ground_shader("./src/texture_vertex.vs", "./src/shader.fs");
	Shader skybox_shader("./src/skybox.vs", "./src/skybox.fs");
	//skybox
	GLuint skyboxVAO;
	GLuint skyboxTexture = makeSkybox(&skyboxVAO);
	//加载模型
	Model ground("res/t.obj", ground_shader);
	//Model body("res/che.obj", shader);//"nanosuit/nanosuit.obj"
	Model body("res/body.obj", shader);
	Model wFL("res/wFL.obj", shader);
	Model wFR("res/wFR.obj", shader);
	Model wBL("res/wBL.obj", shader);
	Model wBR("res/wBR.obj", shader);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式画图元

	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		Do_Movement();

		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//使用着色器
		ground_shader.use();
		// 传递model，view，projection矩阵
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 1.0f, 100.0f);
		//view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(ground_shader.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(ground_shader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale));
		glUniformMatrix4fv(glGetUniformLocation(ground_shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
		ground.Draw();

		shader.use();
		//GLint lightPosLoc = glGetUniformLocation(shader.getProgram(), "light.position");
		GLint lightDirLoc = glGetUniformLocation(shader.getProgram(), "light.direction");
		GLint viewPosLoc = glGetUniformLocation(shader.getProgram(), "viewPos");
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(lightDirLoc, 0.2f, 1.0f, 2.f);
		// Set lights properties
		glUniform3f(glGetUniformLocation(shader.getProgram(), "light.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(shader.getProgram(), "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(shader.getProgram(), "light.specular"), 1.f, 1.f, 1.f);
		glUniform1f(glGetUniformLocation(shader.getProgram(), "material.shininess"), 32.0f);

		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));

		//body
		glm::mat4 bodyModel = glm::mat4();
		bodyModel = glm::translate(bodyModel, bodyPos);
		bodyModel = glm::rotate(bodyModel, glm::radians(curAngle), up);
		bodyModel = glm::scale(bodyModel, glm::vec3(scale));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(bodyModel));
		body.Draw();
		
		//后轮
		glm::mat4 wBackModel = bodyModel;
		//wBackModel = glm::translate(wBackModel, bodyPos);
		//wBackModel = glm::rotate(wBackModel, glm::radians(curAngle), up);
		//wBackModel = glm::scale(wBackModel, glm::vec3(scale));
		
		wBackModel = glm::translate(wBackModel, glm::vec3(-0.75f, 0.85f, 1.55f));
		wBackModel = glm::rotate(wBackModel, wAngle, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(wBackModel));
		wBL.Draw();
		
		wBackModel = bodyModel;//glm::mat4();
		//wBackModel = glm::translate(wBackModel, bodyPos);
		//wBackModel = glm::rotate(wBackModel, glm::radians(curAngle), up);
		//wBackModel = glm::scale(wBackModel, glm::vec3(scale));
		wBackModel = glm::translate(wBackModel, glm::vec3(0.75f,0.85f,1.55f));
		wBackModel = glm::rotate(wBackModel, wAngle, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(wBackModel));
		wBR.Draw();
		
		//前轮
		glm::mat4 wForModel = bodyModel;
		//wForModel = glm::translate(wForModel, bodyPos);
		//wForModel = glm::rotate(wForModel, glm::radians(curAngle), up);
		//wForModel = glm::scale(wForModel, glm::vec3(scale));

		wForModel = glm::translate(wForModel, glm::vec3(-0.75f, 0.85f, -1.3f));
		wForModel = glm::rotate(wForModel, wAngle, glm::vec3(1.0f, 0.0f, 0.0f));
		wForModel = glm::rotate(wForModel, fAngle, up);
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(wForModel));
		wFL.Draw();

		wForModel = bodyModel;//glm::mat4();
		//wForModel = glm::translate(wForModel, bodyPos);
		//wForModel = glm::rotate(wForModel, glm::radians(curAngle), up);
		//wForModel = glm::scale(wForModel, glm::vec3(scale));

		wForModel = glm::translate(wForModel, glm::vec3(0.75f, 0.85f, -1.3f));
		wForModel = glm::rotate(wForModel, fAngle, glm::vec3(0.f, 1.f, 0.f));
		wForModel = glm::rotate(wForModel, wAngle, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(wForModel));
		wFR.Draw();

		// Draw skybox
		glDepthFunc(GL_LEQUAL);
		skybox_shader.use();
		glm::mat4 view1 = glm::mat4(glm::mat3(view));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view1));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(skybox_shader.getProgram(), "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

#pragma region "User input"

// 键盘事件
void Do_Movement()
{
	if (keys[GLFW_KEY_SPACE])
	{
		
	}
	else if (keys[GLFW_KEY_V])
	{
		camera.Position = bodyPos;
		camera.Position.y = bodyPos.y + 1.0;
		view = glm::lookAt(camera.Position, bodyPos + dir * (GLfloat)2, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		camera.Position = bodyPos - dir * (GLfloat)3;
		camera.Position.y = bodyPos.y + 3.0;
		view = glm::lookAt(camera.Position, bodyPos, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (keys[GLFW_KEY_W])//前
	{
		bodyPos = bodyPos + dir * speed  * deltaTime;

		wAngle += wSpeed * deltaTime;
		if (fAngle >= 0)
		{
			fAngle -= fSpeed * deltaTime * 2;
			if (fAngle <= 0)
				fAngle = 0.0;
		}
		else
		{
			fAngle += fSpeed * deltaTime * 2;
			if (fAngle >= 0)
				fAngle = 0.0;
		}
	}
	if (keys[GLFW_KEY_S])//后
	{
		bodyPos = bodyPos - dir * speed  * deltaTime;
		wAngle -= wSpeed * deltaTime;
		if (fAngle >= 0)
		{
			fAngle -= fSpeed * deltaTime * 2;
			if (fAngle <= 0)
				fAngle = 0.0;
		}
		else
		{
			fAngle += fSpeed * deltaTime * 2;
			if (fAngle >= 0)
				fAngle = 0.0;
		}
	}
	if (keys[GLFW_KEY_A])//左
	{
		lastAngle = curAngle;
		curAngle += rSpeed*deltaTime;
		fAngle += fSpeed * deltaTime;
		if (fAngle >= 80 * deltaTime)
		{
			fAngle = 80 * deltaTime;
		}
		glm::vec3 d;
		d.x = -sin(glm::radians(curAngle));
		d.z = -cos(glm::radians(curAngle));
		d.y = dir.y;
		dir = glm::normalize(d);
	}
	if (keys[GLFW_KEY_D]) //右
	{
		lastAngle = curAngle;
		curAngle -= rSpeed*deltaTime;
		fAngle -= fSpeed * deltaTime;
		if (fAngle <= -80 * deltaTime)
		{
			fAngle = -80 * deltaTime;
		}
		glm::vec3 d;
		d.x = -sin(glm::radians(curAngle));
		d.z = -cos(glm::radians(curAngle));
		d.y = dir.y;
		dir = glm::normalize(d);
	}
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset, false);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

#pragma endregion


GLuint makeSkybox(GLuint *skyboxVAO)
{

	GLfloat skyboxVertices[] = {
#pragma region
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
#pragma endregion
	};
	vector<const GLchar*> faces;
	faces.push_back("./res/skybox/right.jpg");
	faces.push_back("./res/skybox/left.jpg");
	faces.push_back("./res/skybox/top.jpg");
	faces.push_back("./res/skybox/bottom.jpg");
	faces.push_back("./res/skybox/back.jpg");
	faces.push_back("./res/skybox/front.jpg");
	// Setup skybox VAO
	GLuint skyboxVBO;
	glGenVertexArrays(1, skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(*skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = stbi_load(faces[i], &width, &height, &nrComponents, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
