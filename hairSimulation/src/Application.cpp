#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "VertextBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Hair.h"

using namespace std;
using namespace glm;
int main(void)
{
	GLFWwindow* window;
	
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(6);
	if (glewInit() != GLEW_OK) {
		cout << "Error" << endl;
	}
	cout << glGetString(GL_VERSION) << endl;

	{
		Shader shader("src/Shaders/vertexShader.GLSL", "src/Shaders/fragmentShader.GLSL");
		
		/*float vertices1[] = {
			-0.5f,  0.5f,
			-0.5f,  0.0f,
			 0.5f,  0.0f
		};

		VertexArray vao1;
		VertextBuffer vb1(vertices1, sizeof(vertices1));
		VertextBufferLayout layout1;
		layout1.push<float>(2);
		vao1.BindBuffer(vb1, layout1);
		
		std::vector<vec3> vertices2 = {
			vec3(-0.5f,  0.5f, 0.0f),
			vec3(0.5f,  0.5f, 0.0f),
			vec3(0.5f,  0.5f, 0.0f),
			vec3(0.5f,  0.0f, 0.0f)
		};

		VertexArray vao2;
		VertextBuffer vb2(vertices2, sizeof(float) * vertices2.size() * 3);
		VertextBufferLayout layout2;
		layout2.push<float>(3);
		vao2.BindBuffer(vb2, layout2);*/
		Hairline strand(vec3(0, 0.9, 0));

		shader.unUse();
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindVertexArray(0));

		/* Loop until the user closes the window */
		float R = 0.9f, G = 0.2f, B = 0.3f, A = 1.0f;
		float inc = 0.01f;
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);
			shader.use();			
			strand.Update();
			strand.Draw();

			shader.SetUniform4f("u_color", R, G, B, A);

			// draw
			//vb1.Bind(vertices1,sizeof(vertices1));
			//vao1.BindBuffer(vb1, layout1);
			//GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

			//update
			//if (vertices1[2] > -0.25)inc = -0.01f;
			//if (vertices1[2] < -0.75)inc = 0.01f;
			//vertices1[2] += inc;
			//// color update
			//R += 0.05f;
			//G += 0.01f;
			//B += 0.02f;
			//if (R > 1)R = 0;
			//if (G > 1)G = 0;
			//if (B > 1)B = 0;
			//shader.SetUniform4f("u_color", R, G, B, A);
			////draw
			//vb2.Bind(vertices2.data(), sizeof(float) * vertices2.size() * 3);
			//vao2.BindBuffer(vb2, layout2);
			//GLCall(glDrawArrays(GL_LINES, 0, 4));
			////update
			//vertices2[3].x -= inc;
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}
/*

*/