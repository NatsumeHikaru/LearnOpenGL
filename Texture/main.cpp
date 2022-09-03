#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cstring>
#include<fstream>
#include<sstream>

#include"Renderer.h"

#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"VertexArray.h"
#include"VertexBufferLayout.h"
#include"Shader.h"
#include"Texture.h"

int main() {
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	// set context switching time
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) std::cout << "Error!" << std::endl;

	float positions[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, // 0
		 0.5f, -0.5f, 1.0f, 0.0f, // 1
		 0.5f,  0.5f, 1.0f, 1.0f, // 2
	    -0.5f,  0.5f, 0.0f, 1.0f, // 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	VertexArray* va = new VertexArray();
	
	VertexBuffer *vb = new VertexBuffer(positions, sizeof(positions));

	VertexBufferLayout* layout = new VertexBufferLayout();
	layout->Push<float>(2);
	layout->Push<float>(2);
	va->AddBuffer(*vb, *layout);

	IndexBuffer* ib = new IndexBuffer(indices, 6);

	Shader* shader = new Shader("res/shader/basic.shader");
	shader->Bind();
	shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

	Texture* texture = new Texture("res/textures/kazuha.jpg");
	texture->Bind();
	shader->SetUniform1i("u_Texture", 0);

	va->unBind();
	vb->unBind();
	va->unBind();
	shader->unBind();

	Renderer* renderer = new Renderer();

	float r = 0.0f;
	float delta = 0.05f;
	while (!glfwWindowShouldClose(window)) {
		renderer->Clear();

		shader->Bind();
		shader->SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

		renderer->Draw(*va, *ib, *shader);

		if (r > 1.0f)
			delta = -0.05f;
		else if (r < 0.0f)
			delta = 0.05f;
		r += delta;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete va, vb, ib, shader;

	glfwTerminate();
	return 0;
}