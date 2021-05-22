
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <Shader/shader.h> //inicializa el fragment y 
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);// almacenar nuestras configuracioines y ventanas
void processInput(GLFWwindow* window); // acciones para la ventana

//medias de pantalla
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
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
	///primero lee el vs y depues fv


//comprobar que glad se este corriendo o se haya inicializado correctamente
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "no se esta ejecutando el alegre" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader nuestroShader("texturas.vs", "texturas.fs");
	Shader segundoShader("vertex.vs", "vertex.fs");

	float vertex2[] =
	{
			0.0f,   0.3f,   0.0f, 0.2f, 0.0f, 1.0f, //0
			-0.3f,  -0.2f,   0.0f, 0.9f, 0.0f, 1.0f, //1
			-0.2f,  -0.2f,   0.0f, 0.7f, 0.0f, 1.0f, //2
			-0.1f,  -0.1f,   0.0f, 0.0f, 0.0f, 1.0f, //3
			-0.05f,  0.0f,   0.0f, 0.08f, 0.0f, 1.0f,//4
			0.0f,   0.1f,   0.0f, 0.3f, 0.0f, 1.0f, //5
			0.05f,  0.0f,   0.0f, 0.1f, 0.0f, 1.0f, //6
			0.1f,  -0.1f,   0.0f, 0.1f, 0.0f, 1.0f, //7
			0.2f,  -0.2f,   0.0f, 0.9f, 0.0f, 1.0f, //8
			0.3f,  -0.2f,   0.0f, 0.03f, 0.0f, 1.0f,//9
	};

	//Deckaramos los vertex
	float vertex[]//------------------------------segunda clase
	{	//posciosiones	      //colores          //texturas  s,t            
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left

#pragma region Letras
	///letra a 
	 /*0.0f,   0.3f,   0.0f, 0.2f, 0.0f, 1.0f, //0
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
	};
	unsigned int index[]  //unsigned que no haya negativos
	{
		//0, 3, 2, //primer triangulo
		//0, 2, 1 //segundo triangulo
		//0, 1, 3,
		//1, 2, 3

	#pragma region Letras
		//Letra A
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
		34,33,32
	#pragma endregion

	};
	unsigned int index2[]
	{
		0,1,2
	};

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);//posciniones 0


	///////---------------------------segundo vertex---------------------
	unsigned int VBO2, VAO2, EBO2; //Vertex Buffer Object, Vertex Array Obj, Extendet Array Obj
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	//Unir o linkear
	glBindVertexArray(VAO2);//dibujo

	glBindBuffer(GL_ARRAY_BUFFER, VBO2); //colores
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex2), vertex2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2); //colores
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index2), index2, GL_STATIC_DRAW);

	//poscisiones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);//posciniones 0

	glBindVertexArray(VAO2);//dibujo //dso veces


	//crear y cargar la textura------------------------------------------------
	unsigned int textura1, textura2;
	//primera textura
	//crear, cargar y liberrar
	glGenTextures(1, &textura1);
	glBindTexture(GL_TEXTURE_2D, textura1);  //tipo de textura
	//configuramos parametros de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Coordenad S
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //Coordenada T
	//configuramos el filtrado de la textura en caso de que se expanda
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //
	//cargar la imagen crea	la textura y generar los mipmaps
	int width, heigth, nrChannels;
	stbi_set_flip_vertically_on_load(true);	//si no hacemos esto nos dan las texturas rotadas
	//cargar nuestro textura
	unsigned char* data = stbi_load("cosa2.jpg", &width, &heigth, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Fallaste en cargar la primera textura perra" << std::endl;
	}
	stbi_image_free(data);


	//segunda textura-------------------
	glGenTextures(1, &textura2);
	glBindTexture(GL_TEXTURE_2D, textura2);  //tipo de textura
	//configuramos parametros de la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Coordenad S
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //Coordenada T
	//configuramos el filtrado de la textura en caso de que se expanda
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //
	//cargar la imagen crea	la textura y generar los mipmaps
	//cargar nuestro textura
	data = stbi_load("cosa1.png", &width, &heigth, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); //RGBA por ser png
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Fallaste en cargar la segunda textura perra" << std::endl;
	}
	stbi_image_free(data);

	//linkeo
	nuestroShader.use();
	nuestroShader.setInt("textura1", 0);
	nuestroShader.setInt("textura2", 1);


	// loop para qeu se pueda visualizar la pantalla 
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//render
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f); //color de fondo 
		glClear(GL_COLOR_BUFFER_BIT);

		//Activado y union de las texturas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textura1);
		//la segunda textura
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textura2);

		nuestroShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);

		//glDrawArrays(GL_TRIANGLES_FAN, 0, 6); //----------------------------tipo de dibujado 
						//GL_TRIANGLES, 100 es la cantidad que puede recibir de vertex
		glDrawElements(GL_TRIANGLES, 100, GL_UNSIGNED_INT, 0);

		//detectar eventos IO
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//Liberar memorias 
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	//glDeleteProgram(nuestroShader);

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
