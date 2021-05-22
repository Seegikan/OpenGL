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
	};

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
	};

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
		/*//Primer lado, frentre
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
	};
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
		
	};

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

		nuestroShader.use();

		//MVP
		/*mat4 view = mat4(1.0f);
		mat4 projection = mat4(1.0f);
		projection = perspective(radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f ); //SE maneja en radianes para mayor exactitud
		view = translate(view, vec3(0.0f, 0.0f, -3.0f));*/

		mat4 projection = perspective(radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 250.0f); //camera
		mat4 view = camera.GetViewMatrix();
		

		nuestroShader.setMat4("projection", projection);
		nuestroShader.setMat4("view", view);

		/*unsigned int transformLoc = glGetUniformLocation(nuestroShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, value_ptr(transform));*/

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