/*****************************************************************************
* Project : Model Viewer
* File    : Texture.cpp
* Date    : 07.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Implementation of CTexture — see Texture.h. This file also carries the
* one allowed STB_IMAGE_IMPLEMENTATION define of the project.
*
* History :
* 07.10.2026 ER Created (V3)
* 08.10.2026 ER V6 review: include order, named RGBA channel count
******************************************************************************/
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "Texture.h"

// stb reports the channel count; 4 means the image carries an alpha channel.
constexpr int32_t I_CHANNELS_RGBA = 4;

CTexture::CTexture(const char* a_szPath)
{
	// Image rows start top-left, OpenGL expects bottom-left.
	stbi_set_flip_vertically_on_load(1);

	int32_t iWidth = 0;
	int32_t iHeight = 0;
	int32_t iChannels = 0;
	unsigned char* pPixels = stbi_load(a_szPath, &iWidth, &iHeight, &iChannels, 0);
	if (pPixels == nullptr)
	{
		std::cout << "ERROR: Could not load texture: " << a_szPath << std::endl;
		return;
	}

	int32_t iInternalFormat = GL_RGB;
	uint32_t uiPixelFormat = GL_RGB;
	if (iChannels == I_CHANNELS_RGBA)
	{
		iInternalFormat = GL_RGBA;
		uiPixelFormat = GL_RGBA;
	}

	glGenTextures(1, &m_uiTextureID);
	glBindTexture(GL_TEXTURE_2D, m_uiTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// The zeros: mipmap base level 0, and the border argument must be 0 by spec.
	glTexImage2D(GL_TEXTURE_2D, 0, iInternalFormat, iWidth, iHeight, 0, uiPixelFormat, GL_UNSIGNED_BYTE, pPixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(pPixels);
}

CTexture::~CTexture()
{
	glDeleteTextures(1, &m_uiTextureID);
}

bool CTexture::IsValid() const
{
	if (m_uiTextureID == 0)
	{
		return false;
	}
	return true;
}

void CTexture::Bind(uint32_t a_uiUnit)
{
	glActiveTexture(GL_TEXTURE0 + a_uiUnit);
	glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
}
