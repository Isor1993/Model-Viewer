/*****************************************************************************
* Project : Model Viewer
* File    : Camera.h
* Date    : 07.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Orbit camera, CPU state only (owns no GPU resource): two angles and a
* distance describe a position on an invisible sphere around the origin.
* GetViewMatrix turns that into the view matrix, always looking at the
* model in the center.
*
* History :
* 07.10.2026 ER Created (V3)
* 08.10.2026 ER ClampPitchToGround private — only used internally (V6 review)
******************************************************************************/
#pragma once

#include <glm/glm.hpp>

class CCamera
{
public:

	CCamera(float a_fDistance);

	/// <summary>
	/// Turns the camera around the object (mouse drag, in pixels).
	/// </summary>
	void Rotate(float a_fYawDeltaPixels, float a_fPitchDeltaPixels);

	/// <summary>
	/// Moves the camera closer or further away (scroll wheel steps).
	/// </summary>
	void Zoom(float a_fWheelDelta);

	/// <summary>
	/// View matrix for the current orbit position, looking at the origin.
	/// </summary>
	glm::mat4 GetViewMatrix() const;

private:

	/// <summary>
	/// Keeps the eye above the ground: raises the pitch floor so the
	/// camera never dips below the minimum eye height.
	/// </summary>
	void ClampPitchToGround();

	float m_fYawDegrees = 0.0f;
	float m_fPitchDegrees = 0.0f;
	float m_fDistance = 3.0f;
};
