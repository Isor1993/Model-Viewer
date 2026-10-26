/*****************************************************************************
* Project : Model Viewer
* File    : Texture.h
* Date    : 07.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Owns one 2D texture on the GPU (RAII): loads the image file with
* stb_image in the constructor, uploads it and builds mipmaps, deletes
* the GPU texture in the destructor. Bind() puts it on a texture unit
* so a sampler uniform can read it.
*
* History :
* 07.10.2026 ER Created (V3)
******************************************************************************/
#pragma once

#include <cstdint>

class CTexture
{
public:

	/// <summary>
	/// Loads the image file and uploads it as a GPU texture with mipmaps.
	/// </summary>
	CTexture(const char* a_szPath);

	~CTexture();

	/// <summary>
	/// True when loading and uploading produced a usable texture.
	/// </summary>
	bool IsValid() const;

	/// <summary>
	/// Puts this texture on the given texture unit for the next draws.
	/// </summary>
	void Bind(uint32_t a_uiUnit);

private:
	uint32_t m_uiTextureID = 0;
};
