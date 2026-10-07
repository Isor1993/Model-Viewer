#include <glad/glad.h>
#include "Shader.h"
#include <fstream>
#include <iostream>
#include <string>


static std::string ReadTextFile(const char* a_szPath)
{
	std::ifstream file(a_szPath);
	if (!file.is_open())
	{
		std::cout << "ERROR: Could not open file: " << a_szPath << std::endl;
		return "";
	}

	std::string sText;
	std::string sLine;
	while (std::getline(file, sLine))
	{
		sText += sLine + "\n";
	}
	return sText;

}

static uint32_t CompileShader(uint32_t a_uiType, const char* a_szSource, const char* a_szPath)
{
	uint32_t uiShaderID = glCreateShader(a_uiType);
	glShaderSource(uiShaderID, 1, &a_szSource, nullptr);
	glCompileShader(uiShaderID);

	int32_t iSuccess = 0;
	glGetShaderiv(uiShaderID, GL_COMPILE_STATUS, &iSuccess);

	if (iSuccess == 0)
	{
		char szLog[512];
		glGetShaderInfoLog(uiShaderID, 512, nullptr, szLog);
		std::cout << "ERROR: Shader compile failed (" << a_szPath << "):\n" << szLog << std::endl;
		glDeleteShader(uiShaderID);
		return 0;
	}
	return uiShaderID;

}

static uint32_t LinkProgram(uint32_t a_uiVertexID, uint32_t a_uiFragmentID)
{
	uint32_t uiProgramID = glCreateProgram();
	glAttachShader(uiProgramID, a_uiVertexID);
	glAttachShader(uiProgramID, a_uiFragmentID);
	glLinkProgram(uiProgramID);

	int32_t iSuccess = 0;
	glGetProgramiv(uiProgramID, GL_LINK_STATUS, &iSuccess);
	if (iSuccess == 0)
	{
		char szLog[512];
		glGetProgramInfoLog(uiProgramID, 512, nullptr, szLog);
		std::cout << "ERROR: Shader program link failed:\n" << szLog << std::endl;
		glDeleteProgram(uiProgramID);
		return 0;
	}
	return uiProgramID;
}

CShader::CShader(const char* a_szVertexPath, const char* a_szFragmentPath)
{
	std::string sVertexSource = ReadTextFile(a_szVertexPath);
	std::string sFragmentSource = ReadTextFile(a_szFragmentPath);

	if (sVertexSource.empty() || sFragmentSource.empty())
	{
		return;
	}

	uint32_t uiVertexID = CompileShader(GL_VERTEX_SHADER, sVertexSource.c_str(), a_szVertexPath);
	uint32_t uiFragmentID = CompileShader(GL_FRAGMENT_SHADER, sFragmentSource.c_str(), a_szFragmentPath);

	if (uiVertexID == 0 || uiFragmentID == 0)
	{
		glDeleteShader(uiVertexID);
		glDeleteShader(uiFragmentID);
		return;
	}

	m_uiProgramID = LinkProgram(uiVertexID, uiFragmentID);

	glDeleteShader(uiVertexID);
	glDeleteShader(uiFragmentID);
}

CShader::~CShader()
{
	glDeleteProgram(m_uiProgramID);
}

bool CShader::IsValid() const
{
	if (m_uiProgramID == 0)
	{
		return false;
	}
	return true;
}

void CShader::Use()
{
	glUseProgram(m_uiProgramID);
}

