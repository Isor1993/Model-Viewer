#pragma once
#include<cstdint>


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

