#include "Camera4.h"
#include "Application.h"
#include "Mtx44.h"

Camera4::Camera4()
{
}

Camera4::~Camera4()
{
}

void Camera4::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	view = (target - position).Normalized();
	right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = right.Cross(view).Normalized();
	defaultUp.Set(0, 1, 0);
}

void Camera4::Update(double dt)
{
	view = (target - position).Normalized();
	right = view.Cross(up);
	static const float CAMERA_SPEED = 50.f;

	if (Application::IsKeyPressed(37)) // Left
	{
		float yaw = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(39)) // Right
	{
		float yaw = (float)(-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(38)) // Up
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}

	if (Application::IsKeyPressed(40)) // Down
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed('N'))
	{
		Vector3 direction = target - position;
		if (direction.Length() > 5)
		{
			Vector3 view = (target - position).Normalized();
			position += view * (float)(30.f * dt);
		}
	}

	if (Application::IsKeyPressed('A'))
	{
		position.x = position.x - right.x;
		position.z = position.z - right.z;
		target = position + view;
	}
	if (Application::IsKeyPressed('D'))
	{
		position.x = position.x + right.x;
		position.z = position.z + right.z;
		target = position + view;
	}
	if (Application::IsKeyPressed('W'))
	{
		position.x = position.x + view.x;
		position.z = position.z + view.z;
		target = position + view;
	}
	if (Application::IsKeyPressed('S'))
	{
		position.x = position.x - view.x;
		position.z = position.z - view.z;
		target = position + view;
	}

	if (Application::IsKeyPressed('M'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * (float)(30.f * dt);
	}
	if (Application::IsKeyPressed('L'))
	{
		Reset();
	}
}

void Camera4::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}