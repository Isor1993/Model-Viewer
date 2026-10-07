/*****************************************************************************
* Project : Model Viewer
* File    : main.cpp
* Date    : 04.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Entry point of the model viewer. Creates the window (CWindow) and runs
* the render loop; render state like the clear color stays here. Further
* modules (camera, mesh, shader, texture, skybox) follow the same
* pattern as the viewer grows.
*
* History :
* 04.10.2026 ER Created
* 04.10.2026 ER GLFW window, GLAD setup and render loop (course baseline)
* 04.10.2026 ER Window code moved into CWindow (RAII)
* 07.10.2026 ER MVP matrix chain at the triangle (V3 start)
* 07.10.2026 ER Sphere replaces the triangle (Geometry, 8-float layout)
* 07.10.2026 ER Depth test on, grass test texture loaded (V3)
******************************************************************************/
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.h"
#include "Shader.h"
#include "Mesh.h"
#include "Geometry.h"
#include "Texture.h"
#include "Camera.h"

#include <cstdint>
#include <vector>

constexpr int32_t I_WINDOW_WIDTH = 1280;
constexpr int32_t I_WINDOW_HEIGHT = 720;
constexpr int32_t I_SPHERE_RINGS = 32;
constexpr int32_t I_SPHERE_SEGMENTS = 32;

constexpr const char* SZ_WINDOW_TITLE = "Model Viewer";
constexpr const char* SZ_SHADER_TRIANGLE_VERT_PATH = "Shaders/triangle.vert";
constexpr const char* SZ_SHADER_TRIANGLE_FRAG_PATH = "Shaders/triangle.frag";
constexpr const char* SZ_TEXTURE_GRASS_PATH = "Textures/grass_color.png";


constexpr float F_CLEAR_RED = 0.07f;
constexpr float F_CLEAR_GREEN = 0.13f;
constexpr float F_CLEAR_BLUE = 0.17f;
constexpr float F_CLEAR_ALPHA = 1.0f;
constexpr float F_CAMERA_DISTANCE = 3.0f;
constexpr float F_FOV_DEGREES = 45.0f;
constexpr float F_NEAR_PLANE = 0.1f;
constexpr float F_FAR_PLANE = 100.0f;
constexpr float F_SPHERE_RADIUS = 1.0f;


/// <summary>
/// Opens the window, prepares OpenGL and runs the render loop until close.
/// </summary>
int main()
{
	CWindow window(I_WINDOW_WIDTH, I_WINDOW_HEIGHT, SZ_WINDOW_TITLE);
	if (!window.IsValid())
	{
		return -1;
	}
	CShader shader(SZ_SHADER_TRIANGLE_VERT_PATH, SZ_SHADER_TRIANGLE_FRAG_PATH);
	if (!shader.IsValid())
	{
		return -1;
	}
	std::vector<float> sphereVertices = GenerateSphereVertices(I_SPHERE_RINGS, I_SPHERE_SEGMENTS, F_SPHERE_RADIUS);
	CMesh mesh(sphereVertices.data(), static_cast<int32_t>(sphereVertices.size()) / I_MESH_FLOATS_PER_VERTEX);
	if(!mesh.IsValid())
	{
		return -1;
	}
	CTexture texture(SZ_TEXTURE_GRASS_PATH);
	if (!texture.IsValid())
	{
		return -1;
	}

	CCamera camera(F_CAMERA_DISTANCE);

	glClearColor(F_CLEAR_RED, F_CLEAR_GREEN, F_CLEAR_BLUE, F_CLEAR_ALPHA);

	glEnable(GL_DEPTH_TEST);

	while (!window.ShouldClose())
	{
		window.ProcessInput();

		camera.Rotate(window.GetMouseDeltaX(), window.GetMouseDeltaY());
		camera.Zoom(window.GetScrollDelta());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();

		texture.Bind(0);
		shader.SetInt("u_Texture", 0);


		glm::mat4 matModel = glm::mat4(1.0f);

		glm::mat4 matView = camera.GetViewMatrix();

		glm::mat4 matProjection = glm::perspective(
			glm::radians(F_FOV_DEGREES),
			window.GetAspectRatio(), 
			F_NEAR_PLANE,
			F_FAR_PLANE);

		shader.SetMat4("u_Model", matModel);
		shader.SetMat4("u_View", matView);
		shader.SetMat4("u_Projection", matProjection);

		mesh.Draw();

		window.SwapBuffers();
		window.PollEvents();
	}

	return 0;
}
