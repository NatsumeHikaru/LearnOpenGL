#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

int main() {
	GLFWwindow* window;

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f, 
		 0.5f, -0.5f
	};

	glfwMakeContextCurrent(window);
	
	if (glewInit() != GLEW_OK) std::cout << "Error!" << std::endl; // 需要有效的上下文窗口

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		/*glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(0.5f, -0.5f);
		glEnd();*/

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
