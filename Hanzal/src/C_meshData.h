#ifndef __C_MESH_DATA_H__
#define __C_MESH_DATA_H__


const int explosionNumQuadVertices = 4;
const float explosionVertexData[explosionNumQuadVertices * 5] = 
{

  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
   1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
  -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
   1.0f,  1.0f, 0.0f, 1.0f, 1.0f,

};

#endif