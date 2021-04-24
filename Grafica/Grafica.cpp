#include <glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);// almacenar nuestras configuracioines y ventanas
void processInput(GLFWwindow* window); // acciones para la ventana

//medias de pantalla
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* VertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos; \n" //poscisiones
"layout (location = 1) in vec3 aColor; \n" //colores  //nueva
"out vec3 ourColor; \n" //unifircar, buffer y bincular
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n" //modificada
"ourColor = aColor; \n"
"}\0";

const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n" "{\n"
" FragColor = vec4(ourColor, 1.0f);\n" "}\n\0"
"}\n\0";

//"out vec4 FragColor; \n"
/*"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.4f, 0.5f, 1.0f);\n" //color temporal 
"}\n\0";*/

int main()
{
	//incia glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//creamos nuestra ventana
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Te amo Alam bb", NULL, NULL);
	if (window == NULL)
	{
		std::cout << " Fallo en cargat GLFW y la ventana date un plomaso loco " << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//comprovamos que glad se este ejecutando
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << " No se ejecuto el alegre " << std::endl;
		return -1;
	}

	//constrit y compilar todo nuestro programa
	//vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//checar si hay errores
	int success;
	char infoLong[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLong);
		std::cout << " error al compilar vertex\n " << infoLong << std::endl;
	}

	//fragment
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLong);
		std::cout << " error al compilar fragment\n " << infoLong << std::endl;
	}

	//linkeamos todo nuestro vertex y fragment a nuestro programa
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// comprobar los linkeos
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLong);
		std::cout << " error en los linkeos del programa\n " << infoLong << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Deckaramos los vertex
	float vertex[]//------------------------------segunda clase
	{

	#pragma region Pato

			0.3f,0.0f, 0.0f,   0.0f, 0.0f, 0.0f,  //0   
			0.1f,0.2f, 0.0f,   0.0f, 0.0f, 0.0f,  //1   
			0.1f,0.4f, 0.0f,   0.0f, 0.0f, 0.0f,  //2   
			0.4f,0.4f, 0.0f,   0.0f, 0.0f, 0.0f,  //3   
			0.2f,0.6f, 0.0f,   0.0f, 0.0f, 0.0f,  //4   
			0.0f,0.6f, 0.0f,   0.0f, 0.0f, 0.0f,  //5   
			-0.1f,0.4f, 0.0f,   0.0f, 0.0f, 0.0f,  //6   
			-0.1f,0.2f, 0.0f,   0.0f, 0.0f, 0.0f,  //7   
			-0.4f,-0.2f, 0.0f,   0.0f, 0.0f, 0.0f,  //8   
			-0.6f,-0.2f, 0.0f,   0.0f, 0.0f, 0.0f,  //9
			-0.3f,-0.5f, 0.0f,   0.0f, 0.0f, 0.0f,  //10   
			0.1f,-0.5f, 0.0f,   0.0f, 0.0f, 0.0f,  //11   
			0.3f,-0.3f, 0.0f,   0.0f, 0.0f, 0.0f,  //12  



	#pragma endregion
	
	#pragma region Letras
		///letra a 
		/* 0.0f,   0.3f,     0.0f, 0.2f, 0.0f, 1.0f //0
		-0.3f,  -0.2f,   0.0f, 0.9f, 0.0f, 1.0f, //1
		-0.2f,  -0.2f,   0.0f, 0.7f, 0.0f, 1.0f, //2
		-0.1f,  -0.1f,   0.0f, 0.0f, 0.0f, 1.0f, //3
		-0.05f,  0.0f,   0.0f, 0.08f, 0.0f, 1.0f,//4
		 0.0f,   0.1f,   0.0f, 0.3f, 0.0f, 1.0f, //5
		 0.05f,  0.0f,   0.0f, 0.1f, 0.0f, 1.0f, //6
		 0.1f,  -0.1f,   0.0f, 0.1f, 0.0f, 1.0f, //7
		 0.2f,  -0.2f,   0.0f, 0.9f, 0.0f, 1.0f, //8
		 0.3f,  -0.2f,   0.0f, 0.03f, 0.0f, 1.0f,//9

		 //Letra S
		 -0.4f,  0.3f,   0.0f, 0.2f, 0.0f, 1.0f, //10
		 -0.8f,  0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //11
		 -0.8f,  0.1f,   0.0f, 0.2f, 0.0f, 1.0f, //12
		 -0.4f,  0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //13
		 -0.4f,  0.1f,   0.0f, 0.2f, 0.0f, 1.0f, //14
		 -0.4f,  0.0f,   0.0f, 0.2f, 0.0f, 1.0f, //15
		 -0.8f, -0.1f,   0.0f, 0.2f, 0.0f, 1.0f, //16
		 -0.8f, -0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //17
		 -0.4f, -0.1f,   0.0f, 0.2f, 0.0f, 1.0f, //18

		 // letra I
		 0.6f,  0.3f,   0.0f, 0.2f, 0.0f, 1.0f, //19
		 0.4f,  0.3f,   0.0f, 0.2f, 0.0f, 1.0f, //20
		 0.4f, -0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //21
		 0.6f, -0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //22

		 //Letra R
		 0.8f,  0.3f,   0.0f, 0.2f, 0.0f, 1.0f, //23 1
		 0.7f,  0.3f,   0.0f, 0.2f, 0.0f, 1.0f, //24
		 0.7f, -0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //25
		 0.8f, -0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //26
		 0.8f, -0.1f,   0.0f, 0.2f, 0.0f, 1.0f, //27 5
		 0.8f,  0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //28
		 1.0f,  0.3f,   0.0f, 0.2f, 0.0f, 1.0f, //29
		 1.0f,  0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //30
		 0.9f,  0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //31
		 0.9f,  0.1f,   0.0f, 0.2f, 0.0f, 1.0f, //32 10
		 1.0f, -0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //33
		 0.9f, -0.2f,   0.0f, 0.2f, 0.0f, 1.0f, //34*/
	#pragma endregion

	#pragma region Examen
		/*
	//posciosiones	x y z      //colores    R G B                  
			0.2f,    0.8f, 0.0f,   0.0f, 0.0f, 0.0f,  //0   T
			0.0f,    0.7f, 0.0f,   0.7f, 1.0f, 0.0f,  //1   T
			0.0f,    0.5f, 0.0f,   0.7f, 1.0f, 0.0f,  //2	T
		    -0.3f,   0.4f, 0.0f,   0.7f, 1.0f, 0.0f,  //3	T
			-0.2f,    0.3f, 0.0f,  0.7f, 1.0f, 0.0f, //4	T
		    -0.5f,   0.2f, 0.0f,   0.7f, 1.0f, 0.0f,  //5	T
			-0.6f,  -0.1f, 0.0f,   1.0f, 0.0f, 0.0f,  //6	B
			-0.4f,  -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  //7	B
			-0.1f,  -0.7f, 0.0f,   1.0f, 0.0f, 0.0f,  //8	B
			0.2f,  -1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   //9	B
			0.6f,  -0.6f, 0.0f,    1.0f, 0.0f, 0.0f,   //10	B
			0.8f,   -0.1f, 0.0f,   0.0f, 0.0f, 0.0f,  //11	B
			0.7f,   0.3f, 0.0f,    1.0f, 0.0f, 0.0f,   //12	
			0.4f,   0.5f, 0.0f,    1.0f, 1.0f, 0.0f,   //13	
			0.1f,   0.5f, 0.0f,    1.0f, 1.0f, 0.0f,   //14
			0.1f,   0.3f, 0.0f,    0.0f, 1.0f, 1.0f,   //15  T 
			0.0f,   0.3f, 0.0f,    1.0f, 1.0f, 0.0f,   //16	I
			0.0f,   0.2f, 0.0f,    0.0f, 0.0f, 0.0f,   //17	I
			0.2f,   0.3f, 0.0f,    1.0f, 1.0f, 0.0f,   //18	D
			-0.2f,  -0.2f, 0.0f,   0.0f, 0.0f, 0.0f,  //19	D
			0.2f,   0.1f, 0.0f,    0.0f, 0.0f, 0.0f,   //20	
			0.2f,   0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   //21
			0.2f,   -0.2f, 0.0f,   1.0f, 0.0f, 0.0f,  //22
			0.2f,   -0.7f, 0.0f,   1.0f, 0.0f, 0.0f,  //23
			*/
	#pragma endregion

	#pragma region Hexagono
		/*
	//    x     y
		0.0f, 1.0f, 0.0f,	  0.0f, 0.2f, 0.0f,//0
		-0.5f, 0.5f, 0.0f,	  1.0f, 0.2f, 0.0f, //1
		-0.5f, -0.5f, 0.0f,   0.0f, 0.2f, 0.0f, //2
		0.0f, -1.0f, 0.0f,	  0.0f, 0.2f, 0.0f, //3
		0.5f, -0.5f, 0.0f,	  1.0f, 0.2f, 0.0f, //4
		0.5f, 0.5f, 0.0f,	  0.0f, 0.2f, 0.0f, //5
		0.0f, 0.0f, 0.0f,	  0.0f, 0.2f, 0.0f//6
		*/
	#pragma endregion 

	#pragma region F
		/*
		-0.3f,   0.3f, 0.0f,    1.0f, 1.0f, 0.0f,  //0
		-0.3f,   0.1f, 0.0f,    1.0f, .0f, 0.4f,  //1	
		-0.3f,  -0.4f, 0.0f,    0.0f, 0.0f, 0.0f,  //2	
		-0.1f,  -0.4f, 0.0f,    1.0f, .0f, 0.4f,  //3	
		-0.1f,  -0.1f, 0.0f,    1.0f, 1.0f, 0.4f,  //4	
		-0.0f,  -0.1f, 0.0f,    1.0f, .0f, 0.4f,  //5
		-0.0f,  -0.0f, 0.0f,    0.0f, 0.0f, 0.0f,  //6
		-0.1f, -0.0f, 0.0f,     1.0f, .0f, 0.4f,  //7
		-0.1f,  0.1f, 0.0f, 	1.0f, 1.0f, 0.4f,  //8	
		0.3f, 0.1f, 0.0f,		1.0f, .0f, 0.0f,  //9	
		0.3f, 0.3f, 0.0f,		1.0f, 1.0f, 0.4f,  //10	
			*/
	#pragma endregion 

	#pragma region Castillo
		/*
		- 0.9f, 0.9f, 0.0f,		 1.0f, 0.8f, 0.0f,  //0
		-0.9f, -0.9f, 0.0f,		 1.0f, 0.0f, 0.0f, //1
		0.9f, -0.9f, 0.0f,		 1.0f, 0.8f, 0.0f,   //2
		0.9f, 0.9f, 0.0f,		1.0f, 0.0f, 0.0f,  //3
		0.5f, 0.9f, 0.0f,		1.0f, 0.8f, 0.0f,  //4
		0.5f, 0.6f, 0.0f,		 1.0f, 0.0f, 0.0f,  //5
		0.3f, 0.6f, 0.0f,		 1.0f, 0.8f, 0.0f,  //6
		0.3f, 0.9f, 0.0f,		1.0f, 0.0f, 0.0f,  //7
		-0.3f, 0.9f, 0.0f,		1.0f, 0.8f, 0.0f,  //8
		-0.3f, 0.6f, 0.0f,		 1.0f, 0.0f, 0.0f,  //9
		-0.5f, 0.6f, 0.0f,		1.0f, 0.8f, 0.0f,  //10
		-0.5f, 0.9f, 0.0f,		1.0f, 0.0f, 0.0f,  //11

		-0.9f, 0.6f, 0.0f,		1.0f, 0.0f, 0.0f,//12----
		0.9f, 0.6f, 0.0f,     1.0f, 0.0f, 0.0f, //13----

		- 0.3f, 0.5f, 0.0f,		 1.0f, 0.3f, 0.2f, //0		14 ///ventanas
		-0.7f, 0.5f, 0.0f,		1.0f, 0.0f, 1.0f, //1		15
		-0.7f, 0.0f, 0.0f,		1.0f, 0.3f, 0.2f,  //2		16
		-0.3f, 0.0f, 0.0f,		1.0f, 0.0f, 1.0f, //3		17

		-0.3f, -0.2f, 0.0f,		1.0f, 0.3f, 0.2f, //4
		-0.3f, -0.9f, 0.0f,		1.0f, 0.0f, 1.0f,  //5
		0.3f, -0.9f, 0.0f,		1.0f, 0.3f, 0.2f,  //6
		0.3f, -0.2f, 0.0f,		 1.0f, 0.0f, 1.0f,  //7

		0.7f, 0.5f, 0.0f,		1.0f, 0.3f, 0.2f,  //8
		0.3f, 0.5f, 0.0f,		1.0f, 0.0f, 1.0f,  //9
		0.3f, 0.0f, 0.0f,		1.0f, 0.3f, 0.2f,  //10
		0.7f, 0.0f, 0.0f,		 1.0f, 0.0f, 1.0f  //11
		*/
	#pragma endregion 
	};
	unsigned int index[]  //unsigned que no haya negativos //forma en que se hacen
	{
	#pragma region Pato

		0,1,2,3,4,5,6,7,8,9,10,11,12

	#pragma endregion
	#pragma region Examen
	/*
		0,1,2,
		2,14,0,
		2,13,15,//tallo

		2,3,4,
		2,4,15,
		4,5,16,//hoja izq

		13,15,12, //DER

		16,5,19,
		16,19,18,
		18,19,21,//a

		5,6,19,
		6,7,19, //B

		19,7,8, //c

		21,19,8,
		21,8,9, //D

		//21,8,9,
		23,9,10, //g

		11,22,23,
		11,23,10, //f

		12,18,22,
		12,22,11, //e

		16,17,15, //hojita izq

		15,20,18, //hojita Der
		*/
   #pragma endregion

	#pragma region Letras
		/*//Letra A
		0,1,2,	
		0,2,5,
		4,3,7,
		6,4,7,
		0,8,9,
		0,5,8,

		//Letra S
		10,11,12,
		12,13,10,
		12,15,14,
		15,16,17,
		17,18,15,

		//Letra I
		19,20,21,
		21,22,19,

		//Letra R
		23,24,25,
		25,26,23,
		29,23,28,
		28,30,29,
		30,31,27,
		32,27,34,
		34,33,32*/
	#pragma endregion

	#pragma region Hexagono
		/*
		0,1,6,
		1,2,6,
		1,2,6,
		2,3,6,
		3,4,6,
		4,5,6,
		5,0,6
		*/
	#pragma endregion

	#pragma region F
		/*
		10,0,1,
		10,1,9,
		8,1,2,
		8,2,3,
		6,7,4
		*/
	#pragma endregion 
	
	#pragma region Castillo
		/*
		11,0,12,   //0
		12,10,11,  //1
		13,12,1,   //2
		1,2,13,    //3
		3,4,5,     //4
		5,13,3,    //5
		7,8,9,     //6
		9,6,7,     //7

		14,15,16,
		16,17,14,
		18,19,20,
		20,21,18,
		22,23,24,
		24,25,22
			*/
	#pragma endregion 

	};
	//sinusoidal

	unsigned int VBO, VAO, EBO; //Vertex Buffer Object, Vertex Array Obj, Extendet Array Obj
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Unir o linkear
	glBindVertexArray(VAO);//dibujo

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //colores
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //colores
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	//poscisiones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);//posciniones 0
	//colores
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);//colores 1

	//Vinculamo snuestras caracteristica 
	///glBindBuffer(GL_ARRAY_BUFFER, 0);
	//indicamos que son posiciones 
	///glBindVertexArray(0);

	//glBindVertexArray(VAO); //tipo de dibujado //nueva posicion
	glUseProgram(shaderProgram);

		// loop para qeu se pueda visualizar la pantalla 
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//render
		glClearColor(0.7f, 0.3f, 1.0f, 1.0f); //color de fondo 
		glClear(GL_COLOR_BUFFER_BIT);

		//dibujamos nuestro primer cuadrado
		//glUseProgram(shaderProgram); //shaderProgram guarda los atributos 
		/*
		float timeValeu = glfwGetTime();
		float senalVerde = sin(timeValeu) / 2.0f + 0.5f; //timepo de cambio
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, senalVerde, 0.0f, 1.0f);
		*/
		glBindVertexArray(VAO);

		//glDrawArrays(GL_TRIANGLES_FAN, 0, 6); //----------------------------tipo de dibujado 
						//GL_TRIANGLES, 100 es la cantidad que puede recibir de vertex
		glDrawElements(GL_LINE_STRIP, 100, GL_UNSIGNED_INT, 0);//------------------------------------------------cambio de dibujado

		//detectar eventos IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//Liberar memorias 
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// configuracionde la camara
	glViewport(0, 0, width, height);
}

/*#include <glad/glad.h> //se inicializa solito 
#include <GLFW/glfw3.h>
#include <iostream>			



//Buffer congunto de datos
void FrameBuffer_Size_CallBack(GLFWwindow* window, int wight, int height); //Donde se tamaño de ventana
void ProcessInput(GLFWwindow* window); //Acciones para ventana 

//medidas de ventana 
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* VertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos; \n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor; \n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.3f, 0.5f, 1.0f);\n"
"}\n\0";

int main()
{
	//Inicilizar glfw

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //compatibilidad del programa y version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //compatibilidad del programa y version
	//inicializar, comprobar, liberar 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//cracion de ventana

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Alex", NULL, NULL); //null en ultimas ya que no usaremos imagenes compartidas
	if (window == NULL) //comprobacion de la crecion de ventana 
	{
		std::cout << "Fallo en crear GLFW y la ventan a date un balazo" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBuffer_Size_CallBack);

	//comprobar que glad se este corriendo hay ainicializa correctamente
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Fallo en crear GLFW y la ventan a date un balazo" << std::endl;
		return -1;
	}
	//Contruir y compilar todo nuestro programa
	//Vertex y shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &VertexShaderSource, NULL);
	glCompileShader(fragmentShader);

	//checar si hay errores en vertex (comprobacion)

	int sucess;
	char infLog[512];
	glGetShaderiv(fragmentShader, GL_COMPARE_REF_TO_TEXTURE, &sucess);
	if (!sucess)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infLog);
		std::cout << "Erro al compilar nuestro fragment" << infLog << std::endl;
	}
	//Fragment Shader
	unsigned int fragment;

	//Loop para que se pueda visalizar nuestra pantalla 
	while (!glfwWindowShouldClose)
	{
		ProcessInput(window);

		//renderizado //MyGlSurfaceView //Color de fondo
		glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Deteccion de eventos de IO
		glfwSwapBuffers(window);
		glfwPollEvents();


	}

	//Liberar nuestras memorias

	glfwTerminate();
	return 0;

}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);
		glfwSetWindowShouldClose(window, true);
}

void FrameBuffer_Size_CallBack(GLFWwindow* window, int wight, int height)
{
	//Nuestra primera camara, configuracion 
	glViewport(0, 0, wight, height); //punto de camara 
}*/