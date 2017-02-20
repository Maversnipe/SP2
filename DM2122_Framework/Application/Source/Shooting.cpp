#include "Shooting.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <iostream>
using namespace std;
//char mapArray[500][500] = {""};
Shooting::Shooting()
{
}

Shooting::~Shooting()
{
}

void Shooting::Init()
{
	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // Set background color to dark blue

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	// Blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");


	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_indexBuffer[0]);

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

	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(0, 20, 10);
	light[0].color.Set(1, 1, 1);
	light[0].power = 3;
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
	Camera3.Init(Vector3(0, 0, 200), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("reference", Color(1, 1, 1), 1, 1);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//color2.tga");	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(1, 1, 1), 1, 1);
	meshList[GEO_FLOOR]->textureID = LoadTGA("Image//Shooting//floor.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1, 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Shooting//front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1, 1);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Shooting//back.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1, 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Shooting//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1, 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Shooting//right.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1, 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Shooting//bottom.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1, 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Shooting//top.tga");

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 0), 1, 1, 1);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightSphere", Color(1, 1, 1), 18, 36, 1);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//PrestigeElite.tga");

	meshList[GEO_ENEMY] = MeshBuilder::GenerateCube("enemy", Color(1, 0, 0), 1, 1, 1);


	//====================================OBJECTS==========================================

	meshList[GEO_TABLE] = MeshBuilder::GenerateOBJ("table", "OBJ//table.obj");
	meshList[GEO_TABLE]->textureID = LoadTGA("Image//Shooting//table.tga");

	meshList[GEO_GUN] = MeshBuilder::GenerateOBJ("gun", "OBJ//gun.obj");
	meshList[GEO_GUN]->textureID = LoadTGA("Image//Shooting//gun.tga");

	meshList[GEO_BULLET] = MeshBuilder::GenerateSphere("bullet", Color(1, 0, 0), 18, 36, 1);
	//===================================LASERS DISPLAY=====================================


	meshList[GEO_LASER0] = MeshBuilder::GenerateQuad("laser0", Color(1, 1, 1), 1, 1);
	meshList[GEO_LASER0]->textureID = LoadTGA("Image//Shooting//laser0.tga");

	meshList[GEO_LASER1] = MeshBuilder::GenerateQuad("laser1", Color(1, 1, 1), 1, 1);
	meshList[GEO_LASER1]->textureID = LoadTGA("Image//Shooting//laser1.tga");

	meshList[GEO_LASER2] = MeshBuilder::GenerateQuad("laser2", Color(1, 1, 1), 1, 1);
	meshList[GEO_LASER2]->textureID = LoadTGA("Image//Shooting//laser2.tga");

	meshList[GEO_LASER3] = MeshBuilder::GenerateQuad("laser3", Color(1, 1, 1), 1, 1);
	meshList[GEO_LASER3]->textureID = LoadTGA("Image//Shooting//laser3.tga");

	meshList[GEO_LASER4] = MeshBuilder::GenerateQuad("laser4", Color(1, 1, 1), 1, 1);
	meshList[GEO_LASER4]->textureID = LoadTGA("Image//Shooting//laser4.tga");

	meshList[GEO_LASER5] = MeshBuilder::GenerateQuad("laser5", Color(1, 1, 1), 1, 1);
	meshList[GEO_LASER5]->textureID = LoadTGA("Image//Shooting//laser5.tga");

	//=======================================HEALTH=========================================
	meshList[GEO_HEALTH] = MeshBuilder::GenerateQuad("health", Color(1, 1, 1), 1, 1);
	meshList[GEO_HEALTH]->textureID = LoadTGA("Image//health.tga");

	//=======================================ROCK===========================================
	meshList[GEO_ROCKS] = MeshBuilder::GenerateQuad("rock", Color(1, 1, 1), 1, 1);
	meshList[GEO_ROCKS]->textureID = LoadTGA("Image//starRocks.tga");
	//======================================================================================
	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 2000.0f);
	projectionStack.LoadMatrix(projection);

	for (int counter = 0; counter < 10; counter++)
	{
		float i = RandomNumber(-250, 250);
		float j = RandomNumber(-250, 250);
		enemyPos[counter].Set(i, 0, j);
		enemyDisappear[counter] = false;
	}
}

void Shooting::Update(double dt)
{
	elapsed_time += dt;
	//==========Enemy movements==========
	for (int counter = 0; counter < 10; counter++)
	{
		if (enemyDisappear[counter] == false)
		{
			if ((Camera3.position - enemyPos[counter]).Length() > 10)
			{
				Vector3 dirVec = Camera3.position - enemyPos[counter];
				enemyPos[counter] += dirVec * dt;

			}
			else
				enemyPos[counter] = enemyPos[counter];
		}
	}
	//====================================
	fps = 1.0f / dt;
	framesPerSec = "FPS: " + std::to_string(fps);

	x = Camera3.position.x;
	y = Camera3.position.y;
	z = Camera3.position.z;
	X = "X: " + std::to_string(x);
	Y = "Y: " + std::to_string(y);
	Z = "Z: " + std::to_string(z);

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

	if ((Camera3.position.z < 6) && (Application::IsKeyPressed('E')))
		pickUpGun = true;

	if (pickUpGun)
	{
		light[0].position.Set(0, 30, -40);
		light[0].color.Set(0,1,0);
		light[0].power = 5;
		light[0].type = Light::LIGHT_POINT;
	}
//=======================BULLET MOVEMENTS===================
	if (Application::IsKeyPressed(VK_LBUTTON) && pickUpGun && (!reload) && elapsed_time > bounce_time)
	{
		bullet[bulletCount].pos = Camera3.position;
		original[bulletCount].pos = bullet[bulletCount].pos;

		bullet[bulletCount].vel = (Camera3.target - Camera3.position).Normalized() * 3;
		rotateLasHori = horizontalRotation;
		rotateLasVert = verticalRotation;

		moveLaser[bulletCount] = true;
		bulletCount++;
		bounce_time = elapsed_time + 0.2;
	}

	for (int i = 0; i < 5; i++)
	{
		if ((original[i].pos - bullet[i].pos).Length() > 50)
			moveLaser[i] = false;
		else if (moveLaser[i])
			bullet[i].pos += bullet[i].vel;
	}
	if (bulletCount >= 5)
	{
		bulletCount = 0;
		reload = true;
		bounce_time = elapsed_time + 2.5;
	}
	if (elapsed_time > bounce_time && reload)
	{
		reload = false;
	}
//===========================================================
	Camera3.Update(dt, &horizontalRotation, &verticalRotation);
}

void Shooting::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Mtx44 MVP;

	//View Stack
	viewStack.LoadIdentity();
	viewStack.LookAt(Camera3.position.x, Camera3.position.y, Camera3.position.z, Camera3.target.x, Camera3.target.y, Camera3.target.z, Camera3.up.x, Camera3.up.y, Camera3.up.z);
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); // Remember, matrix multiplication is the other way around
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP

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


	//---------------------------------------------------------------
	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	RenderSkybox();

	modelStack.PushMatrix();
	modelStack.Translate(0, -5, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FLOOR], true);
	modelStack.PopMatrix();


	for (int i = 0; i < 10; i++)
	{
		if (enemyDisappear[i] == false)
		{
			modelStack.PushMatrix();
			modelStack.Translate(enemyPos[i].x, 0, enemyPos[i].z);
			RenderMesh(meshList[GEO_ENEMY], true);
			modelStack.PopMatrix();
		}
	}


	modelStack.PushMatrix();
	modelStack.Translate(0, -3, 0);

	if (!pickUpGun)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 2.2, 0);
		modelStack.Scale(1, 0.3, 1);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Rotate(90, 0, 0, 1);
		RenderMesh(meshList[GEO_GUN], true);
		modelStack.PopMatrix();
	}

	modelStack.Scale(6, 2, 2);
	RenderMesh(meshList[GEO_TABLE], true);
	modelStack.PopMatrix();

	if (pickUpGun)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Camera3.target.x, Camera3.target.y, Camera3.target.z);
		modelStack.Rotate(horizontalRotation, 0, 1, 0);
		modelStack.Rotate(verticalRotation, 1, 0, 0);

		modelStack.PushMatrix();
		modelStack.Translate(0.3, -0.2, -0.2);
		modelStack.Rotate(10, 0, 1, 0);
		modelStack.Scale(0.15, 0.15, 0.15);
		RenderMesh(meshList[GEO_GUN], true);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}
//==============================BULLETS===================================
	if (moveLaser[0])
	{

		modelStack.PushMatrix();
		modelStack.Translate(bullet[0].pos.x, bullet[0].pos.y, bullet[0].pos.z);
		modelStack.Rotate(rotateLasHori, 0, 1, 0);
		modelStack.Rotate(rotateLasVert, 1, 0, 0);

		modelStack.PushMatrix();
		modelStack.Translate(0.6, -0.4, 0);
		modelStack.Rotate(5, 0, 1, 0);
		modelStack.Scale(0.1, 0.1, 0.8);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}

	if (moveLaser[1])
	{

		modelStack.PushMatrix();
		modelStack.Translate(bullet[1].pos.x, bullet[1].pos.y, bullet[1].pos.z);
		modelStack.Rotate(rotateLasHori, 0, 1, 0);
		modelStack.Rotate(rotateLasVert, 1, 0, 0);

		modelStack.PushMatrix();
		modelStack.Translate(0.6, -0.4, 0);
		modelStack.Rotate(5, 0, 1, 0);
		modelStack.Scale(0.1, 0.1, 0.8);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}

	if (moveLaser[2])
	{

		modelStack.PushMatrix();
		modelStack.Translate(bullet[2].pos.x, bullet[2].pos.y, bullet[2].pos.z);
		modelStack.Rotate(rotateLasHori, 0, 1, 0);
		modelStack.Rotate(rotateLasVert, 1, 0, 0);

		modelStack.PushMatrix();
		modelStack.Translate(0.6, -0.4, 0);
		modelStack.Rotate(5, 0, 1, 0);
		modelStack.Scale(0.1, 0.1, 0.8);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}

	if (moveLaser[3])
	{

		modelStack.PushMatrix();
		modelStack.Translate(bullet[3].pos.x, bullet[3].pos.y, bullet[3].pos.z);
		modelStack.Rotate(rotateLasHori, 0, 1, 0);
		modelStack.Rotate(rotateLasVert, 1, 0, 0);

		modelStack.PushMatrix();
		modelStack.Translate(0.6, -0.4, 0);
		modelStack.Rotate(5, 0, 1, 0);
		modelStack.Scale(0.1, 0.1, 0.8);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}

	if (moveLaser[4])
	{

		modelStack.PushMatrix();
		modelStack.Translate(bullet[4].pos.x, bullet[4].pos.y, bullet[4].pos.z);
		modelStack.Rotate(rotateLasHori, 0, 1, 0);
		modelStack.Rotate(rotateLasVert, 1, 0, 0);

		modelStack.PushMatrix();
		modelStack.Translate(0.6, -0.4, 0);
		modelStack.Rotate(5, 0, 1, 0);
		modelStack.Scale(0.1, 0.1, 0.8);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();

		modelStack.PopMatrix();
	}
//============================================================================


	RenderTextOnScreen(meshList[GEO_TEXT], X, Color(0, 1, 1), 3, 0.5, 2.5);
	RenderTextOnScreen(meshList[GEO_TEXT], Y, Color(0, 1, 1), 3, 0.5, 1.5);
	RenderTextOnScreen(meshList[GEO_TEXT], Z, Color(0, 1, 1), 3, 0.5, 0.5);
//	RenderTextOnScreen(meshList[GEO_TEXT], framesPerSec, Color(0, 1, 1), 3, 0.5, 0.5);
	if (reload)
		RenderTextOnScreen(meshList[GEO_TEXT], "RELOADING...", Color(1, 0, 0), 5, 3, 6.5);

	if (!reload && pickUpGun)
	switch (bulletCount)
	{
	case 0:
		RenderMeshOnScreen(meshList[GEO_LASER5], 65, 5, 20, 20);
		break;
	case 1:
		RenderMeshOnScreen(meshList[GEO_LASER4], 65, 5, 20, 20);
		break;
	case 2:
		RenderMeshOnScreen(meshList[GEO_LASER3], 65, 5, 20, 20);
		break;
	case 3:
		RenderMeshOnScreen(meshList[GEO_LASER2], 65, 5, 20, 20);
		break;
	case 4:
		RenderMeshOnScreen(meshList[GEO_LASER1], 65, 5, 20, 20);
		break;
	}

	else if (reload)
		RenderMeshOnScreen(meshList[GEO_LASER0], 65, 5, 20, 20);

	RenderMeshOnScreen(meshList[GEO_HEALTH], 6, 57, 4, 4);
	RenderMeshOnScreen(meshList[GEO_ROCKS], 75, 57, 4, 4);
//	RenderMeshOnScreen(meshList[GEO_QUAD], 5, 5, 5, 5);//No transform needed
}

void Shooting::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(-24.95 * 20, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FRONT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(24.95 * 20, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -24.95 * 20);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_LEFT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 24.95 * 20);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_RIGHT], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -24.945 * 20, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 24.945 * 20, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_TOP], true);
	modelStack.PopMatrix();


}

void Shooting::RenderMesh(Mesh *mesh, bool enableLight)
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

void Shooting::RenderText(Mesh* mesh, std::string text, Color color)
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

void Shooting::RenderMeshOnScreen(Mesh* mesh, int x, int y, int
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

void Shooting::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

float Shooting::RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

void Shooting::Exit()
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