#version 330 core

in vec3 vertexNormal;
in float vertexHeight;

out vec4 outFragColor;

void main()
{
	if (vertexHeight <= 5.0 )
		outFragColor = vec4(0.0, 1.0, 0.0, 1.0);
	else if (vertexHeight < 11.0)
		outFragColor = vec4(1.0, 0.0, 0.0, 1.0);
	else
		outFragColor = vec4(0.0, 0.0, 1.0, 1.0);
}