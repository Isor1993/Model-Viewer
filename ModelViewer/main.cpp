/*****************************************************************************
* Project : Model Viewer
* File    : main.cpp
* Date    : 04.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Entry point only: creates the window, hands it to CApplication and
* runs the loop. Everything the viewer shows lives in CApplication;
* the scene constants live in Application.cpp.
*
* History :
* 04.10.2026 ER Created
* 04.10.2026 ER GLFW window, GLAD setup and render loop (course baseline)
* 04.10.2026 ER Window code moved into CWindow (RAII)
* 07.10.2026 ER MVP matrix chain at the triangle (V3 start)
* 07.10.2026 ER Sphere replaces the triangle (Geometry, 8-float layout)
* 07.10.2026 ER Depth test on, grass test texture loaded (V3)
* 08.10.2026 ER Outline hull — scaled second pass, front culling (O1)
* 08.10.2026 ER Scene and loop moved into CApplication (V6 cleanup)
* 08.10.2026 ER CApplication gets the window via Initialize (pointer design)
******************************************************************************/
#include <cstdint>

#include "Window.h"
#include "Application.h"

constexpr int32_t I_WINDOW_WIDTH = 1280;
constexpr int32_t I_WINDOW_HEIGHT = 720;

constexpr const char* SZ_WINDOW_TITLE = "Model Viewer";

/// <summary>
/// Opens the window, builds the application around it and runs it.
/// </summary>
int main()
{
	CWindow applicationWindow(I_WINDOW_WIDTH, I_WINDOW_HEIGHT, SZ_WINDOW_TITLE);
	if (!applicationWindow.IsValid())
	{
		return -1;
	}

	CApplication application;
	if (!application.Initialize(&applicationWindow))
	{
		return -1;
	}

	application.Run();
	return 0;
}
