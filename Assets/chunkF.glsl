#version 330 core

in vec3 vertexNormal;

out vec4 outFragColor;

layout(std140) uniform Sun
{
	vec4 sunDirection;
	vec4 sunAmbient;
	vec4 sunDiffuse;
};

void main()
{
	vec4 ambient = sunAmbient;

	// Calculation for sunlight
	vec3 lightDir = normalize(-sunDirection.xyz);
	float diff = max(dot(normalize(vertexNormal), lightDir), 0.0);
	vec4 diffuse = sunDiffuse * diff;

	outFragColor = vec4((vec3(1) + vertexNormal) / 2,1);//ambient + diffuse;
}