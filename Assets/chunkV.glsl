#version 330 core

layout(location = 0) in vec4 vertex;

out vec3 vertexNormal;
out float vertexHeight;

layout(std140) uniform Camera
{
   mat4 proj;
   mat4 view;
};

layout(std140) uniform Normals
{
   vec4 normals[6];
};

uniform mat4 model;

void main()
{
   gl_Position = proj * view * model * vec4(vertex.xyz, 1.0);
   vertexNormal = normals[floatBitsToInt(vertex.w)].xyz;
   vertexHeight = (model * vec4(vertex.xyz, 1.0)).y;
}