#include "SP_Gabriel.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"

#include <cstdlib>
#include <time.h>
#include <stdio.h>

SP_Gabriel::SP_Gabriel()
{
}

SP_Gabriel::~SP_Gabriel()
{
}

void SP_Gabriel::Init()
{
	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // Set background color to dark blue

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	// Enable Culling
//	glEnable(GL_CULL_FACE);

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
	translateX = 0;
	scaleAll = 1;
	
	maxSpawn = 10;
	currentSpawned = 0;
	//enemyUFOdirectionX[10] = 0, enemyUFOdirectionY = 0, enemyUFOdirectionZ[10] = 0;
	for (int count = 0; count < maxSpawn; ++count)
	{
		enemyUFOdirectionX[count] = 0;
		enemyUFOdirectionZ[count] = 0;

		randomDirectionX[count] = 0;
		randomDirectionZ[count] = 0;
	}

	time_t t;
	srand((unsigned)time(&t));
		
	prevValX = 0;

	isPlayerAlive = true;

	for (int count = 0; count < maxSpawn; ++count)
		isEnemyAlive[count] = false; // change to false
	// spawn 
	spawnTimer = 300;
	spawnCounter = spawnTimer - 20;
	forCount = 0;

	renderRef = false;	// remove (used for testing)

	bulletCount = 0;
	maxBullets = 10;
	bulletCD = 0;
	for (int n = 0; n < maxBullets; ++n)
	{
		isShooting[n] = false;

		projectileDirectionX[n] = 0;
		projectileDirectionZ[n] = 0;

		projStartX[n] = 0;
		projStartZ[n] = 0;
	}

	totalDistOfBulletAndObject = 0.5 + 0.15;


	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(0, 30, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
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
	camera3.Init(Vector3(0, 7, 50), Vector3(0, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("reference", Color(1, 1, 1), 1, 1);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//color2.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1, 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//SP_Gabriel_Space_Skybox//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1, 1);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//SP_Gabriel_Space_Skybox//back.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1, 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//SP_Gabriel_Space_Skybox//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1, 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//SP_Gabriel_Space_Skybox//right.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1, 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//SP_Gabriel_Space_Skybox//bottom.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1, 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//SP_Gabriel_Space_Skybox//top.tga");

	meshList[GEO_UFO_TEST] = MeshBuilder::GenerateQuad("UFO on test", Color(1, 1, 1), 1, 1);
	meshList[GEO_UFO_TEST]->textureID = LoadTGA("Image//UFO_test.tga");

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightSphere", Color(1, 1, 1), 18, 36, 1);

	meshList[GEO_UFO] = MeshBuilder::GenerateOBJ("Player UFO", "OBJ//UFO.obj");
	meshList[GEO_UFO]->textureID = LoadTGA("Image//UFO_PSB.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//PrestigeElite.tga");

	meshList[GEO_UFO_ON_SCREEN] = MeshBuilder::GenerateQuad("UFO on screen", Color(1,1,1), 1, 1);
	meshList[GEO_UFO_ON_SCREEN]->textureID = LoadTGA("Image//UFO_test.tga");

	meshList[GEO_PROJECTILE] = MeshBuilder::GenerateSphere("projectile", Color(1, 0, 0), 10, 10, 1);

	meshList[GEO_LOAD1] = MeshBuilder::GenerateQuad("load", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOAD1]->textureID = LoadTGA("Image//loading1.tga");

	meshList[GEO_LOAD2] = MeshBuilder::GenerateQuad("load", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOAD2]->textureID = LoadTGA("Image//loading2.tga");

	meshList[GEO_LOAD3] = MeshBuilder::GenerateQuad("load", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOAD3]->textureID = LoadTGA("Image//loading3.tga");

	meshList[GEO_LOAD4] = MeshBuilder::GenerateQuad("load", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOAD4]->textureID = LoadTGA("Image//loading4.tga");

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 2000.0f);
	projectionStack.LoadMatrix(projection);
}

void SP_Gabriel::Update(double dt)
{

	if (playLoading)
	{
		load_time += dt;
		if (load_time >= 5)
			playLoading = false;
	}


	if (Application::IsKeyPressed(VK_BACK))
		changeScene = 1;
	fps = 1.0f / dt;
	framesPerSec = "FPS: " + std::to_string(fps);
	float LSPEED = 10.f;

	spawnCounter++;
	bulletCD++;
	//std::cout << spawnCounter << "\n";

	if (spawnCounter >= spawnTimer /*&& isPlayerAlive == true*/)		// add is palyer alive check before spawning enemies
	{
		for (int count = forCount; count < maxSpawn; ++count)
		{
			// re-generate enemy 
			if (!isEnemyAlive[count] && isPlayerAlive)	//isEnemyAlive[i]
			{
				// set eneny to alive 
				isEnemyAlive[count] = true;

				// regen position of enemy 
				// set spawn point to (0,0,0)		// y not reseted as y doesnt change and is set to 0

				enemyUFOdirectionX[count] = 0;
				enemyUFOdirectionZ[count] = 0;

				randomDirectionX[count] = 0;
				randomDirectionZ[count] = 0;

				randomDirectionX[count] = rand() % 10 - 5;
					std::cout << count << " X rand()" << randomDirectionX[count] << std::endl;		// for debuging
				randomDirectionZ[count] = rand() % 10 - 5;
					std::cout << count << " Z rand()" << randomDirectionZ[count] << std::endl;		// for debuging

				if (randomDirectionX[count] == 0 && randomDirectionZ[count] == 0)
				{
					randomDirectionX[count] = rand() % 10 - 5;
						std::cout << count << " X rand() in 0 " << randomDirectionX[count] << std::endl;
					randomDirectionZ[count] = rand() % 10 - 5;
						std::cout << count << " Z rand() in 0 " << randomDirectionZ[count] << std::endl;
				}

				forCount++;
				
				if (forCount > maxSpawn)
				{
					forCount = 0;
				}
				break;
			}
		}
		spawnCounter = 0;
		currentSpawned++;
		//std::cout << spawnCounter << "\n";
	} 


	for (int count = 0; count < maxSpawn; ++count)
	{
		enemyUFOdirectionX[count] += (float)(randomDirectionX[count] * dt / 5);
		enemyUFOdirectionZ[count] += (float)(randomDirectionZ[count] * dt / 5);


		// death barrier / death zone
		if (enemyUFOdirectionX[count] > 20 || enemyUFOdirectionX[count] < -20 || enemyUFOdirectionZ[count] > 20 || enemyUFOdirectionZ[count] < -20)
		{
			//isPlayerAlive = false;
			for (int counter = 0; counter < maxSpawn; ++counter)
			{
				isEnemyAlive[counter] = false;	// despawn enemy --> also set isPlayerAlive = false;	set all to false if 3 passes death barrier "deathCount++; if deathCount >= 3 == false"
				isPlayerAlive = false;

				std::cout << isPlayerAlive << " ; " << isEnemyAlive[count] << "\n";
			}

			// insert all this to ifEnemyUFOisKilled once function is working
			enemyUFOdirectionX[count] = 0;
			enemyUFOdirectionZ[count] = 0;

			randomDirectionX[count] = 0;	// derender object to despawn then set pos to 0 of current, do array to store more values for more ships
			randomDirectionZ[count] = 0;

			randomDirectionX[count] = rand() % 10 - 5;
			randomDirectionZ[count] = rand() % 10 - 5;

			if (randomDirectionX[count] == 0 || randomDirectionX[count] == prevValX)	// creates new random number to creation direction for X but checks if its 0 or same as the prev.
			{
				std::cout << count << "\nX PREV Before (if) : " << prevValX << std::endl;
				std::cout << "X DIR  (if) : " << randomDirectionX[count] << std::endl;
				randomDirectionX[count] = rand() % 10 - 5;
				std::cout << "X After should be different (if) : " << randomDirectionX[count] << std::endl;
			}
			if (randomDirectionZ[count] == 0)
			{
				randomDirectionZ[count] = rand() % 10 - 5;
			}

			prevValX = randomDirectionX[count];

			//std::cout << "\nX (Update) : " << randomDirectionX[count] << std::endl;
			//std::cout << "Z (Update) : " << randomDirectionZ[count] << std::endl;
			//std::cout << count << "X Curr (Update) : " << enemyUFOdirectionX[count] << std::endl;
			//std::cout << count << "Z Curr (Update) : " << enemyUFOdirectionZ[count] << std::endl;

			//isEnemyAlive[count] = true;
			//isPlayerAlive = true;	// for testing (remove later)
			break;
		}
		if (!isPlayerAlive)
		{
			// sets coords and movemets to 0,0 
			for (int counter_ = 0; counter_ < maxSpawn; ++counter_)
			{
				enemyUFOdirectionX[counter_] = 0;
				enemyUFOdirectionZ[counter_] = 0;
			}
			break;
		}
	}

	//if ()		// if all is killed set maxSpawn +

	/*
	
	if(UFO is killed)
	{
		maxSpawn+= maxSpawn/2;
	}

	*/

	if (Application::IsKeyPressed(MK_LBUTTON))
	{
		if (bulletCD >= 50)
		{
			++bulletCount;
			if (bulletCount > 9)
			{
				bulletCount = 0;
			}
			isShooting[bulletCount] = true;
			// Shoot "projectile" towards enemy
			//
			// if projObjPos == enemyDirX && enemyDirZ
			// isEnemyAlive[count] = false
			projStartX[bulletCount] = camera3.position.x;
			projStartZ[bulletCount] = camera3.position.z;

			//std::cout << "bool : " << isShooting[bulletCount] << " ; bulletCount : " << bulletCount << "\n";
			bulletCD = 0;
		}
	}

	if (isShooting[bulletCount])
	{
		projectileDirectionX[bulletCount] += (float)(10 * -projStartX[bulletCount] / 15 * dt);
		projectileDirectionZ[bulletCount] += (float)(10 * -projStartZ[bulletCount] / 15 * dt);
	}
	
	//std::cout << projectileDirectionX[bulletCount] << "\n";

	// just use old coordds for ufo and bullets

	for (int i = 0; i <= currentSpawned; ++i)
	{
		for (int j = 1; j <= bulletCount; ++j)
		{
			float bulletX = projStartX[j] + projectileDirectionX[j];
			float bulletZ = projStartZ[j] + projectileDirectionZ[j];

			float coordX = enemyUFOdirectionX[i] - bulletX;
			float coordZ = enemyUFOdirectionZ[i] - bulletZ;
				
			float squareRoot = sqrt((coordX * coordX) + (coordZ * coordZ));
				
			//std::cout << "Outside: testX = " << bulletX << " ; testZ = " << bulletZ << " ; coordX = " << coordX << " ; coordZ = " << coordZ << "\n";
			//std::cout << "Outside: " << i << " ; " << enemyUFOdirectionX[i] << " ; projStartZ[j] = " << projStartZ[j] << " ; projectileDirectionZ[j] = " << projectileDirectionZ[j] << " ; sqrt = " << squareRoot << "\n";
			/*std::cout << "Outside: " << i << " ; " << isEnemyAlive[i] << " ; coordX = " << coordX << " ; coordZ = " << coordZ << " ; sqrt = " << sqrt((coordX * coordX) + (coordZ * coordZ)) << "\n";*/
				
			if (squareRoot < totalDistOfBulletAndObject)
			{
				isEnemyAlive[i] = false;
				isShooting[j] = false;
				// reset pos of ufo here
					
				std::cout << isEnemyAlive[i] << "\n";
				//std::cout << "Inside: " << bulletX << " ; " << bulletZ << "\n";
				//std::cout << "Inside: " << i << " ; " << isEnemyAlive[i] << " ; coordX = " << coordX << " ; coordZ = " << coordZ << " ; sqrt = " << squareRoot << "\n";
				//std::cout << "enemyUFOdirectionX[i] = " << enemyUFOdirectionX[i] << " ; projectileDirectionX[j] = " << projectileDirectionX[j] << "\n";
				//std::cout << "enemyUFOdirectionZ[i] = " << enemyUFOdirectionZ[i] << " ; projectileDirectionZ[j] = " << projectileDirectionZ[j] << "\n";
			}
		}
	}

	//for (int count = 0; count < maxSpawn; count++)
	//{

	//}

	// find this later
	//if (projectileDirectionX + bulletRadius[1] + enemyRadius[1] < bulletRadius[1] + enemyRadius[1])
	//{

	//}

	//if (distanceOfBulletAndObject[1] >= bulletRadius[1] + enemyRadius[1])
	//{
	//	isEnemyAlive[1] = true;		// or do nothing	(just do for d[rB + rO] < rB_COORD + rO_COORD)
	//}

	if (Application::IsKeyPressed('G'))
	{
		renderRef = true;
	}
	else if (Application::IsKeyPressed('H'))
	{
		renderRef = false;
	}


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

	camera3.Update(dt * 3);

	//std::cout << "cam pos X : " << camera3.position.x << "\n";
	//std::cout << "cam pos Z : " << camera3.position.z << "\n";
}

void SP_Gabriel::Render()
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
		camera3.position.x, camera3.position.y, camera3.position.z,
		camera3.target.x, camera3.target.y, camera3.target.z,
		camera3.up.x, camera3.up.y, camera3.up.z);
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	//---------------------------------------------------------------
	if (changeScene != 0)
		RenderMeshOnScreen(meshList[GEO_LOAD1], 40, 20, 80, 80);//No transform needed
	
	else if (playLoading)
	{
		if ((load_time >= 0) && (load_time <= 2))
			RenderMeshOnScreen(meshList[GEO_LOAD2], 40, 20, 80, 80);//No transform needed
		else if ((load_time >= 2) && (load_time <= 3.5))
			RenderMeshOnScreen(meshList[GEO_LOAD3], 40, 20, 80, 80);//No transform needed
		else if ((load_time >= 3.5) && (load_time <= 5))
			RenderMeshOnScreen(meshList[GEO_LOAD4], 40, 20, 80, 80);//No transform needed
	}
	
	else
	{
		if (renderRef)
			RenderMesh(meshList[GEO_AXES], false);

		//modelStack.PushMatrix();
		//modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
		//RenderMesh(meshList[GEO_LIGHTBALL], false);
		//modelStack.PopMatrix();

		RenderSkybox();


		/*

		if(isEnemyAlive == true)
		{
		for loop to render;
		}

		*/

		for (int count = 0; count < maxSpawn; count++)
		{
			if (isEnemyAlive[count]){
				modelStack.PushMatrix();
				modelStack.Translate(enemyUFOdirectionX[count], 0, enemyUFOdirectionZ[count]);
				RenderMesh(meshList[GEO_UFO], false);
				modelStack.PopMatrix();
			}
		}

		if (isShooting[bulletCount])
		{
			modelStack.PushMatrix();
			modelStack.Translate(projStartX[bulletCount] + projectileDirectionX[bulletCount], 0, projStartZ[bulletCount] + projectileDirectionZ[bulletCount]);
			modelStack.Scale(0.3, 0.3, 0.3);
			RenderMesh(meshList[GEO_PROJECTILE], false);
			modelStack.PopMatrix();
			//std::cout << " ; " << " ; projStartZ[j] = " << projStartZ[bulletCount] << " ; projectileDirectionZ[j] = " << projectileDirectionZ[bulletCount] << "\n";
		}

		RenderMeshOnScreen(meshList[GEO_UFO_ON_SCREEN], 40.5, 15, 25, 25);
		RenderTextOnScreen(meshList[GEO_TEXT], framesPerSec, Color(0, 1, 0), 3, 0.5, 0.5);
	}
	
}

void SP_Gabriel::RenderMeshOnScreen(Mesh* mesh, float x, float y, int sizex, int sizey)
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
	RenderMesh(mesh, true); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SP_Gabriel::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SP_Gabriel::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(24.95 * 20, 0, 0);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FRONT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-24.95 * 20, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -24.95 * 20);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_LEFT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 24.95 * 20);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_RIGHT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -24.945 * 20, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 24.945 * 20, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_TOP], true);
	modelStack.PopMatrix();
}

void SP_Gabriel::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if ((enableLight == false))
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

void SP_Gabriel::RenderText(Mesh* mesh, std::string text, Color color)
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

void SP_Gabriel::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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