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
#include "SkyBox.h"

#include <cstdint>
#include <vector>

constexpr int32_t I_WINDOW_WIDTH = 1280;
constexpr int32_t I_WINDOW_HEIGHT = 720;

constexpr int32_t I_SPHERE_RINGS = 32;
constexpr int32_t I_SPHERE_SEGMENTS = 32;

constexpr const char* SZ_WINDOW_TITLE = "Model Viewer";

constexpr const char* SZ_SHADER_TRIANGLE_VERT_PATH = "Shaders/triangle.vert";
constexpr const char* SZ_SHADER_TRIANGLE_FRAG_PATH = "Shaders/triangle.frag";

constexpr const char* SZ_SHADER_UNLIT_VERT_PATH = "Shaders/unlit.vert";
constexpr const char* SZ_SHADER_UNLIT_FRAG_PATH = "Shaders/unlit.frag";

constexpr const char* SZ_SHADER_SKYBOX_VERT_PATH = "Shaders/skybox.vert";
constexpr const char* SZ_SHADER_SKYBOX_FRAG_PATH = "Shaders/skybox.frag";

constexpr const char* SZ_TEXTURE_BALL_PATH = "Textures/ball_color.png";
constexpr const char* SZ_TEXTURE_GRASS_PATH = "Textures/grass_color.png";

constexpr const char* SZ_SKYBOX_FOLDER_PATH = "Textures/Skybox";



constexpr float F_CLEAR_RED = 0.07f;
constexpr float F_CLEAR_GREEN = 0.13f;
constexpr float F_CLEAR_BLUE = 0.17f;
constexpr float F_CLEAR_ALPHA = 1.0f;

constexpr float F_CAMERA_DISTANCE = 3.0f;

constexpr float F_FOV_DEGREES = 45.0f;

constexpr float F_NEAR_PLANE = 0.1f;
constexpr float F_FAR_PLANE = 100.0f;

constexpr float F_SPHERE_RADIUS = 1.0f;

constexpr float F_GROUND_HALF_SIZE = 40.0f;
constexpr float F_GROUND_HEIGHT = -1.0f;
constexpr float F_GROUND_UV_TILES = 5.0f;

constexpr float F_SUN_DISTANCE = 60.0f;
constexpr float F_SUN_RADIUS = 2.5f;

constexpr float F_SHADOW_BLOB_RADIUS = 1.1f;
constexpr float F_SHADOW_BLOB_THICKNESS = 0.01f;
constexpr float F_SHADOW_BLOB_HEIGHT = -0.99f;
constexpr float F_SHADOW_BLOB_OFFSET = 0.88f;

const glm::vec3 VEC3_LIGHT_DIRECTION = glm::vec3(-1.0f, -0.5f, 0.0f);
const glm::vec3 VEC3_LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);

const glm::vec3 VEC3_SUN_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);

const glm::vec3 VEC3_SHADOW_BLOB_COLOR = glm::vec3(0.16f, 0.19f, 0.26f);


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
	std::vector<float> groundVertices = GenerateGroundPlaneVertices(F_GROUND_HALF_SIZE, F_GROUND_HEIGHT, F_GROUND_UV_TILES);
	CMesh groundMesh(groundVertices.data(), static_cast<int32_t>(groundVertices.size()) / I_MESH_FLOATS_PER_VERTEX);
	if (!groundMesh.IsValid())
	{
		return -1;
	}
	std::vector<float> skyboxVertices = GenerateSkyboxCubeVertices();
	CMesh skyboxMesh(skyboxVertices.data(), static_cast<int32_t>(skyboxVertices.size()) / I_MESH_FLOATS_PER_VERTEX);
	if (!skyboxMesh.IsValid())
	{
		return -1;
	}
	CShader unlitShader(SZ_SHADER_UNLIT_VERT_PATH, SZ_SHADER_UNLIT_FRAG_PATH);
	if (!unlitShader.IsValid())
	{
		return -1;
	}
	CShader skyboxShader(SZ_SHADER_SKYBOX_VERT_PATH, SZ_SHADER_SKYBOX_FRAG_PATH);
	if (!skyboxShader.IsValid())
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
	CTexture ballTexture(SZ_TEXTURE_BALL_PATH);
	if (!texture.IsValid())
	{
		return -1;
	}
	CSkyBox skybox(SZ_SKYBOX_FOLDER_PATH);
	if (!skybox.IsValid())
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

		shader.SetInt("u_Texture", 0);

		shader.SetVec3("u_LightDirection", VEC3_LIGHT_DIRECTION);
		shader.SetVec3("u_LightColor", VEC3_LIGHT_COLOR);


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

		ballTexture.Bind(0);
		mesh.Draw();

		texture.Bind(0);
		groundMesh.Draw();

		unlitShader.Use();
		glm::vec3 vSunPosition = -glm::normalize(VEC3_LIGHT_DIRECTION) * F_SUN_DISTANCE;
		glm::mat4 matSunModel = glm::translate(glm::mat4(1.0f), vSunPosition);
		matSunModel = glm::scale(matSunModel, glm::vec3(F_SUN_RADIUS, F_SUN_RADIUS, F_SUN_RADIUS));

		unlitShader.SetMat4("u_Model", matSunModel);
		unlitShader.SetMat4("u_View", matView);
		unlitShader.SetMat4("u_Projection", matProjection);
		unlitShader.SetVec3("u_Color", VEC3_SUN_COLOR);

		mesh.Draw();

		glm::vec3 vLightDir = glm::normalize(VEC3_LIGHT_DIRECTION);
		glm::vec3 vShadowPosition = glm::vec3(vLightDir.x * F_SHADOW_BLOB_OFFSET, F_SHADOW_BLOB_HEIGHT, vLightDir.z * F_SHADOW_BLOB_OFFSET);
		glm::mat4 matShadowModel = glm::translate(glm::mat4(1.0f), vShadowPosition);
		matShadowModel = glm::scale(matShadowModel, glm::vec3(F_SHADOW_BLOB_RADIUS, F_SHADOW_BLOB_THICKNESS, F_SHADOW_BLOB_RADIUS));

		unlitShader.SetMat4("u_Model", matShadowModel);
		unlitShader.SetVec3("u_Color", VEC3_SHADOW_BLOB_COLOR);
		mesh.Draw();


		skyboxShader.Use();
		

		glm::mat4 matSkyboxView = glm::mat4(glm::mat3(matView));
		skyboxShader.SetMat4("u_View", matSkyboxView);
		skyboxShader.SetMat4("u_Projection", matProjection);

		skybox.Bind(0);
		skyboxShader.SetInt("u_Skybox", 0);

		glDepthFunc(GL_LEQUAL);
		skyboxMesh.Draw();
		glDepthFunc(GL_LESS);


		window.SwapBuffers();
		window.PollEvents();
	}

	return 0;
}
