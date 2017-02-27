#include "Camera4.h"
#include "Application.h"
#include "Mtx44.h"
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;
extern GLFWwindow* m_window;

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

void Camera4::Update(double dt, float* rotate,float& fuel)
{
	view = (target - position).Normalized();
	right = view.Cross(up);
	static const float CAMERA_SPEED = 50.f;

	
	if (Application::IsKeyPressed(38)) // Up
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, 0, right.z);
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
		rotation.SetToRotation(pitch, right.x, 0, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (fuel > 0)
	{
		if (Application::IsKeyPressed('A'))
		{
			if (Application::IsKeyPressed('W'))
			{
				float yaw = (float)(CAMERA_SPEED * dt);
				Mtx44 rotation;
				rotation.SetToRotation(yaw, 0, 1, 0);
				view.y = 0;
				view = rotation * view;
				target = position + view;
				up = rotation * up;
				*rotate += yaw;
				fuel = fuel - 10;
			}
			else if (Application::IsKeyPressed('S'))
			{
				float yaw = (float)(-CAMERA_SPEED * dt);
				Mtx44 rotation;
				rotation.SetToRotation(yaw, 0, 1, 0);
				view.y = 0;
				view = rotation * view;
				target = position + view;
				up = rotation * up;
				*rotate += yaw;
				fuel = fuel - 10;
			}
		}
		if (Application::IsKeyPressed('D'))
		{
			if (Application::IsKeyPressed('W'))
			{
				float yaw = (float)(-CAMERA_SPEED * dt);
				Mtx44 rotation;
				rotation.SetToRotation(yaw, 0, 1, 0);
				view.y = 0;
				view = rotation * view;
				target = position + view;
				up = rotation * up;
				*rotate += yaw;
				fuel = fuel - 10;
			}
			else if (Application::IsKeyPressed('S'))
			{
				float yaw = (float)(CAMERA_SPEED * dt);
				Mtx44 rotation;
				rotation.SetToRotation(yaw, 0, 1, 0);
				view.y = 0;
				view = rotation * view;
				target = position + view;
				up = rotation * up;
				*rotate += yaw;
				fuel = fuel - 10;
			}
		}
		if (Application::IsKeyPressed('W'))
		{
			view.y = 0;
			for (int i = 0; i < ObjPos.size(); i++)
			{
				if (((position + view) - ObjPos[i]).Length()>2)
				{
					collision = false;
				}
				else
				{
					collision = true;
					break;
				}
			}
			if (collision == false)
			{
				position = position + view;
				target = position + view;
				fuel = fuel - 10;
			}
		}
		if (Application::IsKeyPressed('S'))
		{
			view.y = 0;
			for (int i = 0; i < ObjPos.size(); i++)
			{
				if (((position -  view) - ObjPos[i]).Length()>2)
				{
					collision = false;
				}
				else
				{
					collision = true;
					break;
				}
			}
			if (collision == false)
			{
				position = position - view;
				target = position + view;
				fuel = fuel - 10;
			}
		}
	}
	if (fuel < 0)
	{
		fuel = 0;
	}
	
}

void Camera4::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}