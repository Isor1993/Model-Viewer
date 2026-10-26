/*****************************************************************************
* Project : Model Viewer
* File    : SkyBox.h
* Date    : 07.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Owns one cubemap texture on the GPU (RAII): loads the six face
* images in the constructor and uploads them as one cubemap, deletes
* the GPU texture in the destructor. Bind() puts it on a texture unit
* so a samplerCube uniform can read it.
*
* History :
* 07.10.2026 ER Created (V5)
******************************************************************************/
#pragma once

#include <cstdint>

class CSkyBox
{
public:

	/// <summary>
	/// Loads the six face images from the folder as one GPU cubemap.
	/// </summary>
	CSkyBox(const char* a_szFolderPath);

	~CSkyBox();

	/// <summary>
	/// True when all six faces loaded and uploaded.
	/// </summary>
	bool IsValid() const;

	/// <summary>
	/// Puts this cubemap on the given texture unit for the next draws.
	/// </summary>
	void Bind(uint32_t a_uiUnit);

private:
	uint32_t m_uiTextureID = 0;
};




