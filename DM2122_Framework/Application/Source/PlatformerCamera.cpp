#include "PlatformerCamera.h"
#include "Application.h"
#include <GLFW/glfw3.h>
#include "Mtx44.h"

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

void PlatformerCamera::Update(double dt, std::vector<Platforms> platformID[], std::vector<PlatformerTreasure> treasure)
{
	elapsed_time += dt;

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

	charMovement(dt, platformID);
	jumping(dt);

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

	if (superSpeedAbility)
		fixedVelocity = 2.1;
	else
		fixedVelocity = 0.7;

	if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
	{ // If character is moving
		currPlatform = noPlatform;	// If player is not on platform
		Platforms toCollide = noPlatform;
		if (velocity < fixedVelocity)
			velocity += (float)(dt * 2); // Acceleration to fixed velocity
		for (int platType = 0; platType < 6; platType++)
		{ // To check which platform the player is on
			for (std::vector<Platforms>::iterator it = platformID[platType].begin(); it < platformID[platType].end(); it++)
			{
				if (((onPlatform(charAABB, it->platformAABB)) && (charAABB.min.y > it->platformAABB.max.y)))
				{
					currPlatform = *it;
					break;
				}
			}
		}
		if (Application::IsKeyPressed('A'))
		{ // Going Left
			newPos = position - (right * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			
			if (((currPlatform.type == 0) && (!jump)) // If player is not jumping and not on any platform
				|| ((position.y < (currPlatform.pos.y - 2.5)) && (!jump))) // If player is lower than the platform, player falls
			{
				position.y += (float)(fallingVelocity * dt);
				target.y += (float)(fallingVelocity * dt);
				fallingVelocity -= (float)(gravity * dt);
			}
			else
			{
				if (((position.y >(currPlatform.pos.y + 7)) && (!jump)))
				{ // If player is higher than platform, player falls
					position.y += (float)(fallingVelocity * dt);
					target.y += (float)(fallingVelocity * dt);
					fallingVelocity -= (float)(gravity * dt);
				}
				if (position.y < currPlatform.pos.y + 7)
				{ // Player stops falling when the platform is reached
					fallingVelocity = 0;
					onGround = true;
					landed = true;
				}
			}

			//==============Collision======================
			for (int platType = 0; platType < 6; platType++)
			{ // To check which platform the player is on
				for (std::vector<Platforms>::iterator it = platformID[platType].begin(); it < platformID[platType].end(); it++)
				{
					if (collideAABBobject(charAABB, it->platformAABB))
					{
						if (((charAABB.min.x < it->platformAABB.min.x) || (charAABB.max.x > it->platformAABB.max.x))
							&& (charAABB.min.z > it->platformAABB.min.z) && (charAABB.max.z < it->platformAABB.max.z))
							X_Or_Z = 1; // For Z Direction
						else if (((charAABB.min.z < it->platformAABB.min.z) || (charAABB.max.z > it->platformAABB.max.z))
							&& (charAABB.min.x > it->platformAABB.min.x) && (charAABB.max.x < it->platformAABB.max.x))
							X_Or_Z = 2; // For X Direction
						toCollide = *it;
						break;
					}
				}
			}
			if (slideAABBobject(charAABB, toCollide.platformAABB) == 1)
			{ // Move in X Direction
				position.x = position.x - (right.x * velocity);
				target.x = position.x + (view.x * velocity);
			}
			else if (slideAABBobject(charAABB, toCollide.platformAABB) == 2)
			{
				position.z = position.z - (right.z * velocity);
				target.z = position.z + (view.z * velocity);
			}
			else
			{ // Move in Z Direction
				if (boundsCheck(newPos) == 1)
				{
					position.x = position.x - (right.x * velocity);
					target.x = position.x + (view.x * velocity);
				}
				else if (boundsCheck(newPos) == 2)
				{
					position.z = position.z - (right.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
				else if (boundsCheck(newPos) == 3)
				{
					position.x = position.x - (right.x * velocity);
					target.x = position.x + (view.x * velocity);
					position.z = position.z - (right.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
				else if (boundsCheck(newPos) == 0)
					position = position;
			}
		}

		if (Application::IsKeyPressed('D'))
		{ // Going Right
			newPos = position + (right * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			
			if (((currPlatform.type == 0) && (!jump)) // If player is not jumping and not on any platform
				|| ((position.y < (currPlatform.pos.y - 2.5)) && (!jump))) // If player is lower than the platform, player falls
			{
				position.y += (float)(fallingVelocity * dt);
				target.y += (float)(fallingVelocity * dt);
				fallingVelocity -= (float)(gravity * dt);
			}
			else
			{
				if (((position.y >(currPlatform.pos.y + 7)) && (!jump)))
				{ // If player is higher than platform, player falls
					position.y += (float)(fallingVelocity * dt);
					target.y += (float)(fallingVelocity * dt);
					fallingVelocity -= (float)(gravity * dt);
				}
				if (position.y < currPlatform.pos.y + 7)
				{ // Player stops falling when the platform is reached
					fallingVelocity = 0;
					onGround = true;
					landed = true;
				}
			}

			//==============Collision======================
			for (int platType = 0; platType < 6; platType++)
			{ // To check which platform the player is on
				for (std::vector<Platforms>::iterator it = platformID[platType].begin(); it < platformID[platType].end(); it++)
				{
					if (collideAABBobject(charAABB, it->platformAABB))
					{
						if (((charAABB.min.x < it->platformAABB.min.x) || (charAABB.max.x > it->platformAABB.max.x))
							&& (charAABB.min.z > it->platformAABB.min.z) && (charAABB.max.z < it->platformAABB.max.z))
							X_Or_Z = 1; // For Z Direction
						else if (((charAABB.min.z < it->platformAABB.min.z) || (charAABB.max.z > it->platformAABB.max.z))
							&& (charAABB.min.x > it->platformAABB.min.x) && (charAABB.max.x < it->platformAABB.max.x))
							X_Or_Z = 2; // For X Direction
						toCollide = *it;
						break;
					}
				}
			}
			if (slideAABBobject(charAABB, toCollide.platformAABB) == 1)
			{ // Move in X Direction
				position.x = position.x + (right.x * velocity);
				target.x = position.x + (view.x * velocity);
			}
			else if (slideAABBobject(charAABB, toCollide.platformAABB) == 2)
			{ // Move in Z Direction
				position.z = position.z + (right.z * velocity);
				target.z = position.z + (view.z * velocity);
			}
			else
			{
				if (boundsCheck(newPos) == 1)
				{
					position.x = position.x + (right.x * velocity);
					target.x = position.x + (view.x * velocity);
				}
				else if (boundsCheck(newPos) == 2)
				{
					position.z = position.z + (right.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
				else if (boundsCheck(newPos) == 3)
				{
					position.x = position.x + (right.x * velocity);
					target.x = position.x + (view.x * velocity);
					position.z = position.z + (right.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
				else if (boundsCheck(newPos) == 0)
					position = position;
			}
		}

		if (Application::IsKeyPressed('W'))
		{ // Going Forward
			newPos = position + (view * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			
			if (((currPlatform.type == 0) && (!jump)) // If player is not jumping and not on any platform
				|| ((position.y < (currPlatform.pos.y - 2.5)) && (!jump))) // If player is lower than the platform, player falls
			{
				position.y += (float)(fallingVelocity * dt);
				target.y += (float)(fallingVelocity * dt);
				fallingVelocity -= (float)(gravity * dt);
			}
			else
			{
				if (((position.y >(currPlatform.pos.y + 7)) && (!jump)))
				{ // If player is higher than platform, player falls
					position.y += (float)(fallingVelocity * dt);
					target.y += (float)(fallingVelocity * dt);
					fallingVelocity -= (float)(gravity * dt);
					if (position.y < currPlatform.pos.y + 7)
					{ // Player stops falling when the platform is reached
						fallingVelocity = 0;
						onGround = true;
						landed = true;
					}
				}
			}

			//==============Collision======================
			for (int platType = 0; platType < 6; platType++)
			{ // To check which platform the player is on
				for (std::vector<Platforms>::iterator it = platformID[platType].begin(); it < platformID[platType].end(); it++)
				{
					if (collideAABBobject(charAABB, it->platformAABB))
					{
						if (((charAABB.min.x < it->platformAABB.min.x) || (charAABB.max.x > it->platformAABB.max.x))
							&& (charAABB.min.z > it->platformAABB.min.z) && (charAABB.max.z < it->platformAABB.max.z))
							X_Or_Z = 1; // For Z Direction
						else if (((charAABB.min.z < it->platformAABB.min.z) || (charAABB.max.z > it->platformAABB.max.z))
							&& (charAABB.min.x > it->platformAABB.min.x) && (charAABB.max.x < it->platformAABB.max.x))
							X_Or_Z = 2; // For X Direction
						toCollide = *it;
						break;
					}
				}
			}
			if (slideAABBobject(charAABB, toCollide.platformAABB) == 1)
			{ // Move in X Direction
				position.x = position.x + (view.x * velocity);
				target.x = position.x + (view.x * velocity);
			}
			else if (slideAABBobject(charAABB, toCollide.platformAABB) == 2)
			{ // Move in Z Direction
				position.z = position.z + (view.z * velocity);
				target.z = position.z + (view.z * velocity);
			}
			else
			{
				if (boundsCheck(newPos) == 1)
				{
					position.x = position.x + (view.x * velocity);
					target.x = position.x + (view.x * velocity);
				}
				else if (boundsCheck(newPos) == 2)
				{
					position.z = position.z + (view.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
				else if (boundsCheck(newPos) == 3)
				{
					position.x = position.x + (view.x * velocity);
					target.x = position.x + (view.x * velocity);
					position.z = position.z + (view.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
				else if (boundsCheck(newPos) == 0)
					position = position;
			}
		}

		if (Application::IsKeyPressed('S'))
		{ // Going Backward
			newPos = position - (view * velocity);
			charAABB.SaveCoord(Vector3(newPos.x - 2, newPos.y - 2, newPos.z - 2), Vector3(newPos.x + 2, newPos.y + 2, newPos.z + 2));
			
			if (((currPlatform.type == 0) && (!jump)) // If player is not jumping and not on any platform
				|| ((position.y < (currPlatform.pos.y - 2.5)) && (!jump))) // If player is lower than the platform, player falls
			{
				position.y += (float)(fallingVelocity * dt);
				target.y += (float)(fallingVelocity * dt);
				fallingVelocity -= (float)(gravity * dt);
			}
			else
			{
				if (((position.y > (currPlatform.pos.y + 7)) && (!jump)))
				{ // If player is higher than platform, player falls
					position.y += (float)(fallingVelocity * dt);
					target.y += (float)(fallingVelocity * dt);
					fallingVelocity -= (float)(gravity * dt);
				}
				if (position.y < currPlatform.pos.y + 7)
				{ // Player stops falling when the platform is reached
					fallingVelocity = 0;
					onGround = true;
					landed = true;
				}
			}

			//==============Collision======================
			for (int platType = 0; platType < 6; platType++)
			{ // To check which platform the player is on
				for (std::vector<Platforms>::iterator it = platformID[platType].begin(); it < platformID[platType].end(); it++)
				{
					if (collideAABBobject(charAABB, it->platformAABB))
					{
						if (((charAABB.min.x < it->platformAABB.min.x) || (charAABB.max.x > it->platformAABB.max.x))
							&& (charAABB.min.z > it->platformAABB.min.z) && (charAABB.max.z < it->platformAABB.max.z))
							X_Or_Z = 1; // For Z Direction
						else if (((charAABB.min.z < it->platformAABB.min.z) || (charAABB.max.z > it->platformAABB.max.z))
							&& (charAABB.min.x > it->platformAABB.min.x) && (charAABB.max.x < it->platformAABB.max.x))
							X_Or_Z = 2; // For X Direction
						toCollide = *it;
						break;
					}
				}
			}
			if (slideAABBobject(charAABB, toCollide.platformAABB) == 1)
			{ // Move in X Direction
				position.x = position.x - (view.x * velocity);
				target.x = position.x + (view.x * velocity);
			}
			else if (slideAABBobject(charAABB, toCollide.platformAABB) == 2)
			{ // Move in Z Direction
				position.z = position.z - (view.z * velocity);
				target.z = position.z + (view.z * velocity);
			}
			else
			{
				if (boundsCheck(newPos) == 1)
				{
					position.x = position.x - (view.x * velocity);
					target.x = position.x + (view.x * velocity);
				}
				else if (boundsCheck(newPos) == 2)
				{
					position.z = position.z - (view.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
				else if (boundsCheck(newPos) == 3)
				{
					position.x = position.x - (view.x * velocity);
					target.x = position.x + (view.x * velocity);
					position.z = position.z - (view.z * velocity);
					target.z = position.z + (view.z * velocity);
				}
				else if (boundsCheck(newPos) == 0)
					position = position;
			}
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
		if (((currPlatform.type == 0) && (!jump)) // If player is not jumping and not on any platform
			|| ((position.y < (currPlatform.pos.y - 2.5)) && (!jump))) // If player is lower than the platform, player falls
		{
			position.y += (float)(fallingVelocity * dt);
			target.y += (float)(fallingVelocity * dt);
			fallingVelocity -= (float)(gravity * dt);
		}
		else
		{
			if (((position.y > (currPlatform.pos.y + 7)) && (!jump)))
			{ // If player is higher than platform, player falls
				position.y += (float)(fallingVelocity * dt);
				target.y += (float)(fallingVelocity * dt);
				fallingVelocity -= (float)(gravity * dt);
			}
			if (position.y < currPlatform.pos.y + 7)
			{ // Player stops falling when the platform is reached
				fallingVelocity = 0;
				onGround = true;
				landed = true;
			}
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
		gravity = 29.4;
	}
	else if (Application::IsKeyPressed(VK_SPACE) && doubleJumpAbility && !doubleJump && !jump && !onGround && landed)
	{
		landed = false;
		jump = true;
		playerOriginalHeight = position.y;
		fallingVelocity = -30;
		doubleJump = true;
		gravity = 29.4;
	}

	if ((jump) && (fallingVelocity < 0))
	{
		float newPosY = position.y - (float)(fallingVelocity * dt);
		if (newPosY < currPlatform.pos.y && currPlatform.type != 0)
		{
			if ((newPosY < currPlatform.platformAABB.min.y))
			{
				position.y -= (float)(fallingVelocity * dt);
				target.y -= (float)(fallingVelocity * dt);
				fallingVelocity += (float)(gravity * dt);
			}
			else
			{
				jump = false;
				doubleJump = false;
				fallingVelocity = 0;
				gravity = 29.4;
			}
		}
		else
		{
			position.y -= (float)(fallingVelocity * dt);
			target.y -= (float)(fallingVelocity * dt);
			fallingVelocity += (float)(gravity * dt);
		}
	}
	else if (position.y > (playerOriginalHeight) && jump)
	{
		doubleJump = false;
		jump = false;
		fallingVelocity = 0;
		gravity = 29.4;
	}
}

bool PlatformerCamera::onPlatform(AABB character, AABB platform)
{
	return((character.min.x < platform.max.x) && (character.max.x > platform.min.x)
		&& (character.min.z < platform.max.z) && (character.max.z > platform.min.z));
}

bool PlatformerCamera::collideAABBobject(AABB charAABB, AABB objectAABB)
{
	return((charAABB.max.x > objectAABB.min.x) && (charAABB.min.x < objectAABB.max.x)
		&& (charAABB.max.y > objectAABB.min.y) && (charAABB.min.y < objectAABB.max.y)
		&& (charAABB.max.z > objectAABB.min.z) && (charAABB.min.z < objectAABB.max.z));
}

int PlatformerCamera::slideAABBobject(AABB charAABB, AABB objectAABB)
{
	if ((charAABB.max.z < objectAABB.max.z || charAABB.min.z < objectAABB.max.z) 
		&& (charAABB.max.y > objectAABB.min.y) && (charAABB.min.y < objectAABB.max.y)
		&& (X_Or_Z == 2))
		return 1; // Slide in X Direction
	else if ((charAABB.max.x < objectAABB.max.x || charAABB.min.x < objectAABB.max.x) 
		&& (charAABB.max.y > objectAABB.min.y) && (charAABB.min.y < objectAABB.max.y)
		&& (X_Or_Z == 1))
		return 2; // Slide in Z Direction
	else
		return 0;

}

int PlatformerCamera::boundsCheck(Vector3 newPos)
{
	if ((newPos.x <= 515) && (newPos.x >= -515) && ((newPos.z > 515) || newPos.z < -515))
	{
		return 1;
	}
	else if ((newPos.z <= 515) && (newPos.z >= -515) && ((newPos.x > 515) || newPos.x < -515))
	{
		return 2;
	}
	else if ((newPos.x <= 515) && (newPos.x >= -515) && (newPos.z <= 515) && (newPos.z >= -515))
	{
		return 3;
	}
	else
		return 0;
}