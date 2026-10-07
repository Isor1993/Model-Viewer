/*****************************************************************************
* Project : Model Viewer
* File    : Mesh.h
* Date    : 06.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Owns one mesh on the GPU (RAII): uploads the vertex array into a VBO,
* records the layout in a VAO, frees both in the destructor. Draw()
* binds the VAO and issues the draw call.
*
* History :
* 06.10.2026 ER Created
* 07.10.2026 ER Vertex layout position+normal+uv, three attributes (V3)
******************************************************************************/
#pragma once
#include <cstdint>

/// <summary>
/// One vertex = position (3) + normal (3) + uv (2) floats, interleaved.
/// </summary>
constexpr int32_t I_MESH_FLOATS_PER_VERTEX = 8;


class CMesh
{
public:
	CMesh(const float* a_pVertices, int32_t a_iVertexCount);
	~CMesh();

	bool IsValid() const;
	void Draw();

private:
	/// <summary>
	/// VAO = Vertex Array Object
	/// </summary>
	uint32_t m_uiVAO = 0;

	/// <summary>
	/// VBO = Vertex Buffer Object
	/// </summary>
	uint32_t m_uiVBO = 0;

	int32_t m_iVertexCount = 0;




};

