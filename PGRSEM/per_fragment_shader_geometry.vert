#version 140

uniform mat4 pvmMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform float alpha;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 texCoord;

out vec2 texCoord_v;
out vec3 vertexNormal_v;
out vec3 vertexPosition_v;
out vec4 fogPosition_v;


void main()
{
	vertexNormal_v = normalize((normalMatrix * vec4(vertexNormal, 0.0f)).xyz);
	vertexPosition_v = vec3(modelMatrix * vec4(vertexPosition, 1.0f));
	fogPosition_v = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f);
	texCoord_v = texCoord;

	vec3 newPosition = mix(vertexPosition, vec3(vertexPosition.x, alpha + vertexPosition.y * (1 + alpha), vertexPosition.z), alpha);
	gl_Position = pvmMatrix * vec4(newPosition, 1.0f);
}