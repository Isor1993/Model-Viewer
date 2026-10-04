/*****************************************************************************
* Project : Model Viewer
* File    : Window.h
* Date    : 04.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Owns the GLFW window and the OpenGL context behind it. Creates both in
* the constructor (RAII), cleans up in the destructor. main talks to the
* window only through the small methods below, never through the raw
* GLFW pointer.
*
* History :
* 04.10.2026 ER Created
******************************************************************************/
#pragma once

#include <cstdint>

struct GLFWwindow;

/// <summary>
/// RAII wrapper around one GLFW window with an OpenGL 4.6 core context.
/// </summary>
class CWindow
{
public:
	CWindow(int32_t a_iWidth, int32_t a_iHeight, const char* a_szTitle);
	~CWindow();

	/// <summary>
	/// True when window and OpenGL context were created successfully.
	/// </summary>
	bool IsValid() const;

	/// <summary>
	/// True once the user asked to close (X button or Escape).
	/// </summary>
	bool ShouldClose() const;

	/// <summary>
	/// Polls the key state once per frame; Escape flags the window to close.
	/// </summary>
	void ProcessInput();

	/// <summary>
	/// Shows the finished back buffer (double buffering).
	/// </summary>
	void SwapBuffers();

	/// <summary>
	/// Empties the event queue (input, resize, close request).
	/// </summary>
	void PollEvents();

private:
	GLFWwindow* m_pWindow = nullptr;
};
