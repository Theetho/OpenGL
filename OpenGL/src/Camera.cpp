#include "pch.h"
#include "Camera.h"

Camera::Camera() :
	m_Speed(0.05f),
	m_Sensitivity(0.05f),
	m_Phi(0.f),
	m_Theta(0.f),
	m_Orientation(3, 3, 3),
	m_VerticalAxis(0, 0, 1),
	m_ForwardMotion(0, 1, 0),
	m_Position(),
	m_Target(0,0,0),
	m_LateralMotion()
{
	m_Orientation = m_Target - m_Position;
	m_Orientation = glm::normalize(m_Orientation);

	InitAngles();
}

Camera::Camera(const glm::vec3 & eyes, const  glm::vec3 & center, const glm::vec3 & up, const float & speed, const float & sensitivity) :
	m_Speed(speed),
	m_Sensitivity(sensitivity),
	m_Phi(0.f),
	m_Theta(0.f),
	m_Orientation(),
	m_VerticalAxis(up),
	m_Position(eyes),
	m_Target(center),
	m_LateralMotion(),
	m_ForwardMotion()
{
	m_Orientation = m_Target - m_Position;
	m_Orientation = glm::normalize(m_Orientation);
	InitAngles();
}

Camera::~Camera()
{
}

void Camera::InitAngles()
{
	if (m_VerticalAxis.x)
	{
		m_Phi = asin(m_Orientation.x);
		m_Theta = acos(m_Orientation.y / cos(m_Phi));

		if (m_Orientation.y < 0)
			m_Theta *= -1.f;
	}
	else if (m_VerticalAxis.y)
	{
		m_Phi = asin(m_Orientation.y);
		m_Theta = acos(m_Orientation.z / cos(m_Phi));

		if (m_Orientation.z < 0)
			m_Theta *= -1.f ;
	}
	else if (m_VerticalAxis.z)
	{
		m_Phi = asin(m_Orientation.x);
		m_Theta = acos(m_Orientation.z / cos(m_Phi));

		if (m_Orientation.x < 0)
			m_Theta *= -1.f;
	}

	m_LateralMotion = glm::cross(m_VerticalAxis, m_Orientation);
	m_LateralMotion = glm::normalize(m_LateralMotion);
	m_ForwardMotion = glm::cross(m_VerticalAxis, m_LateralMotion);
	m_ForwardMotion = glm::normalize(m_ForwardMotion);

	m_Phi *= (180 / M_PI);
	m_Theta *= (180 / M_PI);

	Orientate();
}

void Camera::Orientate()
{
	// Update angles
	m_Phi -= (InputManager::GetMouseRelativePosition().y * m_Sensitivity);
	m_Theta -= (InputManager::GetMouseRelativePosition().x * m_Sensitivity);

	// Limitation of the angle for calculation
	if (m_Phi > 89.f) m_Phi = 89.f;
	else if (m_Phi < -89.f) m_Phi = -89.f;

	// Update the orientation vector depending on the vertical axis
	UpdateOrientation();

	// Update the motions
	m_LateralMotion = glm::cross(m_VerticalAxis, m_Orientation);
	m_LateralMotion = glm::normalize(m_LateralMotion);
	m_ForwardMotion = glm::cross(m_VerticalAxis, m_LateralMotion);
	m_ForwardMotion = glm::normalize(m_ForwardMotion);

	// Remove the component on the vertical axis, so the camera doesn't move inside or over the ground
	if (m_VerticalAxis.x)
	{
		m_ForwardMotion.x = 0;
		m_LateralMotion.x = 0;
	}
	else if (m_VerticalAxis.y)
	{
		m_ForwardMotion.y = 0;
		m_LateralMotion.y = 0;
	}
	else if (m_VerticalAxis.z)
	{
		m_ForwardMotion.z = 0;
		m_LateralMotion.z = 0;
	}

	// Update target point
	m_Target = m_Orientation + m_Position;
}

// Move is basically the functions that handles inputs related to the camera
void Camera::Move()
{
	if (InputManager::DidMouseMove())
		Orientate();
	
	// QWERTY INPUTS
	// Move forward
	if (GetKey(SDL_SCANCODE_W))
	{
		m_Position -= (m_ForwardMotion * m_Speed);
		m_Target = m_Position + m_Orientation;
	}
	// Move backward
	if (GetKey(SDL_SCANCODE_S))
	{
		m_Position += (m_ForwardMotion * m_Speed);
		m_Target = m_Position + m_Orientation;
	}
	// Move right
	if (GetKey(SDL_SCANCODE_A))
	{
		m_Position += (m_LateralMotion * m_Speed);
		m_Target = m_Position + m_Orientation;
	}
	// Move left
	if (GetKey(SDL_SCANCODE_D))
	{
		m_Position -= (m_LateralMotion * m_Speed);
		m_Target = m_Position + m_Orientation;
	}
	// Move up
	if (GetKey(SDL_SCANCODE_SPACE))
	{
		m_Position += (m_VerticalAxis * m_Speed);
		m_Target = m_Position + m_Orientation;
	}
	// Move down
	if (GetKey(SDL_SCANCODE_LSHIFT) && m_Position.y > 1)
	{
		m_Position -= (m_VerticalAxis * m_Speed);
		if (m_Position.y < 1)
			m_Position.y = 1;
		m_Target = m_Position + m_Orientation;
	}

	// Zooms

}

void Camera::LookAt()
{
	m_View = glm::lookAt(m_Position, m_Target, m_VerticalAxis);
}

void Camera::UpdateOrientation()
{
	// Converting the angles in radian
	float phi_radian = m_Phi * M_PI / 180;
	float theta_radian = m_Theta * M_PI / 180;

	if (m_VerticalAxis.x)
	{
		m_Orientation.x = sin(phi_radian);
		m_Orientation.y = cos(phi_radian) * cos(theta_radian);
		m_Orientation.z = cos(phi_radian) * sin(theta_radian);
	}
	else if (m_VerticalAxis.y)
	{
		m_Orientation.x = cos(phi_radian) * sin(theta_radian);
		m_Orientation.y = sin(phi_radian);
		m_Orientation.z = cos(phi_radian) * cos(theta_radian);
	}
	else if (m_VerticalAxis.z)
	{
		m_Orientation.x = cos(phi_radian) * cos(theta_radian);
		m_Orientation.y = cos(phi_radian) * sin(theta_radian);
		m_Orientation.z = sin(phi_radian);
	}
}
