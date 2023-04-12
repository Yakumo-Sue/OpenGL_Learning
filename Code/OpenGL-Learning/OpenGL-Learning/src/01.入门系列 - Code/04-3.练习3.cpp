// 创建两个着色器程序，第二个程序使用一个不同的片段着色器，输出黄色；
// 再次绘制这两个三角形，让其中一个输出为黄色

#define _CRT_SECURE_NO_WARNINGS 1
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

// 函数头
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 定义顶点、片段着色器
const char* vertexShaderSource = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* FragmentShader1Source = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const char* fragmentShader2Source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main()
{
	// 1. 初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 设置 MAC 兼容
#ifdef _APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // _APPLE_

	// 2. 创建窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Use Two Different VAO And VBO", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		// 终止
		glfwTerminate();
		return -1;
	}

	// 3. 窗口的自适应设置
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 4. 初始化 GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	// 顶点和片段着色器 ID
	unsigned int vertexShader, fragmentShaderOrange, fragmentShaderYellow;
	// 创建
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	// 绑定
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShaderOrange, 1, &FragmentShader1Source, NULL);
	glShaderSource(fragmentShaderYellow, 1, &fragmentShader2Source, NULL);
	// 编译
	glCompileShader(vertexShader);
	glCompileShader(fragmentShaderOrange);
	glCompileShader(fragmentShaderYellow);
	// 判断
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}
	glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderOrange, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}
	glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// 链接陈程序 ID
	unsigned int shaderProgramOrange, shaderProgramYellow;
	// 创建
	shaderProgramOrange = glCreateProgram();
	shaderProgramYellow = glCreateProgram();
	// 绑定
	glAttachShader(shaderProgramOrange, vertexShader);
	glAttachShader(shaderProgramOrange, fragmentShaderOrange);
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	// 链接
	glLinkProgram(shaderProgramOrange);
	glLinkProgram(shaderProgramYellow);
	// 判断
	glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramOrange, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}
	glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// 清理顶点和片段
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderOrange);


	// 设置顶点坐标
	float firstPositions[] = {
		-0.9f, -0.5f, 0.0f, // 左侧三角形 - 左顶点
		-0.0f, -0.5f, 0.0f, // 左侧三角形 - 右顶点
		-0.45f, 0.5f, 0.0f // 左侧三角形 - 顶点
	};
	float secondPositions[] = {
		0.0f, -0.5f, 0.0f, // 右侧三角形 - 左顶点
		0.9f, -0.5f, 0.0f, // 右侧三角形 - 右顶点
		0.45f, 0.5f, 0.0f  // 右侧三角形 - 顶点
	};

	// 定义缓冲和数组
	unsigned int VBOs[2], VAOs[2];
	// 使用
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);
	// 第一个 设置
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), firstPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 第二个设置
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), secondPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 5. 循环函数
	while (!glfwWindowShouldClose(window))
	{
		// 输入
		processInput(window);

		// 渲染 视口
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 绘制
		// 第一个
		glUseProgram(shaderProgramOrange);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// 第二个
		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// 缓冲、更新
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgramOrange);
	glDeleteProgram(shaderProgramYellow);

	// 6. 结束
	glfwTerminate();

	return 0;
}

// 自适应
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
// 检擦输入
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}