#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

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
	rotateVert = 0.0;
	rotateHori = 0.0;
}

void Camera3::Update(double dt)
{
	objectDetected = false;
	static const float CAMERA_SPEED = 70.f;
	view = (target - position).Normalized();
	right = view.Cross(up);
	Vector3 newPos;

	if (Application::IsKeyPressed('W'))
	{
		newPos = position + view;
		for (int i = 0; i < 6; i++)
		{
			if (collisionDetection(newPos, i, 6) == true)
			{
				if (boxNotDisappear[i] == false)
					objectDetected = false;
				else
					objectDetected = true;
				crateNumber = i;
				break;
			}
			if (collisionDetection(newPos, 6, i) == true)
			{
				objectDetected = true;
				cactusNumber = i;
				break;
			}
			else
				objectDetected = false;
		}
		if (objectDetected == false)
		{
			if (boundsCheck(newPos) == 1)
			{
				position.x = position.x + view.x;
				target.x = position.x + view.x;
			}
			else if (boundsCheck(newPos) == 2)
			{
				position.z = position.z + view.z;
				target.z = position.z + view.z;
			}
			else if (boundsCheck(newPos) == 3)
			{
				position.x = position.x + view.x;
				target.x = position.x + view.x;
				position.z = position.z + view.z;
				target.z = position.z + view.z;
			}
			else if (boundsCheck(newPos) == 0)
				position = position;
		}
		else
		{
			float distX;
			float distZ;

			if (crateNumber < 6)
			{
				if (cratesDetect[crateNumber].x >= newPos.x)
					distX = cratesDetect[crateNumber].x - newPos.x;
				else
					distX = newPos.x - cratesDetect[crateNumber].x;

				if (cratesDetect[crateNumber].z >= newPos.z)
					distZ = cratesDetect[crateNumber].z - newPos.z;
				else
					distZ = newPos.z - cratesDetect[crateNumber].z;

				if ((distX > 30.0) && (distZ <= 30.0))
				{
					position.x = position.x + view.x;
					target.x = position.x + view.x;
				}
				else if ((distZ > 30.0) && (distX <= 30.0))
				{
					position.z = position.z + view.z;
					target.z = position.z + view.z;
				}
			}
			else if (cactusNumber < 6)
			{
				if (cactusDetect[cactusNumber].x >= newPos.x)
					distX = cactusDetect[cactusNumber].x - newPos.x;
				else
					distX = newPos.x - cactusDetect[cactusNumber].x;

				if (cactusDetect[cactusNumber].z >= newPos.z)
					distZ = cactusDetect[cactusNumber].z - newPos.z;
				else
					distZ = newPos.z - cactusDetect[cactusNumber].z;

				if ((distX > 30.0) && (distZ <= 30.0))
				{
					position.x = position.x + view.x;
					target.x = position.x + view.x;
				}
				else if ((distZ > 30.0) && (distX <= 30.0))
				{
					position.z = position.z + view.z;
					target.z = position.z + view.z;
				}
			}
		}
	}

	if (Application::IsKeyPressed('S'))
	{
		newPos = position - view;
		for (int i = 0; i < 6; i++)
		{
			if (collisionDetection(newPos, i, 6) == true)
			{
				if (boxNotDisappear[i] == false)
					objectDetected = false;
				else
					objectDetected = true;
				crateNumber = i;
				break;
			}
			if (collisionDetection(newPos, 6, i) == true)
			{
				objectDetected = true;
				cactusNumber = i;
				break;
			}
			else
				objectDetected = false;
		}
		if (objectDetected == false)
		{
			if (boundsCheck(newPos) == 1)
			{
				position.x = position.x - view.x;
				target.x = position.x + view.x;
			}
			else if (boundsCheck(newPos) == 2)
			{
				position.z = position.z - view.z;
				target.z = position.z + view.z;
			}
			else if (boundsCheck(newPos) == 3)
			{
				position.x = position.x - view.x;
				target.x = position.x + view.x;
				position.z = position.z - view.z;
				target.z = position.z + view.z;
			}
			else if (boundsCheck(newPos) == 0)
				position = position;
		}
		else
		{
			float distX;
			float distZ;

			if (crateNumber < 6)
			{
				if (cratesDetect[crateNumber].x >= newPos.x)
					distX = cratesDetect[crateNumber].x - newPos.x;
				else
					distX = newPos.x - cratesDetect[crateNumber].x;

				if (cratesDetect[crateNumber].z >= newPos.z)
					distZ = cratesDetect[crateNumber].z - newPos.z;
				else
					distZ = newPos.z - cratesDetect[crateNumber].z;

				if ((distX > 30.0) && (distZ <= 30.0))
				{
					position.x = position.x + view.x;
					target.x = position.x - view.x;
				}
				else if ((distZ > 30.0) && (distX <= 30.0))
				{
					position.z = position.z - view.z;
					target.z = position.z + view.z;
				}
			}
			else if (cactusNumber < 6)
			{
				if (cactusDetect[cactusNumber].x >= newPos.x)
					distX = cactusDetect[cactusNumber].x - newPos.x;
				else
					distX = newPos.x - cactusDetect[cactusNumber].x;

				if (cactusDetect[cactusNumber].z >= newPos.z)
					distZ = cactusDetect[cactusNumber].z - newPos.z;
				else
					distZ = newPos.z - cactusDetect[cactusNumber].z;

				if ((distX > 50.0) && (distZ <= 50.0))
				{
					position.x = position.x - view.x;
					target.x = position.x + view.x;
				}
				else if ((distZ > 50.0) && (distX <= 50.0))
				{
					position.z = position.z - view.z;
					target.z = position.z + view.z;
				}
			}
		}
	}

	if (Application::IsKeyPressed('A'))
	{
		newPos = position - right;
		for (int i = 0; i < 6; i++)
		{
			if (collisionDetection(newPos, i, 6) == true)
			{
				if (boxNotDisappear[i] == false)
					objectDetected = false;
				else
					objectDetected = true;
				crateNumber = i;
				break;
			}
			if (collisionDetection(newPos, 6, i) == true)
			{
				objectDetected = true;
				cactusNumber = i;
				break;
			}
			else
				objectDetected = false;
		}
		if (objectDetected == false)
		{
			if (boundsCheck(newPos) == 1)
			{
				position.x = position.x - right.x;
				target.x = position.x + view.x;
			}
			else if (boundsCheck(newPos) == 2)
			{
				position.z = position.z - right.z;
				target.z = position.z + view.z;
			}
			else if (boundsCheck(newPos) == 3)
			{
				position.x = position.x - right.x;
				target.x = position.x + view.x;
				position.z = position.z - right.z;
				target.z = position.z + view.z;
			}
			else if (boundsCheck(newPos) == 0)
				position = position;
		}
		else
		{
			float distX;
			float distZ;

			if (crateNumber < 6)
			{
				if (cratesDetect[crateNumber].x >= newPos.x)
					distX = cratesDetect[crateNumber].x - newPos.x;
				else
					distX = newPos.x - cratesDetect[crateNumber].x;

				if (cratesDetect[crateNumber].z >= newPos.z)
					distZ = cratesDetect[crateNumber].z - newPos.z;
				else
					distZ = newPos.z - cratesDetect[crateNumber].z;

				if ((distX > 30.0) && (distZ <= 30.0))
				{
					position.x = position.x - right.x;
					target.x = position.x + view.x;
				}
				else if ((distZ > 30.0) && (distX <= 30.0))
				{
					position.z = position.z - right.z;
					target.z = position.z + view.z;
				}
			}
			else if (cactusNumber < 6)
			{
				if (cactusDetect[cactusNumber].x >= newPos.x)
					distX = cactusDetect[cactusNumber].x - newPos.x;
				else
					distX = newPos.x - cactusDetect[cactusNumber].x;

				if (cactusDetect[cactusNumber].z >= newPos.z)
					distZ = cactusDetect[cactusNumber].z - newPos.z;
				else
					distZ = newPos.z - cactusDetect[cactusNumber].z;

				if ((distX > 50.0) && (distZ <= 50.0))
				{
					position.x = position.x - right.x;
					target.x = position.x + view.x;
				}
				else if ((distZ > 50.0) && (distX <= 50.0))
				{
					position.z = position.z - right.z;
					target.z = position.z + view.z;
				}
			}
		}
	}

	if (Application::IsKeyPressed('D'))
	{
		newPos = position + right;
		for (int i = 0; i < 6; i++)
		{
			if (collisionDetection(newPos, i, 6) == true)
			{
				if (boxNotDisappear[i] == false)
					objectDetected = false;
				else
					objectDetected = true;
				crateNumber = i;
				break;
			}
			if (collisionDetection(newPos, 6, i) == true)
			{
				objectDetected = true;
				cactusNumber = i;
				break;
			}
			else
				objectDetected = false;
		}
		if (objectDetected == false)
		{
			if (boundsCheck(newPos) == 1)
			{
				position.x = position.x + right.x;
				target.x = position.x + view.x;
			}
			else if (boundsCheck(newPos) == 2)
			{
				position.z = position.z + right.z;
				target.z = position.z + view.z;
			}
			else if (boundsCheck(newPos) == 3)
			{
				position.x = position.x + right.x;
				target.x = position.x + view.x;
				position.z = position.z + right.z;
				target.z = position.z + view.z;
			}
			else if (boundsCheck(newPos) == 0)
				position = position;
		}
		else
		{
			float distX;
			float distZ;

			if (crateNumber < 6)
			{
				if (cratesDetect[crateNumber].x >= newPos.x)
					distX = cratesDetect[crateNumber].x - newPos.x;
				else
					distX = newPos.x - cratesDetect[crateNumber].x;

				if (cratesDetect[crateNumber].z >= newPos.z)
					distZ = cratesDetect[crateNumber].z - newPos.z;
				else
					distZ = newPos.z - cratesDetect[crateNumber].z;

				if ((distX > 30.0) && (distZ <= 30.0))
				{
					position.x = position.x + right.x;
					target.x = position.x + view.x;
				}
				else if ((distZ > 30.0) && (distX <= 30.0))
				{
					position.z = position.z + right.z;
					target.z = position.z + view.z;
				}
			}
			else if (cactusNumber < 6)
			{
				if (cactusDetect[cactusNumber].x >= newPos.x)
					distX = cactusDetect[cactusNumber].x - newPos.x;
				else
					distX = newPos.x - cactusDetect[cactusNumber].x;

				if (cactusDetect[cactusNumber].z >= newPos.z)
					distZ = cactusDetect[cactusNumber].z - newPos.z;
				else
					distZ = newPos.z - cactusDetect[cactusNumber].z;

				if ((distX > 50.0) && (distZ <= 50.0))
				{
					position.x = position.x + right.x;
					target.x = position.x + view.x;
				}
				else if ((distZ > 50.0) && (distX <= 50.0))
				{
					position.z = position.z + right.z;
					target.z = position.z + view.z;
				}
			}
		}
	}

	if (Application::IsKeyPressed(37))
	{
		float yaw = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		rotateHori += CAMERA_SPEED * dt;
		if (rotateHori >= 180.0)
			rotateHori -= 360.0;
	}

	if (Application::IsKeyPressed(39))
	{
		float yaw = (float)(-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation * up;
		rotateHori -= CAMERA_SPEED * dt;
		if (rotateHori <= -180.0)
			rotateHori += 360.0;
	}

	if (Application::IsKeyPressed(38))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		if (rotateVert <= 60)
		{
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			target = position + view;
			rotateVert += CAMERA_SPEED * dt;
		}
	}

	if (Application::IsKeyPressed(40))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		if (rotateVert >= -60)
		{
			Mtx44 rotation;
			rotation.SetToRotation(pitch, right.x, right.y, right.z);
			view = rotation * view;
			target = position + view;
			rotateVert -= CAMERA_SPEED * dt;
		}
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

int Camera3::boundsCheck(Vector3 NewPos)
{
	if ((NewPos.x <= 240) && (NewPos.x >= -240) && ((NewPos.z > 240) || NewPos.z < -240))
	{
		return 1;
	}
	else if ((NewPos.z <= 240) && (NewPos.z >= -240) && ((NewPos.x > 240) || NewPos.x < -240))
	{
		return 2;
	}
	else if ((NewPos.x <= 240) && (NewPos.x >= -240) && (NewPos.z <= 240) && (NewPos.z >= -240) && (NewPos.y <= 240) && (NewPos.y >= -10))
	{
		return 3;
	}
	else
		return 0;
}

bool Camera3::collisionDetection(Vector3 newPos, int crateNum, int cactusNum)
{
	int distance = 0;
	if ((crateNum < 6) && (crateNum >= 0))
	{
		distance = (newPos - cratesDetect[crateNum]).Length();
		if (distance <= 30)
			return true;
	}
	else if ((cactusNum < 6) && (cactusNum >= 0))
	{
		distance = (newPos - cactusDetect[cactusNum]).Length();
		if (distance <= 30)
			return true;
	}
	
	return false;
}