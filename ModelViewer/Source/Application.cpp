/*****************************************************************************
* Project : Model Viewer
* File    : Application.cpp
* Date    : 08.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Implementation of CApplication — see Application.h. The scene
* constants live here: one place to tune sizes, positions and colors.
*
* History :
* 08.10.2026 ER Created — scene and loop moved out of main (V6 cleanup)
* 08.10.2026 ER Reworked to pointer members, Initialize groups and six
*               single-purpose render stations (design review with Isor)
* 08.10.2026 ER MSAA enabled (V6 polish)
* 26.10.2026 ER Cube and pyramid added; shadow and outline stations
*               parameterized for any object (OCE feedback)
* 26.10.2026 ER Own CC0 textures for cube and pyramid; contact shadow
*               squashes the object's own mesh (footprint look)
******************************************************************************/
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Application.h"
#include "Geometry.h"

// --- shader and asset paths ---
constexpr const char* SZ_SHADER_TOON_VERT_PATH = "Shaders/toon.vert";
constexpr const char* SZ_SHADER_TOON_FRAG_PATH = "Shaders/toon.frag";
constexpr const char* SZ_SHADER_UNLIT_VERT_PATH = "Shaders/unlit.vert";
constexpr const char* SZ_SHADER_UNLIT_FRAG_PATH = "Shaders/unlit.frag";
constexpr const char* SZ_SHADER_SKYBOX_VERT_PATH = "Shaders/skybox.vert";
constexpr const char* SZ_SHADER_SKYBOX_FRAG_PATH = "Shaders/skybox.frag";
constexpr const char* SZ_TEXTURE_BALL_PATH = "Textures/ball_color.png";
constexpr const char* SZ_TEXTURE_GRASS_PATH = "Textures/grass_color.png";
constexpr const char* SZ_SKYBOX_FOLDER_PATH = "Textures/Skybox";
constexpr const char* SZ_TEXTURE_CUBE_PATH = "Textures/cube_color.png";
constexpr const char* SZ_TEXTURE_PYRAMID_PATH = "Textures/pyramid_color.png";

// --- camera and projection ---
constexpr float F_CAMERA_DISTANCE = 3.0f;
constexpr float F_FOV_DEGREES = 45.0f;
constexpr float F_NEAR_PLANE = 0.1f;
constexpr float F_FAR_PLANE = 100.0f;

// --- background ---
constexpr float F_CLEAR_RED = 0.07f;
constexpr float F_CLEAR_GREEN = 0.13f;
constexpr float F_CLEAR_BLUE = 0.17f;
constexpr float F_CLEAR_ALPHA = 1.0f;

// --- scene objects: ground, ball, cube, pyramid, sun ---
constexpr float F_GROUND_HALF_SIZE = 40.0f;
constexpr float F_GROUND_HEIGHT = -1.0f;
constexpr float F_GROUND_UV_TILES = 5.0f;

constexpr int32_t I_SPHERE_RINGS = 32;
constexpr int32_t I_SPHERE_SEGMENTS = 32;
constexpr float F_SPHERE_RADIUS = 1.0f;
const glm::vec3 VEC3_BALL_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);

constexpr float F_CUBE_HALF_SIZE = 0.5f;
const glm::vec3 VEC3_CUBE_POSITION = glm::vec3(-3.5f, -0.5f, 0.0f);

constexpr float F_PYRAMID_HALF_SIZE = 0.7f;
constexpr float F_PYRAMID_HEIGHT = 1.4f;
const glm::vec3 VEC3_PYRAMID_POSITION = glm::vec3(3.5f, F_GROUND_HEIGHT, 0.0f);

constexpr float F_SUN_DISTANCE = 60.0f;
constexpr float F_SUN_RADIUS = 2.5f;

// --- shadow blobs and outline ---
constexpr float F_SHADOW_BLOB_THICKNESS = 0.01f;
constexpr float F_SHADOW_BLOB_HEIGHT = -0.99f;

constexpr float F_BALL_SHADOW_SCALE = 1.1f;
constexpr float F_CUBE_SHADOW_SCALE = 1.1f;
constexpr float F_PYRAMID_SHADOW_SCALE = 1.05f;

// Taller objects push their contact shadow further along the light.
constexpr float F_BALL_SHADOW_OFFSET = 0.88f;
constexpr float F_CUBE_SHADOW_OFFSET = 0.35f;
constexpr float F_PYRAMID_SHADOW_OFFSET = 0.45f;

constexpr float F_OUTLINE_SCALE = 1.03f;

// --- light and colors ---
const glm::vec3 VEC3_LIGHT_DIRECTION = glm::vec3(-1.0f, -0.5f, 0.0f);
const glm::vec3 VEC3_LIGHT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);

const glm::vec3 VEC3_SUN_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);

const glm::vec3 VEC3_SHADOW_BLOB_COLOR = glm::vec3(0.16f, 0.19f, 0.26f);

const glm::vec3 VEC3_OUTLINE_COLOR = glm::vec3(0.0f, 0.0f, 0.0f);

// --- math helpers ---
// The 1.0 fills the matrix diagonal: the "do nothing" identity matrix.
const glm::mat4 MAT4_IDENTITY = glm::mat4(1.0f);

CApplication::CApplication()
{
}

CApplication::~CApplication()
{
	delete m_pSkybox;
	delete m_pPyramidTexture;
	delete m_pCubeTexture;
	delete m_pBallTexture;
	delete m_pGrassTexture;
	delete m_pSkyboxMesh;
	delete m_pCubeMesh;
	delete m_pPyramidMesh;
	delete m_pGroundMesh;
	delete m_pSphereMesh;
	delete m_pSkyboxShader;
	delete m_pUnlitShader;
	delete m_pToonShader;
	delete m_pCamera;
}

bool CApplication::Initialize(CWindow* a_pWindow)
{
	m_pWindow = a_pWindow;
	m_pCamera = new CCamera(F_CAMERA_DISTANCE);

	if (!InitializeShaders())
	{
		return false;
	}
	if (!InitializeMeshes())
	{
		return false;
	}
	if (!InitializeTextures())
	{
		return false;
	}

	glClearColor(F_CLEAR_RED, F_CLEAR_GREEN, F_CLEAR_BLUE, F_CLEAR_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	return true;
}

bool CApplication::InitializeShaders()
{
	m_pToonShader = new CShader(SZ_SHADER_TOON_VERT_PATH, SZ_SHADER_TOON_FRAG_PATH);
	if (!m_pToonShader->IsValid())
	{
		return false;
	}

	m_pUnlitShader = new CShader(SZ_SHADER_UNLIT_VERT_PATH, SZ_SHADER_UNLIT_FRAG_PATH);
	if (!m_pUnlitShader->IsValid())
	{
		return false;
	}

	m_pSkyboxShader = new CShader(SZ_SHADER_SKYBOX_VERT_PATH, SZ_SHADER_SKYBOX_FRAG_PATH);
	if (!m_pSkyboxShader->IsValid())
	{
		return false;
	}
	return true;
}

bool CApplication::InitializeMeshes()
{
	std::vector<float> sphereVertexData = GenerateSphereVertices(I_SPHERE_RINGS, I_SPHERE_SEGMENTS, F_SPHERE_RADIUS);
	m_pSphereMesh = new CMesh(sphereVertexData.data(), static_cast<int32_t>(sphereVertexData.size()) / I_MESH_FLOATS_PER_VERTEX);
	if (!m_pSphereMesh->IsValid())
	{
		return false;
	}

	std::vector<float> groundVertexData = GenerateGroundPlaneVertices(F_GROUND_HALF_SIZE, F_GROUND_HEIGHT, F_GROUND_UV_TILES);
	m_pGroundMesh = new CMesh(groundVertexData.data(), static_cast<int32_t>(groundVertexData.size()) / I_MESH_FLOATS_PER_VERTEX);
	if (!m_pGroundMesh->IsValid())
	{
		return false;
	}

	std::vector<float> skyboxVertexData = GenerateSkyboxCubeVertices();
	m_pSkyboxMesh = new CMesh(skyboxVertexData.data(), static_cast<int32_t>(skyboxVertexData.size()) / I_MESH_FLOATS_PER_VERTEX);
	if (!m_pSkyboxMesh->IsValid())
	{
		return false;
	}

	std::vector<float> cubeVertexData = GenerateCubeVertices(F_CUBE_HALF_SIZE);
	m_pCubeMesh = new CMesh(cubeVertexData.data(), static_cast<int32_t>(cubeVertexData.size()) / I_MESH_FLOATS_PER_VERTEX);
	if (!m_pCubeMesh->IsValid())
	{
		return false;
	}

	std::vector<float> pyramidVertexData = GeneratePyramidVertices(F_PYRAMID_HALF_SIZE, F_PYRAMID_HEIGHT);
	m_pPyramidMesh = new CMesh(pyramidVertexData.data(), static_cast<int32_t>(pyramidVertexData.size()) / I_MESH_FLOATS_PER_VERTEX);
	if (!m_pPyramidMesh->IsValid())
	{
		return false;
	}
	return true;
}

bool CApplication::InitializeTextures()
{
	m_pGrassTexture = new CTexture(SZ_TEXTURE_GRASS_PATH);
	if (!m_pGrassTexture->IsValid())
	{
		return false;
	}

	m_pBallTexture = new CTexture(SZ_TEXTURE_BALL_PATH);
	if (!m_pBallTexture->IsValid())
	{
		return false;
	}

	m_pCubeTexture = new CTexture(SZ_TEXTURE_CUBE_PATH);
	if (!m_pCubeTexture->IsValid())
	{
		return false;
	}

	m_pPyramidTexture = new CTexture(SZ_TEXTURE_PYRAMID_PATH);
	if (!m_pPyramidTexture->IsValid())
	{
		return false;
	}

	m_pSkybox = new CSkyBox(SZ_SKYBOX_FOLDER_PATH);
	if (!m_pSkybox->IsValid())
	{
		return false;
	}
	return true;
}

void CApplication::Run()
{
	while (!m_pWindow->ShouldClose())
	{
		m_pWindow->ProcessInput();

		m_pCamera->Rotate(m_pWindow->GetMouseDeltaX(), m_pWindow->GetMouseDeltaY());
		m_pCamera->Zoom(m_pWindow->GetScrollDelta());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_matView = m_pCamera->GetViewMatrix();
		m_matProjection = glm::perspective(
			glm::radians(F_FOV_DEGREES),
			m_pWindow->GetAspectRatio(),
			F_NEAR_PLANE,
			F_FAR_PLANE);

		RenderBall();
		RenderCube();
		RenderPyramid();
		RenderGround();
		RenderSun();
		RenderShadow(m_pSphereMesh, VEC3_BALL_POSITION, F_BALL_SHADOW_SCALE, F_BALL_SHADOW_OFFSET);
		RenderShadow(m_pCubeMesh, VEC3_CUBE_POSITION, F_CUBE_SHADOW_SCALE, F_CUBE_SHADOW_OFFSET);
		RenderShadow(m_pPyramidMesh, VEC3_PYRAMID_POSITION, F_PYRAMID_SHADOW_SCALE, F_PYRAMID_SHADOW_OFFSET);
		RenderOutline(m_pSphereMesh, VEC3_BALL_POSITION);
		RenderOutline(m_pCubeMesh, VEC3_CUBE_POSITION);
		RenderOutline(m_pPyramidMesh, VEC3_PYRAMID_POSITION);
		RenderSkybox();

		m_pWindow->SwapBuffers();
		m_pWindow->PollEvents();
	}
}

void CApplication::RenderBall()
{
	m_pToonShader->Use();
	m_pToonShader->SetInt("u_Texture", 0);
	m_pToonShader->SetVec3("u_LightDirection", VEC3_LIGHT_DIRECTION);
	m_pToonShader->SetVec3("u_LightColor", VEC3_LIGHT_COLOR);

	glm::mat4 matModel = MAT4_IDENTITY;
	m_pToonShader->SetMat4("u_Model", matModel);
	m_pToonShader->SetMat4("u_View", m_matView);
	m_pToonShader->SetMat4("u_Projection", m_matProjection);

	m_pBallTexture->Bind(0);
	m_pSphereMesh->Draw();
}
void CApplication::RenderCube()
{
	m_pToonShader->Use();
	m_pToonShader->SetInt("u_Texture", 0);
	m_pToonShader->SetVec3("u_LightDirection", VEC3_LIGHT_DIRECTION);
	m_pToonShader->SetVec3("u_LightColor", VEC3_LIGHT_COLOR);

	glm::mat4 matModel = glm::translate(MAT4_IDENTITY, VEC3_CUBE_POSITION);
	m_pToonShader->SetMat4("u_Model", matModel);
	m_pToonShader->SetMat4("u_View", m_matView);
	m_pToonShader->SetMat4("u_Projection", m_matProjection);

	m_pCubeTexture->Bind(0);
	m_pCubeMesh->Draw();
}
void CApplication::RenderPyramid()
{
	m_pToonShader->Use();
	m_pToonShader->SetInt("u_Texture", 0);
	m_pToonShader->SetVec3("u_LightDirection", VEC3_LIGHT_DIRECTION);
	m_pToonShader->SetVec3("u_LightColor", VEC3_LIGHT_COLOR);

	glm::mat4 matModel = glm::translate(MAT4_IDENTITY, VEC3_PYRAMID_POSITION);
	m_pToonShader->SetMat4("u_Model", matModel);
	m_pToonShader->SetMat4("u_View", m_matView);
	m_pToonShader->SetMat4("u_Projection", m_matProjection);

	m_pPyramidTexture->Bind(0);
	m_pPyramidMesh->Draw();
}

void CApplication::RenderGround()
{
	m_pToonShader->Use();
	m_pToonShader->SetInt("u_Texture", 0);
	m_pToonShader->SetVec3("u_LightDirection", VEC3_LIGHT_DIRECTION);
	m_pToonShader->SetVec3("u_LightColor", VEC3_LIGHT_COLOR);

	glm::mat4 matModel = MAT4_IDENTITY;
	m_pToonShader->SetMat4("u_Model", matModel);
	m_pToonShader->SetMat4("u_View", m_matView);
	m_pToonShader->SetMat4("u_Projection", m_matProjection);

	m_pGrassTexture->Bind(0);
	m_pGroundMesh->Draw();
}

void CApplication::RenderSun()
{
	m_pUnlitShader->Use();
	m_pUnlitShader->SetMat4("u_View", m_matView);
	m_pUnlitShader->SetMat4("u_Projection", m_matProjection);

	glm::vec3 vSunPosition = -glm::normalize(VEC3_LIGHT_DIRECTION) * F_SUN_DISTANCE;
	glm::mat4 matSunModel = glm::translate(MAT4_IDENTITY, vSunPosition);
	matSunModel = glm::scale(matSunModel, glm::vec3(F_SUN_RADIUS, F_SUN_RADIUS, F_SUN_RADIUS));
	m_pUnlitShader->SetMat4("u_Model", matSunModel);
	m_pUnlitShader->SetVec3("u_Color", VEC3_SUN_COLOR);
	m_pSphereMesh->Draw();
}

void CApplication::RenderShadow(CMesh* a_pMesh, const glm::vec3& a_vPosition, float a_fFootprintScale, float a_fOffset)
{
	m_pUnlitShader->Use();
	m_pUnlitShader->SetMat4("u_View", m_matView);
	m_pUnlitShader->SetMat4("u_Projection", m_matProjection);

	glm::vec3 vLightDirection = glm::normalize(VEC3_LIGHT_DIRECTION);
	glm::vec3 vShadowPosition = glm::vec3(a_vPosition.x + vLightDirection.x * a_fOffset, F_SHADOW_BLOB_HEIGHT, a_vPosition.z + vLightDirection.z * a_fOffset);
	glm::mat4 matShadowModel = glm::translate(MAT4_IDENTITY, vShadowPosition);
	matShadowModel = glm::scale(matShadowModel, glm::vec3(a_fFootprintScale, F_SHADOW_BLOB_THICKNESS, a_fFootprintScale));
	m_pUnlitShader->SetMat4("u_Model", matShadowModel);
	m_pUnlitShader->SetVec3("u_Color", VEC3_SHADOW_BLOB_COLOR);
	a_pMesh->Draw();
}

void CApplication::RenderOutline(CMesh* a_pMesh, const glm::vec3& a_vPosition)
{
	m_pUnlitShader->Use();
	m_pUnlitShader->SetMat4("u_View", m_matView);
	m_pUnlitShader->SetMat4("u_Projection", m_matProjection);

	glm::mat4 matOutlineModel = glm::translate(MAT4_IDENTITY, a_vPosition);
	matOutlineModel = glm::scale(matOutlineModel, glm::vec3(F_OUTLINE_SCALE, F_OUTLINE_SCALE, F_OUTLINE_SCALE));
	m_pUnlitShader->SetMat4("u_Model", matOutlineModel);
	m_pUnlitShader->SetVec3("u_Color", VEC3_OUTLINE_COLOR);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	a_pMesh->Draw();
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
}

void CApplication::RenderSkybox()
{
	m_pSkyboxShader->Use();

	glm::mat4 matSkyboxView = glm::mat4(glm::mat3(m_matView));
	m_pSkyboxShader->SetMat4("u_View", matSkyboxView);
	m_pSkyboxShader->SetMat4("u_Projection", m_matProjection);

	m_pSkybox->Bind(0);
	m_pSkyboxShader->SetInt("u_Skybox", 0);

	glDepthFunc(GL_LEQUAL);
	m_pSkyboxMesh->Draw();
	glDepthFunc(GL_LESS);
}
