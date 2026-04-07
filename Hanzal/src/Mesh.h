#include "pgr.h"
#include "Shader.h"

#ifndef __MESH_H
#define __MESH_H


typedef struct _MeshGeometry {
	GLuint        vertexBufferObject;   // identifier for the vertex buffer object
	GLuint        elementBufferObject;  // identifier for the element buffer object
	GLuint        vertexArrayObject;    // identifier for the vertex array object
	unsigned int  numTriangles;         // number of triangles in the mesh
	// material
	glm::vec3     ambient;
	glm::vec3     diffuse;
	glm::vec3     specular;
	float         shininess;
	GLuint        texture;

} MeshGeometry;

bool loadSingleMesh(const std::string& fileName, SCommonShaderProgram& shader, MeshGeometry** geometry);

void cleanupGeometry(MeshGeometry* geometry);

#endif