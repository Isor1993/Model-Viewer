/*****************************************************************************
* Project : Model Viewer
* File    : SkyBox.cpp
* Date    : 07.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Implementation of CSkyBox — see SkyBox.h. The texture container is
* created before the load loop because all six face uploads need the
* bound cubemap; the error path tears it down again.
*
* History :
* 07.10.2026 ER Created (V5)
* 08.10.2026 ER V6 review: include order, face/channel constants, flip comment
******************************************************************************/
#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>
#include <string>

#include "SkyBox.h"

constexpr int32_t I_CUBE_FACE_COUNT = 6;

// stb reports the channel count; 4 means the image carries an alpha channel.
constexpr int32_t I_CHANNELS_RGBA = 4;

static const char* SZ_FACE_FILE_NAMES[I_CUBE_FACE_COUNT] = { "right.png","left.png","top.png","bottom.png","front.png","back.png" };

CSkyBox::CSkyBox(const char* a_szFolderPath)
{
	// Cubemap faces are read top-down — no flip here (the 2D textures do flip).
	stbi_set_flip_vertically_on_load(0);

	glGenTextures(1, &m_uiTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTextureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int32_t i = 0; i < I_CUBE_FACE_COUNT; ++i)
	{
		std::string sPath = std::string(a_szFolderPath) + "/" + SZ_FACE_FILE_NAMES[i];
		int32_t iWidth = 0;
		int32_t iHeight = 0;
		int32_t iChannels = 0;
		unsigned char* pPixels = stbi_load(sPath.c_str(), &iWidth, &iHeight, &iChannels, 0);

		if (pPixels == nullptr)
		{
			std::cout << "ERROR: Could not load skybox face: " << sPath << std::endl;
			glDeleteTextures(1, &m_uiTextureID);
			m_uiTextureID = 0;
			return;
		}

		int32_t iInternalFormat = GL_RGB;
		uint32_t uiPixelFormat = GL_RGB;

		if (iChannels == I_CHANNELS_RGBA)
		{
			iInternalFormat = GL_RGBA;
			uiPixelFormat = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, iInternalFormat, iWidth, iHeight, 0, uiPixelFormat, GL_UNSIGNED_BYTE, pPixels);
		stbi_image_free(pPixels);
	}
}

CSkyBox::~CSkyBox()
{
	glDeleteTextures(1, &m_uiTextureID);
}

bool CSkyBox::IsValid() const
{
	if (m_uiTextureID == 0)
	{
		return false;
	}
	return true;
}

void CSkyBox::Bind(uint32_t a_uiUnit)
{
	glActiveTexture(GL_TEXTURE0 + a_uiUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTextureID);
}

