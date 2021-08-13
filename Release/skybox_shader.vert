#version 140

in vec3 position;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix; 

out vec3 texCoord_v;

void main()
{
	texCoord_v = position;
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0f);
}  
