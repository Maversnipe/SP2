#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
#include <map>
#include "Scene.h"
#include "Camera3.h"

class SceneManager
{
public:
	~SceneManager();
	static SceneManager *getInstance();

	void AddScene(Scene*);
	void SetNextScene(int);
	void Update(float);

	int menuSelect = 0;

private:
	SceneManager() {}
	static SceneManager *sceneManager;

	int currSceneID = 0;
	int nextSceneID = 0;
	std::map<int, Scene*> maps;
};

#endif