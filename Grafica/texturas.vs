
	//vs de vertex shader
#version 330 core ///atributos 
layout (location = 0) in vec3 aPos;  //Vector3
layout (location = 1) in vec3 aColor; //RGB
layout (location = 2) in vec3 aTexCoord; //texturas //Vector2

out vec3 nustroColor;
out vec2 nuestraTexturas;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	nustroColor = aColor;
	nuestraTexturas = vec2(aTexCoord.x, aTexCoord.y);
}
