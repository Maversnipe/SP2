#include "Application.h"
#include "SceneManager.h"
#include "MainMenu.h"
#include "Shooting.h"
#include "Platformer.h"
#include "Money.h"
#include "Driving.h"
#include "StudioProject.h"

SceneManager *SceneManager::sceneManager = NULL;

SceneManager::~SceneManager()
{
	for (std::map<std::string, Scene*>::iterator it = maps.begin(); it != maps.end(); it++)
	{
		maps.erase(it);
		it--;
	}
	delete sceneManager;
}

 SceneManager *SceneManager::getInstance()
{
	if (sceneManager == NULL)
	{
		sceneManager = new SceneManager();
		sceneManager->AddScene(new MainMenu(), "Menu");
		sceneManager->SetNextScene("Menu");
		sceneManager->currSceneID = sceneManager->nextSceneID;
		sceneManager->maps["Menu"]->Init();
	}
	return sceneManager;
}

void SceneManager::AddScene(Scene *scene, std::string sceneName)
{
	maps[sceneName] = scene;
}
void SceneManager::SetNextScene(std::string sceneID)
{
	nextSceneID = sceneID;
}

void SceneManager::Update(float dt)
{
	if (currSceneID == "Menu")
	{ // Main Menu Scene
		if (maps[currSceneID]->changeScene == 1)
		{
			maps[currSceneID]->Exit();
			delete maps[currSceneID];
			SetNextScene("MainScene");
			currSceneID = nextSceneID;
			AddScene(new StudioProject(), "MainScene");			
			maps[currSceneID]->Init();
		}
	}
	else if (currSceneID == "MainScene")
	{ // Main Scene (Buildings inside)
		if (maps[currSceneID]->changeScene == 2)
		{
			maps[currSceneID]->Exit();
			delete maps[currSceneID];
			SetNextScene("Shooter");
			currSceneID = nextSceneID;
			AddScene(new Shooting(), "Shooting");
			maps[currSceneID]->Init();
		}
		else if (maps[currSceneID]->changeScene == 3)
		{
			maps[currSceneID]->Exit();
			delete maps[currSceneID];
			SetNextScene("Platformer");
			currSceneID = nextSceneID;
			AddScene(new Platformer(), "Platformer");
			maps[currSceneID]->Init();
		}
		else if (maps[currSceneID]->changeScene == 4)
		{
			maps[currSceneID]->Exit();
			delete maps[currSceneID];
			SetNextScene("Driving");
			currSceneID = nextSceneID;
			AddScene(new Driving(), "Driving");
			maps[currSceneID]->Init();
		}
	}
	else if (currSceneID == "Shooting")
	{ // Shooting Game Scene
		if (maps[currSceneID]->changeScene == 1)
		{
			maps[currSceneID]->Exit();
			delete maps[currSceneID];
			SetNextScene("MainScene");
			currSceneID = nextSceneID;
			AddScene(new StudioProject(), "MainScene");
			maps[currSceneID]->Init();
		}
	}
	else if (currSceneID == "Platformer")
	{ // Platformer Game Scene
		if (maps[currSceneID]->changeScene == 1)
		{
			maps[currSceneID]->Exit();
			delete maps[currSceneID];
			SetNextScene("MainScene");
			currSceneID = nextSceneID;
			AddScene(new StudioProject(), "MainScene");
			maps[currSceneID]->Init();
		}
	}
	else if (currSceneID == "Driving")
	{ // Driving Game Scene
		if (maps[currSceneID]->changeScene == 1)
		{
			maps[currSceneID]->Exit();
			delete maps[currSceneID];
			SetNextScene("MainScene");
			currSceneID = nextSceneID;
			AddScene(new StudioProject(), "MainScene");
			maps[currSceneID]->Init();
		}
	}

	maps[currSceneID]->Update(dt);
	maps[currSceneID]->Render();
}