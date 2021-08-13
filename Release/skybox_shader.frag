#version 140

in vec3 texCoord_v;

out vec4 color_final;

uniform samplerCube skybox;

void main()
{
	color_final = texture(skybox, -texCoord_v);
}
