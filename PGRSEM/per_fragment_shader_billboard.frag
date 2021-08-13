#version 140

struct Light
{
	vec3 Diffuse;
	vec3 Ambient;
	vec3 Specular;
	vec3 Position;
	vec3 Direction;
	
	float CutoffInn;
	float CutoffOut;

	float Constant;
	float Linear;
	float Quadratic;
};

struct Material
{
	vec3 Diffuse;
	vec3 Ambient;
	vec3 Specular;
	float Shininess;
};

struct Fog
{
	vec3 Color;
	float Density;
	float Gradient;
};

in vec2 texCoord_v;
in vec3 vertexPosition_v;
in vec3 vertexNormal_v;
in vec4 fogPosition_v;

uniform bool texUse = false;
uniform sampler2D texSampler; 

uniform Fog fog;
uniform Light sunlight;
uniform Light spotlight;
uniform Light pointlight;
uniform Material material;

uniform mat4 viewMatrix;

uniform float time;

uniform vec3 cameraPosition;

out vec4 color_final;

vec3 lightAmbient(Material material, Light light)
{
	return material.Ambient * light.Ambient;
}

vec3 lightDiffuse(Material material, Light light, float NdotL)
{
	return material.Diffuse * light.Diffuse * NdotL;
}

vec3 lightSpecular(Material material, Light light, float RdotV)
{
	return material.Specular * light.Specular * (material.Shininess > 0.0f ? pow(RdotV, material.Shininess) : 0f);
}

vec4 lightDirectional(Material material, Light light, vec3 vPosition, vec3 vNormal)
{
	vec3 ambient = lightAmbient(material, light);

	vec3 N = normalize(vNormal);
	vec3 L = normalize(light.Position - vPosition);
	vec3 diffuse = lightDiffuse(material, light, max(0.0f, dot(N, L)));

	vec3 R = reflect(-L, N);
	vec3 V = normalize(cameraPosition - vPosition);
	vec3 specular = lightSpecular(material, light, max(0.0f, dot(R, V)));

	return vec4(ambient + diffuse + specular, 1.0f);
}

vec4 lightPoint(Material material, Light light, vec3 vPosition, vec3 vNormal)
{
	vec3 ambient = lightAmbient(material, light);
	
	vec3 N = normalize(vNormal);
	vec3 L = normalize(light.Position - vPosition);
	vec3 diffuse = lightDiffuse(material, light, max(0.0f, dot(N, L)));
	
	vec3 R = reflect(-L, N);
	vec3 V = normalize(cameraPosition - vPosition);
	vec3 specular = lightSpecular(material, light, max(0.0f, dot(R, V)));

	float dist = length(light.Position - vPosition);
	float atten = 1.0f / (light.Constant + light.Linear * dist + light.Quadratic * pow(dist, 2.0f));

	return vec4(ambient * atten + diffuse * atten + specular * atten, 1.0f);
}

vec4 lightSpot(Material material, Light light, vec3 vPosition, vec3 vNormal)
{
	vec3 ambient = lightAmbient(material, light);
	
	vec3 N = normalize(vNormal);
	vec3 L = normalize(light.Position - vPosition);
	vec3 diffuse = lightDiffuse(material, light, max(0.0f, dot(N, L)));

	vec3 R = reflect(-L, N);
	vec3 V = normalize(-vPosition);
	vec3 specular = lightSpecular(material, light, max(0.0f, dot(R, V)));

	float theta = dot(L, normalize(light.Position - light.Direction));
	float epsilon = light.CutoffInn - light.CutoffOut;
	float intensity = clamp((theta - light.CutoffOut) / epsilon, 0.0f, 1.0f);

	return vec4(ambient * intensity + diffuse * intensity + specular * intensity, 1.0f);
}

float fogExp(Fog fog, vec4 fogPosition)
{
	float dist = length(fogPosition.xyz);
	float visibility = exp(-pow((dist * fog.Density), fog.Gradient));

	return clamp(visibility, 0.0f, 1.0f);
}

vec4 animateFunction(int frame)
{
	int temp = frame % 61;
	vec2 texCoordBase = texCoord_v / vec2(8, 8);
	vec2 texCoord = texCoordBase + vec2(temp % 8, 8 - 1 - (temp / 8)) * (vec2(1.0) / vec2(8, 8));

	return texture(texSampler, texCoord);
}

void main()
{
	vec3 globalAmbientLight = vec3(0.13f);
	vec4 color = vec4(material.Ambient * globalAmbientLight, 0.0f);

	color += lightDirectional(material, sunlight, vertexPosition_v, vertexNormal_v);
	color += lightPoint(material, pointlight, vertexPosition_v, vertexNormal_v);
	color += lightSpot(material, spotlight, vertexPosition_v, vertexNormal_v);

	color *= texUse ? animateFunction(int(time / 0.01f)) : vec4(1.0f);
	color_final = mix(vec4(fog.Color, 1.0f), color, fogExp(fog, fogPosition_v));
}
