#include "Scene2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"


Scene2::Scene2()
{
}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // Set background color to dark blue

	// Set default values
	rotateAngle = 0;
	translateX = 0;
	scaleAll = 1;

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat vertex_buffer_data[] =
	{
		-0.25f, 0.75f, 0.0f, // vertex 0 of triangle
		0.25f, 0.75f, 0.0f, // vertex 1 of triangle
		-0.55f, 0.4f, 0.0f, // vertec 2 of triangle
		0.55f, 0.4f, 0.0f,
		-0.55f, -0.20f, 0.0f,
		0.55f, -0.20f, 0.0f,
		-0.25f, -0.55f, 0.0f,
		0.25f, -0.55f, 0.0f,
	};

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);

	//Transer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
		vertex_buffer_data, GL_STATIC_DRAW);

	//An array of 3 vectors which represents the colors of the 3 vertices
	static const GLfloat color_buffer_data[] =
	{
		1.0f, 0.0f, 0.0f, // color of vertex 0
		1.0f, 0.0f, 0.0f, // color of vertex 1
		1.0f, 0.0f, 0.0f, // color of vertex 2
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data),
		color_buffer_data, GL_STATIC_DRAW);


	// Load vertex and fragment shaders
	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader");

	// Get a handle for out "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	// Use our shader
	glUseProgram(m_programID);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
}

void Scene2::Update(double dt)
{
	static float direction = 1;
	static float translateDir = 1;
	static float rotateDir = 1;

	if (Application::IsKeyPressed(VK_SPACE))
		direction *= -1;
	if ((translateX < -40) || (translateX > 40))
		translateDir *= -1;
	if ((rotateAngle < -180) || (rotateAngle > 180))
		rotateDir *= -1;
	if (scaleAll > 10)
		scaleAll = 0;
	else if (scaleAll < 0)
		scaleAll = 10;

	rotateAngle += (float)(45 * rotateDir * dt * direction);
	translateX += (float)(40 * translateDir * dt * direction);
	scaleAll += (float)(2 * dt * direction);

}

void Scene2::Render()
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
	view.SetToIdentity();
	projection.SetToOrtho(-40, 40, -30, 30, -100, 100); // Size of playground (Where the stuff happens)

	scale.SetToScale(10, 10, 10);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(0, 0, 0);
	model = translate * rotate * scale; // Scale, followed by rotate, then translate
	MVP = projection * view * model; // Matrix multiplication is opposite
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); // Update shader with new MVP

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(
		0,		// attribute 0. Must match the layout in the shader. Usually 0 is for vertex
		3,		// size
		GL_FLOAT,		// type
		GL_FALSE,		//normalized?
		0,		// stride
		0		// array buffer offset
		);

	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8); // Starting from vertex 0; 3 vertices = 1 triangle

	//----------------------------
	//----------------------------
	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();

	scale.SetToScale(1, 1, 1);
	rotate.SetToRotation(60, 0, 0, 1);
	translate.SetToTranslation(translateX, 20, -50);
	model = translate * rotate * scale; // Scale, followed by rotate, then translate
	MVP = projection * view * model; // Matrix multiplication is opposite
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); // Update shader with new MVP

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(
		0,		// attribute 0. Must match the layout in the shader. Usually 0 is for vertex
		3,		// size
		GL_FLOAT,		// type
		GL_FALSE,		//normalized?
		0,		// stride
		0		// array buffer offset
		);

	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8); // Starting from vertex 0; 3 vertices = 1 triangle

//--------------
//--------------
	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();

	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(-30, -20, 10);
	model = translate * rotate * scale; // Scale, followed by rotate, then translate
	MVP = projection * view * model; // Matrix multiplication is opposite
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); // Update shader with new MVP

	glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(
		0,		// attribute 0. Must match the layout in the shader. Usually 0 is for vertex
		3,		// size
		GL_FLOAT,		// type
		GL_FALSE,		//normalized?
		0,		// stride
		0		// array buffer offset
		);

	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8); // Starting from vertex 0; 3 vertices = 1 triangle


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Scene2::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
