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

	void AddScene(Scene*, std::string);
	void SetNextScene(std::string);
	void Update(float);

	int menuSelect = 0;

private:
	SceneManager() {}
	static SceneManager *sceneManager;

	std::string currSceneID = "";
	std::string nextSceneID = "";
	std::map<std::string, Scene*> maps;
};

#endif