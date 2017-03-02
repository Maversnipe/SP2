#include "Driving.h"
#include "GL\glew.h"
#include "Money.h"

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <iostream>
using namespace std;
//char mapArray[500][500] = {""};
Driving::Driving()
{
	enemySize = 10;
	objectSize = 60;
	enemyRadius = 0.5;
	ObjectRadius = 1.0;
	enemySpeed = 10;
	healthPack = 10;
	fuelPack = 10;
	car.health = 100;
	car.fuel = 10000.0f;
	rock = 5;
	game_state = GAME_START;
}

Driving::~Driving()
{
}

void Driving::Init()
{
	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // Set background color to dark blue

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	// Blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_indexBuffer[0]);

	// Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	// Get a handle for out "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");

	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	// Use our shader
	glUseProgram(m_programID);

	// Set default values
	rotateAngle = 0;
	fuel = 0;
	for (int i = 0; i < enemySize; i++)
	{
		scaleExplosion[i] = 1;
	}

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0, 100, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(180));
	light[0].cosInner = cos(Math::DegreeToRadian(120));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	// Initialise Camera
	Camera4.Init(Vector3(5, 1, 5), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("reference", Color(1, 0, 0), 1, 1);
	meshList[GEO_HEALTHBAR] = MeshBuilder::GenerateQuad("healthbar", Color(1, 0, 0), 1, 1);
	meshList[GEO_FUELBAR] = MeshBuilder::GenerateQuad("fuelbar", Color(0, 0, 0), 1, 1);
	//meshList[GEO_QUAD]->textureID = LoadTGA("Image//color2.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1, 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//blood-valley_ft.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1, 1);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//blood-valley_bk.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1, 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//blood-valley_rt.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1, 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//blood-valley_lf.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1, 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//blood-valley_dn.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1, 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//blood-valley_up.tga");

	meshList[GEO_BB8] = MeshBuilder::GenerateQuad("reference", Color(1, 1, 1), 1, 1);
	meshList[GEO_BB8]->textureID = LoadTGA("Image//BB8.tga");

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_CUBE1] = MeshBuilder::GenerateCube("cube", Color(1, 0, 0), 1, 1, 1);
	meshList[GEO_CUBE2] = MeshBuilder::GenerateCube("cube", Color(0, 0, 0), 1, 1, 1);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightSphere", Color(1, 1, 1), 18, 36, 1);
	meshList[GEO_EXPLOSION] = MeshBuilder::GenerateSphere("explosion", Color(1, 0, 0), 18, 36, 1);

	meshList[GEO_MODEL1] = MeshBuilder::GenerateOBJ("model1", "OBJ//chair.obj");
	meshList[GEO_MODEL1]->textureID = LoadTGA("Image//chair.tga");

	meshList[GEO_MODEL2] = MeshBuilder::GenerateOBJ("model2", "OBJ//dart.obj");
	meshList[GEO_MODEL2]->textureID = LoadTGA("Image//dart.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//PrestigeElite.tga");

	meshList[GEO_ENEMY] = MeshBuilder::GenerateOBJ("enemy", "OBJ//enemy.obj");
	meshList[GEO_ENEMY]->textureID = LoadTGA("OBJ//enemy.tga");

	meshList[GEO_CAR] = MeshBuilder::GenerateOBJ("car", "OBJ//car.obj");
	meshList[GEO_CAR]->textureID = LoadTGA("Image//car.tga");

	meshList[GEO_ROCK] = MeshBuilder::GenerateOBJ("rock", "OBJ//rock.obj");
	meshList[GEO_ROCK]->textureID = LoadTGA("Image//rock.tga");

	meshList[GEO_FUEL] = MeshBuilder::GenerateOBJ("fuel", "OBJ//fuel.obj");
	meshList[GEO_FUEL]->textureID = LoadTGA("Image//fuel.tga");

	meshList[GEO_HEALTH] = MeshBuilder::GenerateQuad("health", Color(1, 0, 0), 1, 1);
	meshList[GEO_HEALTH]->textureID = LoadTGA("Image//health.tga");

	meshList[GEO_ROCKS] = MeshBuilder::GenerateQuad("rocks", Color(1, 1, 1), 1, 1);
	meshList[GEO_ROCKS]->textureID = LoadTGA("Image//starRocks.tga");
	
	meshList[GEO_LOAD1] = MeshBuilder::GenerateQuad("load1", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOAD1]->textureID = LoadTGA("Image//loading1.tga");

	meshList[GEO_PAUSE] = MeshBuilder::GenerateQuad("pause", Color(1, 1, 1), 1, 1);
	meshList[GEO_PAUSE]->textureID = LoadTGA("Image//Driving//carPause.tga");

	meshList[GEO_PAUSE1] = MeshBuilder::GenerateQuad("pause1", Color(1, 1, 1), 1, 1);
	meshList[GEO_PAUSE1]->textureID = LoadTGA("Image//Driving//carPauseResume.tga");

	meshList[GEO_PAUSE2] = MeshBuilder::GenerateQuad("pause2", Color(1, 1, 1), 1, 1);
	meshList[GEO_PAUSE2]->textureID = LoadTGA("Image//Driving//carPauseInstruction.tga");

	meshList[GEO_PAUSE3] = MeshBuilder::GenerateQuad("pause3", Color(1, 1, 1), 1, 1);
	meshList[GEO_PAUSE3]->textureID = LoadTGA("Image//Driving//carPauseQuit.tga");

	meshList[GEO_INSTRUCTION] = MeshBuilder::GenerateQuad("instruction", Color(1, 1, 1), 1, 1);
	meshList[GEO_INSTRUCTION]->textureID = LoadTGA("Image//Driving//instructionMenu.tga");

	meshList[GEO_INSTRUCTION1] = MeshBuilder::GenerateQuad("instruction1", Color(1, 1, 1), 1, 1);
	meshList[GEO_INSTRUCTION1]->textureID = LoadTGA("Image//Driving//loading1.tga");



	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 2000.0f);
	projectionStack.LoadMatrix(projection);

	for (int counter = 0; counter < enemySize; counter++)
	{
		float i = RandomNumber(-250, 250);
		float j = RandomNumber(-250, 250);
		enemyPos[counter].Set(i, 0, j);
		enemyDead[counter] = false;
		enemyExplosion[counter] = false;
	}
	for (int counter = 0; counter < healthPack; counter++)
	{
		float i = RandomNumber(-250, 250);
		float j = RandomNumber(-250, 250);
		HealthPos[counter].Set(i, 0, j);
		healthPacktaken[counter] = false;
	}
	for (int counter = 0; counter < fuelPack; counter++)
	{
		float i = RandomNumber(-250, 250);
		float j = RandomNumber(-250, 250);
		FuelPos[counter].Set(i, 0, j);
		fuelPacktaken[counter] = false;
	}
	for (int counter = 0; counter < objectSize; counter++)
	{
		float i = RandomNumber(-250, 250);
		float j = RandomNumber(-250, 250);
		ObjectPos[counter].Set(i, 0, j);
		Camera4.ObjPos.push_back(ObjectPos[counter]);
	}
	for (int counter = 0; counter < rock; counter++)
	{
		float i = RandomNumber(-250, 250);
		float j = RandomNumber(-250, 250);
		RockPos[counter].Set(i, 0, j);
		Rocktaken[counter] = false;
	}
}
void Driving::gameUpdate(double dt)
{
	carVec.Set(Camera4.position.x, 0, Camera4.position.z);
	//==========Enemy movements==========
	for (int counter = 0; counter < enemySize; counter++)
	{
		if (enemyDead[counter] == false && enemyExplosion[counter] == false)
		{
			if ((carVec - enemyPos[counter]).Length() > 1)
			{
				int test = 0;
				int test2 = 0;
				int counter3 = 0;

				for (int counter2 = 0; counter2 < enemySize; counter2++)
				{
					for (int counter3 = 0; counter3 < objectSize; counter3++)
					{
						dirVec = (carVec - enemyPos[counter]).Normalize();
						enemyRotation[counter] = Math::RadianToDegree(atan2(dirVec.x, dirVec.z));
						if (counter != counter2)
						{
							if (((enemyPos[counter] + (dirVec*dt * enemySpeed)) - enemyPos[counter2]).Length()>enemyRadius
								&& ((enemyPos[counter]) - enemyPos[counter2]).Length()>enemyRadius
								&& (((enemyPos[counter] + (dirVec*dt * enemySpeed)) - ObjectPos[counter3]).Length() > ObjectRadius + enemyRadius)
								&& (((enemyPos[counter]) - ObjectPos[counter3]).Length() > ObjectRadius + enemyRadius))
							{
								test = 1;
							}
							else
							{
								for (int counter4 = 0; counter4 < enemySize; counter4++)
								{
									if (counter4 != counter)
									{
										if ((enemyPos[counter] - enemyPos[counter4]).Length() < enemyRadius)
										{
											if (rand() % 2< 1)
											{
												enemyPos[counter].x -= dirVec.x * dt * enemySpeed;
											}
											else
											{
												enemyPos[counter].z -= dirVec.z * dt * enemySpeed;
											}
										}
									}
								}
								if ((((enemyPos[counter]) + (dirVec*dt * enemySpeed) - ObjectPos[counter3]).Length() <= ObjectRadius + enemyRadius))
								{
									float z = dirVec.z;
									float x = dirVec.x;
									if (z < 0)
									{
										z = z*-1;
									}
									if (x < 0)
									{
										x = x*-1;
									}
									if (z <= x)
									{
										test2 = 1;
									}
									if (z > x)
									{
										test2 = 2;
									}
								}
								test = 2;
								break;
							}
						}
					}
					if (test == 2)
					{
						break;
					}
				}
				if (test == 1)
				{
					enemyPos[counter] += dirVec * dt*enemySpeed;
				}
				else
				{
					if (test2 == 1)
					{
						enemyPos[counter].z -= dirVec.z * dt * enemySpeed;
					}
					else if (test2 == 2)
					{
						enemyPos[counter].x -= dirVec.x * dt * enemySpeed;
					}
					else
					{
						enemyPos[counter] = enemyPos[counter];
					}
				}
			}
			else
			{
				enemyPos[counter] = enemyPos[counter];
			}
			if ((carVec - enemyPos[counter]).Length() > 1 && (carVec - enemyPos[counter]).Length() < 2)
			{
				ExplosionPos[counter] = enemyPos[counter];
				enemyExplosion[counter] = true;
			}

		}
		if (enemyExplosion[counter] == true)
		{
			if (scaleExplosion[counter] < 2)
			{
				scaleExplosion[counter] += 0.1;
				ExplosionRadius[counter] = scaleExplosion[counter];
				if ((carVec - ExplosionPos[counter]).Length() <= 2 + ExplosionRadius[counter])
				{
					car.health -= 1;
				}
			}
			else
			{
				scaleExplosion[counter] = 1;
				enemyExplosion[counter] = false;
				ExplosionRadius[counter] = 1;
				enemyDead[counter] = true;
			}
		}
		if (enemyDead[counter] == true)
		{
			enemyPos[counter] = {};
			float i = RandomNumber(-250, 250);
			float j = RandomNumber(-250, 250);
			enemyPos[counter].Set(i, 0, j);
			enemyDead[counter] = false;
		}
	}
	for (int counter = 0; counter < healthPack; counter++)
	{
		if (car.health < 100)
		{
			if ((carVec - HealthPos[counter]).Length() <= 2)
			{
				car.health += 20;
				healthPacktaken[counter] = true;
			}
		}
		if (car.health > 100)
		{
			car.health = 100;
		}
		if (healthPacktaken[counter] == true)
		{
			float i = RandomNumber(-250, 250);
			float j = RandomNumber(-250, 250);
			HealthPos[counter].Set(i, 0, j);
			healthPacktaken[counter] = false;
		}
	}
	for (int counter = 0; counter < fuelPack; counter++)
	{
		if (car.fuel < 10000)
		{
			if ((carVec - FuelPos[counter]).Length() <= 2)
			{
				car.fuel += 2000;
				fuelPacktaken[counter] = true;
			}
		}
		if (car.fuel > 10000)
		{
			car.fuel = 10000;
		}
		if (fuelPacktaken[counter] == true)
		{
			float i = RandomNumber(-250, 250);
			float j = RandomNumber(-250, 250);
			FuelPos[counter].Set(i, 0, j);
			fuelPacktaken[counter] = false;
		}
	}
	for (int counter = 0; counter < rock; counter++)
	{
		if ((carVec - RockPos[counter]).Length() <= 2)
		{
			Money::getInstance()->addMoney(5); //Accessing global money
			Rocktaken[counter] = true;
		}
		if (Rocktaken[counter] == true)
		{
			float i = RandomNumber(-250, 250);
			float j = RandomNumber(-250, 250);
			RockPos[counter].Set(i, 0, j);
			Rocktaken[counter] = false;
		}
	}
	//====================================
	fps = 1.0f / dt;
	framesPerSec = "FPS: " + std::to_string(fps);
	float LSPEED = 10.f;
	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	else if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	else if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (Application::IsKeyPressed('5'))
	{
		light[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('6'))
	{
		light[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('7'))
	{
		light[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	if (car.health > 0)
	{
		Camera4.Update(dt, &rotateAngle, car.fuel);
	}
	if (car.health < 0)
	{
		car.health = 0;
	}
	if (car.fuel < 0)
	{
		car.fuel = 0;
	}
	if (car.health == 0)
	{
		changeScene = 3;
	}
	cout << car.health << "            " << car.fuel << "            " << Camera4.CAR_SPEED << endl;
}
void Driving::gamePause(double dt)
{
	glfwGetWindowSize(Application::m_window, &width, &height);
	glfwGetCursorPos(Application::m_window, &xpos, &ypos);
	pause_state = 0;
	cout << xpos << "           " << ypos << endl;
}
void Driving::Update(double dt)
{
	if (Application::IsKeyPressed(VK_BACK))
	{
		if (game_state == GAME_START)
		{
			game_state = GAME_PAUSE;
		}
		else if (game_state == GAME_PAUSE)
		{
			game_state = GAME_START;
		}
	}
	switch (game_state)
	{
	case GAME_START:
		gameUpdate(dt);
		break;
	case GAME_PAUSE:
		gamePause(dt);
		break;
	}
}

void Driving::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color & depth buffer every frame

	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	Mtx44 MVP;

	viewStack.LoadIdentity();
	viewStack.LookAt(
		(Camera4.position.x - (Camera4.view.x * 5)), (Camera4.position.y), (Camera4.position.z - (Camera4.view.z * 5)),
		Camera4.target.x, Camera4.target.y, Camera4.target.z,
		Camera4.up.x, Camera4.up.y, Camera4.up.z);
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	//---------------------------------------------------------------
	if (changeScene != 0)
		RenderMeshOnScreen(meshList[GEO_LOAD1], 40, 20, 80, 80);//No transform needed
	else
	{
		if (game_state == GAME_START)
		{
			RenderMesh(meshList[GEO_AXES], false);

			modelStack.PushMatrix();
			modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
			RenderMesh(meshList[GEO_LIGHTBALL], false);
			modelStack.PopMatrix();

			RenderSkybox();

			for (int i = 0; i < enemySize; i++)
			{
				if (enemyDead[i] == false)
				{
					modelStack.PushMatrix();
					modelStack.Translate(enemyPos[i].x, 0, enemyPos[i].z);
					modelStack.Rotate(enemyRotation[i], 0, 1, 0);
					//modelStack.Rotate(enemyRotation2[i], 1, 0, 0);
					modelStack.Scale(0.25, 0.25, 0.25);
					RenderMesh(meshList[GEO_ENEMY], true);
					modelStack.PopMatrix();
				}
			}
			for (int i = 0; i < objectSize; i++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(ObjectPos[i].x, 0, ObjectPos[i].z);
				modelStack.Scale(1, 1, 1);
				RenderMesh(meshList[GEO_ROCK], true);
				modelStack.PopMatrix();
			}
			for (int i = 0; i < healthPack; i++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(HealthPos[i].x, 0, HealthPos[i].z);
				modelStack.Scale(1, 1, 1);
				RenderMesh(meshList[GEO_HEALTH], true);
				modelStack.PopMatrix();
			}
			for (int i = 0; i < fuelPack; i++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(FuelPos[i].x, 0, FuelPos[i].z);
				modelStack.Scale(1, 1, 1);
				RenderMesh(meshList[GEO_FUEL], true);
				modelStack.PopMatrix();
			}
			for (int i = 0; i < enemySize; i++)
			{
				if (enemyExplosion[i] == true)
				{
					modelStack.PushMatrix();
					modelStack.Translate(ExplosionPos[i].x, 0, ExplosionPos[i].z);
					modelStack.Rotate(enemyRotation[i], 0, 1, 0);
					modelStack.Scale(ExplosionRadius[i], ExplosionRadius[i], ExplosionRadius[i]);
					RenderMesh(meshList[GEO_EXPLOSION], true);
					modelStack.PopMatrix();
				}
			}
			for (int i = 0; i < rock; i++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(RockPos[i].x, 0, RockPos[i].z);
				modelStack.Scale(1, 1, 1);
				RenderMesh(meshList[GEO_ROCKS], true);
				modelStack.PopMatrix();
			}
			modelStack.PushMatrix();
			modelStack.Translate(carVec.x, carVec.y, carVec.z);
			modelStack.Rotate(rotateAngle, 0, 1, 0);
			modelStack.Scale(0.25, 0.25, 0.25);
			RenderMesh(meshList[GEO_CAR], true);
			modelStack.PopMatrix();
			RenderTextOnScreen(meshList[GEO_TEXT], framesPerSec, Color(0, 1, 1), 3, 0.5, 0.5);
			for (int i = 0; i <= car.health; i++)
			{
				RenderMeshOnScreen(meshList[GEO_HEALTHBAR], 14 + (i / 10), 57, 1, 2);//No transform needed
			}
			RenderTextOnScreen(meshList[GEO_TEXT], "Health: ", Color(1, 1, 1), 2, 0.5, 28.5);

			for (int i = 0; i < (car.fuel + 1000) / 100; i++)
			{
				RenderMeshOnScreen(meshList[GEO_FUELBAR], 14 + (i / 10), 54, 1, 2);//No transform needed
			}
			RenderTextOnScreen(meshList[GEO_TEXT], "Fuel: ", Color(1, 1, 1), 2, 0.5, 27);
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(Money::getInstance()->getMoney()), Color(0, 1, 1), 3, 23, 19);
			RenderMeshOnScreen(meshList[GEO_ROCKS], 75, 57, 4, 4);
			
		}
		else if (game_state == GAME_PAUSE)
		{
			if (pause_state == 0)
			{
				RenderMeshOnScreen(meshList[GEO_PAUSE], 40, 30, 80, 60);
			}
			else if (pause_state == 1)
			{
				RenderMeshOnScreen(meshList[GEO_PAUSE1], 40, 30, 80, 60);
			}
			else if (pause_state == 2)
			{
				RenderMeshOnScreen(meshList[GEO_PAUSE2], 40, 30, 80, 60);
			}
			else if (pause_state == 3)
			{
				RenderMeshOnScreen(meshList[GEO_PAUSE3], 40, 30, 80, 60);
			}
		}
	}
}

void Driving::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(-24.95 * 10, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(500, 500, 500);
	RenderMesh(meshList[GEO_FRONT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(24.95 * 10, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(500, 500, 500);
	RenderMesh(meshList[GEO_BACK], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -24.95 * 10);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(500, 500, 500);
	RenderMesh(meshList[GEO_LEFT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 24.95 * 10);
	modelStack.Scale(500, 500, 500);
	RenderMesh(meshList[GEO_RIGHT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(500, 500, 500);
	RenderMesh(meshList[GEO_BOTTOM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 24.945 * 10, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(500, 500, 500);
	RenderMesh(meshList[GEO_TOP], true);
	modelStack.PopMatrix();


}

void Driving::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if ((enableLight))
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render();

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Driving::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void Driving::RenderMeshOnScreen(Mesh* mesh, int x, int y, int
	sizex, int sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Driving::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

float Driving::RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

void Driving::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		if (meshList != NULL)
			delete meshList[i];
	}
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}