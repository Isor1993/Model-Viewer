/*****************************************************************************
* Project : Model Viewer
* File    : Shader.h
* Date    : 05.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Owns one GPU shader program (RAII): loads both shader files, compiles
* and links them in the constructor, deletes the program in the
* destructor. Draw code activates it with Use() and feeds per-draw
* values through the Set* methods.
*
* History :
* 05.10.2026 ER Created
* 06.10.2026 ER Compile errors print the file path (sabotage test finding)
* 07.10.2026 ER SetMat4 and SetInt for the V3 uniforms
* 07.10.2026 ER SetVec3 for the V4 light uniforms
* 08.10.2026 ER GetUniformLocation helper deduplicates the Set methods
******************************************************************************/
#pragma once

#include <cstdint>
#include <glm/glm.hpp>

class CShader
{
public:

	/// <summary>
	/// Loads, compiles and links both shader files into one GPU program.
	/// </summary>
	CShader(const char* a_szVertexPath, const char* a_szFragmentPath);

	~CShader();

	/// <summary>
	/// True when compiling and linking produced a usable program.
	/// </summary>
	bool IsValid() const;

	/// <summary>
	/// Makes this program the active one for the next draw calls.
	/// </summary>
	void Use();

	/// <summary>
	/// Uploads a 4x4 matrix to the named uniform. Call Use() first.
	/// </summary>
	void SetMat4(const char* a_szName, const glm::mat4& a_matValue);

	/// <summary>
	/// Sets an int uniform, e.g. which texture unit a sampler reads.
	/// </summary>
	void SetInt(const char* a_szName, int32_t a_iValue);

	/// <summary>
	/// Uploads a vec3 to the named uniform, e.g. light direction or color.
	/// </summary>
	void SetVec3(const char* a_szName, const glm::vec3& a_vec3Value);

private:

	/// <summary>
	/// Looks up the uniform by name; prints an error when the program has
	/// no such uniform. The returned -1 is safe to hand to glUniform* —
	/// the API ignores it by spec, so callers need no guard.
	/// </summary>
	int32_t GetUniformLocation(const char* a_szName) const;

	uint32_t m_uiProgramID = 0;
};
