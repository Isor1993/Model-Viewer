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
* 07.10.2026 ER Mouse drag, scroll wheel and aspect ratio for V3
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
	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetScrollCallback(m_pWindow, HandleScroll);

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

	double dCursorX = 0.0;
	double dCursorY = 0.0;
	glfwGetCursorPos(m_pWindow, &dCursorX, &dCursorY);

	m_fMouseDeltaX = 0.0f;
	m_fMouseDeltaY = 0.0f;
	if (m_bHasLastCursor && glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		m_fMouseDeltaX = static_cast<float>(dCursorX - m_dLastCursorX);
		m_fMouseDeltaY = static_cast<float>(dCursorY - m_dLastCursorY);
	}
	m_dLastCursorX = dCursorX;
	m_dLastCursorY = dCursorY;
	m_bHasLastCursor = true;

	// The callback collects between frames, this frame reads one snapshot.
	m_fScrollDelta = m_fScrollCollected;
	m_fScrollCollected = 0.0f;
}

float CWindow::GetMouseDeltaX() const
{
	return m_fMouseDeltaX;
}

float CWindow::GetMouseDeltaY() const
{
	return m_fMouseDeltaY;
}

float CWindow::GetScrollDelta() const
{
	return m_fScrollDelta;
}

float CWindow::GetAspectRatio() const
{
	int32_t iWidth = 0;
	int32_t iHeight = 0;
	glfwGetFramebufferSize(m_pWindow, &iWidth, &iHeight);
	if (iHeight <= 0)
	{
		return 1.0f;
	}
	return static_cast<float>(iWidth) / static_cast<float>(iHeight);
}

void CWindow::HandleScroll(GLFWwindow* a_pWindow, double, double a_dOffsetY)
{
	CWindow* pSelf = static_cast<CWindow*>(glfwGetWindowUserPointer(a_pWindow));
	if (pSelf == nullptr)
	{
		return;
	}
	pSelf->m_fScrollCollected += static_cast<float>(a_dOffsetY);
}

void CWindow::SwapBuffers()
{
	glfwSwapBuffers(m_pWindow);
}

void CWindow::PollEvents()
{
	glfwPollEvents();
}
