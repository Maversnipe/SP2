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

void Camera4::Update(double dt, float* rotate,int& fuel)
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
				float yaw = (float)(CAR_SPEED * dt);
				Mtx44 rotation;
				rotation.SetToRotation(yaw, 0, 1, 0);
				view.y = 0;
				view = rotation * view;
				target = position + view;
				up = rotation * up;
				*rotate += yaw;
				fuel = fuel - 1;
		}
		if (Application::IsKeyPressed('D'))
		{
			float yaw = (float)(-CAR_SPEED * dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			view.y = 0;
			view = rotation * view;
			target = position + view;
			up = rotation * up;
			*rotate += yaw;
			fuel = fuel - 1;
		}
		if (Application::IsKeyPressed('W'))
		{
			view.y = 0;
			for (int i = 0; i < ObjPos.size(); i++)
			{
				if (((position + (view*dt * CAR_SPEED)) - ObjPos[i]).Length()>1.5)
				{
					collision = false;
				}
				else
				{
					collision = true;
					CAR_SPEED = 0;
					break;
				}
			}
			if (collision == false)
			{
				if (CAR_SPEED < 50.0f)
				{
					CAR_SPEED += 0.4f;
				}
				else
				{
					CAR_SPEED = 50.0f;
				}
				Vector3 newPos = position + (view * dt * CAR_SPEED);
				if (boundsCheck(newPos) == 3)
				{
					position = position + (view*dt * CAR_SPEED);
					target = position + view;
				}
				else
				{
					position = position;
					CAR_SPEED = 0;
				}
				fuel = fuel - 5;
			}
		}
		if (Application::IsKeyPressed('S'))
		{
			if (CAR_SPEED > -30.0f)
			{
				CAR_SPEED -= 0.2f;
			}
			else
			{
				CAR_SPEED = -30.0f;
			}
			view.y = 0;
			for (int i = 0; i < ObjPos.size(); i++)
			{
				if (((position+(view*dt * CAR_SPEED)) - ObjPos[i]).Length()>1.5)
				{
					collision = false;
				}
				else
				{
					collision = true;
					CAR_SPEED = 0;
					break;
				}
			}
			if (collision == false)
			{
				Vector3 newPos = position + (view * dt * CAR_SPEED);
				if (boundsCheck(newPos) == 3)
				{
					position = position + (view*dt * CAR_SPEED);
					target = position + view;
				}
				else
				{
					position = position;
					CAR_SPEED = 0;
				}
				fuel = fuel - 5;
			}
		}
		if (Application::IsKeyPressed(VK_SPACE))
		{
			if (CAR_SPEED > -1 && CAR_SPEED < 1 && CAR_SPEED != 0)
			{
				CAR_SPEED -= 0.01;
			}
			else if (CAR_SPEED > 0)
			{
				CAR_SPEED -= 1.0f;
				for (int i = 0; i < ObjPos.size(); i++)
				{
					if (((position + (view*dt * CAR_SPEED)) - ObjPos[i]).Length()>1.5)
					{
						collision = false;
					}
					else
					{
						collision = true;
						CAR_SPEED = 0;
						break;
					}
				}
				if (collision == false)
				{
					Vector3 newPos = position + (view * dt * CAR_SPEED);
					if (boundsCheck(newPos) == 3)
					{
						position = position + (view*dt * CAR_SPEED);
						target = position + view;
					}
					else
					{
						position = position;
						CAR_SPEED = 0;
					}
					fuel = fuel - 5;
				}
			}
			else if (CAR_SPEED < 0)
			{
				CAR_SPEED += 1.0;
				for (int i = 0; i < ObjPos.size(); i++)
				{
					if (((position - (view*dt * CAR_SPEED)) - ObjPos[i]).Length()>1.5)
					{
						collision = false;
					}
					else
					{
						collision = true;
						CAR_SPEED = 0;
						break;
					}
				}
				if (collision == false)
				{
					Vector3 newPos = position + (view * dt * CAR_SPEED);
					if (boundsCheck(newPos) == 3)
					{
						position = position + (view*dt * CAR_SPEED);
						target = position + view;
					}
					else
					{
						position = position;
						CAR_SPEED = 0;
					}
					fuel = fuel - 5;
				}
			}
		}
		if (Application::IsKeyPressed('S') != true && Application::IsKeyPressed('W') != true && Application::IsKeyPressed(VK_SPACE) != true)
		{
			if (CAR_SPEED > -0.1 && CAR_SPEED < 0.1)
			{
				CAR_SPEED = 0;
			}
			else if (CAR_SPEED > 0)
			{
				CAR_SPEED -= 0.2f;
				for (int i = 0; i < ObjPos.size(); i++)
				{
					if (((position + (view*dt * CAR_SPEED)) - ObjPos[i]).Length()>1.5)
					{
						collision = false;
					}
					else
					{
						collision = true;
						CAR_SPEED = 0;
						break;
					}
				}
				if (collision == false)
				{
					Vector3 newPos = position + (view * dt * CAR_SPEED);
					if (boundsCheck(newPos) == 3)
					{
						position = position + (view*dt * CAR_SPEED);
						target = position + view;
					}
					else
					{
						position = position;
						CAR_SPEED = 0;
					}
				}
			}
			else if (CAR_SPEED < 0)
			{
				CAR_SPEED += 0.2f;
				for (int i = 0; i < ObjPos.size(); i++)
				{
					if (((position - (view*dt * CAR_SPEED)) - ObjPos[i]).Length()>1.5)
					{
						collision = false;
					}
					else
					{
						collision = true;
						CAR_SPEED = 0;
						break;
					}
				}
				if (collision == false)
				{
					Vector3 newPos = position + (view * dt * CAR_SPEED);
					if (boundsCheck(newPos) == 3)
					{
						position = position + (view*dt * CAR_SPEED);
						target = position + view;
					}
					else
					{
						position = position;
						CAR_SPEED = 0;
					}
					fuel = fuel - 5;
				}
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

int Camera4::boundsCheck(Vector3 NewPos)
{
	if ((NewPos.x <= 245) && (NewPos.x >= -245) && ((NewPos.z > 245) || NewPos.z < -245))
	{
		return 1;
	}
	else if ((NewPos.z <= 245) && (NewPos.z >= -245) && ((NewPos.x > 245) || NewPos.x < -245))
	{
		return 2;
	}
	else if ((NewPos.x <= 245) && (NewPos.x >= -245) && (NewPos.z <= 245) && (NewPos.z >= -245))
	{
		return 3;
	}
	else
		return 0;
}