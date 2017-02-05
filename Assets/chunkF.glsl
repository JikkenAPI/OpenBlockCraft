#version 330 core

in vec3 vertexNormal;
flat in float vertexTextureIndex;

out vec4 outFragColor;

layout(std140) uniform Sun
{
	vec4 sunDirection;
	vec4 sunAmbient;
	vec4 sunDiffuse;
};

const int GRASS = 1;
const int DIRT = 2;
const int COBBLESTONE = 3;
const int WATER = 4;

void main()
{
	vec4 ambient = sunAmbient;

	// Calculation for sunlight
	vec3 lightDir = normalize(-sunDirection.xyz);
	float diff = max(dot(normalize(vertexNormal), lightDir), 0.0);
	vec4 diffuse = sunDiffuse * diff;
	diffuse.w = 1.0;

	int texId = int(vertexTextureIndex);

	// TODO: check for grass
	vec4 colorMult;
	if (texId == DIRT)
		colorMult = vec4(0.4, 0.2, 0.0, 1.0);
	else if (texId == COBBLESTONE)
		colorMult = vec4(0.5, 0.5, 0.5, 1.0);
	else if (texId == WATER)
		colorMult = vec4(0.0, 0.0, 0.3, 0.5);
	else
		colorMult = vec4(1.0, 0.0, 0.0, 0.1);

	outFragColor = ambient + (diffuse * colorMult);
}