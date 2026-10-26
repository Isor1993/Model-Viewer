/*****************************************************************************
* Project : Model Viewer
* File    : Geometry.cpp
* Date    : 07.10.2026
* Author  : Eric Rosenberg
* Note    : Sphere generator written with AI assistance (Claude),
*           reviewed and walked through together on 07.10.2026.
*           Cube and pyramid built together step by step on 26.10.2026,
*           typed by hand.
*
* Description :
* Implementation of the geometry builders — see Geometry.h.
*
* History :
* 07.10.2026 ER Created with GenerateSphereVertices (V3)
* 07.10.2026 ER GenerateSkyboxCubeVertices (V5)
* 07.10.2026 ER GenerateGroundPlaneVertices (V5 ground)
* 08.10.2026 ER Sphere winding flipped to CCW — front culling exposed it (O1)
* 08.10.2026 ER V6 review: named minimum sphere size and skybox vertex count
* 26.10.2026 ER Cube and pyramid generators plus the two face helpers
*               (OCE feedback: additional objects)
******************************************************************************/
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cmath>

#include "Geometry.h"
#include "Mesh.h"

// A sphere needs its two pole caps; a ring only closes from 3 segments.
constexpr int32_t I_MIN_SPHERE_RINGS = 2;
constexpr int32_t I_MIN_SPHERE_SEGMENTS = 3;

// 6 faces x 2 triangles x 3 corners = 36 vertices.
constexpr int32_t I_SKYBOX_VERTEX_COUNT = 36;

// Same 6 x 2 x 3 as the skybox cube, but with real normals and uvs.
constexpr int32_t I_CUBE_VERTEX_COUNT = 36;

// Base quad (6) plus four triangle sides (4 x 3).
constexpr int32_t I_PYRAMID_VERTEX_COUNT = 18;

/// <summary>
/// Point on the sphere surface. Phi runs top to bottom (latitude, 0 at
/// the north pole), theta runs around (longitude).
/// </summary>
static glm::vec3 SpherePoint(float a_fPhi, float a_fTheta, float a_fRadius)
{
	float fX = a_fRadius * std::sin(a_fPhi) * std::cos(a_fTheta);
	float fY = a_fRadius * std::cos(a_fPhi);
	float fZ = a_fRadius * std::sin(a_fPhi) * std::sin(a_fTheta);
	return glm::vec3(fX, fY, fZ);
}

/// <summary>
/// Appends one vertex in the CMesh layout (position, normal, uv).
/// </summary>
static void PushVertex(std::vector<float>& a_vertices, const glm::vec3& a_vPosition, const glm::vec3& a_vNormal, float a_fU, float a_fV)
{
	a_vertices.push_back(a_vPosition.x);
	a_vertices.push_back(a_vPosition.y);
	a_vertices.push_back(a_vPosition.z);
	a_vertices.push_back(a_vNormal.x);
	a_vertices.push_back(a_vNormal.y);
	a_vertices.push_back(a_vNormal.z);
	a_vertices.push_back(a_fU);
	a_vertices.push_back(a_fV);
}

/// <summary>
/// Appends one quad face as two triangles, all six vertices sharing
/// a_vNormal. Corners are named as seen from outside, wound CCW.
/// </summary>
static void PushQuadFace(std::vector<float>& a_vertices, const glm::vec3& a_vBottomLeft, const glm::vec3& a_vBottomRight, const glm::vec3& a_vTopRight, const glm::vec3& a_vTopLeft, const glm::vec3& a_vNormal)
{
	PushVertex(a_vertices, a_vBottomLeft, a_vNormal, 0.0f, 0.0f);
	PushVertex(a_vertices, a_vBottomRight, a_vNormal, 1.0f, 0.0f);
	PushVertex(a_vertices, a_vTopRight, a_vNormal, 1.0f, 1.0f);

	PushVertex(a_vertices, a_vTopRight, a_vNormal, 1.0f, 1.0f);
	PushVertex(a_vertices, a_vTopLeft, a_vNormal, 0.0f, 1.0f);
	PushVertex(a_vertices, a_vBottomLeft, a_vNormal, 0.0f, 0.0f);
}

/// <summary>
/// Appends one triangle whose normal is computed from the corner order:
/// wound CCW seen from outside, the cross product points outward.
/// </summary>
static void PushTriangleFace(std::vector<float>& a_vertices, const glm::vec3& a_vA, const glm::vec3& a_vB, const glm::vec3& a_vC)
{
	glm::vec3 vNormal = glm::normalize(glm::cross(a_vB - a_vA, a_vC - a_vA));
	PushVertex(a_vertices, a_vA, vNormal, 0.0f, 0.0f);
	PushVertex(a_vertices, a_vB, vNormal, 1.0f, 0.0f);
	PushVertex(a_vertices, a_vC, vNormal, 0.5f, 1.0f);
}

std::vector<float> GenerateSphereVertices(int32_t a_iRings, int32_t a_iSegments, float a_fRadius)
{
	std::vector<float> vertices;
	if (a_iRings < I_MIN_SPHERE_RINGS || a_iSegments < I_MIN_SPHERE_SEGMENTS || a_fRadius <= 0.0f)
	{
		return vertices;
	}

	const int32_t iTrianglesPerCell = 2;
	const int32_t iVerticesPerTriangle = 3;
	vertices.reserve(static_cast<size_t>(a_iRings) * a_iSegments * iTrianglesPerCell * iVerticesPerTriangle * I_MESH_FLOATS_PER_VERTEX);

	for (int32_t iRing = 0; iRing < a_iRings; ++iRing)
	{
		float fPhiTop = glm::pi<float>() * static_cast<float>(iRing) / static_cast<float>(a_iRings);
		float fPhiBottom = glm::pi<float>() * static_cast<float>(iRing + 1) / static_cast<float>(a_iRings);
		float fVTop = 1.0f - static_cast<float>(iRing) / static_cast<float>(a_iRings);
		float fVBottom = 1.0f - static_cast<float>(iRing + 1) / static_cast<float>(a_iRings);

		for (int32_t iSegment = 0; iSegment < a_iSegments; ++iSegment)
		{
			float fThetaLeft = glm::two_pi<float>() * static_cast<float>(iSegment) / static_cast<float>(a_iSegments);
			float fThetaRight = glm::two_pi<float>() * static_cast<float>(iSegment + 1) / static_cast<float>(a_iSegments);
			float fULeft = static_cast<float>(iSegment) / static_cast<float>(a_iSegments);
			float fURight = static_cast<float>(iSegment + 1) / static_cast<float>(a_iSegments);

			glm::vec3 vTopLeft = SpherePoint(fPhiTop, fThetaLeft, a_fRadius);
			glm::vec3 vTopRight = SpherePoint(fPhiTop, fThetaRight, a_fRadius);
			glm::vec3 vBottomLeft = SpherePoint(fPhiBottom, fThetaLeft, a_fRadius);
			glm::vec3 vBottomRight = SpherePoint(fPhiBottom, fThetaRight, a_fRadius);

			// On a sphere the normal is just the direction away from the center.
			// Wound counter-clockwise seen from outside (OpenGL front-face default).
			PushVertex(vertices, vTopLeft, vTopLeft / a_fRadius, fULeft, fVTop);
			PushVertex(vertices, vBottomRight, vBottomRight / a_fRadius, fURight, fVBottom);
			PushVertex(vertices, vBottomLeft, vBottomLeft / a_fRadius, fULeft, fVBottom);

			PushVertex(vertices, vTopLeft, vTopLeft / a_fRadius, fULeft, fVTop);
			PushVertex(vertices, vTopRight, vTopRight / a_fRadius, fURight, fVTop);
			PushVertex(vertices, vBottomRight, vBottomRight / a_fRadius, fURight, fVBottom);
		}
	}
	return vertices;
}

std::vector<float> GenerateSkyboxCubeVertices()
{
	// The eight corners of a unit cube around the origin.
	const glm::vec3 CORNERS[8] = {
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f)
	};

	// Two triangles per face; the cube is only ever seen from inside.
	const int32_t INDICES[I_SKYBOX_VERTEX_COUNT] = {
		0, 1, 2,  2, 3, 0,
		4, 7, 6,  6, 5, 4,
		0, 3, 7,  7, 4, 0,
		1, 5, 6,  6, 2, 1,
		3, 2, 6,  6, 7, 3,
		0, 4, 5,  5, 1, 0
	};

	std::vector<float> vertices;
	vertices.reserve(I_SKYBOX_VERTEX_COUNT * I_MESH_FLOATS_PER_VERTEX);
	for (int32_t i = 0; i < I_SKYBOX_VERTEX_COUNT; ++i)
	{
		PushVertex(vertices, CORNERS[INDICES[i]], glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	}
	return vertices;
}

std::vector<float> GenerateGroundPlaneVertices(float a_fHalfSize, float a_fHeight, float a_fUVTiles)
{
	std::vector<float> vertices;
	if (a_fHalfSize <= 0.0f || a_fUVTiles <= 0.0f)
	{
		return vertices;
	}

	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 vBackLeft = glm::vec3(-a_fHalfSize, a_fHeight, -a_fHalfSize);
	glm::vec3 vBackRight = glm::vec3(a_fHalfSize, a_fHeight, -a_fHalfSize);
	glm::vec3 vFrontLeft = glm::vec3(-a_fHalfSize, a_fHeight, a_fHalfSize);
	glm::vec3 vFrontRight = glm::vec3(a_fHalfSize, a_fHeight, a_fHalfSize);

	vertices.reserve(6 * I_MESH_FLOATS_PER_VERTEX);
	PushVertex(vertices, vBackLeft, UP, 0.0f, a_fUVTiles);
	PushVertex(vertices, vFrontLeft, UP, 0.0f, 0.0f);
	PushVertex(vertices, vFrontRight, UP, a_fUVTiles, 0.0f);

	PushVertex(vertices, vBackLeft, UP, 0.0f, a_fUVTiles);
	PushVertex(vertices, vFrontRight, UP, a_fUVTiles, 0.0f);
	PushVertex(vertices, vBackRight, UP, a_fUVTiles, a_fUVTiles);
	return vertices;
}

std::vector<float> GenerateCubeVertices(float a_fHalfSize)
{
	std::vector<float> vertices;
	if (a_fHalfSize <= 0.0f)
	{
		return vertices;
	}

	const glm::vec3 CORNERS[8] = {
		glm::vec3(-a_fHalfSize, -a_fHalfSize, -a_fHalfSize),
		glm::vec3(a_fHalfSize, -a_fHalfSize, -a_fHalfSize),
		glm::vec3(a_fHalfSize,  a_fHalfSize, -a_fHalfSize),
		glm::vec3(-a_fHalfSize,  a_fHalfSize, -a_fHalfSize),
		glm::vec3(-a_fHalfSize, -a_fHalfSize,  a_fHalfSize),
		glm::vec3(a_fHalfSize, -a_fHalfSize,  a_fHalfSize),
		glm::vec3(a_fHalfSize,  a_fHalfSize,  a_fHalfSize),
		glm::vec3(-a_fHalfSize,  a_fHalfSize,  a_fHalfSize)
	};

	// One call per face: front, back, left, right, top, bottom.
	vertices.reserve(I_CUBE_VERTEX_COUNT * I_MESH_FLOATS_PER_VERTEX);
	PushQuadFace(vertices, CORNERS[4], CORNERS[5], CORNERS[6], CORNERS[7], glm::vec3(0.0f, 0.0f, 1.0f));
	PushQuadFace(vertices, CORNERS[1], CORNERS[0], CORNERS[3], CORNERS[2], glm::vec3(0.0f, 0.0f, -1.0f));
	PushQuadFace(vertices, CORNERS[0], CORNERS[4], CORNERS[7], CORNERS[3], glm::vec3(-1.0f, 0.0f, 0.0f));
	PushQuadFace(vertices, CORNERS[5], CORNERS[1], CORNERS[2], CORNERS[6], glm::vec3(1.0f, 0.0f, 0.0f));
	PushQuadFace(vertices, CORNERS[7], CORNERS[6], CORNERS[2], CORNERS[3], glm::vec3(0.0f, 1.0f, 0.0f));
	PushQuadFace(vertices, CORNERS[0], CORNERS[1], CORNERS[5], CORNERS[4], glm::vec3(0.0f, -1.0f, 0.0f));
	return vertices;
}

std::vector<float> GeneratePyramidVertices(float a_fHalfSize, float a_fHeight)
{
	std::vector<float> vertices;
	if (a_fHalfSize <= 0.0f || a_fHeight <= 0.0f)
	{
		return vertices;
	}

	const glm::vec3 BASE[4] = {
		glm::vec3(-a_fHalfSize, 0.0f, -a_fHalfSize),
		glm::vec3(a_fHalfSize, 0.0f, -a_fHalfSize),
		glm::vec3(a_fHalfSize, 0.0f,  a_fHalfSize),
		glm::vec3(-a_fHalfSize, 0.0f,  a_fHalfSize)
	};
	const glm::vec3 TIP = glm::vec3(0.0f, a_fHeight, 0.0f);

	vertices.reserve(I_PYRAMID_VERTEX_COUNT * I_MESH_FLOATS_PER_VERTEX);
	// The base faces down, so its corners are named as seen from below.
	PushQuadFace(vertices, BASE[0], BASE[1], BASE[2], BASE[3], glm::vec3(0.0f, -1.0f, 0.0f));
	// Each side runs along its base edge, then up to the tip.
	PushTriangleFace(vertices, BASE[3], BASE[2], TIP);
	PushTriangleFace(vertices, BASE[2], BASE[1], TIP);
	PushTriangleFace(vertices, BASE[1], BASE[0], TIP);
	PushTriangleFace(vertices, BASE[0], BASE[3], TIP);
	return vertices;
}
