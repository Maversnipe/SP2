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

void Camera4::Update(double dt, float* rotate)
{
	view = (target - position).Normalized();
	right = view.Cross(up);
	static const float CAMERA_SPEED = 50.f;

	move = (0, 0, 0);
	//up = right.Cross(view).Normalized();

	//Mtx44 rotation, yaw, pitch;
	//yaw = mouseY;
	//pitch = mouseX;
	//Mtx44 camPitch, camYaw;

	//double CamSpeed = 5.f;
	//int width, height;
	//glfwGetWindowSize(m_window, &width, &height);
	//glfwGetCursorPos(m_window, &xpos, &ypos);

	//int mid_x = width / 2;
	//int mid_y = height / 2;

	//glfwSetCursorPos(m_window, mid_x, mid_y);

	//rotateHori = (mid_x - xpos) * dt * CamSpeed;
	//rotateVert = (mid_y - ypos) * dt * CamSpeed;

	//// control vertical limit
	//verticalAngle += dt *rotateVert;
	//horizontalAngle += dt * rotateHori;
	//if (verticalAngle > 1)
	//{
	//	verticalAngle = 1;
	//	rotateVert = 0;
	//}
	//else if (verticalAngle < -1)
	//{
	//	verticalAngle = -1;
	//	rotateVert = 0;
	//}
	//right.y = 0;
	//right.Normalize();
	//up = right.Cross(view).Normalized();

	//view = target - position;
	//right = view.Cross(up).Normalized();

	//camPitch.SetToRotation(rotateVert, right.x, right.y, right.z);
	//camYaw.SetToRotation(rotateHori, 0, 1, 0);
	//rotation = camPitch * camYaw;
	//view = (rotation * view).Normalized();
	//target = (position + view);
	//up = camYaw * up;
	//right = camPitch * right;


	//verticalAngle = Math::RadianToDegree(verticalAngle);
	//horizontalAngle = Math::RadianToDegree(horizontalAngle);
	////*vertical = verticalAngle;
	////*horizontal = horizontalAngle;
	//verticalAngle = Math::DegreeToRadian(verticalAngle);
	//horizontalAngle = Math::DegreeToRadian(horizontalAngle);

	//if (Application::IsKeyPressed(37)) // Left
	//{
	//	float yaw = (float)(CAMERA_SPEED * dt);
	//	Mtx44 rotation;
	//	rotation.SetToRotation(yaw, 0, 1, 0);
	//	view = rotation * view;
	//	target = position + view;
	//	up = rotation * up;
	//}
	//if (Application::IsKeyPressed(39)) // Right
	//{
	//	float yaw = (float)(-CAMERA_SPEED * dt);
	//	Mtx44 rotation;
	//	rotation.SetToRotation(yaw, 0, 1, 0);
	//	view = rotation * view;
	//	target = position + view;
	//	up = rotation * up;
	//}
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
	/*if (Application::IsKeyPressed('N'))
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
	}*/
	if (car.fuel > 0)
	{
		if (Application::IsKeyPressed('A'))
		{
			if (Application::IsKeyPressed('W'))
			{
				float yaw = (float)(CAMERA_SPEED * dt);
				Mtx44 rotation;
				rotation.SetToRotation(yaw, 0, 1, 0);
				view = rotation * view;
				target = position + view;
				up = rotation * up;
				*rotate += yaw;
				car.fuel = car.fuel - 10;
			}
			else if (Application::IsKeyPressed('S'))
			{
				float yaw = (float)(-CAMERA_SPEED * dt);
				Mtx44 rotation;
				rotation.SetToRotation(yaw, 0, 1, 0);
				view = rotation * view;
				target = position + view;
				up = rotation * up;
				*rotate += yaw;
				car.fuel = car.fuel - 10;
			}
		}
		if (Application::IsKeyPressed('D'))
		{
			if (Application::IsKeyPressed('W'))
			{
				float yaw = (float)(-CAMERA_SPEED * dt);
				Mtx44 rotation;
				rotation.SetToRotation(yaw, 0, 1, 0);
				view = rotation * view;
				target = position + view;
				up = rotation * up;
				*rotate += yaw;
				car.fuel = car.fuel - 10;
			}
			else if (Application::IsKeyPressed('S'))
			{
				float yaw = (float)(CAMERA_SPEED * dt);
				Mtx44 rotation;
				rotation.SetToRotation(yaw, 0, 1, 0);
				view = rotation * view;
				target = position + view;
				up = rotation * up;
				*rotate += yaw;
				car.fuel = car.fuel - 10;
			}
		}
		if (Application::IsKeyPressed('W'))
		{
			move.x += view.x;
			move.z += view.z;
			position.x = position.x + view.x;
			position.z = position.z + view.z;
			target = position + view;
			car.fuel = car.fuel - 10;
		}
		if (Application::IsKeyPressed('S'))
		{
			move.x -= view.x;
			move.z -= view.z;
			position.x = position.x - view.x;
			position.z = position.z - view.z;
			target = position + view;
			car.fuel = car.fuel - 10;
		}
	}
	
	cout <<"            " << car.fuel << endl;
}

void Camera4::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}