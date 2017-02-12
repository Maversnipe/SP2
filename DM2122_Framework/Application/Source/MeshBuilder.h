#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateCircle(const std::string &meshName, Color color, unsigned numSlices = 100.f, float radius = 1.f);
	static Mesh* GenerateRing(const std::string &meshName, Color color, unsigned numSlices, float outerRadius, float innerRadius, float height);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices, float radius = 1.f);
	static Mesh* GenerateHemisphere(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices, float radius = 1.f);
	static Mesh* GenerateCylinder(const std::string &meshName, Color color, float height, unsigned numStacks, unsigned numSlices, float radius = 1.f);
	static Mesh* GenerateCone(const std::string &meshName, Color color, float height, unsigned numStacks, unsigned numSlices, float radius = 1.f);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
	static Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);
};

#endif