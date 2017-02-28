#include "PlatformerCamera.h"
#include "Application.h"
#include <GLFW/glfw3.h>
#include "Mtx44.h"
extern GLFWwindow* m_window;

PlatformerCamera::PlatformerCamera()
{
}

PlatformerCamera::~PlatformerCamera()
{
}

void PlatformerCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
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

	noPlatform.type = 0;
}

void PlatformerCamera::Update(double dt, std::vector<Platforms> platformID[])
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

	int mid_x = width / 2;
	int mid_y = height / 2;

	glfwSetCursorPos(m_window, mid_x, mid_y);

	rotateHori = (mid_x - xpos) * dt * CamSpeed;
	rotateVert = (mid_y - ypos) * dt * CamSpeed;

	// control vertical limit
	verticalAngle += (float)(dt * rotateVert);
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

	jumping(dt);
	charMovement(dt, platformID);

	camPitch.SetToRotation(rotateVert, right.x, right.y, right.z);
	camYaw.SetToRotation(rotateHori, 0, 1, 0);
	rotation = camPitch * camYaw;
	view = (rotation * view).Normalized();
	target = (position + view);
	up = camYaw * up;
	right = camPitch * right;
}

void PlatformerCamera::charMovement(double dt, std::vector<Platforms> platformID[])
{ //======================Character Movement====================
	Vector3 newPos;

	if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
	{ // If character is moving
		nextPlatform = noPlatform;
		currPlatform = noPlatform;
		if (velocity < 0.7)
			velocity += (float)(dt * 2);
		if (Application::IsKeyPressed('A'))
		{
			newPos = position - (right * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			for (int platType = 0; platType < 6; platType++)
			{
				for (std::vector<Platforms>::iterator it = platformID[platType].begin(); it < platformID[platType].end(); it++)
				{
					if (onPlatform(charAABB, it->platformAABB))
					{
						nextPlatform = *it;
						break;
					}
				}
			}
			if ((nextPlatform.type == 0) && (!jump))
			{
				position.y += (float)(fallingVelocity * dt);
				target.y += (float)(fallingVelocity * dt);
				fallingVelocity -= (float)(gravity * dt);
				onGround = false;
			}
			else
			{
				if ((position.y > (nextPlatform.pos.y + 7)) && (!jump))
				{
					position.y += (float)(fallingVelocity * dt);
					target.y += (float)(fallingVelocity * dt);
					fallingVelocity -= (float)(gravity * dt);
				}
				if (position.y < nextPlatform.pos.y + 7)
					onGround = true;
			}
			position.x = position.x - (right.x * velocity);
			target.x = position.x + (view.x * velocity);
			position.z = position.z - (right.z * velocity);
			target.z = position.z + (view.z * velocity);
		}

		if (Application::IsKeyPressed('D'))
		{
			newPos = position + (right * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			for (int platType = 0; platType < 6; platType++)
			{
				for (std::vector<Platforms>::iterator it = platformID[platType].begin(); it < platformID[platType].end(); it++)
				{
					if (onPlatform(charAABB, it->platformAABB))
					{
						nextPlatform = *it;
						break;
					}
				}
			}
			if ((nextPlatform.type == 0) && (!jump))
			{
				position.y += (float)(fallingVelocity * dt);
				target.y += (float)(fallingVelocity * dt);
				fallingVelocity -= (float)(gravity * dt);
			}
			else
			{
				if ((position.y > (nextPlatform.pos.y + 7)) && (!jump))
				{
					position.y += (float)(fallingVelocity * dt);
					target.y += (float)(fallingVelocity * dt);
					fallingVelocity -= (float)(gravity * dt);
				}
				if (position.y < nextPlatform.pos.y + 7)
					onGround = true;
			}
			position.x = position.x + (right.x * velocity);
			target.x = position.x + (view.x * velocity);
			position.z = position.z + (right.z * velocity);
			target.z = position.z + (view.z * velocity);
		}

		if (Application::IsKeyPressed('W'))
		{
			newPos = position + (view * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			for (int platType = 0; platType < 6; platType++)
			{
				for (std::vector<Platforms>::iterator it = platformID[platType].begin(); it < platformID[platType].end(); it++)
				{
					if (onPlatform(charAABB, it->platformAABB))
					{
						nextPlatform = *it;
						break;
					}
				}
			}
			if ((nextPlatform.type == 0) && (!jump))
			{
				position.y += (float)(fallingVelocity * dt);
				target.y += (float)(fallingVelocity * dt);
				fallingVelocity -= (float)(gravity * dt);
			}
			else
			{
				if ((position.y > (nextPlatform.pos.y + 7)) && (!jump))
				{
					position.y += (float)(fallingVelocity * dt);
					target.y += (float)(fallingVelocity * dt);
					fallingVelocity -= (float)(gravity * dt);
				}
				if (position.y < nextPlatform.pos.y + 7)
					onGround = true;
			}
			position.x = position.x + (view.x * velocity);
			target.x = position.x + (view.x * velocity);
			position.z = position.z + (view.z * velocity);
			target.z = position.z + (view.z * velocity);
		}

		if (Application::IsKeyPressed('S'))
		{
			newPos = position - (view * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			for (int platType = 0; platType < 6; platType++)
			{
				for (std::vector<Platforms>::iterator it = platformID[platType].begin(); it < platformID[platType].end(); it++)
				{
					if (onPlatform(charAABB, it->platformAABB))
					{
						nextPlatform = *it;
						break;
					}
				}
			}
			if ((nextPlatform.type == 0) && (!jump))
			{
				position.y += (float)(fallingVelocity * dt);
				target.y += (float)(fallingVelocity * dt);
				fallingVelocity -= (float)(gravity * dt);
			}
			else
			{
				if ((position.y > (nextPlatform.pos.y + 7)) && (!jump))
				{
					position.y += (float)(fallingVelocity * dt);
					target.y += (float)(fallingVelocity * dt);
					fallingVelocity -= (float)(gravity * dt);
				}
				if (position.y < nextPlatform.pos.y + 7)
					onGround = true;
			}
			position.x = position.x - (view.x * velocity);
			target.x = position.x + (view.x * velocity);
			position.z = position.z - (view.z * velocity);
			target.z = position.z + (view.z * velocity);
		}
	}
	else
	{ // If character is not moving
		velocity = 0;
		charAABB.SaveCoord(Vector3(position.x - 2, position.y - 2, position.z - 2), Vector3(position.x + 2, position.y + 2, position.z + 2));
		for (int platType = 0; platType < 6; platType++)
		{
			for (std::vector<Platforms>::iterator it = platformID[platType].begin(); it < platformID[platType].end(); it++)
			{
				if (onPlatform(charAABB, it->platformAABB))
				{
					currPlatform = *it;
					break;
				}
			}
		}
		if (currPlatform.type == 0 && !jump)
		{
			position.y += (float)(fallingVelocity * dt);
			target.y += (float)(fallingVelocity * dt);
			fallingVelocity -= (float)(gravity * dt);
		}
		else
		{
			if ((position.y > (currPlatform.pos.y + 7)) && (!jump))
			{
				position.y += (float)(fallingVelocity * dt);
				target.y += (float)(fallingVelocity * dt);
				fallingVelocity -= (float)(gravity * dt);
			}
			if (position.y < currPlatform.pos.y + 7)
				onGround = true;
		}
	}
}

void PlatformerCamera::jumping(double dt)
{ //===================Jumping===========================
	if (Application::IsKeyPressed(VK_SPACE) && !jump && onGround)
	{
		jump = true;
		playerOriginalHeight = position.y;
		fallingVelocity = -30;
		onGround = false;
		gravity = 19.6;
	}

	if ((jump) && fallingVelocity < 0)
	{
		position.y -= (float)(fallingVelocity * dt);
		target.y -= (float)(fallingVelocity * dt);
		fallingVelocity += (float)(gravity * dt);
	}
	else if (position.y >(playerOriginalHeight) && jump)
	{
		jump = false;
		fallingVelocity = 0;
		gravity = 29.4;
	}
}

bool PlatformerCamera::onPlatform(AABB character, AABB platform)
{
	return((character.min.x < platform.max.x) && (character.max.x > platform.min.x)
		&& (character.min.z < platform.max.z) && (character.max.z > platform.min.z)
		&& (character.min.y > platform.max.y));
}