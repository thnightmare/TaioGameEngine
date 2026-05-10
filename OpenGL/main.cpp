#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderClass.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Texture.h"

const int width = 1000;
const int height = 1000;

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, 
	GLFWwindow* window = glfwCreateWindow(width, height, "TEST WINDOW", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	

	// Vertices coordinates
	GLfloat vertices[] =
	{
		//VERTICES COORDINATES /   COLOURS       /    TEXTURE
		 0.5f,  0.5f, 0.5f,    0.6f, 0.0f, 0.0f,    0.0f, 0.0f,// top right
		 0.5f, -0.5f, 0.5f,    0.0f, 0.7f, 0.f,     0.0f, 1.0f,// bottom right
		-0.5f, -0.5f, 0.5f,    0.5f, 0.0f, 0.0f,    1.0f, 1.0f,// bottom left
		-0.5f,  0.5f, 0.5f,    0.0f, 0.0f, 0.6f,    1.0f, 0.0f,// top left

		0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, // 4: Top Right
		0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 5: Bottom Right
	   -0.5f, -0.5f, -0.5f,   0.5f, 0.5f, 0.5f,   0.0f, 0.0f, // 6: Bottom Left
	   -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // 7: Top Left
	};
;

	// Indices for vertices order
	GLuint indices[] =
	{
				0, 3, 2,   2, 1, 0, // Front
				4, 5, 6,   6, 7, 4, // Back
				3, 7, 6,   6, 2, 3, // Left
				4, 0, 1,   1, 5, 4, // Right
				4, 7, 3,   3, 0, 4, // Top
				1, 2, 6,   6, 5, 1  // Bottom
	};

	Shader shaderProgram("default.vert", "default.frag");

	// Creates a Vertex Array Object
	VAO VAO1;
	VAO1.Bind();

	//Generates VBO and links to vertices
	VBO VBO1(vertices, sizeof(vertices));
	//Generates EBO and links to indices
	EBO EBO1(indices, sizeof(indices));

	//Link VBO to VAO
	//Position Attribute
	VAO1.LinkVertexAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	//Color Attribute
	VAO1.LinkVertexAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//Texture Attribute
	VAO1.LinkVertexAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	float rotation = 0.0f;
	double PrevTime = glfwGetTime();


	//gets the location of the uniform and sets the value
	GLuint unID = glGetUniformLocation(shaderProgram.ID, "scale");

	// TEXTURE
	Texture Wood ("wood texture.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	Wood.texUnit(shaderProgram, "tex0", 0);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Enables the Z depth buffer

		glEnable(GL_DEPTH_TEST);
		// Specify the color of the background
		glClearColor(0.00f, 0.02f, 0.05f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		shaderProgram.Activate();

		//Helps the Rotation of the Matrix 
		double crntTime = glfwGetTime();
		if (crntTime - PrevTime >= 0.01)
		{
			rotation += 1.0f;
			PrevTime = crntTime;
		};
		
		// Initialize matrices to be used in transformations so they are not set to null
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);

		// Assigns different  transformations to each matrix
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
		projection = glm::perspective(glm::radians(45.0f), float(width / height), 0.4f, 100.0f);

		// Inputs Matrices to Vertex Shader
		int ModelLocation = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(model));
		int ViewLocation = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, glm::value_ptr(view));
		int ProjLocation = glGetUniformLocation(shaderProgram.ID, "projection");
		glUniformMatrix4fv(ProjLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glUniform1f(unID, 0.5f);
		Wood.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	EBO1.Delete();
	VBO1.Delete();
	Wood.DeleteTexture();
	shaderProgram.Deactivate();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}