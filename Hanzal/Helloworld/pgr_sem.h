#ifndef __PGR_SEM_H
#define __PGR_SEM_H

#include "pgr.h"

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct Camera
{
    glm::vec3 eye;
    glm::vec3 center;
    glm::vec3 up;
};

typedef struct _MeshGeometry {
  GLuint        vertexBufferObject;   // identifier for the vertex buffer object
  GLuint        elementBufferObject;  // identifier for the element buffer object
  GLuint        vertexArrayObject;    // identifier for the vertex array object
  unsigned int  numTriangles;         // number of triangles in the mesh

} MeshGeometry;

#endif