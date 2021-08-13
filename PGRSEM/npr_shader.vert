#version 140

uniform vec3 lightPosition = vec3(2.5, 5, 5);
uniform mat4 pvmMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

in vec3 position;
in vec3 normal;
in vec2 aTexCoords;

out vec3 N;
out vec3 P;
out vec3 V;
out vec3 L;

smooth out vec2 texCoord_v;  // outgoing texture coordinates

void main()
{
	P = position;
	N = normalize((viewMatrix * normalMatrix * vec4(normal, 0.0f)).xyz);
//	V = normalize(-P);
	V = -vec3(viewMatrix * modelMatrix * vec4(position, 1.0));
//	L = normalize(lightPosition - P);
	L = normalize(vec3(viewMatrix * modelMatrix * vec4(lightPosition - position, 0.0f)));

	texCoord_v = aTexCoords;
	gl_Position = pvmMatrix * vec4(position, 1.0f);
}