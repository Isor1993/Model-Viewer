/*****************************************************************************
* Project : Model Viewer
* File    : Geometry.cpp
* Date    : 07.10.2026
* Author  : Eric Rosenberg
* Note    : Sphere generator written with AI assistance (Claude),
*           reviewed and walked through together on 07.10.2026.
*
* Description :
* Implementation of the geometry builders — see Geometry.h.
*
* History :
* 07.10.2026 ER Created with GenerateSphereVertices (V3)
******************************************************************************/
#include "Geometry.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

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

std::vector<float> GenerateSphereVertices(int32_t a_iRings, int32_t a_iSegments, float a_fRadius)
{
	std::vector<float> vertices;
	if (a_iRings < 2 || a_iSegments < 3 || a_fRadius <= 0.0f)
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
			PushVertex(vertices, vTopLeft, vTopLeft / a_fRadius, fULeft, fVTop);
			PushVertex(vertices, vBottomLeft, vBottomLeft / a_fRadius, fULeft, fVBottom);
			PushVertex(vertices, vBottomRight, vBottomRight / a_fRadius, fURight, fVBottom);

			PushVertex(vertices, vTopLeft, vTopLeft / a_fRadius, fULeft, fVTop);
			PushVertex(vertices, vBottomRight, vBottomRight / a_fRadius, fURight, fVBottom);
			PushVertex(vertices, vTopRight, vTopRight / a_fRadius, fURight, fVTop);
		}
	}
	return vertices;
}
