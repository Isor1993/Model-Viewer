/*****************************************************************************
* Project : Model Viewer
* File    : Window.cpp
* Date    : 04.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Implementation of CWindow — see Window.h.
*
* History :
* 04.10.2026 ER Created
******************************************************************************/
#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

/// <summary>
/// GLFW callback: keeps the drawable area in sync when the window is resized.
/// </summary>
/// <param name="a_iWidth">New framebuffer width in pixels.</param>
/// <param name="a_iHeight">New framebuffer height in pixels.</param>
void HandleFramebufferSize(GLFWwindow*, int a_iWidth, int a_iHeight)
{
	glViewport(0, 0, a_iWidth, a_iHeight);
}

CWindow::CWindow(int32_t a_iWidth, int32_t a_iHeight, const char* a_szTitle)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_pWindow = glfwCreateWindow(a_iWidth, a_iHeight, a_szTitle, nullptr, nullptr);

	if (m_pWindow == nullptr)
	{
		std::cout << "ERROR: GLFW window creation failed" << std::endl;		
		return;
	}
	glfwMakeContextCurrent(m_pWindow);
	glfwSetFramebufferSizeCallback(m_pWindow, HandleFramebufferSize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR: GLAD initialization failed" << std::endl;
		m_pWindow = nullptr;		
		return;
	}	
}

CWindow::~CWindow()
{
	glfwTerminate();
}

bool CWindow::IsValid() const
{	
	if (m_pWindow == nullptr)
	{
		return false;
	}
	return true;
}

bool CWindow::ShouldClose() const
{
	return glfwWindowShouldClose(m_pWindow) == GLFW_TRUE;
}

void CWindow::ProcessInput()
{
	if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_pWindow, true);
	}	
}

void CWindow::SwapBuffers()
{
	glfwSwapBuffers(m_pWindow);
}

void CWindow::PollEvents()
{
	glfwPollEvents();
}
