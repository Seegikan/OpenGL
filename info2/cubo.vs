
//
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; //efectos de luces

out vec3 FragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	fragPos = vec3(model * vec4(aPos, 1.0)) 
	normal = mat3(transpose(inverse(model))) * aNormal; //capacidad de poder recibir luz, al objeto

	gl_Position = projection * view * vec4(FragPos, 1.0);
}