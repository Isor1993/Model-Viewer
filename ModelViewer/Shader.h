#pragma once

#include <cstdint>

class CShader
{
public:

	CShader(const char* a_szVertexPath, const char* a_szFragmentPath);

	~CShader();

	bool IsValid() const;

	void Use();

private:
	uint32_t m_uiProgramID = 0;
};

