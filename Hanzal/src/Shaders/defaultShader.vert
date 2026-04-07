#version 140


in vec3 position;           // vertex position in world space
in vec3 normal;             // vertex normal
in vec2 texCoord;           // incoming texture coordinates

uniform mat4 PVMmatrix;     // Projection * View * Model
uniform mat4 Vmatrix;       // View
uniform mat4 Mmatrix;       // Model
uniform mat4 normalMatrix;  // inverse transposed Mmatrix

// Promìnné posílané do fragment shaderu
smooth out vec3 vertexPosition_v; 
smooth out vec3 vertexNormal_v;
smooth out vec2 texCoord_v;

void main() {
  // Pozice vertexu v souøadnicích kamery (eye coordinates)
  vertexPosition_v = (Vmatrix * Mmatrix * vec4(position, 1.0)).xyz;
  
  // Normála v souøadnicích kamery (musí být normalizována po transformaci)
  vertexNormal_v = normalize((Vmatrix * normalMatrix * vec4(normal, 0.0)).xyz);
  
  // Pøedání texturovacích souøadnic
  texCoord_v = texCoord;

  // Finální transformace na obrazovku
  gl_Position = PVMmatrix * vec4(position, 1.0);
}