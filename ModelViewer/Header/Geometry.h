/*****************************************************************************
* Project : Model Viewer
* File    : Geometry.h
* Date    : 07.10.2026
* Author  : Eric Rosenberg
* Note    : Sphere generator written with AI assistance (Claude),
*           reviewed and walked through together on 07.10.2026.
*           Cube and pyramid built together step by step on 26.10.2026,
*           typed by hand.
*
* Description :
* Classless geometry builders (CPU only, no GPU calls). They produce
* interleaved vertex data in the CMesh layout; main feeds the result
* into a CMesh, which owns the GPU side.
*
* History :
* 07.10.2026 ER Created with GenerateSphereVertices (V3)
* 07.10.2026 ER GenerateSkyboxCubeVertices (V5)
* 07.10.2026 ER GenerateGroundPlaneVertices (V5 ground)
* 26.10.2026 ER GenerateCubeVertices and GeneratePyramidVertices
*               (OCE feedback: additional objects)
******************************************************************************/
#pragma once

#include <cstdint>
#include <vector>

/// <summary>
/// Builds a UV sphere as interleaved vertex data (position, normal, uv),
/// two triangles per grid cell. Returns empty on senseless arguments.
/// </summary>
std::vector<float> GenerateSphereVertices(int32_t a_iRings, int32_t a_iSegments, float a_fRadius);

/// <summary>
/// Builds a unit cube around the origin in the CMesh layout; normals and
/// uvs stay zero. The skybox shader only reads the corner positions.
/// </summary>
std::vector<float> GenerateSkyboxCubeVertices();

/// <summary>
/// Builds a flat square ground plane (two triangles, normals up). The
/// uvs run 0 to a_fUVTiles so a repeating texture tiles across it.
/// </summary>
std::vector<float> GenerateGroundPlaneVertices(float a_fHalfSize, float a_fHeight, float a_fUVTiles);

/// <summary>
/// Builds a textured cube around the origin, one normal per face —
/// corners are stored once per adjacent face, so edges stay hard.
/// </summary>
std::vector<float> GenerateCubeVertices(float a_fHalfSize);

/// <summary>
/// Builds a square pyramid: base quad on y = 0 (the origin is the
/// footprint), tip at a_fHeight. Side normals come from cross products.
/// </summary>
std::vector<float> GeneratePyramidVertices(float a_fHalfSize, float a_fHeight);