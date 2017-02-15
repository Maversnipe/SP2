#include "Camera3.h"
#include "Application.h"
#include <GLFW/glfw3.h>
#include "Mtx44.h"
extern GLFWwindow* m_window;

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	view = (target - position).Normalized();
	right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = right.Cross(view).Normalized();
	defaultUp.Set(0, 1, 0);
	ShowCursor(false);
}

void Camera3::Update(double dt)
{
	view = (target - position).Normalized();
	right = view.Cross(up);


	up = right.Cross(view).Normalized();

	Mtx44 rotation, yaw, pitch;
	yaw = mouseY;
	pitch = mouseX;
	Mtx44 camPitch, camYaw;

	double CamSpeed = 5.f;
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	glfwGetCursorPos(m_window, &xpos, &ypos);

	int mid_x = width / 2;	//issue 1
	int mid_y = height / 2;

	glfwSetCursorPos(m_window, mid_x, mid_y);

	rotateHori = (mid_x - xpos) * dt * CamSpeed;
	rotateVert = (mid_y - ypos) * dt * CamSpeed;

	// control vertical limit
	verticalAngle += dt *rotateVert;
	if (verticalAngle > 1)
	{
		verticalAngle = 1;
		rotateVert = 0;
	}
	else if (verticalAngle < -1)
	{
		verticalAngle = -1;
		rotateVert = 0;

	}
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();

	view = target - position;
	right = view.Cross(up).Normalized();



	static const float CAMERA_SPEED = 50.f;
	Vector3 newPos;



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

	if(Application::IsKeyPressed('A'))
	{
		newPos = position - right;
		for (int i = 0; i < 4; i++)
		{
			if (buildingCollide(newPos, buildings[i]))
			{
				collideBuilding = true;
				buildingSlide = buildings[i];
				break;
			}
			else
				collideBuilding = false;
		}

		if (collideBuilding)
		{
			float distX = 0.0f;
			float distZ = 0.0f;

			if (buildingSlide.x >= newPos.x)
				distX = buildingSlide.x - newPos.x;

			else
				distX = newPos.x - buildingSlide.x;

			if (buildingSlide.z >= newPos.z)
				distZ = buildingSlide.z - newPos.z;
			else
				distZ = newPos.z - buildingSlide.z;

			//Finding distance 

			if ((distX <= 40.f) && (distZ >= 20.f))
			{
				position.x = position.x - right.x;
				target.x = position.x + view.x;
			}
			else if ((distX >= 20.f) && (distZ <= 40.f))
			{
				position.z = position.z - right.z;
				target.z = position.z + view.z;
			}
		}

		else
		{
			position = position - right;
			position.y = 0;
			target = position + view;
		}

	}
	if (Application::IsKeyPressed('D'))
	{
	    newPos = position + right;
		for (int i = 0; i < 4; i++)
		{
			if (buildingCollide(newPos, buildings[i]))
			{
				collideBuilding = true;
				buildingSlide = buildings[i];
				break;
			}
			else
				collideBuilding = false;
		}

		if (collideBuilding)
		{
			float distX = 0.0f;
			float distZ = 0.0f;

			if (buildingSlide.x >= newPos.x)
				distX = buildingSlide.x - newPos.x;

			else
				distX = newPos.x - buildingSlide.x;

			if (buildingSlide.z >= newPos.z)
				distZ = buildingSlide.z - newPos.z;
			else
				distZ = newPos.z - buildingSlide.z;

			//Finding distance 

			if ((distX <= 40.f) && (distZ >= 20.f))
			{
				position.x = position.x + right.x;
				target.x = position.x + view.x;
			}
			else if ((distX >= 20.f) && (distZ <= 40.f))
			{
				position.z = position.z + right.z;
				target.z = position.z + view.z;
			}
		}
		else
		{
			position = position + right;
			position.y = 0;
			target = position + view;
		}
	
	}
	if (Application::IsKeyPressed('W'))
	{
		newPos = position + view;
		for (int i = 0; i < 4; i++)
		{
			if (buildingCollide(newPos, buildings[i]))
			{
				collideBuilding = true;
				buildingSlide = buildings[i];
				break;
			}
			else
				collideBuilding = false;
		}

		if (collideBuilding)
		{
			float distX = 0.0f;
			float distZ = 0.0f;

			if (buildingSlide.x >= newPos.x)
				distX = buildingSlide.x - newPos.x;

			else
				distX = newPos.x - buildingSlide.x;

			if (buildingSlide.z >= newPos.z)
				distZ = buildingSlide.z - newPos.z;
			else
				distZ = newPos.z - buildingSlide.z;

			//Finding distance 

			if ((distX <= 40.f) && (distZ >= 20.f))
			{
				position.x = position.x + view.x;
				target.x = position.x + view.x;
			}
			else if ((distX >= 20.f) && (distZ <= 40.f))
			{
				position.z = position.z + view.z;
				target.z = position.z + view.z;
			}
		}
		else
		{
			position = position + view;
			position.y = 0;
			target = position + view;
		}
	
	}
	if (Application::IsKeyPressed('S'))
	{
		newPos = position - view;
		for (int i = 0; i < 4; i++)
		{
			if (buildingCollide(newPos, buildings[i]))
			{
				collideBuilding = true;
				buildingSlide = buildings[i];
				break;
			}
			else
				collideBuilding = false;
		}

		if (collideBuilding)
		{
			float distX = 0.0f;
			float distZ = 0.0f;

			if (buildingSlide.x >= newPos.x)
				distX = buildingSlide.x - newPos.x;

			else
				distX = newPos.x - buildingSlide.x;

			if (buildingSlide.z >= newPos.z)
				distZ = buildingSlide.z - newPos.z;
			else
				distZ = newPos.z - buildingSlide.z;

			//Finding distance 

			if ((distX <= 40.f) && (distZ >= 20.f))
			{
				position.x = position.x - view.x;
				target.x = position.x + view.x;
			}
			else if ((distX >= 20.f) && (distZ <= 40.f))
			{
				position.z = position.z - view.z;
				target.z = position.z + view.z;
			}
		}
		else
		{
			position = position - view;
			position.y = 0;
			target = position + view;
		}
		
	}

	camPitch.SetToRotation(rotateVert, right.x, right.y, right.z);
	camYaw.SetToRotation(rotateHori, 0, 1, 0);
	rotation = camPitch * camYaw;
	view = (rotation * view).Normalized();
	target = (position + view);
	up = camYaw * up;
	right = camPitch * right;

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

bool Camera3::buildingCollide(Vector3 newPos, Vector3 buildingPos)
{
	if (((newPos - buildingPos)).Length() < 35)
		return true;
	else
		return false;
}