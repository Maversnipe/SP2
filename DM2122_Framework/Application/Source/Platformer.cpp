#include "Platformer.h"
#include "GL\glew.h"

#include <time.h>
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "FileReading.h"
#include "Money.h"

Platformer::Platformer()
{
}

Platformer::~Platformer()
{
}

void Platformer::Init()
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

	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(0, 20, 0);
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
	camera.Init(Vector3(-485, 22 * 5, 0), Vector3(-480, 22 * 5, 0), Vector3(0, 1, 0));

	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1, 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//enemy.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1, 1);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//enemy.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1, 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//enemy.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1, 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//enemy.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1, 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//enemy.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1, 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//enemy.tga");

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 0), 1, 1, 1);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightSphere", Color(1, 1, 1), 18, 36, 1);

	//=======================OBJECTS================================================================
	meshList[ONE_BLOCK] = MeshBuilder::GenerateOBJ("one sized", "OBJ//Platformer//1-Platform.obj");
	meshList[ONE_BLOCK]->textureID = LoadTGA("Image//MainScene//building4.tga");

	meshList[TWO_BLOCK] = MeshBuilder::GenerateOBJ("two sized", "OBJ//Platformer//2-Platform.obj");
	meshList[TWO_BLOCK]->textureID = LoadTGA("Image//MainScene//building3.tga");

	meshList[THREE_BLOCK] = MeshBuilder::GenerateOBJ("three sized", "OBJ//Platformer//3-Platform.obj");
	meshList[THREE_BLOCK]->textureID = LoadTGA("Image//MainScene//building2.tga");

	meshList[FOUR_BLOCK] = MeshBuilder::GenerateOBJ("four sized", "OBJ//Platformer//4-Platform.obj");
	meshList[FOUR_BLOCK]->textureID = LoadTGA("Image//MainScene//building.tga");

	meshList[FIVE_BLOCK] = MeshBuilder::GenerateOBJ("five sized", "OBJ//Platformer//5-Platform.obj");
	meshList[FIVE_BLOCK]->textureID = LoadTGA("Image//MainScene//building5.tga");

	meshList[START_END] = MeshBuilder::GenerateOBJ("start-end", "OBJ//Platformer//4-Platform.obj");
	meshList[START_END]->textureID = LoadTGA("Image//MainScene//building6.tga");

	meshList[TREASURE] = MeshBuilder::GenerateOBJ("treasure", "OBJ//Platformer//PlatformerTreasure.obj");
	meshList[TREASURE]->textureID = LoadTGA("Image//Platformer//PlatformerTreasure.tga");
	//===============================TEXT===============================================
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//PrestigeElite.tga");

	meshList[GEO_LOAD1] = MeshBuilder::GenerateQuad("load", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOAD1]->textureID = LoadTGA("Image//loading1.tga");
	
	meshList[GEO_LOAD2] = MeshBuilder::GenerateQuad("load", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOAD2]->textureID = LoadTGA("Image//loading2.tga");

	meshList[GEO_LOAD3] = MeshBuilder::GenerateQuad("load", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOAD3]->textureID = LoadTGA("Image//loading3.tga");

	meshList[GEO_LOAD4] = MeshBuilder::GenerateQuad("load", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOAD4]->textureID = LoadTGA("Image//loading4.tga");

	meshList[PAUSE_SCREEN] = MeshBuilder::GenerateQuad("pause", Color(1, 1, 1), 1, 1);
	meshList[PAUSE_SCREEN]->textureID = LoadTGA("Image//pause.tga");

	meshList[PAUSE2_SCREEN] = MeshBuilder::GenerateQuad("pauseResume", Color(1, 1, 1), 1, 1);
	meshList[PAUSE2_SCREEN]->textureID = LoadTGA("Image//pauseResume.tga");

	meshList[PAUSE3_SCREEN] = MeshBuilder::GenerateQuad("pauseQuit", Color(1, 1, 1), 1, 1);
	meshList[PAUSE3_SCREEN]->textureID = LoadTGA("Image//pauseQuit.tga");

	meshList[GAME_OVER] = MeshBuilder::GenerateQuad("gameover", Color(1, 1, 1), 1, 1);
	meshList[GAME_OVER]->textureID = LoadTGA("Image//GameOver1.tga");

	meshList[GAME_OVER2] = MeshBuilder::GenerateQuad("gameover1", Color(1, 1, 1), 1, 1);
	meshList[GAME_OVER2]->textureID = LoadTGA("Image//GameOver2.tga");

	meshList[GAME_OVER3] = MeshBuilder::GenerateQuad("gameover2", Color(1, 1, 1), 1, 1);
	meshList[GAME_OVER3]->textureID = LoadTGA("Image//GameOver3.tga");

	setPlatforms();

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 2000.0f);
	projectionStack.LoadMatrix(projection);

	changeScene = 0;
	game_state = ABILITY_SELECT;
}

void Platformer::Update(double dt)
{
	if (playLoading)
	{
		load_time += dt;
		if (load_time >= 5)
			playLoading = false;
	}

	elapsed_time += dt;
	
	switch (game_state)
	{
	case GAME:
		camera.Update(dt, platformID, treasure);
		pickUpTreasure();

		if (Application::IsKeyPressed(VK_BACK))
			game_state = PAUSE;
		
		if (camera.position.y < -150)
			game_state = GAMEOVER;

		// If all treasure is collected
		if (camera.numPickedUp == numOfTreasureSet)
			allCollected = true;

		// If all treasure is collected and player is on the end block, game ends
		if (allCollected)
		{
			if ((camera.position - platformID[5].at(1).pos).Length() < 10)
				game_state = GAMEOVER;
		}

		//============Rotate the treasure=======================
		if (treasureRotate < 360)
			treasureRotate += dt * 90;
		else
			treasureRotate = 0;

		// Notifies player about how many treasure needs to be collected
		treasureCollected = std::to_string(camera.numPickedUp) + "/" + std::to_string(numOfTreasureSet) + " TREASURE COLLECTED";
		break;

	case PAUSE:
		if ((Application::IsKeyPressed(VK_DOWN)) && (elapsed_time > pause_bounce_time)) // Down
		{
			if (pauseSelect == 1)
			{
				pauseSelect++;
			}
			pause_bounce_time = elapsed_time + 0.2;
		}
		if ((Application::IsKeyPressed(VK_UP)) && (elapsed_time > pause_bounce_time))// Up
		{
			if (pauseSelect == 2)
			{
				pauseSelect--;
			}
			pause_bounce_time = elapsed_time + 0.2;
		}
		if (pauseSelect == 1 && (Application::IsKeyPressed(VK_RETURN)))
			game_state = GAME;
		else if (pauseSelect == 2 && (Application::IsKeyPressed(VK_RETURN)))
		{
			changeScene = 1;
		}
		break;
	case GAMEOVER:
		if ((Application::IsKeyPressed(VK_DOWN)) && (elapsed_time > gameover_bounce_time)) // Down
		{
			if (gameoverSelect == 1)
			{
				gameoverSelect++;
			}
			gameover_bounce_time = elapsed_time + 0.2;
		}
		if ((Application::IsKeyPressed(VK_UP)) && (elapsed_time > gameover_bounce_time)) // Up
		{
			if (gameoverSelect == 2)
			{
				gameoverSelect--;
			}
			gameover_bounce_time = elapsed_time + 0.2;
		}
		if (gameoverSelect == 1 && (Application::IsKeyPressed(VK_RETURN)))
			changeScene = 2;
		else if (gameoverSelect == 2 && (Application::IsKeyPressed(VK_RETURN)))
			changeScene = 1;
		break;
	case ABILITY_SELECT:
		if (Application::IsKeyPressed('1'))
		{
			camera.doubleJumpAbility = true;
			Money::getInstance()->deductMoney(5);
			game_state = GAME;
		}
		else if (Application::IsKeyPressed('2'))
		{
			camera.superSpeedAbility = true;
			Money::getInstance()->deductMoney(5);
			game_state = GAME;
		}
		else if (Application::IsKeyPressed('0'))
			game_state = GAME;
		break;
	}
	
}

void Platformer::Render()
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
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);
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
		modelStack.PushMatrix();
		modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
		RenderMesh(meshList[GEO_LIGHTBALL], false);
		modelStack.PopMatrix();

		RenderSkybox();
		if (game_state == GAME)
		RenderPlatforms();

		RenderTextOnScreen(meshList[GEO_TEXT], treasureCollected, Color(1, 0, 0), 3, 0.5, 19.5);
		RenderTextOnScreen(meshList[GEO_TEXT], X, Color(0, 1, 1), 3, 1.5, 2.5);
		RenderTextOnScreen(meshList[GEO_TEXT], Y, Color(0, 1, 1), 3, 1.5, 1.5);
		RenderTextOnScreen(meshList[GEO_TEXT], Z, Color(0, 1, 1), 3, 1.5, 0.5);
		
		if (game_state == PAUSE)
			RenderPause();

		if (game_state == GAMEOVER)
			RenderGameover();

		if (game_state == ABILITY_SELECT)
			RenderAbilitySelect();
	}
	
}

void Platformer::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(-24.95f * 21.f, 0, 0);
	modelStack.Rotate(-90.f, 0, 1.f, 0);
	modelStack.Scale(1050.f, 1050.f, 1050.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(24.95f * 21.f, 0, 0);
	modelStack.Rotate(90.f, 0, 1.f, 0);
	modelStack.Scale(1050.f, 1050.f, 1050.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -24.95f * 21.f);
	modelStack.Rotate(180.f, 0, 1.f, 0);
	modelStack.Scale(1050.f, 1050.f, 1050.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 24.95f * 21.f);
	modelStack.Scale(1050.f, 1050.f, 1050.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -24.945f * 21.f, 0);
	modelStack.Rotate(-90.f, 0, 1.f, 0);
	modelStack.Rotate(90.f, 1.f, 0, 0);
	modelStack.Scale(1050.f, 1050.f, 1050.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 24.945f * 21.f, 0);
	modelStack.Rotate(-90.f, 0, 1.f, 0);
	modelStack.Rotate(-90.f, 1.f, 0, 0);
	modelStack.Scale(1050.f, 1050.f, 1050.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
}

void Platformer::RenderMesh(Mesh *mesh, bool enableLight)
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

void Platformer::RenderText(Mesh* mesh, std::string text, Color color)
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

void Platformer::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
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
	modelStack.Translate((float)x, (float)y, 0);
	modelStack.Scale((float)sizex, (float)sizey, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Platformer::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	float spacingX = 0.f;
	float spacingY = 0.f;
	int count = 0;

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
		if (text[i] == '@')
		{
			spacingX += 1.0f * count;
			spacingY -= 1.0f;
			text[i] = ' ';
			count = 0;
		}

		else
			characterSpacing.SetToTranslation(i * 1.0f - spacingX, spacingY, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
		count++;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void Platformer::Exit()
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

void Platformer::setPlatforms()
{
	for (int j = 0; j < 9; j++)
	{
		for (int i = 0; i < 40; i++)
		{
			for (int z = 0; z < 100; z++)
			{
				mapData[j][i][z] = 0;
			}
		}
	}	

	srand((int)time(NULL));
	for (int startPoint = 0; startPoint < 4; startPoint++)
		mapData[4][mapHeight][startPoint] = 7;
	Platforms first;
	first.type = 4;
	first.pos.Set(-480.f, (float)(mapHeight * 5), 0.f);
	first.platformAABB.SaveCoord(Vector3(first.pos.x - 10, first.pos.y - (float)2.5, first.pos.z - 10),
		Vector3(first.pos.x + 10, first.pos.y + (float)2.5, first.pos.z + 10));
	platformID[5].push_back(first);
	
	bool placeTreasure = false;

	for (int mapZpos = 0; mapZpos < 9; mapZpos++)
	{
		mapHeight = 20;
		if (mapZpos == 4 || mapZpos == 6 || mapZpos == 8)
		{
			if (numOfTreasureSet < 4)
				placeTreasure = true;
		}
		for (int mapPos = 4; mapPos < 96; mapPos++)
		{
			Platforms platformCreated;
			int platformType;
			int randomNum = (int)RandomNumber(1.f, 9.f);
			if (randomNum % 8 != 0 || (haveSpace))
			{ // Don't have empty spaces
				haveSpace = false;
				randomNum = (int)RandomNumber(1.f, 5.f);
				if ((randomNum == 3) || (randomNum == 1) && (mapHeight < 35) && (!haveSpace))
				{ // Go Up
					mapHeight += 3;
					randomNum = (int)RandomNumber(1.f, 5.f);
					for (int blockNum = 0; blockNum < randomNum; blockNum++)
					{
						mapData[mapZpos][mapHeight][mapPos + blockNum] = randomNum;
					}
					mapPos += (randomNum - 1);
					platformType = randomNum;
				}
				else if ((randomNum == 4) || (randomNum == 2) && (mapHeight > 5) && (!haveSpace))
				{ // Go Down
					mapHeight -= 3;
					randomNum = (int)RandomNumber(1.f, 6.f);
					for (int blockNum = 0; blockNum < randomNum; blockNum++)
					{
						mapData[mapZpos][mapHeight][mapPos + blockNum] = randomNum;
					}
					mapPos += (randomNum - 1);
					platformType = randomNum;
				}
				else
				{ // Stay Same Height
					randomNum = (int)RandomNumber(1.f, 6.f);
					for (int blockNum = 0; blockNum < randomNum; blockNum++)
					{
						mapData[mapZpos][mapHeight][mapPos + blockNum] = randomNum;
					}
					mapPos += (randomNum - 1);
					platformType = randomNum;
				}

				platformCreated.type = platformType;

				switch (platformType)
				{
				case 1:
					platformCreated.pos.Set(((((float)(mapPos)+(float)0.5) * 10) - 500), (float)(mapHeight * 5), (float)(mapZpos * 50 - 125));
					platformCreated.platformAABB.SaveCoord(Vector3(platformCreated.pos.x - 5, platformCreated.pos.y - (float)2.5, platformCreated.pos.z - 10),
						Vector3(platformCreated.pos.x + 5.f, platformCreated.pos.y + (float)2.5, platformCreated.pos.z + 10.f));
					platformID[0].push_back(platformCreated);
					break;
				case 2:
					platformCreated.pos.Set((((float)((mapPos - 1) + 1) * 10) - 500), (float)(mapHeight * 5), (float)(mapZpos * 50 - 125));
					platformCreated.platformAABB.SaveCoord(Vector3(platformCreated.pos.x - 5, platformCreated.pos.y - (float)2.5, platformCreated.pos.z - 10),
						Vector3(platformCreated.pos.x + 5.f, platformCreated.pos.y + (float)2.5, platformCreated.pos.z + 10.f));
					platformID[1].push_back(platformCreated);
					break;
				case 3:
					platformCreated.pos.Set((((float)((mapPos - 2) + 1.5f) * 10) - 500), (float)(mapHeight * 5), (float)(mapZpos * 50 - 125));
					platformCreated.platformAABB.SaveCoord(Vector3(platformCreated.pos.x - 5, platformCreated.pos.y - (float)2.5, platformCreated.pos.z - 15),
						Vector3(platformCreated.pos.x + 5, platformCreated.pos.y + (float)2.5, platformCreated.pos.z + 15));
					platformID[2].push_back(platformCreated);
					break;
				case 4:
					if (numOfTreasureSet < 4)
					{
						if ((int)RandomNumber(1, 20) == 1 || placeTreasure)
						{
							PlatformerTreasure setTreasure;
							setTreasure.pickedUp = false;
							setTreasure.pos.Set((((float)((mapPos - 4) + 2.5f) * 10) - 500), (float)(mapHeight * 5 + 5), (float)(mapZpos * 50 - 125));
							setTreasure.treasureAABB.SaveCoord(Vector3(setTreasure.pos.x - (float)2.5, setTreasure.pos.y - 2.5f, setTreasure.pos.z - 2.5f),
								Vector3((float)setTreasure.pos.x + 2.5f, setTreasure.pos.y + (float)2.5, setTreasure.pos.z + 2.5f));
							treasure.push_back(setTreasure);
							numOfTreasureSet++;
							placeTreasure = false;
						}
					}
					platformCreated.pos.Set(((((float)(mapPos - 3) + 2) * 10) - 500), (float)(mapHeight * 5), (float)(mapZpos * 50 - 125));
					platformCreated.platformAABB.SaveCoord(Vector3(platformCreated.pos.x - 10.f, (float)platformCreated.pos.y - 2.5f, platformCreated.pos.z - 20.f),
						Vector3(platformCreated.pos.x + 10.f, (float)platformCreated.pos.y + 2.5f, platformCreated.pos.z + 20.f));
					platformID[3].push_back(platformCreated);
					break;
				case 5:
					if (numOfTreasureSet < 4)
					{
						if ((int)RandomNumber(1, 20) == 1 || placeTreasure)
						{
							PlatformerTreasure setTreasure;
							setTreasure.pickedUp = false;
							setTreasure.pos.Set(((((float)((float)mapPos - 4.f) + 2.5f) * 10.f) - 500.f), (float)((float)mapHeight * 5.f + 5.f), (float)(mapZpos * 50.f - 125.f));
							setTreasure.treasureAABB.SaveCoord(Vector3(setTreasure.pos.x - 2.5f, setTreasure.pos.y - 2.5f, setTreasure.pos.z - 2.5f),
								Vector3(setTreasure.pos.x + 2.5f, setTreasure.pos.y + 2.5f, setTreasure.pos.z + 2.5f));
							treasure.push_back(setTreasure);
							numOfTreasureSet++;
							placeTreasure = false;
						}
					}
					platformCreated.pos.Set(((((float)mapPos - 4.f) + 2.5f) * 10.f) - 500.f, (float)mapHeight * 5.f, (float)mapZpos * 50.f - 125.f);
					platformCreated.platformAABB.SaveCoord(Vector3(platformCreated.pos.x - 12.5f, platformCreated.pos.y - 2.5f, platformCreated.pos.z - 20.f),
						Vector3(platformCreated.pos.x + 12.5f, platformCreated.pos.y + 2.5f, platformCreated.pos.z + 20.f));
					platformID[4].push_back(platformCreated);
					break;
				}
			}
			else
			{ // Empty blocks
				haveSpace = true;
				for (int emptyBlocks = 0; emptyBlocks < 2; emptyBlocks++)
				{
					mapData[mapZpos][mapHeight + 4][mapPos + emptyBlocks] = 6;
				}
				mapPos++;
			}
		}
	}
	
	if (mapHeight < 35)
		mapHeight += 3;

	for (int lastBlock = 96; lastBlock < 100; lastBlock++)
	{
		mapData[4][mapHeight][lastBlock] = 7;
	}
	Platforms last;
	last.type = 4;
	last.pos.Set(480.f, (float)mapHeight * 5.f, 0);
	last.platformAABB.SaveCoord(Vector3(last.pos.x - 10, (float)last.pos.y - 2.5f, last.pos.z - 10),
		Vector3(last.pos.x + 10, (float)last.pos.y + 2.5f, last.pos.z + 10));
	platformID[5].push_back(last);
}	

void Platformer::RenderPlatforms()
{
	for (int width = 0; width < 9; width++)
	{
		for (int height = 0; height < 40; height++)
		{
			for (int length = 0; length < 100; length++)
			{
				switch (mapData[width][height][length])
				{
				case 1:
					modelStack.PushMatrix();
					modelStack.Translate(((((float)length) + 0.5f) * 10.f) - 500.f, (float)height * 5.f, (float)width * 50.f - 125.f);
					modelStack.Scale(10.f, 5.f, 10.f);
					RenderMesh(meshList[ONE_BLOCK], false);
					modelStack.PopMatrix();
					break;
				case 2:
					modelStack.PushMatrix();
					modelStack.Translate(((((float)length) + 1.f) * 10.f) - 500.f, (float)height * 5.f, (float)width * 50.f - 125.f);
					modelStack.Rotate(90.f, 0, 1.f, 0);
					modelStack.Scale(10.f, 5.f, 5.f);
					RenderMesh(meshList[TWO_BLOCK], false);
					modelStack.PopMatrix();
					length += 1;
					break;
				case 3:
					modelStack.PushMatrix();
					modelStack.Translate(((((float)length) + 1.5f) * 10.f) - 500.f, (float)height * 5.f, (float)width * 50.f - 125.f);
					modelStack.Rotate(90.f, 0, 1.f, 0);
					modelStack.Scale(10.f, 5.f, 5.f);
					RenderMesh(meshList[THREE_BLOCK], false);
					modelStack.PopMatrix();
					length += 2;
					break;
				case 4:
					modelStack.PushMatrix();
					modelStack.Translate(((((float)length) + 2.f) * 10.f) - 500.f, (float)height * 5.f, (float)width * 50.f - 125.f);
					modelStack.Rotate(90.f, 0, 1.f, 0);
					modelStack.Scale(10.f, 5.f, 5.f);
					RenderMesh(meshList[FOUR_BLOCK], false);
					modelStack.PopMatrix();
					length += 3;
					break;
				case 5:
					modelStack.PushMatrix();
					modelStack.Translate(((((float)length) + 2.5f) * 10.f) - 500, (float)height * 5.f, (float)width * 50.f - 125.f);
					modelStack.Scale(5.f, 5.f, 10.f);
					RenderMesh(meshList[FIVE_BLOCK], false);
					modelStack.PopMatrix();
					length += 4;
					break;
				case 7:
					modelStack.PushMatrix();
					modelStack.Translate(((((float)length) + 2.f) * 10.f) - 500.f, (float)height * 5.f, 0);
					modelStack.Scale(5.f, 5.f, 5.f);
					RenderMesh(meshList[START_END], false);
					modelStack.PopMatrix();
					length += 3;
					break;
				}
			}
		}
	}
	for (std::vector<PlatformerTreasure>::iterator treasureNum = treasure.begin(); treasureNum != treasure.end(); treasureNum++)
	{
		if (treasureNum->pickedUp == false)
		{
			modelStack.PushMatrix();
			modelStack.Translate((*treasureNum).pos.x, (*treasureNum).pos.y, (*treasureNum).pos.z);
			modelStack.Rotate((float)treasureRotate, 0, 1.f, 0);
			modelStack.Scale(5.f, 5.f, 5.f);
			RenderMesh(meshList[TREASURE], false);
			modelStack.PopMatrix();
		}		
	}
}

float Platformer::RandomNumber(float min, float max)
{
	return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}

void Platformer::pickUpTreasure()
{
	for (std::vector<PlatformerTreasure>::iterator it = treasure.begin(); it != treasure.end(); it++)
	{
		if (((it->pos - camera.position).Length() < 5) && (Application::IsKeyPressed('E')) && (it->pickedUp == false) && (elapsed_time > bounce_time))
		{
			bounce_time = elapsed_time + 0.5;
			it->pickedUp = true;
			camera.numPickedUp++;
			break;
		}
	}
}

void Platformer::RenderPause()
{
	if (pauseSelect == 0)
	{
		RenderMeshOnScreen(meshList[PAUSE_SCREEN], 40, 22, 80, 80);
	}
	if (pauseSelect == 1)
	{
		RenderMeshOnScreen(meshList[PAUSE2_SCREEN], 40, 22, 80, 80);
	}		
	if (pauseSelect == 2)
	{
		RenderMeshOnScreen(meshList[PAUSE3_SCREEN], 40, 22, 80, 80);
	}
}

void Platformer::RenderGameover()
{
	if (gameoverSelect == 0)
	{
		RenderMeshOnScreen(meshList[GAME_OVER], 40, 22, 80, 80);
	}
	if (gameoverSelect == 1)
	{
		RenderMeshOnScreen(meshList[GAME_OVER2], 40, 22, 80, 80);
	}
	if (gameoverSelect == 2)
	{
		RenderMeshOnScreen(meshList[GAME_OVER3], 40, 22, 80, 80);
	}
}

void Platformer::RenderAbilitySelect()
{
	if (abilitySelect == 0)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], FileReading::getInstance()->getWords(4), Color(0, 0, 0), 3, 4, 10);
	}
}