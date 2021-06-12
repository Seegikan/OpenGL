
/// parte de luces<<<<<<<<<<<<-------------------------------------------------------------------------------
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp> //operaciones avanzadas
#include <glm/gtc/matrix_transform.hpp> //transformaciones matriciales
#include <glm/gtc/type_ptr.hpp> //transformaciones de tipos de datos

#include <shader/shader.h>
#include <camara/camara.h>

#include <iostream>
#include <vector>//sky box------------------------------------------
using namespace std;
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //almacenar nuestra ventana y configs
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window); //acciones para nuestra ventana
unsigned int loadCubeMap(vector<string> faces);//sky box------------------------------------------
unsigned int LoadTexture(const char* path);

//medidas de la pantalla
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

//camara
Camara camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//el punto donde va estar posicionada la luz
glm::vec3 posLuz(1.2f, 1.0f, 2.0f);



int main() {
	//inicializar glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//creamos nuestra ventana
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Alexander", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fallo en gcrear GLFW y la ventana date un balazo" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//decirle a glfw que va a recibir señales de mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//comprobar que glad se este corriendo o se haya inicializado correctamente
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "no se esta ejecutando el alegre" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST); //comprobacion del buffer z

	Shader nuestroShader("cubo.vs", "cubo.fs");
	Shader luzShader("luz.vs", "luz.fs");
	Shader skyboxShader("SkyBox.vs", "SkyBox.fs");//sky box------------------------------------------

	float verticesSkybox[] = //sky box------------------------------------------
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	float vertices[]
	{
		//posiciones			//colores			//texturas
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int indices[]
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		14, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//sky box------------------------------------------
	unsigned int sVBO, sVAO; //Vertex Buffer Object, Vertex Array Object y Extendet Array Object
	glGenVertexArrays(1, &sVAO);

	glGenBuffers(1, &sVBO);
	glBindVertexArray(sVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSkybox), &verticesSkybox, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	vector<string> faces
	{
		("skybox/right.jpg"),
		("skybox/left.jpg"),
		("skybox/top.jpg"),
		("skybox/bottom.jpg"),
		("skybox/front.jpg"),
		("skybox/back.jpg")
	};

	unsigned int cubeMapTexture = loadCubeMap(faces);

	//Configurar el shader del skybox 
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0); //asignar buffer



	unsigned int VBO, VAO, EBO; //Vertex Buffer Object, Vertex Array Object y Extendet Array Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//unir o linkear
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Luces
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Segundo shader
	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	/*while (!glfwWindowShouldClose(window))
	{
		//calculo para el mouse para que pueda captar los movimeintos en tiempo real
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		//Renderizado
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpieza del buffer z

		
		//propiedades de la luz
		nuestroShader.use();
		nuestroShader.setVec3("light.position", 0.5f, 0.5f, 0.5f); //color
		nuestroShader.setVec3("viewPos", camera.Position);
		//nuestroShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		//nuestroShader.setVec3("lightPos", posLuz);

		//Propiedades de la luz (Advanced)
		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffusseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffusseColor * glm::vec3(0.2f);
		nuestroShader.setVec3("light.ambien", ambientColor);
		nuestroShader.setVec3("light.diffuse", diffusseColor);
		nuestroShader.setVec3("light.specular", glm::vec3(1.0f));
		

		//Propiedades de los materiales
		/*nuestroShader.setVec3("material.ambien", 0.0215f, 0.1745f, 0.0215f);
		nuestroShader.setVec3("material.diffuse", 0.07568f, 0.61424f, 0.07568f);
		nuestroShader.setVec3("material.specular", 0.0633f, 0.727811f, 0.633f);
		nuestroShader.setFloat("material.shininess", 32.0f);*/

	/*
		//mvp
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		nuestroShader.setMat4("projection", projection);
		nuestroShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);


		model = glm::translate(model, glm::vec3(10.0, 0.0f, 0.0f));

		nuestroShader.setMat4("model", model);

		//Renderizado de cubo
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

		//Configuramos nuestro punto de luz
		luzShader.use();
		luzShader.setMat4("projection", projection);
		luzShader.setMat4("view", view);

		model = glm::mat4(1.0f);					//x                  y	    z	 
		model = glm::translate(model, glm::vec3((float)glfwGetTime() / 2, 1.1f, 0.0f));

		posLuz = glm::vec3((float)glfwGetTime() / 2, 1.1f, 0.0f);

		model = glm::scale(model, glm::vec3(0.2f));
		luzShader.setMat4("model", model);

		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

		#pragma region ESMERALDA
		//Propiedades de los materiales
		/*nuestroShader.setVec3("material.ambient", 0.0215f, 0.1745f, 0.0215f);
		nuestroShader.setVec3("material.diffuse", 0.07568f, 0.61424f, 0.07568f);
		nuestroShader.setVec3("material.specular", 0.633f, 0.727811f, 0.633f);*/

	/*
		nuestroShader.setFloat("material.shininess", 128.0f * 0.6f);

		model = glm::translate(model, glm::vec3(1.2f, 0.0f, 0.0f));
		nuestroShader.setMat4("model", model);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angulos = 20.0f * i;
			model = glm::rotate(model, glm::radians(angulos), glm::vec3(1.0f, 0.3f, 0.5f));
			//model = scale(model, vec3(40, 40, 40));

			nuestroShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0); //Aqui se delimita los tringulos TRUCHA-------------------------
		}

		// JADE
		nuestroShader.setVec3("material.ambient", 0.135f, 0.2225f, 0.1575f);
		nuestroShader.setVec3("material.diffuse", 0.54f, 0.89f, 0.63f);
		nuestroShader.setVec3("material.specular", 0.316228f, 0.316228f, 0.316228f);
		nuestroShader.setFloat("material.shininess", 128.0f * 0.1f);

		model = glm::translate(model, glm::vec3(1.2f, 0.0f, 0.0f));
		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

		// OBSIDIANA
		model = glm::translate(model, glm::vec3(1.2f, 0.0f, 0.0f));
		nuestroShader.setVec3("material.ambient", 0.05375f, 0.05f, 0.06625f);
		nuestroShader.setVec3("material.diffuse", 0.18275f, 0.17f, 0.22525f);
		nuestroShader.setVec3("material.specular", 0.332741f, 0.328634f, 0.346435f);

		nuestroShader.setFloat("material.shininess", 128.0f * 0.3f);
		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

		// PERLA
		nuestroShader.setVec3("material.ambient", 0.25f, 0.20725f, 0.20725f);
		nuestroShader.setVec3("material.diffuse", 1.0f, 0.829f, 0.829f);
		nuestroShader.setVec3("material.specular", 0.296648f, 0.296648f, 0.296648f);
		nuestroShader.setFloat("material.shininess", 128.0f * 0.088f);

		model = glm::translate(model, glm::vec3(1.2f, 0.0f, 0.0f));
		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

		//	 RUBY
		nuestroShader.setVec3("material.ambient", 0.1745f, 0.01175f, 0.01175f);
		nuestroShader.setVec3("material.diffuse", 0.61424f, 0.04136f, 0.04136f);
		nuestroShader.setVec3("material.specular", 0.727811f, 0.626959f, 0.626959f);
		nuestroShader.setFloat("material.shininess", 128.0f * 0.6f);

		model = glm::translate(model, glm::vec3(1.2f, 0.0f, 0.0f));
		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

		// TURQUESA
		nuestroShader.setVec3("material.ambient", 0.1f, 0.18725f, 0.1745f);
		nuestroShader.setVec3("material.diffuse", 0.396f, 0.74151f, 0.69102f);
		nuestroShader.setVec3("material.specular", 0.297254f, 0.30829f, 0.306678f);
		nuestroShader.setFloat("material.shininess", 128.0f * 0.1f);

		model = glm::translate(model, glm::vec3(1.2f, 0.0f, 0.0f));
		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

		//Configurar punto de luz
		luzShader.use();
		luzShader.setMat4("projection", projection);
		luzShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = translate(model, posLuz);
		model = glm::scale(model, glm::vec3(0.2f));
		//model = translate(model, vec3(posLuz.x, posLuz.y, -glfwGetTime()));
		model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		luzShader.setMat4("model", model);

		//sky box------------------------------------------
		glDepthFunc(GL_LEQUAL); //da profundidad 
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);

		glBindVertexArray(VAO2);//nuevo

		glBindVertexArray(sVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		//detecte eventos de IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	/*
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteVertexArrays(1, &sVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &sVBO);
	glfwTerminate();
	return 0;
}
*/


unsigned int diffuseMap = LoadTexture("Box.png");
unsigned int specularMap = LoadTexture("Box_Specular.png");

while (!glfwWindowShouldClose(window))
{
	//calculo para el mouse para que pueda captar los movimeintos en tiempo real
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	processInput(window);

	//Renderizado
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpieza del buffer z

	//Propiedades de la luz
	nuestroShader.use();
	//nuestroShader.setVec3("light.position", posLuz);
	nuestroShader.setVec3("light.position", camera.Position);
	nuestroShader.setVec3("light.direction", camera.Front);
	nuestroShader.setFloat("light.cutOff", cos(glm::radians(12.5f)));
	nuestroShader.setFloat("light.outerCutOff", cos(glm::radians(17.5f)));
	nuestroShader.setVec3("viewPos", camera.Position);

	nuestroShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
	nuestroShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
	nuestroShader.setVec3("light.specular", glm::vec3(1.0f));
	nuestroShader.setFloat("light.constant", 1.0f);
	nuestroShader.setFloat("light.linear", 0.1f);
	nuestroShader.setFloat("light.quadratic", 0.0032f);

	nuestroShader.setFloat("material.shininess", 32.0f);

	//posLuz = vec3(posLuz.x, posLuz.y, 2.0f - (glfwGetTime() * 0.25f));
	/*float radio = 3.0f;
	float multipVelocidad = 1.2f;
	posLuz = vec3((radio * cos(glfwGetTime() * multipVelocidad)), posLuz.y, posLuz.z);*/

	//nuestroShader.setVec3("lightPos", posLuz);
	//nuestroShader.setVec3("viewPos", camera.Position);


	//Propiedades avanzadas de la luz
	/*vec3 lightColor(1.0f);
	lightColor.x = sin(glfwGetTime() * 2.21f);
	lightColor.y = sin(glfwGetTime() * 1.33f);
	lightColor.z = sin(glfwGetTime() * 3.18f);
	vec3 diffuseColor = lightColor * vec3(0.5f);
	vec3 ambientColor = diffuseColor * vec3(0.2f);*/


	//mvp
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	nuestroShader.setMat4("projection", projection);
	nuestroShader.setMat4("view", view);

	glBindVertexArray(VAO);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-4.2f, 0.0f, 0.0f));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	for (unsigned int i = 0; i < 10; i++)
	{
		mat4 model = mat4(1.0f);
		model = translate(model, cubePositions[i]);
		float angulos = 20.0f * i;
		model = rotate(model, radians(angulos), vec3(1.0f, 0.3f, 0.5f));
		//model = scale(model, vec3(40, 40, 40));

		nuestroShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0); //Aqui se delimita los tringulos TRUCHA-------------------------
	}

	#pragma region ESMERALDA
			
			nuestroShader.setFloat("material.shininess", 128.0f * 0.6f);
	
			model = translate(model, vec3(0.0f, 0.0f, 0.0f));
			nuestroShader.setMat4("model", model);
	
			
	#pragma endregion

	
	
	#pragma region OBSIDIANA
			model = translate(model, vec3(1.2f, 0.0f, 0.0f));
			nuestroShader.setVec3("material.ambient", 0.05375f, 0.05f, 0.06625f);
			nuestroShader.setVec3("material.diffuse", 0.18275f, 0.17f, 0.22525f);
			nuestroShader.setVec3("material.specular", 0.332741f, 0.328634f, 0.346435f);
			nuestroShader.setFloat("material.shininess", 128.0f * 0.3f);
			nuestroShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
	#pragma endregion

#pragma region JADE
			nuestroShader.setVec3("material.ambient", 0.135f, 0.2225f, 0.1575f);
			nuestroShader.setVec3("material.diffuse", 0.54f, 0.89f, 0.63f);
			nuestroShader.setVec3("material.specular", 0.316228f, 0.316228f, 0.316228f);
			nuestroShader.setFloat("material.shininess", 128.0f * 0.1f);
			model = translate(model, vec3(1.2f, 0.0f, 0.0f));//Propiedades de los materiales
			nuestroShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
#pragma endregion

	#pragma region PERLA
			nuestroShader.setVec3("material.ambient", 0.25f, 0.20725f, 0.20725f);
			nuestroShader.setVec3("material.diffuse", 1.0f, 0.829f, 0.829f);
			nuestroShader.setVec3("material.specular", 0.296648f, 0.296648f, 0.296648f);
			nuestroShader.setFloat("material.shininess", 128.0f * 0.088f);
			model = translate(model, vec3(1.2f, 0.0f, 0.0f));
			nuestroShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
	#pragma endregion

#pragma region TURQUESA
			nuestroShader.setVec3("material.ambient", 0.1f, 0.18725f, 0.1745f);
			nuestroShader.setVec3("material.diffuse", 0.396f, 0.74151f, 0.69102f);
			nuestroShader.setVec3("material.specular", 0.297254f, 0.30829f, 0.306678f);
			nuestroShader.setFloat("material.shininess", 128.0f * 0.1f);
			model = translate(model, vec3(1.2f, 0.0f, 0.0f));
			nuestroShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
#pragma endregion

	#pragma region RUBY
			nuestroShader.setVec3("material.ambient", 0.1745f, 0.01175f, 0.01175f);
			nuestroShader.setVec3("material.diffuse", 0.61424f, 0.04136f, 0.04136f);
			nuestroShader.setVec3("material.specular", 0.727811f, 0.626959f, 0.626959f);
			nuestroShader.setFloat("material.shininess", 128.0f * 0.6f);
			model = translate(model, vec3(1.2f, 0.0f, 0.0f));
			nuestroShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);
	#pragma endregion
	
	

			//Configurar punto de luz
			luzShader.use();
			luzShader.setMat4("projection", projection);
			luzShader.setMat4("view", view);
			model = mat4(1.0f);
			model = translate(model, posLuz);
			model = scale(model, vec3(0.2f));
			//model = translate(model, vec3(posLuz.x, posLuz.y, -glfwGetTime()));
			model = rotate(model, 45.0f, vec3(0.0f, 1.0f, 0.0f));
			luzShader.setMat4("model", model);

			//renderizar luz
	glBindVertexArray(VAO2);
	glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

	//detecte eventos de IO
	glfwSwapBuffers(window);
	glfwPollEvents();
}

glDeleteVertexArrays(1, &VAO);
glDeleteVertexArrays(1, &VAO2);
glDeleteBuffers(1, &VBO);
glDeleteBuffers(1, &EBO);
glfwTerminate();
return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//nuestra primera configuracion de la camara
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

//sky box------------------------------------------
unsigned int loadCubeMap(vector<string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int wight, height, nrChannels;

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &wight, &height, &nrChannels,0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, wight, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			cout << "Fallo en carga las texturas de nuestro skybox, la textura que fallo es: " << faces[i] << endl;
			stbi_image_free(data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}
	return textureID;
}
unsigned int LoadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else {
		std::cout << "Error en la textura: " << path << std::endl;
	}

	stbi_image_free(data);

	return textureID;
}
/*
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp> //operaciones avanzadas
#include <glm/gtc/matrix_transform.hpp> //transformaciones matriciales
#include <glm/gtc/type_ptr.hpp> //transformaciones de tipos de datos

#include <shader/shader.h>
#include <camara/camara.h>


#include <iostream>
using namespace glm;

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //almacenar nuestra ventana y configs
void mouse_callback(GLFWwindow* window, double xPos, double yPos); 
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window); //acciones para nuestra ventana

//medidas de la pantalla
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

//Cámara
Camara camera(vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;



int main() {
	//inicializar glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//creamos nuestra ventana
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Taquito Blandito De Barbacoa", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fallo en gcrear GLFW y la ventana date un balazo" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//DECIRLE a glfw que va recibir señales del mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//comprobar que glad se este corriendo o se haya inicializado correctamente
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "no se esta ejecutando el alegre" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST); //Esto es para evitar la sobreposición de las figuras

	Shader nuestroShader("texturas.vs", "texturas.fs");
	Shader segundoShader("vertex2.vs", "vertex2.fs");

	float vertices[]{
		   //posiciones			//colores			//texturas
		                                  //S,      t
		//CASA
		/*0.0f, 	1.0f, 	1.0f,				0.0f, 	0.0f,//0 //Primera cara
		1.0f,	0.17f,	1.0f,				0.0f, 	0.0f,
		0.93f,	0.06f, 	1.0f,				0.0f, 	0.0f,
		0.75f,	0.21f,	1.0f,				0.0f, 	0.0f,
		0.13f,	0.7f,	1.0f,				0.0f, 	0.0f,
		0.0f,	0.8f,	1.0f,				0.0f, 	0.0f,//5
		0.75f,	-0.87f,	1.0f,				1.0f, 	0.0f,
		0.13f,	0.41f,	1.0f,				0.0f, 	0.0f,
		0.0f,	0.41f,	1.0f,				0.0f, 	0.0f,
		0.29f,	-0.07f,	1.0f,				0.0f, 	0.0f,
		0.28f,	-0.87f,	1.0f,				0.0f, 	0.0f,//10
		0.0f,	-0.07f,	1.0f,				0.0f, 	0.0f,
		-1.0f,	0.17f,	1.0f,				0.0f, 	0.0f,
		-0.93f,	0.06f, 	1.0f,				0.0f, 	0.0f,
		-0.75f,	0.21f,	1.0f,				0.0f, 	0.0f,
		-0.13f,	0.7f,	1.0f,				0.0f, 	0.0f,//15
		-0.13f,	0.41f,	1.0f,				0.0f, 	0.0f,
		-0.75f,	-0.87f,	1.0f,				-1.0f, 	0.0f,
		-0.28f,	-0.87f,	1.0f,				0.0f, 	0.0f,
		-0.29f,	-0.07f,	1.0f,				0.0f, 	0.0f,//19

		//Prate trasera
		0.0f,	1.0f,	-1.0f,				1.0f, 	0.0f,//20
		-1.0f,	0.17f,	-1.0f,				1.0f, 	0.0f,
		-0.93f,	0.06f,	-1.0f,				1.0f, 	0.0f,
		-0.75f,	0.21f,	-1.0f,				1.0f, 	0.0f,
		0.0f,	0.8f,	-1.0f,				1.0f, 	0.0f,
		-0.74f,	-0.87f,	-1.0f,				1.0f, 	0.0f,//25
		0.0f,	-0.87f,	-1.0f,				1.0f, 	0.0f,
		0.74f,	-0.87f,	-1.0f,				1.0f, 	0.0f,
		0.75f,	0.21f,	-1.0f,				1.0f, 	0.0f,
		0.93f,	0.06f,	-1.0f,				1.0f, 	0.0f,
		1.0f,	0.17f,	-1.0f,				1.0f, 	0.0f //30
		*/

		//Castillo
/*
		//Abajo
		0.55f,	-1.0f,	0.76f,				1.0f, 	0.0f,	//0
		0.70f,	-1.0f,	0.70f,				0.0f,	1.0f, 	//1
		1.0f,	-1.0f,	0.0f,				1.0f, 	0.0f,	//2
		0.70f,	-1.0f,	-0.70f,				0.0f,	1.0f, 	//3
		0.0f,	-1.0f,	-1.0f,				1.0f, 	0.0f,	//4
		-0.70f,	-1.0f,	-0.70f,				0.0f,	1.0f, 	//5
		-1.0f,	-1.0f,	0.0f,				1.0f, 	0.0f,	//6
		-0.70f,	-1.0f,	0.70f,				0.0f,	1.0f, 	//7
		0.0f,	-1.0f,	1.0f,				1.0f, 	0.0f,	//8
		0.14f,	-1.0f,	0.93f,				0.0f,	1.0f, 	//9

		//Arriba
		0.0f,	1.0f,	-1.0f,				1.0f, 	0.0f,	//10
		0.70f,	1.0f,	-0.70f,				0.0f,	1.0f, 	//11
		1.0f,	1.0f,	0.0f,				1.0f, 	0.0f,	//12
		0.70f,	1.0f,	0.70f,				0.0f,	1.0f, 	//13
		0.0f,	1.0f,	1.0f,				1.0f, 	0.0f,	//14
		-0.70f,	1.0f,	0.70f,				0.0f,	1.0f, 	//15
		-1.0f,	1.0f,	0.0f,				1.0f, 	0.0f,	//16
		-0.70f,	1.0f,	-0.70f,				0.0f,	1.0f, 	//17

		//Puerta
		0.55f,	-0.2f,	0.76f,				1.0f, 	0.0f,	//18
		0.14f,	-0.2f,	0.93f,				0.0f,	1.0f, 	//19

		//Barras
		0.0f,	1.4f,	-1.0f,				1.0f, 	0.0f,	//20
		0.70f,	1.4f,	-0.70f,				0.0f,	1.0f, 	//21
		1.0f,	1.4f,	0.0f,				1.0f, 	0.0f,	//22
		0.70f,	1.4f,	0.70f,				0.0f,	1.0f, 	//23
		0.0f,	1.4f,	1.0f,				1.0f, 	0.0f,	//24
		-0.70f,	1.4f,	0.70f,				0.0f,	1.0f, 	//25
		-1.0f,	1.4f,	0.0f,				1.0f, 	0.0f,	//26
		-0.70f,	1.4f,	-0.70f,				0.0f,	1.0f, 	//27




		  //Depas
		/*
		 1.0f,  1.0f,  1.0f,			 1.0f, 0.0f,
		 -1.0f, 1.0f,  1.0f,			 1.0f, 0.0f,
		 -1.0f, 1.0f, -1.0f,		     1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f,			 1.0f, 0.0f, //3
		 
		  1.0f, -1.0f,  1.0f,			 1.0f, 1.0f,
		 -1.0f, -1.0f,  1.0f,			 0.0f, 1.0f,//5
		 -1.0f, -1.0f, -1.0f,			 0.0f, 0.0f,
		  1.0f, -1.0f, -1.0f,			 1.0f, 0.0f,
		 
		  //Primera muro
		-0.33f,   1.0f,  1.0f,			 1.0f, 0.0f,//8
		 -0.33,   1.0f,  0.66f,			 1.0f, 0.0f,
		 -0.33,  -1.0f,  0.66f,			 1.0f, 0.0f,
		-0.33f,  -1.0f,  1.0f,			 1.0f, 0.0f,//11

		//Segundo muro
		 0.33f,   1.0f,  0.33f,			 1.0f, 0.0f,//12
		 1.0f,   1.0f,  0.33f,			 1.0f, 0.0f,
		 1.0f,  -1.0f,  0.33f,			 1.0f, 0.0f,
		 0.33f,  -1.0f,  0.33f,			 1.0f, 0.0f,//15

		 //Tercer muro
		-1.0f,    1.0f,  0.33f,		     1.0f, 0.0f,//16
		-0.33f,    1.0f,  0.33f,		 1.0f, 0.0f,
		-0.33f,   -1.0f,  0.33f,		 1.0f, 0.0f,
		-1.0f,   -1.0f,   0.33f,		 1.0f, 0.0f,//19

		 //Cuarto muro
		-1.0f,    1.0f,  -0.33f,		 1.0f, 0.0f,//20
		-0.66f,    1.0f,  -0.33f,		 1.0f, 0.0f,
		-0.66f,   -1.0f,  -0.33f,		 1.0f, 0.0f,
		-1.0f,   -1.0f,   -0.33f,		 1.0f, 0.0f,//23

		//Quinto muro
		 0.33f,   1.0f,  -0.33f,		 1.0f, 0.0f,//24
		 0.33f,   1.0f,  -1.0f,			 1.0f, 0.0f,
		 0.33f,  -1.0f,  -1.0f,			 1.0f, 0.0f,
		 0.33f,  -1.0f,  -0.33f,		 1.0f, 0.0f,//27

		 //Sexto muro
		 -0.33f,   1.0f,  -0.33f,		 1.0f, 0.0f,//28
		 -0.33f,   1.0f,  -1.0f,		 1.0f, 0.0f,
		 -0.33f,  -1.0f,  -1.0f,		 1.0f, 0.0f,
		 -0.33f,  -1.0f,  -0.33f,		 1.0f, 0.0f,//31

		 //Primer piso
		 -1.0f,   -0.5f,   -1.0f,		 1.0f, 0.0f,//32
		  1.0f,   -0.5f,   -1.0f,		 0.0f, 0.0f,//33
		  1.0f,   -0.5f,   1.0f,		 1.0f, 1.0f,//34
		 -1.0f,   -0.5f,   1.0f,		 0.0f, 1.0f,//35

		 

		 //Septimo muro
		 -1.0f,   1.0f,   -0.33f,		 1.0f, 0.0f,//36
		 -0.66f,   1.0f,   -0.33f,		 0.0f, 0.0f,//37
		 -0.66f,   -1.0f,   -0.33f,		 1.0f, 1.0f,//38
		 -1.0f,   -1.0f,   -0.33f,		 0.0f, 1.0f,//39

		 //Segundo piso
		 -1.0f,   -0.0f,   -1.0f,		 0.0f, 0.0f,//40
		  1.0f,   -0.0f,   -1.0f,		 1.0f, 0.0f,//
		  1.0f,   -0.0f,   1.0f,		 1.0f, 1.0f,//
		 -1.0f,   -0.0f,   1.0f,		 0.0f, 1.0f,//43

		//Tercer piso
		-1.0f, 0.5f, -1.0f,				0.0f, 0.0f,//44
		1.0f, 0.5f, -1.0f,				1.0f, 0.0f,//
		1.0f, 0.5f, 1.0f,				1.0f, 1.0f,//
		-1.0f, 0.5f, 1.0f,			    0.0f, 1.0f,//47
		*/

/*
		//Cubo del profe
		/*
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f*/
	//};
	/*
	float triangulos[] = 
	{
		
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f   // top 
		 

		 //Cubo
		/* 30.0f,  10.0f,  30.0f,			 1.0f, 0.0f,
		 -30.0f, 10.0f,  30.0f,			 1.0f, 0.0f,
		 -30.0f, 10.0f, -30.0f,		     1.0f, 0.0f,
		 30.0f,  30.0f, -30.0f,			 1.0f, 0.0f, //3

		  30.0f, -10.0f,  30.0f,		 0.0f, 1.0f,
		 -30.0f, -10.0f,  30.0f,		 0.0f, 1.0f,
		 -30.0f, -10.0f, -30.0f,		 0.0f, 1.0f,
		  30.0f, -10.0f, -30.0f,		 0.0f, 1.0f,*/
	//};
	/*
	unsigned int indices[]
	{
		//Castillo

		//inferir
		1,3,2,
		1,3,4,

		1,4,5,
		8,1,5,

		8,5,6,
		8,6,7,

		//Supeior
		10,11,12,
		10,12,13,

		17,10,13,
		17,13,14,

		17,14,15,
		17,15,16,

		//puerta
		13,1,0,
		13,0,18,

		13,18,14,
		14,18,19,

		14,19,9,
		14,9,8,

		//muros 
		15,14,8,
		15,8,7,

		16,15,7,
		16,7,6,

		17,16,6,
		17,6,5,

		10,17,5,
		10,5,4,

		11,10,4,
		11,4,3,

		12,11,3,
		12,3,2,

		13,12,2,
		13,2,1,

		//barras
		21,20,10,
		21,10,11,

		23,22,12,
		23,12,13,

		25,24,14,
		25,14,15,

		27,26,16,
		27,16,17,
		



		//Casa
		/*
		//Primer lado, frentre
		0,1,2,
		2,5,0,

		4,3,6,
		4,6,7,

		8,7,6,

		9,6,10,


		8,9,11,

		5,4,7,
		5,7,8,

		//Segundo lado frente
		12,0,5,
		12,5,13,

		14,15,17,
		15,16,17,

		16,8,17,

		19,17,18,

		8,11,19,

		15,5,8,
		15,8,16,

		//Parte Trasera
		20,21,22,
		20,22,24,

		24,23,25,
		24,25,26,

		28,24,26,
		28,26,27,

		30,20,24,
		30,24,29,

		//Pared Derecha
		20,0,12,
		20,12,21,

		21,12,13,
		21,13,22,

		23,14,13,
		23,13,22,

		23,14,17,
		23,17,25,

		//pared Izquierda
		0,20,30,
		0,30,1,

		1,30,29,
		1,29,2,

		3,28,29,
		3,29,2,

		3,28,27,
		3,27,6,

		//Suelo
		25,27,6,
		25,6,17,
		*/

		//Depas
		/*
		//cuadro
		2,1,5,
		2,5,6,

		3,2,6,
		3,6,7,

		0,3,7,
		0,7,4,

		2,3,0,
		2,0,1,

		5,4,7,
		5,7,6,

		//Primer muro
		8,9,10,
		8,10,11,

		//Seundo muro
		12, 13, 14,
		12, 14, 15,

		//Tercer muro
		16, 17, 18,
		16, 18, 19,

		//Cuarto muro
		20, 21, 22,
		20, 22, 23,

		//Quinto muro
		24, 25, 26,
		24, 26, 27,

		//Sexto muro
		28, 29, 30,
		28, 30, 31,

		//Septimo muro
		36, 37, 38,
		36, 38, 39,

		//Primer piso
		32, 33, 34,
		32, 34, 35,

		//Segundo piso
		40, 41, 42,
		40, 42, 43,


		//Tercer piso
		44, 45, 46,
		44, 46, 47
		*/

		//Cubo del profe
		/* //Cubo del profe
		1,2,5,
		2,5,6,

		2,3,6,
		3,6,7,

		3,4,7,
		4,7,8,

		4,1,8,
		1,8,5,

		1,2,3,
		3,1,4,

		7,8,5,
		5,7,6*/

		/*0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		14, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35*/
	//};
	/*
	unsigned int indicestriangulo[]
	{
		//cuadro
		1,0,4,
		/*1,4,5,

		2,1,5,
		2,5,6,

		3,2,6,
		3,6,7,

		0,3,7,
		0,7,4,

		2,3,0,
		2,0,1,

		5,4,7,
		5,7,6*/
		
	//};
	/*
	vec3 posicionesCubo[] = 
	{
		//    x      y      z
		vec3(0.0f,  -20.0f,  -100.0f),
		//vec3(2.0f,  5.0f, -15.0f),
		//vec3(-1.5f, -2.2f, -2.5f),
		//vec3(-3.8f, -2.0f, -12.3f),
		//vec3(2.4f, -0.4f, -3.5f),
		//vec3(-1.7f,  3.0f, -7.5f),
		//vec3(1.3f, -2.0f, -2.5f),
		//vec3(1.5f,  2.0f, -2.5f),
		//vec3(1.5f,  0.2f, -1.5f),
		//vec3(-1.3f,  1.0f, -1.5f)
	};
	
	//---------------primer vertex-----------------------

	unsigned int VBO, VAO, EBO; //Vertex Buffer Object, Vertex Array Object y Extendet Array Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//unir o linkear
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texturas
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//--------------- segundo vertex------------
	unsigned int VBO2, VAO2, EBO2; //Vertex Buffer Object, Vertex Array Object y Extendet Array Object
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);
	//unir o linkear
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangulos), triangulos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicestriangulo), indicestriangulo, GL_STATIC_DRAW);
	//posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//crear y cargar nuestras texturas
	unsigned int textura1, textura2;
	//primera textura
	glGenTextures(1, &textura1);
	glBindTexture(GL_TEXTURE_2D, textura1);
	//configurar el comportamiento de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//configuramos el filtrado de la textura en caso que se expanda
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//cargar la imagen, crear la textura y generar los mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	//cargar nuestra textura
	unsigned char* data = stbi_load("cosa2.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "fallaste en cargar la primera textura" << std::endl;
	}
	stbi_image_free(data);
	
	//Textura2
	glGenTextures(1, &textura2);
	glBindTexture(GL_TEXTURE_2D, textura2);
	//configurar el comportamiento de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//configuramos el filtrado de la textura en caso que se expanda
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//cargar nuestra textura
	data = stbi_load("cosa1.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "fallaste en cargar la segunda textura" << std::endl;
	}
	stbi_image_free(data);

	nuestroShader.use();
	nuestroShader.setInt("textura1", 0);
	nuestroShader.setInt("textura2", 1);

	glBindVertexArray(VAO2);
	//loop para que se pueda visualizar nuestra pantalla
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		//Renderizado
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //LIMPIEZA DEL BUFFER Z

		//activado  y union de las texturas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textura1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textura2);

		//transformaciones
		/*mat4 transform = mat4(1.0f); //inicializamos con una matriz identidad
		transform = translate(transform, vec3(0.3f, -0.5f, 0.0f));
		transform = rotate(transform, (float)glfwGetTime(), vec3(0.0f, 1.0f, 1.0f));*/
/*
		nuestroShader.use();

		//MVP
		/*mat4 view = mat4(1.0f);
		mat4 projection = mat4(1.0f);
		projection = perspective(radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f ); //SE maneja en radianes para mayor exactitud
		view = translate(view, vec3(0.0f, 0.0f, -3.0f));*/
/*
		mat4 projection = perspective(radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 250.0f); //camera
		mat4 view = camera.GetViewMatrix();
		

		nuestroShader.setMat4("projection", projection);
		nuestroShader.setMat4("view", view);

		/*unsigned int transformLoc = glGetUniformLocation(nuestroShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));*/
/*
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			mat4 model = mat4(1.0f);
			model = translate(model, posicionesCubo[i]);
			float angulos = (float)glfwGetTime() * (i + 1);
			//model = rotate(model, radians(angulos), vec3(1.0f, 0.3f, 0.5f));
			model = scale(model, vec3(40, 40, 40));

			nuestroShader.setMat4("model", model);
			glDrawElements(GL_TRIANGLES, 150, GL_UNSIGNED_INT, 0); //Aqui se delimita los tringulos TRUCHA-------------------------
		}
		glDrawElements(GL_TRIANGLES, 150, GL_UNSIGNED_INT, 0);

		
		mat4 transform2 = mat4(1.0f);
		//transform2 = translate(transform2, vec3(-0.3f, 0.5f, 0.0f));
		//transform2 = rotate(transform2, ((float)glfwGetTime() * -1) * 5, vec3(1.0f, 1.0f, 1.0f));
		transform2 = scale(transform2, vec3(50, 50, 50));

		segundoShader.use();

		unsigned int transformLoc2 = glGetUniformLocation(segundoShader.ID, "transform");
		glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, value_ptr(transform2));

		float colorverde = sin(glfwGetTime()) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(segundoShader.ID, "nuestroColor");
		glUniform4f(vertexColorLocation, 0.0f, colorverde, 0.0f, 1.0f);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		//detecte eventos de IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//nuestra primera configuracion de la camara
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) 
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;
	
	camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}
*/