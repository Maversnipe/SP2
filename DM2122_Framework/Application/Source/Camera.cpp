#include "Camera.h"
#include "Application.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
Camera::Camera()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
Camera::~Camera()
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
void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

/******************************************************************************/
/*!
\brief
Reset the camera settings
*/
/******************************************************************************/
void Camera::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void Camera::Update(double dt)
{
	//if (Application::IsKeyPressed(37)) // Left
	//{
	//}
	//if (Application::IsKeyPressed(39)) // Right
	//{

	//}
	//if (Application::IsKeyPressed(38)) // Up
	//{
	//	/*switch (select)
	//	{
	//	case 1:
	//		select = 0;
	//	}*/
	//	if (select > 0 && select < 2)
	//		select--;
	//}

	//if (Application::IsKeyPressed(40)) // Down
	//{
	//	/*switch (select)
	//	{
	//	case 0: 
	//		select = 1;
	//	}*/
	//	if (select > 0 && select < 2)
	//		select++;
	//}
	if (Application::IsKeyPressed('N'))
	{
	}

	if (Application::IsKeyPressed('A'))
	{
	}
	if (Application::IsKeyPressed('D'))
	{
	}
	if (Application::IsKeyPressed('W'))
	{
	}
	if (Application::IsKeyPressed('S'))
	{

	}
}