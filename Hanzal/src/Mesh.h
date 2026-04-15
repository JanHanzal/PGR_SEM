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
	Material material;

} MeshGeometry;

bool loadSingleMesh(const std::string& fileName, ObjectShader& shader, MeshGeometry** geometry);

void cleanupGeometry(MeshGeometry* geometry);

#endif