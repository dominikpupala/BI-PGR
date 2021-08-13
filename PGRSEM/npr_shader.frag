#version 140

uniform vec3 color;
uniform vec3 warmColor = vec3(0.6, 0.6, 0.0);
uniform vec3 coolColor = vec3(0.2, 0.2, 0.3);
uniform bool useTexture = false;
uniform float coolDiffuse = 0.45;
uniform float warmDiffuse = 0.45;
uniform sampler2D texSampler;  // sampler for the texture access

in vec3 N;
in vec3 P;
in vec3 V;
in vec3 L;

smooth in vec2 texCoord_v;     // fragment texture coordinates

out vec4 color_final;

void main()
{
	vec3 l = normalize(L);
    vec3 n = normalize(N);
    vec3 v = normalize(V);
    vec3 h = normalize(l + v);

//    float diffuse = dot(l, n);
    float specular = pow(max((dot(n, h) + 1) * 0.5f, 0.0f), 32.0f);
//
//    vec3 cool = min(coolColor + color, 1.0f);
//    vec3 warm = min(warmColor + color, 1.0f);

    vec3 cool = min(coolColor + coolDiffuse * color, 1.0);
    vec3 warm = min(warmColor + warmDiffuse * color, 1.0);

//    vec3 temp = min(mix(cool, warm, diffuse) + specular, 1.0f);

    vec3 temp = mix(cool, warm, dot(l, n));

//	color_final = ;
//    color_final = vec4(color, 1.0f);

	if (useTexture)
		color_final = vec4(min(temp + specular, 1.0f), 1.0f) * texture(texSampler, texCoord_v);
	else
		color_final = vec4(min(temp + specular, 1.0f), 1.0f);
}
