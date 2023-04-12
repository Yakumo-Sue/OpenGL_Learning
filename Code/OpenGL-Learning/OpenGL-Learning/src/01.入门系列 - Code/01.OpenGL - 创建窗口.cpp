#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
// GL ADdress
// GLAD的头文件包含了正确的OpenGL头文件
 #include <glad/glad.h>
// OpenGL 的轻量级工具程序库
// Graphics Library Framework（图形库框架）
#include <GLFW/glfw3.h>
using namespace std;

// 定义函数头
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	// GLFW
	// 1. 初始化GLFW
	glfwInit();
	// 第一个参数：选项的名称，以GLFW_开头的枚举值中选择；
	// 第二个参数：接受一个整型，用来设置这个选项的值
	// 详细信息：https://www.glfw.org/docs/latest/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 如果使用的是Mac OS X系统，你还需要加下面这行代码到你的初始化代码中这些配置才能起作用（将上面的代码解除注释）：
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// 2. 创建窗口
	// 第一个参数：窗口的宽
	// 第二个参数：窗口的高
	// 第三个参数：窗口的名称（标题）
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	// 如果返回 NULL 说明创建失败
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// GLAD
	// GLAD是用来管理OpenGL的函数指针的

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 设置窗口最大化的回调函数，窗口最大化或复位时调用
	// GLFWAPI GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow* window, GLFWwindowclosefun callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 渲染循环(Render Loop)
	// glfwWindowShouldClose 函数在每次循环的开始前检查一次GLFW是否被要求退出
	// 如果是的话该函数返回true然后渲染循环便结束了
	while (!glfwWindowShouldClose(window))
	{
		// 输入
		processInput(window);

		// 渲染指令
		// ...
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲）
		// 它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwSwapBuffers(window);
		// glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）
		// 更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
		glfwPollEvents();
	}

	// 释放/删除之前的分配的所有资源
	glfwTerminate();

	return 0;
}

// 封装设置窗口大小，以便后续修改
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// 设置窗口的维度(Dimension)：
	glViewport(0, 0, width, height);
}

// 输入控制
void processInput(GLFWwindow* window)
{
	// 需要一个窗口以及一个按键作为输入
	// 这里检查用户是否按下了返回键(Esc)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}