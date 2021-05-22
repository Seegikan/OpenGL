
	//vs de vertex shader
#version 330 core ///atributos 
layout (location = 0) in vec3 aPos;  //Vector3
//layout (location = 1) in vec3 aColor; //RGB
layout (location = 1) in vec2 aTexCoord; //texturas //Vector2

//out vec3 nuestroColor;
out vec2 nuestraTexturas;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
	//nuestroColor = aColor;
	nuestraTexturas = vec2(aTexCoord.x, aTexCoord.y);
}
