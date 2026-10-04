/*****************************************************************************
* Project : Model Viewer
* File    : main.cpp
* Date    : 04.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Entry point of the model viewer. Creates the window (CWindow) and runs
* the render loop; render state like the clear color stays here. Further
* modules (camera, mesh, shader, texture, skybox) follow the same
* pattern as the viewer grows.
*
* History :
* 04.10.2026 ER Created
* 04.10.2026 ER GLFW window, GLAD setup and render loop (course baseline)
* 04.10.2026 ER Window code moved into CWindow (RAII)
******************************************************************************/
#include <glad/glad.h>
#include "Window.h"

#include <cstdint>


constexpr int32_t I_WINDOW_WIDTH = 1280;
constexpr int32_t I_WINDOW_HEIGHT = 720;
constexpr const char* SZ_WINDOW_TITLE = "Model Viewer";

constexpr float F_CLEAR_RED = 0.07f;
constexpr float F_CLEAR_GREEN = 0.13f;
constexpr float F_CLEAR_BLUE = 0.17f;
constexpr float F_CLEAR_ALPHA = 1.0f;


/// <summary>
/// Opens the window, prepares OpenGL and runs the render loop until close.
/// </summary>
int main()
{
	CWindow window(I_WINDOW_WIDTH, I_WINDOW_HEIGHT, SZ_WINDOW_TITLE);
	if (!window.IsValid())
	{
		return -1;
	}

	glClearColor(F_CLEAR_RED, F_CLEAR_GREEN, F_CLEAR_BLUE, F_CLEAR_ALPHA);

	while (!window.ShouldClose())
	{
		window.ProcessInput();
		glClear(GL_COLOR_BUFFER_BIT);

		window.SwapBuffers();
		window.PollEvents();
	}
	
	return 0;
}
