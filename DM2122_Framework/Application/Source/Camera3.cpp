#include "Camera3.h"
#include "Application.h"
#include <GLFW/glfw3.h>
#include "Mtx44.h"
//extern GLFWwindow* m_window;



Camera3::Camera3()
{
	int width, height;
	glfwGetWindowSize(Application::m_window, &width, &height);

	int mid_x = width / 2;
	int mid_y = height / 2;

	glfwSetCursorPos(Application::m_window, mid_x, mid_y);
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

	buildings[0].Set(-80, 0, -23);
	buildings[1].Set(0, 0, -23);
	buildings[2].Set(80, 0, -23);
	buildings[3].Set(0, -2, 200);
	buildings[4].Set(160, 0, -23);

	buildingAABB[0].SaveCoord(Vector3(buildings[0].x - 20, buildings[0].y - 20, buildings[0].z - 20), Vector3(buildings[0].x + 25, buildings[0].y + 25, buildings[0].z + 25));
	buildingAABB[1].SaveCoord(Vector3(buildings[1].x - 20, buildings[1].y - 20, buildings[1].z - 20), Vector3(buildings[1].x + 25, buildings[1].y + 25, buildings[1].z + 25));
	buildingAABB[2].SaveCoord(Vector3(buildings[2].x - 20, buildings[2].y - 20, buildings[2].z - 20), Vector3(buildings[2].x + 25, buildings[2].y + 25, buildings[2].z + 25));
	buildingAABB[3].SaveCoord(Vector3(buildings[3].x - 18, buildings[3].y - 18, buildings[3].z - 18), Vector3(buildings[3].x + 18, buildings[3].y + 18, buildings[3].z + 18));
	buildingAABB[4].SaveCoord(Vector3(buildings[4].x - 20, buildings[4].y - 20, buildings[4].z - 20), Vector3(buildings[4].x + 25, buildings[4].y + 25, buildings[4].z + 25));
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
	glfwGetWindowSize(Application::m_window, &width, &height);
	glfwGetCursorPos(Application::m_window, &xpos, &ypos);

	int mid_x = width / 2;
	int mid_y = height / 2;

	glfwSetCursorPos(Application::m_window, mid_x, mid_y);

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
	//Limiting movement for carousell
	if (!playCarousell)
	{
		if (Application::IsKeyPressed('A'))
		{
			newPos = position - (right * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			buildingNum = 5;
			for (int i = 0; i < 5; i++)
			{
				if (collideAABBbuilding(charAABB, buildingAABB[i]))
				{
					if (((charAABB.min.x < buildingAABB[i].min.x) || (charAABB.max.x > buildingAABB[i].max.x))
						&& (charAABB.min.z > buildingAABB[i].min.z) && (charAABB.max.z < buildingAABB[i].max.z))
						X_Or_Z = 1;
					else if (((charAABB.min.z < buildingAABB[i].min.z) || (charAABB.max.z > buildingAABB[i].max.z))
						&& (charAABB.min.x > buildingAABB[i].min.x) && (charAABB.max.x < buildingAABB[i].max.x))
						X_Or_Z = 2;
					buildingNum = i;
					break;
				}
			}
			if (buildingNum < 5)
			{
				if (slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 1 && slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 1)
				{
					position.x = position.x - (right.x * velocity);
					target.x = position.x + (view.x * velocity);
				}
				if (slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 2 && slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 2)
				{
					position.z = position.z - (right.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
			}
			else
			{
				position.x = position.x - (right.x * velocity);
				target.x = position.x + (view.x * velocity);
				position.z = position.z - (right.z * velocity);
				target.z = position.z + (view.z * velocity);
			}
		}

		if (Application::IsKeyPressed('D'))
		{
			newPos = position + (right * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			buildingNum = 5;
			for (int i = 0; i < 5; i++)
			{
				if (collideAABBbuilding(charAABB, buildingAABB[i]))
				{
					if (((charAABB.min.x < buildingAABB[i].min.x) || (charAABB.max.x > buildingAABB[i].max.x))
						&& (charAABB.min.z > buildingAABB[i].min.z) && (charAABB.max.z < buildingAABB[i].max.z))
						X_Or_Z = 1;
					else if (((charAABB.min.z < buildingAABB[i].min.z) || (charAABB.max.z > buildingAABB[i].max.z))
						&& (charAABB.min.x > buildingAABB[i].min.x) && (charAABB.max.x < buildingAABB[i].max.x))
						X_Or_Z = 2;
					buildingNum = i;
					break;
				}
			}
			if (buildingNum < 5)
			{
				if (slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 1 && slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 1)
				{
					position.x = position.x + (right.x * velocity);
					target.x = position.x + (view.x * velocity);
				}
				if (slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 2 && slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 2)
				{
					position.z = position.z + (right.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
			}
			else
			{
				position.x = position.x + (right.x * velocity);
				target.x = position.x + (view.x * velocity);
				position.z = position.z + (right.z * velocity);
				target.z = position.z + (view.z * velocity);
			}
		}

		if (Application::IsKeyPressed('W'))
		{
			newPos = position + (view * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			buildingNum = 5;
			for (int i = 0; i < 5; i++)
			{
				if (collideAABBbuilding(charAABB, buildingAABB[i]))
				{
					if (((charAABB.min.x < buildingAABB[i].min.x) || (charAABB.max.x > buildingAABB[i].max.x))
						&& (charAABB.min.z > buildingAABB[i].min.z) && (charAABB.max.z < buildingAABB[i].max.z))
						X_Or_Z = 1;
					else if (((charAABB.min.z < buildingAABB[i].min.z) || (charAABB.max.z > buildingAABB[i].max.z))
						&& (charAABB.min.x > buildingAABB[i].min.x) && (charAABB.max.x < buildingAABB[i].max.x))
						X_Or_Z = 2;
					buildingNum = i;
					break;
				}
			}
			if (buildingNum < 5)
			{
				if (slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 1 && slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 1)
				{
					position.x = position.x + (view.x * velocity);
					target.x = position.x + (view.x * velocity);
				}
				if (slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 2 && slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 2)
				{
					position.z = position.z + (view.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
			}
			else
			{
				position.x = position.x + (view.x * velocity);
				target.x = position.x + (view.x * velocity);
				position.z = position.z + (view.z * velocity);
				target.z = position.z + (view.z * velocity);
			}
		}

		if (Application::IsKeyPressed('S'))
		{
			newPos = position - (view * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			buildingNum = 5;
			for (int i = 0; i < 5; i++)
			{
				if (collideAABBbuilding(charAABB, buildingAABB[i]))
				{
					if (((charAABB.min.x < buildingAABB[i].min.x) || (charAABB.max.x > buildingAABB[i].max.x))
						&& (charAABB.min.z > buildingAABB[i].min.z) && (charAABB.max.z < buildingAABB[i].max.z))
						X_Or_Z = 1;
					else if (((charAABB.min.z < buildingAABB[i].min.z) || (charAABB.max.z > buildingAABB[i].max.z))
						&& (charAABB.min.x > buildingAABB[i].min.x) && (charAABB.max.x < buildingAABB[i].max.x))
						X_Or_Z = 2;
					buildingNum = i;
					break;
				}
			}
			if (buildingNum < 5)
			{
				if (slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 1 && slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 1)
				{
					position.x = position.x - (view.x * velocity);
					target.x = position.x + (view.x * velocity);
				}
				if (slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 2 && slideAABBbuilding(charAABB, buildingAABB[buildingNum]) == 2)
				{
					position.z = position.z - (view.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
			}
			else
			{
				position.x = position.x - (view.x * velocity);
				target.x = position.x + (view.x * velocity);
				position.z = position.z - (view.z * velocity);
				target.z = position.z + (view.z * velocity);
			}
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

int Camera3::slideAABBbuilding(AABB charAABB, AABB buildingAABB)
{
	if ((charAABB.max.x < buildingAABB.max.x || charAABB.min.x < buildingAABB.max.x) && (X_Or_Z == 1))
		return 2;
	else if ((charAABB.max.z < buildingAABB.max.z || charAABB.min.z < buildingAABB.max.z) && (X_Or_Z == 2))
		return 1;
	else 
		return 0;

}

bool Camera3::collideAABBbuilding(AABB charAABB, AABB buildingAABB)
{
	return((charAABB.max.x > buildingAABB.min.x) && (charAABB.max.z > buildingAABB.min.z)
		&& (charAABB.min.x < buildingAABB.max.x) && (charAABB.min.z < buildingAABB.max.z));
}