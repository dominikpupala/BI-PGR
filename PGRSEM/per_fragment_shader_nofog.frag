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

in vec2 texCoord_v;
in vec3 vertexPosition_v;
in vec3 vertexNormal_v;

uniform bool texUse = false;
uniform sampler2D texSampler; 

uniform Light sunlight;
uniform Light spotlight;
uniform Light pointlight;
uniform Material material;

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

void main()
{
	vec3 globalAmbientLight = vec3(0.13f);
	vec4 color = vec4(material.Ambient * globalAmbientLight, 0.0f);

	color += lightDirectional(material, sunlight, vertexPosition_v, vertexNormal_v);
	color += lightPoint(material, pointlight, vertexPosition_v, vertexNormal_v);
	color += lightSpot(material, spotlight, vertexPosition_v, vertexNormal_v);

	color_final = color * (texUse ? texture(texSampler, texCoord_v) : vec4(1.0f));
}
