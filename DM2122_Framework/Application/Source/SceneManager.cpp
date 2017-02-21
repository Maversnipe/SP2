#include "Application.h"
#include "SceneManager.h"
#include "MainMenu.h"

SceneManager *SceneManager::sceneManager = NULL;

SceneManager::~SceneManager()
{
	currSceneID = 0;
	while (maps[currSceneID] != NULL)
	{
		delete maps[currSceneID];
		currSceneID++;
	}
	delete sceneManager;
}

 SceneManager *SceneManager::getInstance()
{
	if (sceneManager == NULL)
	{
		sceneManager = new SceneManager();
		sceneManager->AddScene(new MainMenu());
		sceneManager->maps[0]->Init();
	}
	return sceneManager;
}

void SceneManager::AddScene(Scene *scene)
{
	maps[maps.size()] = scene;
}
void SceneManager::SetNextScene(int sceneID)
{
	nextSceneID = sceneID;
}

void SceneManager::Update(float dt)
{
	if (currSceneID == 0)
	{
		if (maps[currSceneID]->changeScene == 1)
		{
			maps[currSceneID]->Exit();
			SetNextScene(1);
			currSceneID = nextSceneID;
			maps[currSceneID]->Init();
		}
	}
	else if (currSceneID == 1)
	{
		if (maps[currSceneID]->changeScene == 2)
		{
			maps[currSceneID]->Exit();
			SetNextScene(2);
			currSceneID = nextSceneID;
			maps[currSceneID]->Init();
		}
		else if (maps[currSceneID]->changeScene == 3)
		{
			maps[currSceneID]->Exit();
			SetNextScene(3);
			currSceneID = nextSceneID;
			maps[currSceneID]->Init();
		}
	}

	maps[currSceneID]->Update(dt);
	maps[currSceneID]->Render();
}