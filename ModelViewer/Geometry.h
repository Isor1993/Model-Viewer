/*****************************************************************************
* Project : Model Viewer
* File    : Geometry.h
* Date    : 07.10.2026
* Author  : Eric Rosenberg
* Note    : Sphere generator written with AI assistance (Claude),
*           reviewed and walked through together on 07.10.2026.
*
* Description :
* Classless geometry builders (CPU only, no GPU calls). They produce
* interleaved vertex data in the CMesh layout; main feeds the result
* into a CMesh, which owns the GPU side.
*
* History :
* 07.10.2026 ER Created with GenerateSphereVertices (V3)
******************************************************************************/
#pragma once

#include <cstdint>
#include <vector>

/// <summary>
/// Builds a UV sphere as interleaved vertex data (position, normal, uv),
/// two triangles per grid cell. Returns empty on senseless arguments.
/// </summary>
std::vector<float> GenerateSphereVertices(int32_t a_iRings, int32_t a_iSegments, float a_fRadius);
