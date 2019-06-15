// GlassVolume.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Volume.h"
#include "TIFFTexture.h"

int main()
{
	//init settings
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(1366, 720, "Electric Field Simulator", nullptr, nullptr); // Windowed

	glfwMakeContextCurrent(window);

	//init OpenGL and link functions in a dynamic way
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	// check OpenGL errors
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error: " << err << std::endl;
	}

	Camera cam = Camera();
	cam.view = glm::lookAt(
		glm::vec3(10.0f, 10.0f, 10.0f), // position
		glm::vec3(0.0f, 0.0f, 0.0f), // camera center
		glm::vec3(0.0f, 0.0f, 1.0f) // up axis
	);

	TIFFTexture volumeData = TIFFTexture("assets/overview.tif");
	Volume volumeObject = Volume(&volumeData);

	float lastTime = 0.0f;
	float speed = 3.0f; // 3 units / second
	float pitch = 0.0f;
	float yaw = 0.0f;

	//main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();

		double currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//get inputs
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			pitch += speed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			pitch -= speed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			yaw -= speed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			yaw += speed * deltaTime;
		}

		cam.position.x = 0 + (cos(yaw)  * sin(pitch) * 2);
		cam.position.y = 0 + (sin(yaw) * sin(pitch) * 2);
		cam.position.z = 2 + (cos(pitch) * 2);

		cam.view = glm::lookAt(
			cam.position,             // position
			glm::vec3(0.0f, 0.0f, 00.0f), // camera center
			glm::vec3(0.0f, 0.0f, 1.0f)                    // up axis
		);

		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		volumeObject.render(cam);

	}

	glfwTerminate();

	return 0;
}
