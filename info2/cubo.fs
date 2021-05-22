
#version 330 core
out vec4 FragPos;

in vec3 FragPos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	//luz ambiental  la mas pesada ya que calcula en global
	float ambientStraght = 0.5;
	vec3 ambien = ambientStraght * lightColor;

	//Luz difusa
	vec3 norm = normalize(normal); //porcentaje de Luz
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max (dot(norm, lightDir)), 0.0);
	vec3 diffuse = diff * lightColor;

	//Luz espectacular
	float specularStreght = 0.5;
	vec3 viewDir = normalize(view - FragPos);
	vec3 reflactDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir - reflactDir), 0.0), 32);
	vec3 specular = specularStreght * spec * lightColor

	//Convolicionar todo
	vec3 result = (ambien + diffuse + specular + objectColor) * objectColor; //Generamos una luz combinada
	fragColor = vec4(result, 1.0);
}