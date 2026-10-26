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
* 07.10.2026 ER Mouse drag, scroll wheel and aspect ratio for V3
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
	/// Polls key and mouse state once per frame; Escape flags the window
	/// to close, cursor movement becomes this frame's mouse deltas.
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

	/// <summary>
	/// Horizontal mouse movement of this frame while the left button is held.
	/// </summary>
	float GetMouseDeltaX() const;

	/// <summary>
	/// Vertical mouse movement of this frame while the left button is held.
	/// </summary>
	float GetMouseDeltaY() const;

	/// <summary>
	/// Scroll wheel movement collected since the last frame.
	/// </summary>
	float GetScrollDelta() const;

	/// <summary>
	/// Current width divided by height, for the projection matrix.
	/// </summary>
	float GetAspectRatio() const;

private:
	/// <summary>
	/// GLFW callback for the scroll wheel. GLFW is a C library — the
	/// window user pointer carries our object into the callback.
	/// </summary>
	static void HandleScroll(GLFWwindow* a_pWindow, double a_dOffsetX, double a_dOffsetY);

	GLFWwindow* m_pWindow = nullptr;
	double m_dLastCursorX = 0.0;
	double m_dLastCursorY = 0.0;
	bool m_bHasLastCursor = false;
	float m_fMouseDeltaX = 0.0f;
	float m_fMouseDeltaY = 0.0f;
	float m_fScrollCollected = 0.0f;
	float m_fScrollDelta = 0.0f;
};
