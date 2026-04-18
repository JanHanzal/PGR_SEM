#version 140


in vec3 position;           
in vec3 normal;             
in vec2 texCoord;           



uniform mat4 PVMmatrix;     
uniform mat4 Vmatrix;       
uniform mat4 Mmatrix;       
uniform mat4 normalMatrix;  
uniform mat4 textureMatrix;


smooth out vec3 vertexPosition_v; 
smooth out vec3 vertexNormal_v;
smooth out vec2 texCoord_v;

void main() {
  vertexPosition_v = (Vmatrix * Mmatrix * vec4(position, 1.0)).xyz;
  
  vertexNormal_v = normalize((Vmatrix * normalMatrix * vec4(normal, 0.0)).xyz);
    
  texCoord_v = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;;


  gl_Position = PVMmatrix * vec4(position, 1.0);
}