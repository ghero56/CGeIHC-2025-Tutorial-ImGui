// Author: Fernando Arciga Guzmán
// Base: práctica 3: Modelado Geométrico y Cámara Sintética.
// Last Update: 10/11/2024
// Modifications:   Incluye ImGui y recrea la casa del ejercicio de la práctica pero con selectores para cada objeto en pantalla.
//					Incluye la demo/tutorial para la creación de nuevos componentes de UI con ImGui.

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include "Mesh.h"
#include "Shader.h"
#include "Sphere.h"
#include "Window.h"
#include "Camera.h"

// dearImGUI headers para la UI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks

// variables para la UI
// cubo rojo
glm::vec3 redCubePos = glm::vec3(0.0f, 0.0f, -4.0f);
glm::vec3 redCubeRotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 redCubeScale = glm::vec3(1.4f, 1.4f, 1.4f);
glm::vec3 redCubeColor = glm::vec3(1.0f, 0.0f, 0.0f);

// techo azul
glm::vec3 blueRoofPos = glm::vec3(0.0f, 1.2f, -4.0f);
glm::vec3 blueRoofRotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 blueRoofScale = glm::vec3(1.7f, 1.0f, 1.7f);
glm::vec3 blueRoofColor = glm::vec3(0.0f, 0.0f, 1.0f);

// árboles
glm::vec3 treeColor = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 tree1Pos = glm::vec3(1.0f, 0.0f, -4.0f);
glm::vec3 tree1Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 tree1Scale = glm::vec3(0.5f, 0.7f, 0.5f);

glm::vec3 tree2Pos = glm::vec3(-1.0f, 0.0f, -4.0f);
glm::vec3 tree2Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 tree2Scale = glm::vec3(0.5f, 0.7f, 0.5f);

// troncos
glm::vec3 logColor = glm::vec3(0.5f, 0.35f, 0.05f);

glm::vec3 log1Pos = glm::vec3(1.0f, -0.5f, -4.0f);
glm::vec3 log1Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 log1Scale = glm::vec3(0.20f, 0.4f, 0.20f);

glm::vec3 log2Pos = glm::vec3(-1.0f, -0.5f, -4.0f);
glm::vec3 log2Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 log2Scale = glm::vec3(0.20f, 0.4f, 0.20f);

// ventanas
glm::vec3 windowColor = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 window1Pos = glm::vec3(0.3f, 0.3f, 0.01f);
glm::vec3 window1Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 window1Scale = glm::vec3(0.20f, 0.20f, 0.20f);

glm::vec3 window2Pos = glm::vec3(-0.3f, 0.3f, 0.01f);
glm::vec3 window2Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 window2Scale = glm::vec3(0.20f, 0.20f, 0.20f);

// puerta
glm::vec3 doorColor = glm::vec3(0.0f, 1.0f , 0.0f);

glm::vec3 doorPos = glm::vec3(0.0f, -0.5f, 0.01f);
glm::vec3 doorRotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 doorScale = glm::vec3(0.30f, 0.50f, 0.30f);

// esfera azul
glm::vec3 blueSphereColor = glm::vec3(0.0f, 0.0f, 1.0f);

glm::vec3 blueSpherePos = glm::vec3(0.0f, 0.0f, 0.6f);
glm::vec3 blueSphereRotation = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 blueSphereScale = glm::vec3(0.25f, 0.25f, 0.25f);

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

// Pirámide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}
/*
Crear cilindro, cono y esferas con arreglos dinámicos vector creados en el Semestre 2023 - 1 : por Sánchez Pérez Omar Alejandro
*/
void CrearCilindro(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			case 3:
				vertices.push_back(x);
				break;
			case 4:
				vertices.push_back(0.5);
				break;
			case 5:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(-0.5f);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(0.5);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}

	//Se generan los indices de los vértices
	for (i = 0; i < vertices.size(); i++) indices.push_back(i);

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMeshGeometry(vertices, indices, vertices.size(), indices.size());
	meshList.push_back(cilindro);
}

//función para crear un cono
void CrearCono(int res, float R) {

	//constantes utilizadas en los ciclos for
	int n, i;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;

	vector<GLfloat> vertices;
	vector<unsigned int> indices;

	//caso inicial para crear el cono
	vertices.push_back(0.0);
	vertices.push_back(0.5);
	vertices.push_back(0.0);

	//ciclo for para crear los vértices de la circunferencia del cono
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices.push_back(x);
				break;
			case 1:
				vertices.push_back(y);
				break;
			case 2:
				vertices.push_back(z);
				break;
			}
		}
	}
	vertices.push_back(R * cos(0) * dt);
	vertices.push_back(-0.5);
	vertices.push_back(R * sin(0) * dt);


	for (i = 0; i < res + 2; i++) indices.push_back(i);

	//se genera el mesh del cono
	Mesh* cono = new Mesh();
	cono->CreateMeshGeometry(vertices, indices, vertices.size(), res + 2);
	meshList.push_back(cono);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	vector<unsigned int> piramidecuadrangular_indices = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	vector<GLfloat> piramidecuadrangular_vertices = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMeshGeometry(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}

// función para crear la UI de cada objeto, esto es parte de la modificación de la práctica
void CreateComponentUI() {
	ImGui::Begin("Propiedades del cubo rojo");
	ImGui::SliderFloat3("Posición", glm::value_ptr(redCubePos), -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotación", glm::value_ptr(redCubeRotation), -180.0f, 180.0f);
	ImGui::SliderFloat3("Escala", glm::value_ptr(redCubeScale), 0.001f, 2.0f);
	ImGui::ColorEdit3("Color", glm::value_ptr(redCubeColor));
	ImGui::End();

	ImGui::Begin("Propiedades del techo azul");
	ImGui::SliderFloat3("Posición", glm::value_ptr(blueRoofPos), -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotación", glm::value_ptr(blueRoofRotation), -180.0f, 180.0f);
	ImGui::SliderFloat3("Escala", glm::value_ptr(blueRoofScale), 0.001f, 2.0f);
	ImGui::ColorEdit3("Color", glm::value_ptr(blueRoofColor));
	ImGui::End();

	ImGui::Begin("Propiedades de los árboles");
	ImGui::SliderFloat3("Posición arbol 1", glm::value_ptr(tree1Pos), -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotación arbol 1", glm::value_ptr(tree1Rotation), -180.0f, 180.0f);
	ImGui::SliderFloat3("Escala arbol 1", glm::value_ptr(tree1Scale), 0.001f, 2.0f);
	ImGui::Separator();
	ImGui::SliderFloat3("Posición arbol 2", glm::value_ptr(tree2Pos), -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotación arbol 2", glm::value_ptr(tree2Rotation), -180.0f, 180.0f);
	ImGui::SliderFloat3("Escala arbol 2", glm::value_ptr(tree2Scale), 0.001f, 2.0f);
	ImGui::Separator();
	ImGui::ColorEdit3("Color arboles", glm::value_ptr(treeColor));
	ImGui::End();

	ImGui::Begin("Propiedades de los troncos");
	ImGui::SliderFloat3("Posición tronco 1", glm::value_ptr(log1Pos), -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotación tronco 1", glm::value_ptr(log1Rotation), -180.0f, 180.0f);
	ImGui::SliderFloat3("Escala tronco 1", glm::value_ptr(log1Scale), 0.001f, 2.0f);
	ImGui::Separator();
	ImGui::SliderFloat3("Posición tronco 2", glm::value_ptr(log2Pos), -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotación tronco 2", glm::value_ptr(log2Rotation), -180.0f, 180.0f);
	ImGui::SliderFloat3("Escala tronco 2", glm::value_ptr(log2Scale), 0.001f, 2.0f);
	ImGui::Separator();
	ImGui::ColorEdit3("Color troncos", glm::value_ptr(logColor));
	ImGui::End();

	/*
	* Tutorial section 0: 3D transformations faltantes
	* Objetivos: Implementa en cada objeto la rotación sobre los ejes X, Y y Z
	* la translación y escala de cada objeto, así como sus respectivos colores.
	* 
	* Nota: en esta sección declara sus componentes de UI para cada objeto
	*/

	/* T
	* Tutorial section 1: Floor
	* Objetivos: Implementa el cambio de color del piso de la escena
	* 
	* Notas: en esta sección declara sus componentes de UI para el piso
	* puedes basarte en la declaración de los componenetes de BACKGROUND
	* declarado en el loop principal
	*/
}

int main()
{
	mainWindow = Window();
	// Esta inicialización está modificada checa window.cpp para que Dear ImGui funcione
	mainWindow.Initialize(720, 720);


	CrearCubo();																//índice 0 en MeshList
	CrearPiramideTriangular();													//índice 1 en MeshList
	CrearCilindro(4, 1.0f);														//índice 2 en MeshList
	CrearCono(25, 2.0f);														//índice 3 en MeshList
	CrearPiramideCuadrangular();												//índice 4 en MeshList
	
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 10000.0f);

	//Loop de la esfera mientras no se cierra la ventana
	sp.init();	//inicializar esfera
	sp.load();	//enviar la esfera al shader

	glm::mat4 model(1.0);
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

	// variable extra para el color del fondo, debe estar inicializada si se quiere usar DearImGui
	float backgroundColor[3] = {0.0f, 0.0f, 0.0f};

	while (!mainWindow.ShouldClose())
	{
		// conteo del tiempo
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		
		//Recibir eventos del usuario
		glfwPollEvents();

		//Limpiar la ventana
		glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderList[0].useShader();

		// movimientos de cámara
		camera.mouseControl(mainWindow.GetMouseX(), mainWindow.GetMouseY());
		camera.mouseButtons(mainWindow.GetMouseButtons());
		// con llamadas extras a los buffers por las modificaciones de este modo de explorar el proyecto
		camera.scrollControl(mainWindow.GetScrollY(), deltaTime);
		camera.keyControl(mainWindow.GetKeys(), deltaTime);

		// Iniciar la UI de ImGUI, esto debe ir justo despues del glClear
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// ImGui::ShowDemoWindow();

		// Crear la UI de cada objeto
		ImGui::Begin("Background");
		ImGui::ColorPicker3("Color", (float*)backgroundColor);
		ImGui::End();

		// tambien se puede mandar llamar la función para crear la UI de cada objeto
		CreateComponentUI();

		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// the red cube 
		model = glm::mat4(1.0);
		model = glm::translate(model, redCubePos);
		model = glm::rotate(model, redCubeRotation.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, redCubeRotation.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, redCubeRotation.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, redCubeScale);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		color = redCubeColor;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[0]->RenderMesh();

		// the blue roof
		model = glm::mat4(1.0);
		model = glm::translate(model, blueRoofPos);
		model = glm::rotate(model, blueRoofRotation.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, blueRoofRotation.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, blueRoofRotation.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, blueRoofScale);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = blueRoofColor;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[4]->RenderMesh();

		// the trees 
		color = treeColor;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos

		model = glm::mat4(1.0);
		model = glm::translate(model, tree1Pos);
		model = glm::rotate(model, tree1Rotation.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, tree1Rotation.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, tree1Rotation.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, tree1Scale);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, tree2Pos);
		model = glm::rotate(model, tree2Rotation.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, tree2Rotation.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, tree2Rotation.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, tree2Scale);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[4]->RenderMesh();

		// the logs 
		color = logColor;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos

		model = glm::mat4(1.0);
		model = glm::translate(model, log1Pos);
		model = glm::rotate(model, log1Rotation.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, log1Rotation.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, log1Rotation.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, log1Scale);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, log2Pos);
		model = glm::rotate(model, log2Rotation.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, log2Rotation.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, log2Rotation.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, log2Scale);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();
		
		/*
		* Tutorial section 0: 3D transformations faltantes
		*		Objetivos: Implementa en cada objeto la rotación sobre los ejes X, Y y Z
		*						 la translación y escala de cada objeto, así como sus respectivos colores.
		*
		*		Objetos:
		*						- Ventanas (2)
		*						- Puerta
		*						- Esfera azul
		* 
		*		Notas: el orden de las rotaciones es importante, si se cambia el orden de las rotaciones se obtendrá un resultado diferente
		*		puedes probar con diferentes combinaciones de rotaciones para obtener diferentes resultados
		*/
		// the windows
		color = windowColor; // primer ayuda :P
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.3f, 0.3f, -3.39f));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.3f, 0.3f, -3.39f));
		model = glm::scale(model, glm::vec3(0.20f, 0.20f, 0.20f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// the door
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.4f));
		model = glm::scale(model, glm::vec3(0.30f, 0.50f, 0.30f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// the blue sphere
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.6f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		// model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sp.render(); //dibuja esfera

		/* 
		* Tutorial section 1: Floor
		* Objetivos: Implementa el cambio de color del piso de la escena
		* 
		* Notas: este objeto por defecto no tiene declaradas las variables globales como los objetos anteriores
		* tienes la opción de usar variables globales o locales para cambiar el color del piso, recuerda que de
		* igual manera dearImGui solo recibe apuntadores
		*/

		// the floor
		model = glm::mat4(1.0);

		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		
		model = glm::translate(model, glm::vec3(0.0f, -0.7f, -4.0f));
		model = glm::scale(model, glm::vec3(5.0f, 0.01f, 5.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		glUseProgram(0);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mainWindow.SwapBuffers();
	}
	// ImGui shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return 0;
}