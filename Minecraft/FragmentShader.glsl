#version 450

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D specularTex;
	sampler2D diffuseTex;
};

struct PointLight
{
	vec3 position;
	float intensity;
	vec3 color;
	float constant;
	float linear;
	float cuadratic;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texCoord;
in vec3 vs_normal;
flat in float vs_clr;

out vec4 fs_color;

uniform Material material;
uniform PointLight pointLight;
uniform vec3 lightPos;
uniform vec3 cameraPos;

vec4 calculateAmbient()
{
	return vec4(material.ambient, 1.0);
}

vec4 calculateDiffuse(vec3 vs_normal)
{
	vec3 posToLightDirVec = normalize(pointLight.position - vs_position);
	float diffuse = clamp(dot(posToLightDirVec, normalize(vs_normal)) * pointLight.intensity, 0, 1);
	vec3 diffuseFinal = material.diffuse * diffuse;

	return vec4(diffuseFinal, 1.0);
}

vec4 calculateSpecular(vec3 vs_normal)
{
	vec3 lightToPosDirVec = normalize(vs_position - pointLight.position);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 30.0);
	vec3 specularFinal = material.specular * specularConstant * texture(material.specularTex, vs_texCoord).rgb;

	return vec4(specularFinal, 1.0);
}

vec4 calculateAttenuation()
{
	float distance = length(pointLight.position - vs_position);
	float attenuation = pointLight.constant / (1.0 + pointLight.linear * distance + pointLight.cuadratic * (distance * distance));
	return vec4(attenuation, attenuation, attenuation, 1.0);
}

void main()
{
	float distance = length(vs_position - cameraPos);
	float val = 1.0 * vs_clr;
	vec4 green = vec4(max(val, 0.43), max(val, 1), max(val, 0.43), 1);
	fs_color = //(1 - clamp(distance / 40, 0, 1)) * 
		texture(material.diffuseTex, vs_texCoord) * green
	*
	//(
		(
			calculateAmbient() + 
			calculateDiffuse(vs_normal) 
			//+ calculateSpecular(vs_normal)
		) *
		 calculateAttenuation()
	//) + vec4(vec3(0.4, 0.4, 0.4) * (clamp(distance / 20, 0, 1)), 1.0)
	;
}