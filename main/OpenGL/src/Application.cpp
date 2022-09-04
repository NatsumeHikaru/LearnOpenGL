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

#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"

#include"imgui/imgui.h"
#include"imgui/imgui_impl_glfw_gl3.h"

int main() {
	if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	// set context switching time
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) std::cout << "Error!" << std::endl;

	float positions[] = {
		100.0f, 100.0f, 0.0f, 0.0f, // 0
		 200.0f, 100.0f, 1.0f, 0.0f, // 1
		 200.0f,  200.0f, 1.0f, 1.0f, // 2
	    100.0f,  200.0f, 0.0f, 1.0f, // 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	/*GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));*/

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

	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0), glm::vec3(-100, 0, 0));

	Shader* shader = new Shader("res/shader/basic.shader");
	shader->Bind();


	Texture* texture = new Texture("res/textures/kazuha.jpg");
	texture->Bind();
	shader->SetUniform1i("u_Texture", 0);

	va->unBind();
	vb->unBind();
	va->unBind();
	shader->unBind();

	Renderer* renderer = new Renderer();

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	glm::vec3 translation(200, 200, 0);

	float r = 0.0f;
	float delta = 0.05f;
	while (!glfwWindowShouldClose(window)) {
		renderer->Clear();

		ImGui_ImplGlfwGL3_NewFrame();

		glm::mat4 model = glm::translate(glm::mat4(1.0), translation);
		glm::mat4 mvp = proj * view * model;

		shader->Bind();
		shader->SetUniformMat4f("u_MVP", mvp);

		renderer->Draw(*va, *ib, *shader);

		// demo
		{
			ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);           
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete va, vb, ib, shader, texture;

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}