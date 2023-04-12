#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
// GL ADdress
// GLAD的头文件包含了正确的OpenGL头文件
#include <glad/glad.h>
// OpenGL 的轻量级工具程序库
// Graphics Library Framework（图形库框架）
#include <GLFW/glfw3.h>
using namespace std;

// 定义函数头声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 顶点着色器 C 风格
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// 片段着色器 C 风格
const char* fragmentShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
	// 1. 初始化GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 确保兼容性
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 2. 创建窗口指针
	GLFWwindow* window = glfwCreateWindow(800, 800, "Create Triangle", NULL, NULL);
	// 判断是否成功
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		// 释放
		glfwTerminate();
		// 终止
		return -1;
	}

	// 4. 设置自适应窗口
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 3. 初始化 GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 一、创建顶点着色器 ID
	unsigned int vertexShader;
	// 创建顶点着色器
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// 将着色器源码附加到着色器上
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// 附加后进行编译
	glCompileShader(vertexShader);
	// 检测顶点着色器是否成功编译完成
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	// 判断
	if (!success)
	{
		// 写入错误报告
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// 二、创建片段着色器 ID
	unsigned int fragmentShader;
	// 创建片段着色器
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// 将片段着色器源码附加到片段着色器上
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// 编译
	glCompileShader(fragmentShader);
	// 检查编译受否成功
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	// 判断
	if (!success)
	{
		// 写入错误报告
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// 三、创建着色器程序对象
	unsigned int shaderProgram;
	// 创建
	shaderProgram = glCreateProgram();
	// 将顶点着色器和片段着色器附加到程序对象上
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// 顶点和片段链接
	glLinkProgram(shaderProgram);
	// 判断着色器程序是否成功
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	// 判断
	if (!success)
	{
		// 写入错误报告
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// 四、删除顶点和片段
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 定义三角形的三个三维顶点
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	// 定义元素\顶点缓冲、数组对象
	unsigned int VBO, VAO;
	// 使用元素\顶点缓冲、数组函数
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// 绑定元素\顶点缓冲、数组对象
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 元素\顶点数据传入
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positions, GL_STATIC_DRAW);

	// 五、解析顶点数据 -  设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 5. 循环渲染窗口
	while (!glfwWindowShouldClose(window))
	{
		// input 输入获取
		// -----
		processInput(window);

		// render 渲染部分
		// ...
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// 激活程序对象
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		// OpenGL 自带API
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// 状态缓冲
		glfwSwapBuffers(window);
		// 更新事件
		glfwPollEvents();
	}

	// 删除
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// 6. 终止运行
	glfwTerminate();

	return 0;
}

// 定义窗口大小函数封装
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// 设置视口大小
	glViewport(0, 0, width, height);
}

// 定义获取输入函数封装
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}