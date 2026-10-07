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
******************************************************************************/
#include <glad/glad.h>
#include "SkyBox.h"

#include <stb_image.h>

#include <iostream>
#include <string>

static const char* SZ_FACE_FILE_NAMES[6] = { "right.png","left.png","top.png","bottom.png","front.png","back.png" };

CSkyBox::CSkyBox(const char* a_szFolderPath)
{
	stbi_set_flip_vertically_on_load(0);

	glGenTextures(1, &m_uiTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTextureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int32_t i = 0;i < 6;i++)
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

		if (iChannels == 4)
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

