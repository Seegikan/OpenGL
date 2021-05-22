
#version 330 core
out vec4 FragColor;

//in vec3 nuestroColor;
in vec2 nuestraTexturas;

//buffer para las texturas
uniform sampler2D textura1;
uniform sampler2D textura2;

void main()
{
	//FragColor =(mix( texture(textura1, nuestraTexturas), texture(textura2, nuestraTexturas), 0.4) * vec4(nuestroColor, 1.0)); //para a;adir el color
	FragColor = mix(texture(textura1, nuestraTexturas), texture(textura2, nuestraTexturas), 0.4);
}

