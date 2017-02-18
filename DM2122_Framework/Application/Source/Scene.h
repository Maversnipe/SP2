#ifndef SCENE_H
#define SCENE_H
#include "Vector3.h"

class Scene
{
public:
	Scene() {}
	~Scene() {}

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	//==============================================================
	int changeScene = 0; // To check for scene changing
};


#endif