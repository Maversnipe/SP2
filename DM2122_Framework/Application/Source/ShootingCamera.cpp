#include "ShootingCamera.h"
#include "Application.h"
#include <GLFW/glfw3.h>
#include "Mtx44.h"
extern GLFWwindow* m_window;

ShootingCamera::ShootingCamera()
{
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);

	int mid_x = width / 2;
	int mid_y = height / 2;

	glfwSetCursorPos(m_window, mid_x, mid_y);
}

ShootingCamera::~ShootingCamera()
{
}

void ShootingCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
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

void ShootingCamera::Update(double dt, float* horizontal, float* vertical)
{
	if (!switchTreasure)
		objectAABB.SaveCoord(Vector3(object.x - 10, 0, object.z - 3), Vector3(object.x + 10, 0, object.z + 3));
	else
		objectAABB.SaveCoord(Vector3(object.x - 2, 0, object.z - 2), Vector3(object.x + 2, 0, object.z + 2));


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

	int mid_x = width / 2;
	int mid_y = height / 2;

	glfwSetCursorPos(m_window, mid_x, mid_y);

	rotateHori = (mid_x - xpos) * dt * CamSpeed;
	rotateVert = (mid_y - ypos) * dt * CamSpeed;

	// control vertical limit
	verticalAngle += dt *rotateVert;
	horizontalAngle += dt * rotateHori;
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
	//if (Application::IsKeyPressed(38)) // Up
	//{
	//	float pitch = (float)(CAMERA_SPEED * dt);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//	Mtx44 rotation;
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	view = rotation * view;
	//	target = position + view;
	//}

	//if (Application::IsKeyPressed(40)) // Down
	//{
	//	float pitch = (float)(-CAMERA_SPEED * dt);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//	Mtx44 rotation;
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	view = rotation * view;
	//	target = position + view;
	//}
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
		newPos = position - (right * velocity);
		charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
		//========Enemy collisions =========
		if (enemyPos.size() != 0)
		{
			for (int i = 0; i < enemyPos.size(); i++)
			{
				collideEnemy = false;
				if (collideEnemies(newPos, enemyPos[i]))
				{
					collideEnemy = collideEnemies(newPos, enemyPos[i]);
					break;
				}
			}
		}
		else 
			collideEnemy = false;
		//==============AABB================
		if (collideAABBobject(charAABB, objectAABB))
			{
				if (((charAABB.min.x < objectAABB.min.x) || (charAABB.max.x > objectAABB.max.x))
					&& (charAABB.min.z > objectAABB.min.z) && (charAABB.max.z < objectAABB.max.z))
					X_Or_Z = 1;
				else if (((charAABB.min.z < objectAABB.min.z) || (charAABB.max.z > objectAABB.max.z))
					&& (charAABB.min.x > objectAABB.min.x) && (charAABB.max.x < objectAABB.max.x))
					X_Or_Z = 2;

				if (slideAABBobject(charAABB, objectAABB) == 1)
				{
					position.x = position.x - (right.x * velocity);
					target.x = position.x + (view.x * velocity);
				}
				if (slideAABBobject(charAABB, objectAABB) == 2)
				{
					position.z = position.z - (right.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
			}
		//========Enemy collisions =========
		else if (collideEnemy)
			position = position;
		//==================================
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
		//========Enemy collisions =========
		if (enemyPos.size() != 0)
		{
			for (int i = 0; i < enemyPos.size(); i++)
			{
				collideEnemy = false;
				if (collideEnemies(newPos, enemyPos[i]))
				{
					collideEnemy = collideEnemies(newPos, enemyPos[i]);
					break;
				}
			}
		}
		else
			collideEnemy = false;
		//==============AABB================
		if (collideAABBobject(charAABB, objectAABB))
		{
			if (((charAABB.min.x < objectAABB.min.x) || (charAABB.max.x > objectAABB.max.x))
				&& (charAABB.min.z > objectAABB.min.z) && (charAABB.max.z < objectAABB.max.z))
				X_Or_Z = 1;
			else if (((charAABB.min.z < objectAABB.min.z) || (charAABB.max.z > objectAABB.max.z))
				&& (charAABB.min.x > objectAABB.min.x) && (charAABB.max.x < objectAABB.max.x))
				X_Or_Z = 2;

			if (slideAABBobject(charAABB, objectAABB) == 1)
			{
				position.x = position.x + (right.x * velocity);
				target.x = position.x + (view.x * velocity);
			}
			if (slideAABBobject(charAABB, objectAABB) == 2)
			{
				position.z = position.z + (right.z * velocity);
				target.z = position.z + (view.z * velocity);
			}
		}
		//========Enemy collisions =========
		else if (collideEnemy)
			position = position;
		//==================================
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
		//========Enemy collisions =========
		if (enemyPos.size() != 0)
		{
			for (int i = 0; i < enemyPos.size(); i++)
			{
				collideEnemy = false;
				if (collideEnemies(newPos, enemyPos[i]))
				{
					collideEnemy = collideEnemies(newPos, enemyPos[i]);
					break;
				}
			}
		}
		else
			collideEnemy = false;
		//==============AABB================
		if (collideAABBobject(charAABB, objectAABB))
		{
			if (((charAABB.min.x < objectAABB.min.x) || (charAABB.max.x > objectAABB.max.x))
				&& (charAABB.min.z > objectAABB.min.z) && (charAABB.max.z < objectAABB.max.z))
				X_Or_Z = 1;
			else if (((charAABB.min.z < objectAABB.min.z) || (charAABB.max.z > objectAABB.max.z))
				&& (charAABB.min.x > objectAABB.min.x) && (charAABB.max.x < objectAABB.max.x))
				X_Or_Z = 2;

			if (slideAABBobject(charAABB, objectAABB) == 1)
			{
				position.x = position.x + (view.x * velocity);
				target.x = position.x + (view.x * velocity);
			}
			if (slideAABBobject(charAABB, objectAABB) == 2)
			{
				position.z = position.z + (view.z * velocity);
				target.z = position.z + (view.z * velocity);
			}
		}
		//========Enemy collisions =========
		else if (collideEnemy)
			position = position;
		//==================================
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
		//========Enemy collisions =========
		if (enemyPos.size() != 0)
		{
			for (int i = 0; i < enemyPos.size(); i++)
			{
				collideEnemy = false;
				if (collideEnemies(newPos, enemyPos[i]))
				{
					collideEnemy = collideEnemies(newPos, enemyPos[i]);
					break;
				}
			}
		}
		else
			collideEnemy = false;
		//==============AABB================
		if (collideAABBobject(charAABB, objectAABB))
		{
			if (((charAABB.min.x < objectAABB.min.x) || (charAABB.max.x > objectAABB.max.x))
				&& (charAABB.min.z > objectAABB.min.z) && (charAABB.max.z < objectAABB.max.z))
				X_Or_Z = 1;
			else if (((charAABB.min.z < objectAABB.min.z) || (charAABB.max.z > objectAABB.max.z))
				&& (charAABB.min.x > objectAABB.min.x) && (charAABB.max.x < objectAABB.max.x))
				X_Or_Z = 2;

			if (slideAABBobject(charAABB, objectAABB) == 1)
			{
				position.x = position.x - (view.x * velocity);
				target.x = position.x + (view.x * velocity);
			}
			if (slideAABBobject(charAABB, objectAABB) == 2)
			{
				position.z = position.z - (view.z * velocity);
				target.z = position.z + (view.z * velocity);
			}
		}
		//========Enemy collisions =========
		else if (collideEnemy)
			position = position;
		//==================================
		else
		{
			position.x = position.x - (view.x * velocity);
			target.x = position.x + (view.x * velocity);
			position.z = position.z - (view.z * velocity);
			target.z = position.z + (view.z * velocity);
		}
	}

	//if (enemyPos.size() != 0)
	//{
	//	for (int i = 0; i < enemyPos.size(); i++)
	//	{
	//		hitNoti(enemyPos[i]);
	//	}
	//}
	//else
	//{
	//	for (int i = 0; i < 4; i++)
	//	{
	//		sideNoti[i] = 0;
	//	}
	//}

	camPitch.SetToRotation(rotateVert, right.x, right.y, right.z);
	camYaw.SetToRotation(rotateHori, 0, 1, 0);
	rotation = camPitch * camYaw;
	view = (rotation * view).Normalized();
	target = (position + view);
	up = camYaw * up;
	right = camPitch * right;


	verticalAngle = Math::RadianToDegree(verticalAngle);
	horizontalAngle = Math::RadianToDegree(horizontalAngle);
	*vertical = verticalAngle;
	*horizontal = horizontalAngle;
	verticalAngle = Math::DegreeToRadian(verticalAngle);
	horizontalAngle = Math::DegreeToRadian(horizontalAngle);

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

int ShootingCamera::slideAABBobject(AABB charAABB, AABB buildingAABB)
{
	if ((charAABB.max.x < buildingAABB.max.x || charAABB.min.x < buildingAABB.max.x) && (X_Or_Z == 1))
		return 2;
	else if ((charAABB.max.z < buildingAABB.max.z || charAABB.min.z < buildingAABB.max.z) && (X_Or_Z == 2))
		return 1;
	else
		return 0;

}

bool ShootingCamera::collideAABBobject(AABB charAABB, AABB buildingAABB)
{
	return((charAABB.max.x > buildingAABB.min.x) && (charAABB.max.z > buildingAABB.min.z)
		&& (charAABB.min.x < buildingAABB.max.x) && (charAABB.min.z < buildingAABB.max.z));
}

bool ShootingCamera::collideEnemies(Vector3 character, Vector3 enemy)
{
	if ((character - enemy).Length() < 3)
		return true;
	else
		return false;
}

void ShootingCamera::hitNoti(Vector3 enemy)
{
	Vector3 newPosA = position - (right * velocity); //Left side
	Vector3 newPosD = position + (right * velocity); //Right side
	Vector3 newPosW = position + (view * velocity); //Front side
	Vector3 newPosS = position - (view * velocity); //Back side

	//Left side
	if (collideEnemies(newPosA, enemy))
		sideNoti[0] = 1;
	//Right side
	if (collideEnemies(newPosD, enemy))
		sideNoti[1] = 2;
	//Front side
	if (collideEnemies(newPosW, enemy))
		sideNoti[2] = 3;
	//Back side
	if (collideEnemies(newPosS, enemy))
		sideNoti[3] = 4;

}