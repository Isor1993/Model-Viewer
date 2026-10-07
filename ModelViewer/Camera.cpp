/*****************************************************************************
* Project : Model Viewer
* File    : Camera.cpp
* Date    : 07.10.2026
* Author  : Eric Rosenberg
*
* Description :
* Implementation of CCamera — see Camera.h.
*
* History :
* 07.10.2026 ER Created (V3)
******************************************************************************/
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

constexpr float F_DEGREES_PER_PIXEL = 0.3f;
constexpr float F_DISTANCE_PER_WHEEL_STEP = 0.3f;
constexpr float F_PITCH_LIMIT_DEGREES = 89.0f;
constexpr float F_MIN_DISTANCE = 1.5f;
constexpr float F_MAX_DISTANCE = 20.0f;

CCamera::CCamera(float a_fDistance)
{
	m_fDistance = a_fDistance;
}

void CCamera::Rotate(float a_fYawDeltaPixels, float a_fPitchDeltaPixels)
{
	m_fYawDegrees -= a_fYawDeltaPixels * F_DEGREES_PER_PIXEL;
	m_fPitchDegrees += a_fPitchDeltaPixels * F_DEGREES_PER_PIXEL;

	// The poles stay out of reach, otherwise the up direction flips.
	if (m_fPitchDegrees > F_PITCH_LIMIT_DEGREES)
	{
		m_fPitchDegrees = F_PITCH_LIMIT_DEGREES;
	}
	if (m_fPitchDegrees < -F_PITCH_LIMIT_DEGREES)
	{
		m_fPitchDegrees = -F_PITCH_LIMIT_DEGREES;
	}
}

void CCamera::Zoom(float a_fWheelDelta)
{
	// Wheel up (positive) zooms in, so the distance shrinks.
	m_fDistance -= a_fWheelDelta * F_DISTANCE_PER_WHEEL_STEP;

	if (m_fDistance < F_MIN_DISTANCE)
	{
		m_fDistance = F_MIN_DISTANCE;
	}
	if (m_fDistance > F_MAX_DISTANCE)
	{
		m_fDistance = F_MAX_DISTANCE;
	}
}

glm::mat4 CCamera::GetViewMatrix() const
{
	float fYaw = glm::radians(m_fYawDegrees);
	float fPitch = glm::radians(m_fPitchDegrees);

	// Same recipe as the sphere generator: two angles and a radius
	// become one point — the camera rides its own orbit sphere.
	float fX = m_fDistance * std::cos(fPitch) * std::sin(fYaw);
	float fY = m_fDistance * std::sin(fPitch);
	float fZ = m_fDistance * std::cos(fPitch) * std::cos(fYaw);

	return glm::lookAt(
		glm::vec3(fX, fY, fZ),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
}
