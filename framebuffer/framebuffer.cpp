#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <algorithm> 
#include <map>

#include "Shader.h"
#include "Camera.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader shader("easy.vs","easy.fs");
	Shader screenShader("framebuffer/framebuffer_screen.vs", "framebuffer/framebuffer_screen.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
										  // Front face
										  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
										  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
										  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
										  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
										  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
										  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
																			// Left face
																			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
																			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
																			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
																			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
																			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
																			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
																											  // Right face
																											  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
																											  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
																											  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
																											  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
																											  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
																											  0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
																																			   // Bottom face
																																			   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
																																			   0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
																																			   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
																																			   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
																																			   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
																																			   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
																																												 // Top face
																																												 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
																																												 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
																																												 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
																																												 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
																																												 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
																																												 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	};
	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	GLfloat quadVertices[] = {   // Vertex attributes for a quad that in Normalized Device Coordinates. NOTE that this plane is now much smaller and at the top of the screen
								 // Positions   // TexCoords
		-0.3f,  1.0f,  0.0f, 1.0f,
		-0.3f,  0.7f,  0.0f, 0.0f,
		0.3f,  0.7f,  1.0f, 0.0f,

		-0.3f,  1.0f,  0.0f, 1.0f,
		0.3f,  0.7f,  1.0f, 0.0f,
		0.3f,  1.0f,  1.0f, 1.0f
	};
	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// load textures
	// -------------
	unsigned int cubeTexture = loadTexture("container.jpg");
	unsigned int floorTexture = loadTexture("metal.png");

	//frame buffers
	unsigned int  framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Generate texture
	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, cubeTexture, 0);*/
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cubeTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDeleteFramebuffers(1, &framebuffer);
	// shader configuration
	// --------------------
	shader.use();
	shader.setInt("texture1", 0);
	screenShader.use();
	screenShader.setInt("screenTexture", 0);
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------

		// First pass
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
		glEnable(GL_DEPTH_TEST);
		//floor
		shader.use();
		glm::mat4 model;
		camera.Yaw += 180.0f;
		camera.Pitch += 180.0f; // Turn the camera's yaw 180 degrees around
		camera.ProcessMouseMovement(0, 0, false);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shader.setMat4("model", glm::mat4());
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.01f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Second pass
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		//floor
		shader.use();
		camera.Yaw -= 180.0f;
		camera.Pitch -= 180.0f; // Turn the camera's yaw 180 degrees around
		camera.ProcessMouseMovement(0, 0, false);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shader.setMat4("model", glm::mat4());
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.01f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		screenShader.use();
		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		//glDrawArrays(GL_LINE_LOOP, 0, 6);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------/
unsigned int loadTexture(char const *path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//需要注意的是，当采样纹理边缘的时候，
		//OpenGL在边界值和下一个重复的纹理的值之间进行插值（因为我们把它的放置方式设置成了GL_REPEAT）。
		//这样就行了，但是由于我们使用的是透明值，纹理图片的上部获得了它的透明值是与底边的纯色值进行插值的。
		//结果就是一个有点半透明的边，你可以从我们的纹理四边形的四周看到。
		//为了防止它的出现，当你使用alpha纹理的时候要把纹理环绕方式设置为GL_CLAMP_TO_EDGE：
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
