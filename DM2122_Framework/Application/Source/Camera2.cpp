#include "Camera2.h"
#include "Application.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera2::Camera2()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera2::~Camera2()
{
}

/******************************************************************************/
/*!
\brief
Initialize camera

\param pos - position of camera
\param target - where the camera is looking at
\param up - up vector of camera
*/
/******************************************************************************/
void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
	this->view = target - pos;
	this->right = view.Cross(up);
	phi = 0;
	theta = 0;
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera2::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera2 will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera2::Update(double dt)
{
	view = target - position;
	right = view.Cross(up);

	// Moving Camera
	if (Application::IsKeyPressed('W'))
	{
		position.y += 50.f * dt;
	}
	if (Application::IsKeyPressed('S'))
	{
		position.y -= 50.f * dt;
	}
	if (Application::IsKeyPressed('A'))
	{
		position.x -= 50.f * dt;
	}
	if (Application::IsKeyPressed('D'))
	{
		position.x += 50.f * dt;
	}

	// Zooming In And Out
	if (Application::IsKeyPressed(187)) // Zoom In (Equal Key)
	{
		position += view * dt;
	}
	if (Application::IsKeyPressed(189)) // Zoom Out (Minus Key)
	{
		position -= view * dt;
	}

	// Rotating Camera
	if (Application::IsKeyPressed(38)) // Up Key
	{
		theta += 1.f;
	}
	if (Application::IsKeyPressed(40)) // Down Key
	{
		theta -= 1.f;
	}
	if (Application::IsKeyPressed(37)) // Left Key
	{
		phi -= 1.f;
	}
	if (Application::IsKeyPressed(39)) // Right Key
	{
		phi += 1.f;
	}
}

float Camera2::getPhi()
{
	return phi;
}

float Camera2::getTheta()
{
	return theta;
}