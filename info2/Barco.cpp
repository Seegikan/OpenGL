
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
#include <vector>


/*
Tenemos luz:
	-Ambiental: luz homogénea, que se va reflejar en un ángulo de 90° que va a iluminar todos los objetos de nuestro shader
	-Especular: es un punto de luz concentrado dentro de la figura. Entre más grande sea el valor, se concentrará más en un punto
	-combinada: Para crear efectos mejores
*/
using namespace glm;
using namespace std;

void Instantiate(Shader& _shader, vec3 _position);
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale);
void Instantiate(Shader& _shader, vec3 _position, float _angle, vec3 _axis);
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale, float _angle, vec3 _axis);

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //almacenar nuestra ventana y configs
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window); //acciones para nuestra ventana
unsigned int LoadTexture(const char* path);
unsigned int LoadCubeMap(vector<string> faces);

//medidas de la pantalla
const unsigned int WIDTH = 1920;
const unsigned int HEIGHT = 1024;

//Número de vértices en la figura
const unsigned int cubeVertexNum = 100;

//camara
Camara camera(glm::vec3(0.0f, 5.0f, 30.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Mis texturas----------------------------------------------------
unsigned int concreto, cubierta, oxido, pared, escalon, bandera;


//Punto donde estará posicionada nuestra luz
vec3 posLuz(0.0f, 2.0f, 0.0f);

float tempCoord = 0.0f;

int main() 
{
	//inicializar glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//creamos nuestra ventana
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Guapo Alexander", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fallo en crear GLFW y la ventana" << std::endl;
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

	Shader nuestroShader("Cubo.vs", "Cubo.fs");
	Shader luzShader("Luz.vs", "Luz.fs");
	Shader skyboxShader("Skybox.vs", "Skybox.fs");

	float vertices[]{
		//posiciones		//cómo afecta
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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

	vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
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

	float verticesSkybox[] = {
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//luces
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Texturas
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//segundo shader
	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Skybox Shader
	unsigned int sVAO, sVBO;
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

	//crear la textura
	unsigned int cubemapTexture = LoadCubeMap(faces);

	//Configurar el shader del skybox
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	unsigned int diffuseMap = LoadTexture("Box.png");
	unsigned int specularMap = LoadTexture("Box_Specular.png");

	//Mis texturas
	/*texMetal = LoadTexture("Metal.png");
	texGrass = LoadTexture("Grass.jpg");
	texWood = LoadTexture("Wood.jpg");
	texWhiteWood = LoadTexture("whiteWood.jpeg");
	texWool = LoadTexture("Wool.jpg");
	texWool2 = LoadTexture("Wool2.jpg");
	texStone = LoadTexture("Stone.jpg");
	texStoneGray = LoadTexture("StoneGray.jpg");*/

	//CARGA DE TEXTURAS-------------------------------------------------------------
	concreto = LoadTexture("concrete.jpg"); //Palos
	cubierta = LoadTexture("escalon.jpg"); //Palos
	oxido = LoadTexture("metal.jpg"); //Palos
	//pared = LoadTexture("Paared.jpg"); //Palos
	//escalon = LoadTexture("escalon.jpg"); //Palos
	//bandera = LoadTexture("Negro.jpg"); //Palos


	nuestroShader.use();
	nuestroShader.setInt("material.diffuse", 0);
	nuestroShader.setInt("material.specular", 1);

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
		nuestroShader.setVec3("light.position", posLuz);
		nuestroShader.setVec3("light.position", camera.Position);
		nuestroShader.setVec3("light.direction", camera.Front);
		nuestroShader.setFloat("light.cutOff", cos(radians(25.0f)));
		nuestroShader.setFloat("light.outerCutOff", cos(radians(32.0f)));
		nuestroShader.setVec3("viewPos", camera.Position);

		nuestroShader.setVec3("light.ambient", vec3(0.6f));
		nuestroShader.setVec3("light.ambient", 0.4f, 0.1f, 0.0f);
		nuestroShader.setVec3("light.diffuse", 0.9f, 0.9f, 0.9f);
		nuestroShader.setVec3("light.specular", vec3(0.9f));
		nuestroShader.setFloat("light.constant", 1.0f);
		nuestroShader.setFloat("light.linear", 0.1f);
		nuestroShader.setFloat("light.quadratic", 0.0032f);

		nuestroShader.setFloat("material.shininess", 32.0f);

		//mvp
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 300.0f);
		glm::mat4 view = camera.GetViewMatrix();

		nuestroShader.setMat4("projection", projection);
		nuestroShader.setMat4("view", view);

		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);

		//Pilares-------------------------------------------
		glBindTexture(GL_TEXTURE_2D, concreto); //Poner textura por defecto
		tempCoord = -179.7f;
		for (int i = 0; i < 300; i++)
		{
			Instantiate(nuestroShader, vec3(tempCoord, 1.8f, 60.0f), vec3(0.2f, 2.0f, 0.2f));
			tempCoord += 2.0f;
		}





		//Faro ----------------------------------------------
		//Sin rota
		//glBindTexture(GL_TEXTURE_2D, concreto); //Poner textura por defecto
		Instantiate(nuestroShader, vec3(2.7f, 32.1f, -47.3f), vec3(11.8f, 43.7f, 3)); //Tierra
		Instantiate(nuestroShader, vec3(2.7f, 50.6f, -45.1f), vec3(13.7f, 7.7f, 2.7f)); //Tierra
		Instantiate(nuestroShader, vec3(2.7f, 13.6f, -45.1f), vec3(14.8f, 8.37f, 3)); //Tierra
		//con rota
		//									pos								escala			angulo					eje
		Instantiate(nuestroShader, vec3(10.3f, 32.1f, -52.9f), vec3(11.8f, 43.71f, 3.0f), 72.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-5.0f, 32.1f, -52.9f), vec3(11.8f, 43.71f, 3.0f), -72.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-2.0f, 32.1f, -61.8f), vec3(11.8f, 43.71f, 3.0f), -144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(7.4f, 32.1f, -61.8f), vec3(11.8f, 43.71f, 3.0f), 144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		//Instantiate(nuestroShader, vec3(-2.0f, 13.5f, -45.1f), vec3(14.8f, 8.3f, 3.0f), -144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(12.2f, 13.6f, -52.0f), vec3(14.8f, 8.37f, 3.0f), 72.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-6.8f, 13.5f, -52.0f), vec3(14.8f, 8.37f, 3.0f), -72.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-3.19f, 13.6f, -63.2f), vec3(14.8f, 8.37f, 3.0f), -144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(8.58f, 13.6f, -63.2f), vec3(14.8f, 8.37f, 3.0f), 144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(11.5f, 50.6f, -52.0f), vec3(13.7f, 7.75f, 2.7f), 72.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-6.8f, 50.6f, -52.3f), vec3(13.7f, 7.7f, 2.7f), -72.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-2.7f, 50.6f, -62.3f), vec3(13.7f, 7.7f, 2.7f), -144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(8.1f, 50.6f, -62.3f), vec3(13.7f, 7.7f, 2.7f), 144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta






		//Faro1 ----------------------------------------------
		Instantiate(nuestroShader, vec3(-125.1f, 32.1f, -47.3f), vec3(11.8f, 43.7f, 3)); //Tierra
		Instantiate(nuestroShader, vec3(-125.1f, 13.1f, -45.1f), vec3(13.7f, 7.7f, 2.7f)); //Tierra
		Instantiate(nuestroShader, vec3(-125.1f, 50.61f, -45.1f), vec3(14.8f, 8.37f, 3)); //Tierra
		//con rota
		//									pos								escala			angulo					eje
		Instantiate(nuestroShader, vec3(-117.5f, 32.1f, -52.9f), vec3(11.8f, 43.71f, 3.0f), 72.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-132.7f, 32.1f, -52.9f), vec3(11.8f, 43.71f, 3.0f), -72.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-129.8f, 32.1f, -61.8f), vec3(11.8f, 43.71f, 3.0f), -144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-120.4f, 32.1f, -61.8f), vec3(11.8f, 43.71f, 3.0f), 144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		//Instantiate(nuestroShader, vec3(-2.0f, 13.5f, -45.1f), vec3(14.8f, 8.3f, 3.0f), -144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-115.1f, 13.6f, -52.0f), vec3(14.8f, 8.37f, 3.0f), 72.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-134.6f, 13.5f, -52.0f), vec3(14.8f, 8.37f, 3.0f), -72.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-130.9f, 13.6f, -63.2f), vec3(14.8f, 8.37f, 3.0f), -144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-119.2f, 13.6f, -63.2f), vec3(14.8f, 8.37f, 3.0f), 144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader,			vec3(-119.6f, 50.6f, -62.0f), vec3(13.7f, 7.75f, 2.7f), 144.0f, vec3(0.0f, 1.0f, 0.0f)); //Puerta
		Instantiate(nuestroShader, vec3(-116.2f, 50.6f, -52.3f), vec3(13.7f, 7.7f, 2.7f), 72.0f, vec3(0.0f, 1.0f, 0.0f)); 
		Instantiate(nuestroShader, vec3(-133.9f, 50.6f, -52.3f), vec3(13.7f, 7.7f, 2.7f), -72.0f, vec3(0.0f, 1.0f, 0.0f)); 
		Instantiate(nuestroShader, vec3(-130.5f, 50.6f, -62.3f), vec3(13.7f, 7.7f, 2.7f), -144.0f, vec3(0.0f, 1.0f, 0.0f)); 

		//Barco----------------------------------------------
		glBindTexture(GL_TEXTURE_2D, cubierta); 
		Instantiate(nuestroShader, vec3(0.68f, 4.1f, 0.0f), vec3(19.3f, 5.9f, 19.3f)); 
		Instantiate(nuestroShader, vec3(-18.8f, 4.1f, 0.0f), vec3(19.3f, 5.9f, 16.3f));
		Instantiate(nuestroShader, vec3(-5.64f, 23.5f, 0.0f), vec3(1, 32.7f, 1));
		Instantiate(nuestroShader, vec3(-15.4f, 17, 0.0f), vec3(1, 20.0f, 1));
		Instantiate(nuestroShader, vec3(-24.9f, 7.7f, 0.0f), vec3(8, 1.4f, 16.38f));
		Instantiate(nuestroShader, vec3(-15.4f, 17, 0.0f), vec3(1, 20.0f, 1));
		//Tubos--
		glBindTexture(GL_TEXTURE_2D, oxido); 

		Instantiate(nuestroShader, vec3(-28.8, 8.5f, 0.0f), vec3(0.3f, 0.3f, 16.3f));
		Instantiate(nuestroShader, vec3(-28.8, 10, 0.0f), vec3(0.3f, 0.3f, 16.3f));
		Instantiate(nuestroShader, vec3(-24.9, 8.5f, -8.0f), vec3(0.3f, 0.3f, 7.9f), 90.0f, vec3(0.0f, 1.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-24.9, 8.5f, 8.0f), vec3(0.3f, 0.3f, 7.9f), 90.0f, vec3(0.0f, 1.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-24.9, 10.0f, 8.0f), vec3(0.3f, 0.3f, 7.9f), 90.0f, vec3(0.0f, 1.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-24.9, 10, -8.0f), vec3(0.3f, 0.3f, 7.9f), 90.0f, vec3(0.0f, 1.0f, 0.0f));

		//escaleraas--
		glBindTexture(GL_TEXTURE_2D, cubierta); 
		Instantiate(nuestroShader, vec3(6.3, 9, 0.0f), vec3(8.5f, 4.0f, 19.3f));
		Instantiate(nuestroShader, vec3(-20.6, 7.4f, 0.0f), vec3(0.7f, 0.9f, 16.38f));
		Instantiate(nuestroShader, vec3(-19.9, 7.19f, 0.0f), vec3(0.7f, 0.4f, 16.38f));

		//Tubos--
		glBindTexture(GL_TEXTURE_2D, oxido);
		Instantiate(nuestroShader, vec3(-28.82, 9.2f, 8.0f), vec3(0.3f, 1.29f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f));

		Instantiate(nuestroShader, vec3(-28.82, 9.2f, 8.0f), vec3(0.3f, 1.29f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-28.82, 9.2f, -8.0f), vec3(0.3f, 1.29f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-21.1, 9.2f, -8.0f), vec3(0.3f, 1.29f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-21.1, 9.2f, 8.0f), vec3(0.3f, 1.29f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f));

		Instantiate(nuestroShader, vec3(-14.5f, 7.3f, -8.0f), vec3(11.2f, 0.4f, 0.3f)); //33
		Instantiate(nuestroShader, vec3(-14.5f, 7.3f, 8.0f), vec3(11.2f, 0.4f, 0.3f)); 
		Instantiate(nuestroShader, vec3(-3.2f, 7.3f, 9.5f), vec3(11.2f, 0.4f, 0.3f)); //35
		Instantiate(nuestroShader, vec3(-3.2f, 7.3f, -9.5f), vec3(11.2f, 0.4f, 0.3f)); //36

		Instantiate(nuestroShader, vec3(-8.82, 7.3f, 8.7f), vec3(1.7f, 0.4f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f));
		Instantiate(nuestroShader, vec3(-8.82, 7.3f, -8.7f), vec3(1.7f, 0.4f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f));

		Instantiate(nuestroShader, vec3(-15.1f, 9.0f, 8.0f), vec3(12.2f, 0.4f, 0.3f)); //39
		Instantiate(nuestroShader, vec3(-4.1f, 9.0f, 9.5f), vec3(9.6f, 0.4f, 0.3f)); //40

		Instantiate(nuestroShader, vec3(-8.82, 9.0f, 8.7f), vec3(1.7f, 0.4f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f));

		Instantiate(nuestroShader, vec3(-15.1f, 9.0f, -8.0f), vec3(12.2f, 0.4f, 0.3f)); //

		Instantiate(nuestroShader, vec3(-3.2f, 9.0f, -9.5f), vec3(11.3f, 0.4f, 0.3f)); //43

		Instantiate(nuestroShader, vec3(-8.82, 9.0f, -8.7f), vec3(1.7f, 0.4f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); 
		Instantiate(nuestroShader, vec3(-20.3, 8.0f, -8.0f), vec3(0.3f, 1.6f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //32
		Instantiate(nuestroShader, vec3(-8.82, 8.0f, -8.3f), vec3(0.3f, 1.6f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //45
		Instantiate(nuestroShader, vec3(-8.7, 8.0f, -9.5f), vec3(0.3f, 1.6f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //46
		Instantiate(nuestroShader, vec3(-20, 8.0f, 8.0f), vec3(0.3f, 1.6f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //47
		Instantiate(nuestroShader, vec3(-8.82, 7.9f, 8.0f), vec3(0.3f, 1.6f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f));//48
		Instantiate(nuestroShader, vec3(-8.8, 8.0f, -9.5f), vec3(0.3f, 1.6f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //49

		Instantiate(nuestroShader, vec3(5.4f, 11.2f, 9.5f), vec3(10.4f, 0.3f, 0.3f)); //54
		Instantiate(nuestroShader, vec3(5.4f, 11.2f, -9.5f), vec3(10.4f, 0.3f, 0.3f)); //55

		Instantiate(nuestroShader, vec3(0.3, 10.0f, -9.5f), vec3(0.3f, 1.9f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //53
		Instantiate(nuestroShader, vec3(0.3, 10.0f, 9.5f), vec3(0.3f, 1.9f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //56
		Instantiate(nuestroShader, vec3(10.3, 11.2f, 0.0f), vec3(1.9f, 0.3f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //57
		Instantiate(nuestroShader, vec3(10.3, 13.1f, 0.0f), vec3(1.9f, 0.3f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //60
		Instantiate(nuestroShader, vec3(0.3, 12.2f, -9.5f), vec3(0.3f, 1.9f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //61
		Instantiate(nuestroShader, vec3(10.3, 12.1f, -9.5f), vec3(1.3f, 1.9f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //62
		Instantiate(nuestroShader, vec3(10.5, 11.9f, 9.4f), vec3(0.3f, 1.9f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //63
		Instantiate(nuestroShader, vec3(0.3, 11.9f, 9.5f), vec3(0.3f, 1.9f, 0.3f), 90.0f, vec3(0.0f, 1.0f, 0.0f)); //61




		Instantiate(nuestroShader, vec3(5.4f, 13.2f, 9.5f), vec3(10.4f, 0.3f, 0.3f)); //58
		Instantiate(nuestroShader, vec3(5.4f, 13.2f, -9.5f), vec3(10.4f, 0.3f, 0.3f)); //58
		Instantiate(nuestroShader, vec3(-15.5f, 20.1f, -0.5f), vec3(0.1f, 9.1f, 12.4f)); //65
		Instantiate(nuestroShader, vec3(-6.0f, 20.1f, -0.5f), vec3(0.1f, 12.6f, 21.1f)); //66
		Instantiate(nuestroShader, vec3(-6.0f, 33.2f, -0.5f), vec3(0.1f, 7.6f, 8.7f)); //67

		//Timon
		Instantiate(nuestroShader, vec3(2.6f, 12.3f, -0.0f), vec3(0.4f, 3.1f, 8.2f)); //66






		//Escaleras--
		glBindTexture(GL_TEXTURE_2D, cubierta);
		Instantiate(nuestroShader, vec3(1.3f, 8.2f, .0f), vec3(1.5f, 2.4f, 19.3f)); //50
		Instantiate(nuestroShader, vec3(-0.17f, 7.7f, .0f), vec3(1.5f, 1.5f, 19.3f)); //51
		Instantiate(nuestroShader, vec3(-0.17f, 7.3f, .0f), vec3(1.5f, 0.66f, 19.3f)); //52

























//#pragma region SUELO
//		glBindTexture(GL_TEXTURE_2D, texDirt); //Poner textura por defecto
//		Instantiate(nuestroShader, vec3(0.0f, 0.0f, 5.0f), vec3(50.0f, 1.0f, 60.0f)); //Tierra
//#pragma endregion
//
//#pragma region BOXES
//		glBindTexture(GL_TEXTURE_2D, texBox);
//		Instantiate(nuestroShader, vec3(12.0f, 2.0f, -6.25f));
//		Instantiate(nuestroShader, vec3(12.0f, 1.0f, -7.0f));
//		Instantiate(nuestroShader, vec3(8.25f, 1.25f, 14.75f));
//		Instantiate(nuestroShader, vec3(7.0f, 1.25f, 14.75f));
//		Instantiate(nuestroShader, vec3(7.75f, 2.25f, 15.0f));
//		Instantiate(nuestroShader, vec3(8.75f, 1.25f, -18.25f));
//		Instantiate(nuestroShader, vec3(-9.25f, 1.0f, -18.25f));
//		Instantiate(nuestroShader, vec3(-12.25f, 1.25f, -5.75f));
//		Instantiate(nuestroShader, vec3(-16.0f, 1.0f, -7.5f));
//		Instantiate(nuestroShader, vec3(12.0f, 1.0f, -5.75f));
//		Instantiate(nuestroShader, vec3(10.75f, 1.0f, -6.0f));
//		Instantiate(nuestroShader, vec3(6.0f, 1.0f, -12.5f));
//		Instantiate(nuestroShader, vec3(4.0f, 1.0f, -23.0f));
//		Instantiate(nuestroShader, vec3(-7.75f, 1.0f, -20.75f));
//		Instantiate(nuestroShader, vec3(-3.0f, 1.0f, -8.0f));
//		Instantiate(nuestroShader, vec3(6.0f, 1.0f, -4.0f));
//		Instantiate(nuestroShader, vec3(-5.5f, 1.0f, 7.75f));
//
//#pragma endregion
//
//#pragma region Postes
//		glBindTexture(GL_TEXTURE_2D, texWood4);
//		Instantiate(nuestroShader, vec3(-5.5f, 2.5f, -23.5f), vec3(3.0f, 0.25f, 0.25f));
//		Instantiate(nuestroShader, vec3(-4.5f, 1.5f, -23.5f), vec3(0.25f, 1.75f, 0.25f));
//		Instantiate(nuestroShader, vec3(-6.5f, 1.5f, -23.5f), vec3(0.25f, 1.75f, 0.25f));
//
//		Instantiate(nuestroShader, vec3(-9.0f, 2.5f, -8.75f), vec3(0.25f, 0.25f, 3.0f));
//		Instantiate(nuestroShader, vec3(-9.0f, 1.5f, -9.75f), vec3(0.25f, 1.75f, 0.25f));
//		Instantiate(nuestroShader, vec3(-9.0f, 1.5f, -7.75f), vec3(0.25f, 1.75f, 0.25f));
//
//		Instantiate(nuestroShader, vec3(-5.75f, 2.5f, -2.5f), vec3(0.25f, 0.25f, 3.0f));
//		Instantiate(nuestroShader, vec3(-5.75f, 1.5f, -3.5f), vec3(0.25f, 1.75f, 0.25f));
//		Instantiate(nuestroShader, vec3(-5.75f, 1.5f, -1.5f), vec3(0.25f, 1.75f, 0.25f));
//
//		Instantiate(nuestroShader, vec3(5.75f, 2.5f, -1.75f), vec3(0.25f, 0.25f, 3.0f));
//		Instantiate(nuestroShader, vec3(5.75f, 1.5f, -2.75f), vec3(0.25f, 1.75f, 0.25f));
//		Instantiate(nuestroShader, vec3(5.75f, 1.5f, -0.75f), vec3(0.25f, 1.75f, 0.25f));
//
//#pragma endregion
//
//#pragma region VALLA FRONTAL
//		glBindTexture(GL_TEXTURE_2D, texWood);
//		Parte izquierda
//		tempCoord = -21.0f;
//		for (int i = 0; i < 9; i++)
//		{
//			Instantiate(nuestroShader, vec3(tempCoord, 1.5f, -22.0f), vec3(0.2f, 2.0f, 0.2f));
//			tempCoord += 2.0f;
//		}
//		Parte derecha
//		tempCoord = 5.0f;
//		for (int i = 0; i < 9; i++)
//		{
//			Instantiate(nuestroShader, vec3(tempCoord, 1.5f, -22.0f), vec3(0.2f, 2.0f, 0.2f));
//			tempCoord += 2.0f;
//		}
//
//		Entrada principal
//		Instantiate(nuestroShader, vec3(-3.0f, 3.5f, -22.0f), vec3(0.4f, 6.0f, 0.4f));
//		Instantiate(nuestroShader, vec3(3.0f, 3.5f, -22.0f), vec3(0.4f, 6.0f, 0.4f));
//		Instantiate(nuestroShader, vec3(0.0f, 6.75f, -22.0f), vec3(7.5f, 0.5f, 0.5f));
//		Instantiate(nuestroShader, vec3(-1.0f, 6.25f, -22.0f), vec3(0.1f, 0.5f, 0.1f)); //Palito 1
//		Instantiate(nuestroShader, vec3(1.0f, 6.25f, -22.0f), vec3(0.1f, 0.5f, 0.1f)); //Palito 2
//
//		Tablas horizontales
//		Instantiate(nuestroShader, vec3(12.25f, 2.25f, -22.15f), vec3(18.0f, 0.2f, 0.1f));
//		Instantiate(nuestroShader, vec3(12.25f, 1.5f, -22.15f), vec3(18.0f, 0.2f, 0.1f));
//		Instantiate(nuestroShader, vec3(12.25f, 0.75f, -22.15f), vec3(18.0f, 0.2f, 0.1f));
//
//		Instantiate(nuestroShader, vec3(-12.25f, 2.25f, -22.15f), vec3(18.0f, 0.2f, 0.1f));
//		Instantiate(nuestroShader, vec3(-12.25f, 1.5f, -22.15f), vec3(18.0f, 0.2f, 0.1f));
//		Instantiate(nuestroShader, vec3(-12.25f, 0.75f, -22.15f), vec3(18.0f, 0.2f, 0.1f));
//
//
//		glBindTexture(GL_TEXTURE_2D, texOldMetal);
//		Instantiate(nuestroShader, vec3(0.0f, 5.5f, -22.0f), vec3(4.0f, 1.0f, 0.1f)); //Letrero
//
//
//#pragma endregion
//
//#pragma region VALLA DERECHA
//		glBindTexture(GL_TEXTURE_2D, texWood);
//		tempCoord = -20.0f;
//		for (int i = 0; i < 27; i++)
//		{
//			Instantiate(nuestroShader, vec3(21.0f, 1.5f, tempCoord), vec3(0.2f, 2.0f, 0.2f));
//			tempCoord += 2.0f;
//		}
//
//		Instantiate(nuestroShader, vec3(21.15f, 2.25f, 5.0f), vec3(0.1f, 0.2f, 54.0f));
//		Instantiate(nuestroShader, vec3(21.15f, 1.5f, 5.0f), vec3(0.1f, 0.2f, 54.0f));
//		Instantiate(nuestroShader, vec3(21.15f, 0.75f, 5.0f), vec3(0.1f, 0.2f, 54.0f));
//
//#pragma endregion
//
//#pragma region VALLA IZQUIERDA
//		tempCoord = -20.0f;
//		for (int i = 0; i < 27; i++)
//		{
//			Instantiate(nuestroShader, vec3(-21.0f, 1.5f, tempCoord), vec3(0.2f, 2.0f, 0.2f));
//			tempCoord += 2.0f;
//		}
//
//		Instantiate(nuestroShader, vec3(-21.15f, 2.25f, 5.0f), vec3(0.1f, 0.2f, 54.0f));
//		Instantiate(nuestroShader, vec3(-21.15f, 1.5f, 5.0f), vec3(0.1f, 0.2f, 54.0f));
//		Instantiate(nuestroShader, vec3(-21.15f, 0.75f, 5.0f), vec3(0.1f, 0.2f, 54.0f));
//
//#pragma endregion
//
//#pragma region VALLA TRASERA
//		tempCoord = -19.0f;
//		for (int i = 0; i < 20; i++)
//		{
//			Instantiate(nuestroShader, vec3(tempCoord, 1.5f, 32.0f), vec3(0.2f, 2.0f, 0.2f));
//			tempCoord += 2.0f;
//		}
//
//		Instantiate(nuestroShader, vec3(0.0f, 2.25f, 32.15f), vec3(42.0f, 0.2f, 0.1f));
//		Instantiate(nuestroShader, vec3(0.0f, 1.5f, 32.15f), vec3(42.0f, 0.2f, 0.1f));
//		Instantiate(nuestroShader, vec3(0.0f, 0.75f, 32.15f), vec3(42.0f, 0.2f, 0.1f));
//
//#pragma endregion
//
//#pragma region MOTEL
//		glBindTexture(GL_TEXTURE_2D, texWood2);
//		Instantiate(nuestroShader, vec3(-15.0f, 8.5f, -14.0f), vec3(10.0f, 16.0f, 12.0f));
//		Instantiate(nuestroShader, vec3(-9.0f, 4.5f, -13.75f), vec3(3.0f, 0.2f, 8.0f), -20.0f, vec3(0.0f, 0.0f, 1.0f)); //Techito
//
//		glBindTexture(GL_TEXTURE_2D, texDoor);
//		Instantiate(nuestroShader, vec3(-10.0f, 2.0f, -13.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f)); //Puerta
//		Instantiate(nuestroShader, vec3(-10.0f, 2.0f, -15.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f)); //Puerta
//
//		glBindTexture(GL_TEXTURE_2D, texWoodenWindow);
//		Instantiate(nuestroShader, vec3(-10.0f, 7.5f, -14.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
//		Instantiate(nuestroShader, vec3(-10.0f, 7.5f, -10.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
//		Instantiate(nuestroShader, vec3(-10.0f, 7.5f, -18.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
//
//		Instantiate(nuestroShader, vec3(-10.0f, 12.5f, -18.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
//		Instantiate(nuestroShader, vec3(-10.0f, 12.5f, -14.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
//		Instantiate(nuestroShader, vec3(-10.0f, 12.5f, -10.0f), vec3(0.2f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
//
//		glBindTexture(GL_TEXTURE_2D, texOldMetal);
//		Instantiate(nuestroShader, vec3(-10.0f, 17.25f, -14.0f), vec3(0.2f, 3.0f, 8.0f));
//
//		glBindTexture(GL_TEXTURE_2D, texWood);
//		Instantiate(nuestroShader, vec3(-9.0f, 2.5f, -11.0f), vec3(0.25f, 4.0f, 0.25f));
//		Instantiate(nuestroShader, vec3(-9.0f, 2.5f, -17.0f), vec3(0.25f, 4.0f, 0.25f));
//
//#pragma endregion
//
//#pragma region SHERIFF COMISARIA
//		glBindTexture(GL_TEXTURE_2D, texWood4);
//		Instantiate(nuestroShader, vec3(13.5f, 0.5f, -14.0f), vec3(13.0f, 0.5f, 12.0f)); //Piso
//		techito y postes
//		glBindTexture(GL_TEXTURE_2D, texWood);
//		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -18.5f), vec3(0.2f, 4.25f, 0.2f));
//		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -9.5f), vec3(0.2f, 4.25f, 0.2f));
//		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -12.0f), vec3(0.2f, 4.25f, 0.2f));
//		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -16.0f), vec3(0.2f, 4.25f, 0.2f));
//		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -17.25f), vec3(0.1f, 0.4f, 3.0f));
//		Instantiate(nuestroShader, vec3(7.5f, 2.75f, -10.75f), vec3(0.1f, 0.4f, 3.0f));
//		Instantiate(nuestroShader, vec3(8.5f, 5.5f, -14.0f), vec3(3.0f, 0.2f, 9.9f), 30.0f, vec3(0.0f, 0.0f, 1.0f)); //Techito
//		Instantiate(nuestroShader, vec3(14.0f, 7.5f, -14.0f), vec3(9.5f, 0.5f, 10.5f)); //Techote
//
//		glBindTexture(GL_TEXTURE_2D, texWood3);
//		Instantiate(nuestroShader, vec3(14.0f, 4.0f, -14.0f), vec3(9.0f, 7.0f, 10.0f)); //Cubote casa
//
//		glBindTexture(GL_TEXTURE_2D, texDoor);
//		Instantiate(nuestroShader, vec3(9.5f, 2.25f, -14.0f), vec3(0.1f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f)); //Puerta
//
//		glBindTexture(GL_TEXTURE_2D, texMetal);
//		Instantiate(nuestroShader, vec3(9.75f, 7.5f, -14.0f), vec3(0.25f, 4.0f, 4.0f), 45.0f, vec3(1.0f, 0.0f, 0.0f));
//
//		glBindTexture(GL_TEXTURE_2D, texWoodenWindow);
//		Instantiate(nuestroShader, vec3(9.5f, 3.25f, -17.0f), vec3(0.1f, 2.0f, 1.5f), 90.0f, vec3(1.0f, 0.0f, 0.0f)); //Ventana
//		Instantiate(nuestroShader, vec3(9.5f, 3.25f, -11.0f), vec3(0.1f, 2.0f, 1.5f), 90.0f, vec3(1.0f, 0.0f, 0.0f)); //Ventana
//
//#pragma endregion
//
//#pragma region CARCEL
//		glBindTexture(GL_TEXTURE_2D, texWood4);
//		Instantiate(nuestroShader, vec3(13.5f, 0.5f, 1.0f), vec3(13.0f, 0.5f, 10.0f)); //Piso
//
//		glBindTexture(GL_TEXTURE_2D, texStone);
//		Paredes
//		Instantiate(nuestroShader, vec3(18.5f, 4.75f, 1.0f), vec3(0.5f, 8.0f, 8.0f));
//		Instantiate(nuestroShader, vec3(9.0f, 4.75f, 2.5f), vec3(0.5f, 8.0f, 5.0f));
//		Instantiate(nuestroShader, vec3(13.75f, 4.75f, 4.75f), vec3(9.0f, 8.0f, 0.5f));
//		Instantiate(nuestroShader, vec3(9.0f, 4.75f, -2.5f), vec3(0.5f, 8.0f, 1.0f));
//		Instantiate(nuestroShader, vec3(13.75f, 4.75f, -2.75f), vec3(9.0f, 8.0f, 0.5f));
//		Instantiate(nuestroShader, vec3(9.0f, 1.25f, -1.0f), vec3(0.5f, 1.0f, 2.0f));
//		Instantiate(nuestroShader, vec3(9.0f, 6.75f, -1.0f), vec3(0.5f, 4.0f, 2.0f));
//
//		glBindTexture(GL_TEXTURE_2D, texWood);
//		Instantiate(nuestroShader, vec3(13.75f, 9.0f, 1.0f), vec3(10.0f, 0.5f, 8.0f)); //Techo
//		Cosito lateral con techo
//		Instantiate(nuestroShader, vec3(13.75f, 7.0f, 7.25f), vec3(9.0f, 0.25f, 5.0f), 20.0f, vec3(1.0f, 0.0f, 0.0f)); //Techito
//		Instantiate(nuestroShader, vec3(9.75f, 3.5f, 8.75f), vec3(0.25f, 6.0f, 0.25f));
//		Instantiate(nuestroShader, vec3(18.0f, 3.5f, 8.75f), vec3(0.25f, 6.0f, 0.25f));
//		Instantiate(nuestroShader, vec3(9.75f, 2.5f, 6.75f), vec3(0.2f, 0.25f, 4.0f));
//		Instantiate(nuestroShader, vec3(18.0f, 2.5f, 6.75f), vec3(0.2f, 0.25f, 4.0f));
//
//		glBindTexture(GL_TEXTURE_2D, texOldMetal);
//		Instantiate(nuestroShader, vec3(9.0f, 10.25f, 1.0f), vec3(0.5f, 2.0f, 5.0f));
//
//		glBindTexture(GL_TEXTURE_2D, texMetal);
//		Instantiate(nuestroShader, vec3(9.0f, 3.25f, -1.0f), vec3(0.2f, 3.0f, 0.2f));
//		Instantiate(nuestroShader, vec3(9.0f, 3.25f, -1.5f), vec3(0.2f, 3.0f, 0.2f));
//		Instantiate(nuestroShader, vec3(9.0f, 3.25f, -0.5f), vec3(0.2f, 3.0f, 0.2f));
//
//		glBindTexture(GL_TEXTURE_2D, texDoor);
//		Instantiate(nuestroShader, vec3(8.75f, 2.25f, 2.5f), vec3(0.1f, 2.0f, 3.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
//
//#pragma endregion
//
//#pragma region CANTINA
//		glBindTexture(GL_TEXTURE_2D, texWood4);
//		Instantiate(nuestroShader, vec3(-13.5f, 0.5f, 1.5f), vec3(13.0f, 0.5f, 15.0f)); //Piso
//
//		glBindTexture(GL_TEXTURE_2D, texWhiteWood);
//		Paredes
//		Instantiate(nuestroShader, vec3(-18.0f, 4.75f, 1.5f), vec3(0.5f, 8.0f, 13.5f));
//		Instantiate(nuestroShader, vec3(-8.5f, 4.75f, 5.75f), vec3(0.5f, 8.0f, 5.0f));
//		Instantiate(nuestroShader, vec3(-13.25f, 4.75f, 8.0f), vec3(9.0f, 8.0f, 0.5f));
//		Instantiate(nuestroShader, vec3(-13.25f, 4.75f, -5.0f), vec3(9.0f, 8.0f, 0.5f));
//		Instantiate(nuestroShader, vec3(-8.5f, 4.75f, -2.75f), vec3(0.5f, 8.0f, 5.0f));
//		Instantiate(nuestroShader, vec3(-8.5f, 6.75f, 1.5f), vec3(0.5f, 4.0f, 3.5f));
//		Instantiate(nuestroShader, vec3(-13.25f, 8.0f, 1.5f), vec3(9.0f, 0.5f, 12.5f)); //Techo
//
//		glBindTexture(GL_TEXTURE_2D, texWood2);
//		Instantiate(nuestroShader, vec3(-9.25f, 2.5f, 2.5f), vec3(0.1f, 2.0f, 2.0f), 35.0f, vec3(0.0f, 1.0f, 0.0f));
//		Instantiate(nuestroShader, vec3(-9.25f, 2.5f, 0.5f), vec3(0.1f, 2.0f, 2.0f), -35.0f, vec3(0.0f, 1.0f, 0.0f));
//
//		glBindTexture(GL_TEXTURE_2D, texWoodenWindow);
//		Instantiate(nuestroShader, vec3(-8.25f, 3.0f, 5.75f), vec3(0.1f, 2.5f, 2.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
//		Instantiate(nuestroShader, vec3(-8.25f, 3.0f, -2.75f), vec3(0.1f, 2.5f, 2.0f), 90.0f, vec3(1.0f, 0.0f, 0.0f));
//
//		glBindTexture(GL_TEXTURE_2D, texWood);
//		Instantiate(nuestroShader, vec3(-5.75f, 8.25f, 7.0f), vec3(5.0f, 0.5f, 0.5f));
//		Instantiate(nuestroShader, vec3(-7.0f, 7.5f, 7.0f), vec3(0.25f, 1.0f, 0.25f));
//		Instantiate(nuestroShader, vec3(-4.5f, 7.5f, 7.0f), vec3(0.25f, 1.0f, 0.25f));
//
//		glBindTexture(GL_TEXTURE_2D, texOldMetal);
//		Instantiate(nuestroShader, vec3(-5.75f, 6.5f, 7.0f), vec3(3.5f, 2.0f, 0.1f));
//
//#pragma endregion
//
//#pragma region LA CASONA
//		glBindTexture(GL_TEXTURE_2D, texWood4);
//		Instantiate(nuestroShader, vec3(0.0f, 0.5f, 20.0f), vec3(24.0f, 0.5f, 18.0f)); //Piso
//
//		glBindTexture(GL_TEXTURE_2D, texWood);
//		Instantiate(nuestroShader, vec3(0.0f, 16.0f, 22.0f), vec3(22.0f, 0.5f, 14.0f)); //TEcho
//		Instantiate(nuestroShader, vec3(0.0f, 17.75f, 22.0f), vec3(16.0f, 3.0f, 10.0f)); //Techo
//
//		Tablones
//		Instantiate(nuestroShader, vec3(0.0f, 9.0f, 13.75f), vec3(20.0f, 0.5f, 4.0f)); //Piso 2
//		Instantiate(nuestroShader, vec3(3.5f, 4.75f, 12.5f), vec3(0.5f, 8.0f, 0.5f));
//		Instantiate(nuestroShader, vec3(-3.5f, 4.75f, 12.5f), vec3(0.5f, 8.0f, 0.5f));
//		Instantiate(nuestroShader, vec3(9.5f, 4.75f, 12.5f), vec3(0.5f, 8.0f, 0.5f));
//		Instantiate(nuestroShader, vec3(-9.5f, 4.75f, 12.5f), vec3(0.5f, 8.0f, 0.5f));
//		Instantiate(nuestroShader, vec3(9.5f, 2.0f, 14.25f), vec3(0.5f, 0.5f, 3.0f));
//		Instantiate(nuestroShader, vec3(9.5f, 3.5f, 14.25f), vec3(0.5f, 0.5f, 3.0f));
//		Instantiate(nuestroShader, vec3(-9.5f, 2.0f, 14.25f), vec3(0.5f, 0.5f, 3.0f));
//		Instantiate(nuestroShader, vec3(-9.5f, 3.5f, 14.25f), vec3(0.5f, 0.5f, 3.0f));
//		Instantiate(nuestroShader, vec3(-2.0f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
//		Instantiate(nuestroShader, vec3(2.0f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
//		Instantiate(nuestroShader, vec3(6.0f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
//		Instantiate(nuestroShader, vec3(9.5f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
//		Instantiate(nuestroShader, vec3(-6.0f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
//		Instantiate(nuestroShader, vec3(-9.5f, 10.0f, 12.5f), vec3(0.25f, 1.5f, 0.25f));
//		Instantiate(nuestroShader, vec3(0.0f, 9.75f, 12.5f), vec3(20.0f, 0.25f, 0.2f));
//		Instantiate(nuestroShader, vec3(0.0f, 10.5f, 12.5f), vec3(20.0f, 0.25f, 0.2f));
//		Instantiate(nuestroShader, vec3(9.5f, 10.0f, 14.0f), vec3(0.25f, 1.5f, 0.25f));
//		Instantiate(nuestroShader, vec3(-9.5f, 10.0f, 14.0f), vec3(0.25f, 1.5f, 0.25f));
//		Instantiate(nuestroShader, vec3(9.5f, 10.5f, 14.0f), vec3(0.2f, 0.25f, 4.0f));
//		Instantiate(nuestroShader, vec3(9.5f, 9.75f, 14.0f), vec3(0.2f, 0.25f, 4.0f));
//		Instantiate(nuestroShader, vec3(-9.5f, 10.5f, 14.0f), vec3(0.2f, 0.25f, 4.0f));
//		Instantiate(nuestroShader, vec3(-9.5f, 9.75f, 14.0f), vec3(0.2f, 0.25f, 4.0f));
//
//		glBindTexture(GL_TEXTURE_2D, texWoodenWindow);
//		Ventanas
//		Instantiate(nuestroShader, vec3(6.0f, 3.5f, 15.75f), vec3(3.0f, 3.0f, 0.1f));
//		Instantiate(nuestroShader, vec3(-6.0f, 3.5f, 15.75f), vec3(3.0f, 3.0f, 0.1f));
//		Instantiate(nuestroShader, vec3(6.0f, 12.25f, 15.75f), vec3(3.0f, 3.0f, 0.1f));
//		Instantiate(nuestroShader, vec3(0.25f, 12.25f, 15.75f), vec3(3.0f, 3.0f, 0.1f));
//		Instantiate(nuestroShader, vec3(-6.0f, 12.25f, 15.75f), vec3(3.0f, 3.0f, 0.1f));
//
//		glBindTexture(GL_TEXTURE_2D, texMetal);
//		Instantiate(nuestroShader, vec3(0.0f, 20.75f, 17.5f), vec3(6.0f, 3.0f, 0.5f)); //Letrero
//
//		glBindTexture(GL_TEXTURE_2D, texWood2);
//		Instantiate(nuestroShader, vec3(1.25f, 3.25f, 16.5f), vec3(2.0f, 3.0f, 0.1f), 35.0f, vec3(0.0f, 1.0f, 0.0f));
//		Instantiate(nuestroShader, vec3(-1.25f, 3.25f, 16.5f), vec3(2.0f, 3.0f, 0.1f), -35.0f, vec3(0.0f, 1.0f, 0.0f));
//
//		glBindTexture(GL_TEXTURE_2D, texWood5);
//		Paredes
//		Instantiate(nuestroShader, vec3(6.0f, 8.25f, 16.0f), vec3(8.0f, 15.0f, 0.5f));
//		Instantiate(nuestroShader, vec3(0.0f, 10.75f, 16.0f), vec3(4.0f, 10.0f, 0.5f));
//		Instantiate(nuestroShader, vec3(-6.0f, 8.25f, 16.0f), vec3(8.0f, 15.0f, 0.5f));
//		Instantiate(nuestroShader, vec3(9.75f, 8.25f, 22.25f), vec3(0.5f, 15.0f, 12.0f));
//		Instantiate(nuestroShader, vec3(-9.75f, 8.25f, 22.25f), vec3(0.5f, 15.0f, 12.0f));
//		Instantiate(nuestroShader, vec3(-0.0f, 8.25f, 28.0f), vec3(19.0f, 15.0f, 0.5f));
//
//#pragma endregion
//
//#pragma region POZO
//		glBindTexture(GL_TEXTURE_2D, texStone2);
//		Instantiate(nuestroShader, vec3(1.0f, 1.25f, -6.75f), vec3(0.25f, 1.5f, 2.25f));
//		Instantiate(nuestroShader, vec3(-1.0f, 1.25f, -6.75f), vec3(0.25f, 1.5f, 2.25f));
//		Instantiate(nuestroShader, vec3(0.0f, 1.25f, -7.75f), vec3(1.75f, 1.5f, 0.25f));
//		Instantiate(nuestroShader, vec3(0.0f, 1.25f, -5.75f), vec3(1.75f, 1.5f, 0.25f));
//
//		Instantiate(nuestroShader, vec3(-0.75f, 4.25f, -6.75f), vec3(2.35f, 0.25f, 3.0f), 45.0f, vec3(0.0f, 0.0f, 1.0f));
//		Instantiate(nuestroShader, vec3(0.75f, 4.25f, -6.75f), vec3(2.0f, 0.25f, 3.0f), -45.0f, vec3(0.0f, 0.0f, 1.0f));
//
//		glBindTexture(GL_TEXTURE_2D, texWood);
//		Instantiate(nuestroShader, vec3(1.0f, 3.0f, -5.75f), vec3(0.25f, 2.0f, 0.25f));
//		Instantiate(nuestroShader, vec3(-1.0f, 3.0f, -5.75f), vec3(0.25f, 2.0f, 0.25f));
//		Instantiate(nuestroShader, vec3(-1.0f, 3.0f, -7.75f), vec3(0.25f, 2.0f, 0.25f));
//		Instantiate(nuestroShader, vec3(1.0f, 3.0f, -7.75f), vec3(0.25f, 2.0f, 0.25f));
//
//		Instantiate(nuestroShader, vec3(0.0f, 3.75f, -6.75f), vec3(2.5f, 0.25f, 0.25f));
//		Instantiate(nuestroShader, vec3(0.0f, 2.75f, -6.75f), vec3(0.1f, 2.0f, 0.1f));
//		Instantiate(nuestroShader, vec3(0.0f, 2.75f, -6.75f), vec3(0.1f, 2.0f, 0.1f));
//
//
//		glBindTexture(GL_TEXTURE_2D, texStone);
//		Instantiate(nuestroShader, vec3(0.0f, 0.5f, -6.75f), vec3(4.0f, 0.25f, 4.0f));
//
//#pragma endregion
//
//#pragma region POSTER WANTED
//		glBindTexture(GL_TEXTURE_2D, texWantedSign);
//		Instantiate(nuestroShader, vec3(-10.0f, 3.0f, -18.25f), vec3(0.1f, 1.0f, 2.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f)); //Lateral
//		Instantiate(nuestroShader, vec3(3.0f, 3.0f, -22.25f), vec3(1.0f, 2.0f, 0.1f), 180.0f, vec3(0.0f, 0.0f, 1.0f)); //Frente
//		Instantiate(nuestroShader, vec3(10.75f, 3.0f, -19.0f), vec3(1.0f, 2.0f, 0.1f), 180.0f, vec3(0.0f, 0.0f, 1.0f)); //Frente
//		Instantiate(nuestroShader, vec3(8.75f, 3.0f, -2.5f), vec3(0.1f, 1.0f, 2.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f)); //Lateral
//		Instantiate(nuestroShader, vec3(-3.5f, 3.0f, 12.25f), vec3(1.0f, 2.0f, 0.1f), 180.0f, vec3(0.0f, 0.0f, 1.0f)); //Frente
//		Instantiate(nuestroShader, vec3(-8.25f, 3.0f, -1.0f), vec3(0.1f, 1.0f, 2.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f)); //Lateral
//#pragma endregion

		//renderizar luz
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 50, GL_UNSIGNED_INT, 0);

		//Dibujamos el skybox
		// Función de profundidad encargada del cálculo entre el usuario y la profundidad del skybox para generación de infinidad
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		view = mat4(mat3(camera.GetViewMatrix()));
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);

		glBindVertexArray(sVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		//detecte eventos de IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteVertexArrays(1, &sVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &sVBO);
	glfwTerminate();
	return 0;
}

//Instancía un cubo, sin rotación.
void Instantiate(Shader& _shader, vec3 _position)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, cubeVertexNum, GL_UNSIGNED_INT, 0);
}

//Instanciar con una escala
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = scale(model, _scale);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, cubeVertexNum, GL_UNSIGNED_INT, 0);
}

//Instancia un cubo, con la rotación indicada 
void Instantiate(Shader& _shader, vec3 _position, float _angle, vec3 _axis)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = rotate(model, radians(_angle), _axis);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, cubeVertexNum, GL_UNSIGNED_INT, 0);
}

//Instancia un cubo, con escala y rotación indicadas 
void Instantiate(Shader& _shader, vec3 _position, vec3 _scale, float _angle, vec3 _axis)
{
	mat4 model = mat4(1.0f);
	model = translate(model, _position);
	model = rotate(model, radians(_angle), _axis);
	model = scale(model, _scale);
	_shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, cubeVertexNum, GL_UNSIGNED_INT, 0);
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

unsigned int LoadCubeMap(vector<string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			cout << "fallo en cargar las texturas de nuestro skybox, la textura que fallo es: " << faces[i] << endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
