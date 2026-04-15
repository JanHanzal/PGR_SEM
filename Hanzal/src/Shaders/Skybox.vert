#version 140

uniform mat4 inversePVMatrix;

in vec2 screenCoords;


out vec3 texCoord_v;

void main(){
	vec4 farPlaneCoord = vec4(screenCoords, 0.999999, 1.0);
	vec4 worldViewCoord = inversePVMatrix * farPlaneCoord;

	texCoord_v = worldViewCoord.xyz / worldViewCoord.w;

	gl_Position = farPlaneCoord;
}