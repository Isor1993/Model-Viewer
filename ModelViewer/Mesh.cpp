/*****************************************************************************
* Project : Model Viewer
* File    : Mesh.cpp
* Date    : 06.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Implementation of CMesh — see Mesh.h.
*
* History :
* 06.10.2026 ER Created
* 07.10.2026 ER Vertex layout position+normal+uv, three attributes (V3)
* 08.10.2026 ER V6 review: shader-contract comments, formatting pass
******************************************************************************/
#include <glad/glad.h>

#include "Mesh.h"

// Attribute slots — must match the layout(location = N) lines in the shaders.
constexpr int32_t I_POSITION_LOCATION = 0;
constexpr int32_t I_NORMAL_LOCATION = 1;
constexpr int32_t I_UV_LOCATION = 2;

// Floats per attribute in the interleaved layout (3 + 3 + 2 = 8 per vertex).
constexpr int32_t I_POSITION_FLOATS = 3;
constexpr int32_t I_NORMAL_FLOATS = 3;
constexpr int32_t I_UV_FLOATS = 2;

constexpr int32_t I_NORMAL_OFFSET_FLOATS = I_POSITION_FLOATS;
constexpr int32_t I_UV_OFFSET_FLOATS = I_POSITION_FLOATS + I_NORMAL_FLOATS;


CMesh::CMesh(const float* a_pVertices, int32_t a_iVertexCount)
{
	if (a_pVertices == nullptr || a_iVertexCount <= 0)
	{
		return;
	}

	m_iVertexCount = a_iVertexCount;

	glGenVertexArrays(1, &m_uiVAO);
	glGenBuffers(1, &m_uiVBO);

	glBindVertexArray(m_uiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);

	const int32_t iStrideBytes = I_MESH_FLOATS_PER_VERTEX * static_cast<int32_t>(sizeof(float));
	glBufferData(GL_ARRAY_BUFFER, static_cast<int64_t>(a_iVertexCount) * iStrideBytes, a_pVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(I_POSITION_LOCATION, I_POSITION_FLOATS, GL_FLOAT, GL_FALSE, iStrideBytes, nullptr);
	glEnableVertexAttribArray(I_POSITION_LOCATION);

	glVertexAttribPointer(I_NORMAL_LOCATION, I_NORMAL_FLOATS, GL_FLOAT, GL_FALSE, iStrideBytes, reinterpret_cast<const void*>(I_NORMAL_OFFSET_FLOATS * sizeof(float)));
	glEnableVertexAttribArray(I_NORMAL_LOCATION);

	glVertexAttribPointer(I_UV_LOCATION, I_UV_FLOATS, GL_FLOAT, GL_FALSE, iStrideBytes, reinterpret_cast<const void*>(I_UV_OFFSET_FLOATS * sizeof(float)));
	glEnableVertexAttribArray(I_UV_LOCATION);

	// Unbind so later GL calls cannot accidentally change this VAO.
	glBindVertexArray(0);
}

CMesh::~CMesh()
{
	glDeleteVertexArrays(1, &m_uiVAO);
	glDeleteBuffers(1, &m_uiVBO);
}

bool CMesh::IsValid() const
{
	if (m_uiVAO == 0 || m_uiVBO == 0)
	{
		return false;
	}
	return true;
}

void CMesh::Draw()
{
	glBindVertexArray(m_uiVAO);
	glDrawArrays(GL_TRIANGLES, 0, m_iVertexCount);
}
