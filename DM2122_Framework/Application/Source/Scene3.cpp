#include "Scene3.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"


Scene3::Scene3()
{
}

Scene3::~Scene3()
{
}

void Scene3::Init()
{
	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // Set background color to dark blue

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

		// Load vertex and fragment shaders
	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader");

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_indexBuffer[0]);

	// Get a handle for out "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	// Use our shader
	glUseProgram(m_programID);

	// Set default values
	rotateAngle = 0;
	translateX = 0;
	scaleAll = 1;

	// Initialise Camera
	camera.Init(Vector3(4, 3, 3), Vector3(0, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("reference", Color(0, 1, 1), 1, 1);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 0), 1, 1, 1);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(0, 1, 0), 20, 1);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("circle", Color(1, 0, 0), 20, 1, 0.7, 6);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 1), 18, 36, 1);
	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("hemisphere", Color(1, 0, 1), 9, 18, 1);
}

void Scene3::Update(double dt)
{
	static float direction = 1;
	static float translateDir = 1;
	static float rotateDir = 1;

	if (Application::IsKeyPressed(VK_SPACE))
		direction *= -1;
	if ((translateX < -40) || (translateX > 40))
		translateDir *= -1;
	if ((rotateAngle < -180) || (rotateAngle > 180))
		rotateDir *= 1;
	if (scaleAll > 10)
		scaleAll = 0;
	else if (scaleAll < 0)
		scaleAll = 10;

	rotateAngle += (float)(90 * rotateDir * dt * direction);
	translateX += (float)(40 * translateDir * dt * direction);
	scaleAll += (float)(2 * dt * direction);

	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	else if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	else if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	camera.Update(dt);
}

void Scene3::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color & depth buffer every frame

	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToLookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);
	projection.SetToOrtho(-40, 40, -30, 30, -100, 100); // Size of playground (Where the stuff happens)
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f); //FOV, Aspect Ratio, Near Plane, Far Plane

	//-----------------------------------
	model.SetToIdentity();
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_AXES]->Render();

	//---------------------------------
	scale.SetToScale(1, 1, 1);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(2, 1, 0);
	model = translate * rotate * scale; // Scale, followed by rotate, then translate
	MVP = projection * view * model; // Matrix multiplication is opposite
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); // Update shader with new MVP

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	meshList[GEO_CIRCLE]->Render();

	//-----------------------------------------------
	translate.SetToIdentity();
	model.SetToIdentity();

	translate.SetToTranslation(1, 2, 0);
	model = translate * rotate * scale; // Scale, followed by rotate, then translate
	MVP = projection * view * model; // Matrix multiplication is opposite
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); // Update shader with new MVP

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	meshList[GEO_RING]->Render();

	//----------------------------------------------------------------
	translate.SetToIdentity();
	model.SetToIdentity();

	translate.SetToTranslation(-2, -1, 0);
	model = translate * rotate * scale; // Scale, followed by rotate, then translate
	MVP = projection * view * model; // Matrix multiplication is opposite
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); // Update shader with new MVP

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	meshList[GEO_CUBE]->Render();

	//------------------------------------------
	translate.SetToIdentity();
	model.SetToIdentity();

	translate.SetToTranslation(2, -1, 0);
	model = translate * rotate * scale; // Scale, followed by rotate, then translate
	MVP = projection * view * model; // Matrix multiplication is opposite
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); // Update shader with new MVP

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	meshList[GEO_SPHERE]->Render();

	//---------------------------------------------
	translate.SetToIdentity();
	model.SetToIdentity();

	translate.SetToTranslation(-1, -3, 0);
	model = translate * rotate * scale; // Scale, followed by rotate, then translate
	MVP = projection * view * model; // Matrix multiplication is opposite
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); // Update shader with new MVP

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	meshList[GEO_HEMISPHERE]->Render();

	//---------------------------------------------
	translate.SetToIdentity();
	model.SetToIdentity();

	translate.SetToTranslation(1, 3, 0);
	model = translate * rotate * scale; // Scale, followed by rotate, then translate
	MVP = projection * view * model; // Matrix multiplication is opposite
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); // Update shader with new MVP

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	meshList[GEO_QUAD]->Render();
}

void Scene3::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
