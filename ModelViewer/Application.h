/*****************************************************************************
* Project : Model Viewer
* File    : Application.h
* Date    : 08.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Owns the whole scene (camera, shaders, meshes, textures, skybox) and
* runs the render loop. All owned objects live behind pointer members:
* born as nullptr, created with new in the Initialize groups, freed
* with delete in the destructor. The window is borrowed from main and
* never owned — the pointer type documents the ownership.
*
* History :
* 08.10.2026 ER Created — scene and loop moved out of main (V6 cleanup)
* 08.10.2026 ER Reworked to pointer members, Initialize groups and six
*               single-purpose render stations (design review with Isor)
******************************************************************************/
#pragma once

#include <glm/glm.hpp>

#include "Window.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "SkyBox.h"

/// <summary>
/// The viewer itself: owns every scene object and runs the render loop.
/// </summary>
class CApplication
{
public:

	CApplication();

	/// <summary>
	/// Frees every owned object (reverse creation order). The borrowed
	/// window belongs to main and is not touched.
	/// </summary>
	~CApplication();

	/// <summary>
	/// Creates all scene objects in groups; false when anything failed.
	/// </summary>
	/// <param name="a_pWindow">Valid window from main, borrowed only.</param>
	bool Initialize(CWindow* a_pWindow);

	/// <summary>
	/// Runs the render loop until the window asks to close.
	/// </summary>
	void Run();

private:

	/// <summary>
	/// Creates the toon, unlit and skybox shader programs.
	/// </summary>
	bool InitializeShaders();

	/// <summary>
	/// Generates the vertex data and uploads sphere, ground and skybox
	/// cube as GPU meshes; the CPU data dies at the end of the method.
	/// </summary>
	bool InitializeMeshes();

	/// <summary>
	/// Loads the grass and ball textures and the skybox cubemap.
	/// </summary>
	bool InitializeTextures();

	/// <summary>
	/// Toon ball in the center, at the origin.
	/// </summary>
	void RenderBall();

	/// <summary>
	/// Grass ground plane under the ball.
	/// </summary>
	void RenderGround();

	/// <summary>
	/// White sun disc, placed opposite the light direction.
	/// </summary>
	void RenderSun();

	/// <summary>
	/// Flattened contact shadow under the ball.
	/// </summary>
	void RenderShadow();

	/// <summary>
	/// Inverted-hull outline: the sphere again, scaled up, black, with
	/// front faces culled so only the back shell's rim survives.
	/// </summary>
	void RenderOutline();

	/// <summary>
	/// Skybox last, at depth 1.0 (xyww), so it only fills untouched pixels.
	/// </summary>
	void RenderSkybox();

	CWindow* m_pWindow = nullptr;

	CCamera* m_pCamera = nullptr;

	CShader* m_pToonShader = nullptr;
	CShader* m_pUnlitShader = nullptr;
	CShader* m_pSkyboxShader = nullptr;

	CMesh* m_pSphereMesh = nullptr;
	CMesh* m_pGroundMesh = nullptr;
	CMesh* m_pSkyboxMesh = nullptr;

	CTexture* m_pGrassTexture = nullptr;
	CTexture* m_pBallTexture = nullptr;
	CSkyBox* m_pSkybox = nullptr;

	// The matrices of the running frame — set once per frame in Run,
	// read by every render station.
	glm::mat4 m_matView = glm::mat4(1.0f);
	glm::mat4 m_matProjection = glm::mat4(1.0f);
};
