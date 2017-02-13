#version 330 core

layout(location = 0) in vec4 vertex;

out vec3 vertexNormal;
flat out float vertexTextureIndex;

layout(std140) uniform Camera
{
   mat4 proj;
   mat4 view;
};

layout(std140) uniform Normals
{
   vec4 normals[6];
};

layout(std140) uniform ChunkModelMatrix
{
	mat4 model;
};

void main()
{
   gl_Position = proj * view * model * vec4(vertex.xyz, 1.0);

   int data = floatBitsToInt(vertex.w);
   int normalIndex = data & 0xFFFF;
   int textureIndex = (data >> 16) & 0xFFFF;

   vertexNormal = mat3(transpose(inverse(model))) * normals[normalIndex].xyz;
   vertexTextureIndex = float(textureIndex);
}