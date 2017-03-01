#include "CameraAzimuth.h"
#include "Application.h"
#include "Mtx44.h"
#include "Vector3.h"

/******************************************************************************/
/*!
\brief
Default constructor
*/
/******************************************************************************/
CameraAzimuth::CameraAzimuth()
{
}

/******************************************************************************/
/*!
\brief
Destructor
*/
/******************************************************************************/
CameraAzimuth::~CameraAzimuth()
{
}

/******************************************************************************/
/*!
\brief
Initialize Camera2

\param pos - position of Camera2
\param target - where the Camera2 is looking at
\param up - up vector of Camera2
*/
/******************************************************************************/
void CameraAzimuth::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
	Vector3 view = (target - position).Normalized();
	//view.Normalize();
	Vector3 right = view.Cross(up).Normalized();
	//right.Normalize();
	this->up = right.Cross(view);
}

/******************************************************************************/
/*!
\brief
Reset the Camera2 settings
*/
/******************************************************************************/
void CameraAzimuth::Reset()
{
}

/******************************************************************************/
/*!
\brief
To be called every frame. Camera2 will get user inputs and update its position and orientation

\param dt - frame time
*/
/******************************************************************************/
void CameraAzimuth::Update(double dt)
{
	Vector3 view = (target - position).Normalized();
	//view.Normalize();
	Vector3 right = view.Cross(up).Normalized();
	//right.Normalize();

	if (Application::IsKeyPressed('A'))
	{
		float yaw = -40.0f * dt;
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}
	if (Application::IsKeyPressed('D'))
	{
		float yaw = 40.0f * dt;
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}
	//if (Application::IsKeyPressed('W'))
	//{
	//	float pitch = -40.0f * dt;
	//	Mtx44 rotation;
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	position = rotation * position;
	//	up = right.Cross(view);
	//}
	//if (Application::IsKeyPressed('S'))
	//{
	//	float pitch = 40.0f * dt;
	//	Mtx44 rotation;
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	position = rotation * position;
	//	up = right.Cross(view);
	//}
	if (Application::IsKeyPressed('N'))
	{
		position += 40.0f * view * dt;
	}
	if (Application::IsKeyPressed('M'))
	{
		position -= 40.0f * view * dt;
	}
}