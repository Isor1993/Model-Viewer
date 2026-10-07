#include <glad/glad.h>
#include "Mesh.h"

constexpr int32_t I_FLOATS_PER_VERTEX = 3;
constexpr int32_t I_POSITION_LOCATION = 0;





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
	glBufferData(GL_ARRAY_BUFFER, a_iVertexCount * I_FLOATS_PER_VERTEX * sizeof(float), a_pVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(I_POSITION_LOCATION, I_FLOATS_PER_VERTEX, GL_FLOAT, GL_FALSE, I_FLOATS_PER_VERTEX * sizeof(float), nullptr);
	glEnableVertexAttribArray(I_POSITION_LOCATION);

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
