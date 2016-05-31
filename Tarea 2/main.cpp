#pragma once
#include<iostream>
#include<vector>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;

string loadShaderCode(const char* filename)
{
	string shaderCode;
	ifstream file(filename, std::ios::in);

	if (!file.good())
	{
		std::cout << "Can't read file" << filename << std::endl;
		exit(1);
	}

	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());

	file.seekg(0, std::ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();

	return shaderCode;
}

void compileChecking(GLuint shader)
{
	GLint Compilado = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &Compilado); //Objeto shader que quiero, parametro que solicito, apuntador al parametro solicitado
	if (Compilado == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		//maxLength incluye NULL
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		std::cout << errorLog.data() << std::endl;

		fflush(stdin);
		std::cin.get();

		// Provee la información de infolog que considere mejor.
		// Salir si hay error
		glDeleteShader(shader); // ¿Qué no escape del shader?

		exit(1);
	}
}

void linkChecking(GLuint sProgram)
{
	GLint Enlazado = 0;
	glGetProgramiv(sProgram, GL_LINK_STATUS, &Enlazado);
	if (Enlazado == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(sProgram, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(sProgram, maxLength, &maxLength, &errorLog[0]);

		std::cout << errorLog.data() << std::endl;

		fflush(stdin);
		std::cin.get();

		glDeleteShader(sProgram);

		exit(1);
	}
}

int main()
{
	//1. Inicializar GLFW
	glfwInit();
	//2.1 Configurar la versión de OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//2.2 Configurar el perfil de OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//3.1 Crear la ventana y contexto para la aplicación
	GLFWwindow* window = glfwCreateWindow(512, 512, "OpenGL App", NULL, NULL); //Wiegthpx, Heigthpx, Title, FULL mode, 1 window
																			   //3.1.1 Prueba de creación exitosa de la ventana
	if (window == NULL)
	{
		std::cerr << "Error creando la ventana de GLFW";
		return 1;
	}
	//3.2 Recibe un apuntador al obj. ventana que acaba de crear y le indica el contexto que va a utilizar
	glfwMakeContextCurrent(window);
	//4. Inicializar GLEW
	//Nota lo que no notó glfw con true
	glewExperimental = GL_TRUE;
	//Inicializa GLEW
	glewInit();
	//5. Definir el ViewPort
	glViewport(0, 0, 512, 512); //Coordenadas de la esquina sup. izq, ancho y alto
	//a. Obtener el código fuente del shader de vértices
	std::string vShaderCodeStr = loadShaderCode("vertex_shader.glsl");
	const GLchar* vShaderCode = vShaderCodeStr.c_str();
	//b. Crear una variable para la referencia al objeto shader
	GLuint vShaderObject = glCreateShader(GL_VERTEX_SHADER);
	//c. Almacenar el código fuente en el objeto shader
	glShaderSource(vShaderObject, 1, &vShaderCode, NULL);
	//d. Compilar el objeto shader
	glCompileShader(vShaderObject);
	//¿Compilación exitosa?
	compileChecking(vShaderObject);
	//e. Obtener el código fuente del shader de fragmentos
	std::string fShaderCodeStr = loadShaderCode("fragment_shader.glsl");
	const GLchar* fShaderCode = fShaderCodeStr.c_str();
	//f. Crear una variable para la referencia al objeto shader
	GLuint fShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	//g. Almacenar el código fuente en el objeto shader
	glShaderSource(fShaderObject, 1, &fShaderCode, NULL);
	//h. Compilar el objeto shader
	glCompileShader(fShaderObject);
	//¿Compilación exitosa?
	compileChecking(fShaderObject);
	//i. Crear una variable para la referencia del programa de shaders
	GLuint shaderProgram = glCreateProgram();
	//j. Asociar los objetos shader al programa de shaders
	glAttachShader(shaderProgram, vShaderObject);
	glAttachShader(shaderProgram, fShaderObject);
	//El enlazado que no se olvide
	glLinkProgram(shaderProgram);
	//¿Enlazado correcto?
	linkChecking(shaderProgram);
	//i. Especificar los datos
	GLfloat vertices[] =
	{
		//Primer triangulo antihorario
		//Posición			//Color
		//-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,
		//-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,
		//0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
		//Dos triangulos horario
		//-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 0.0f,
		//0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,
		//0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f

		//Triagulos contiguos
		0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f, 1.0f,
		0.25f, 0.35f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f,
		//Dos triangulos horario
		0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,
		-0.25f, 0.35f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f

	};
	//Para el vertex array object, almacena el estado de los datos
	GLuint vao;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	//ii. Solicitar un nombre para el buffer y almacenarlo
	//en una variable de tipo GLuint
	GLuint vbo;
	glGenBuffers(1, &vbo);
	//iii. Asignar el buffer al punto de asignación GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//iv. Cargar los datos en el buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//Cambia el color de la ventana
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//6. Definir el gameloop
	while (!glfwWindowShouldClose(window))
	{
		//Evita que se ponga en estado de no responde
		glfwPollEvents();
		//Para que le haga caso al ClearColor hay que decirle a OpenGL
		glClear(GL_COLOR_BUFFER_BIT);
		//Para el shader
		glUseProgram(shaderProgram);
		//7. Realizar el renderizado deseado
		//glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 6); //Inicia desde el inicio y dibuja 3 vertices o 6 segín los triangulos

		glBindVertexArray(0);

		//8. Intercambiar los buffers frontal y posterior
		glfwSwapBuffers(window);
	}
	//9. Liberar recursos y terminar el GLFW
	glDeleteVertexArrays(1, &vao);//No de objetos, dirección del objeto a liberar
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return 0;
}