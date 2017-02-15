#include "CameraMainMenu.h"
#include "Application.h"
#include <GLFW/glfw3.h>
#include "Mtx44.h"
extern GLFWwindow* m_window;

CameraMainMenu::CameraMainMenu()
{
}

CameraMainMenu::~CameraMainMenu()
{
}

void CameraMainMenu::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
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

void CameraMainMenu::Update(double dt)
{
	if (Application::IsKeyPressed(37)) // Left
	{
	}
	if (Application::IsKeyPressed(39)) // Right
	{
		
	}
	if (Application::IsKeyPressed(38)) // Up
	{
	}

	if (Application::IsKeyPressed(40)) // Down
	{
	}
	if (Application::IsKeyPressed('N'))
	{
	}

	if (Application::IsKeyPressed('A'))
	{
	}
	if (Application::IsKeyPressed('D'))
	{
	}
	if (Application::IsKeyPressed('W'))
	{
	}
	if (Application::IsKeyPressed('S'))
	{
		
	}
}