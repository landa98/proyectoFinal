
/*
Semestre 2020-2
Animación por keyframes
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"


#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>

// Generación números random
#include <stdlib.h>
#include <time.h>


const float toRadians = 3.14159265f / 180.0f;
float movCoche;
float movOffset;
float rotapato;
bool avanza, patoadelante;
float posHeli;
float posHeliX;
float posHeliY;
float posHeliZ;
float rotHeliX;
float rotHeliY;
float rotHeliZ;
float rotHelices;
float incHelices;
float posCarro;
float posCarroX = -60.0f;
float posCarroY;
float posCarroZ;
float rotCarroX;
float rotCarroY;
float rotCarroZ;
float tiempoOffset;
float retardo;
float Colibri_TX = 0.0f;
float Colibri_TY = 0.0f;
float Colibri_TZ = 0.0f;
float Colibri_RX = 0.0f;
float Colibri_RY = 0.0f;
float Colibri_RZ = 0.0f;
// Variables Grandpa Rick
float rotacionAvatar1 = 0.0f;
float headingAvatar1 = 0.0f;
float movXAvatar1 = 0.0f;
float movYAvatar1 = 0.0f;
float movZAvatar1 = 0.0f;
bool cambia_sentido1 = false;
// Variables Ryougi Shiki
float rotacionAvatar2 = 0.0f;
float headingAvatar2 = 0.0f;
float movXAvatar2 = 0.0f;
float movYAvatar2 = 0.0f;
float movZAvatar2 = 0.0f;
bool cambia_sentido2 = false;

bool t1 = true;
bool t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20, ta, tb, espectaculo = false, siguiente = true;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;
Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;
Texture Tverde;
Texture Tgris;
Texture tPisoAlameda;
Texture tPisoJardin;
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Helices_M;
Model rotorTras_M;
Model Brick_M;
Model Reja_M;
Model Banca_M;
Model Luminaria_M;
Model Tree_M;
Model Bush_M;
Model shortTree_M;
Model greenBin_M;
Model grayBin_M;
Model esquinaRejas_M;
Model esquinaRejasExt_M;
Model esquinaRejasInv_M;
Model nave_M;
Model pickle_M;
Model fuente_M;
Model cabina_M;
Model Colibri;
Model Bicicleta;

//Primitivas kiosco
Model base_M;
Model columna_M;
Model barandalRecto_M;
Model triangulo_M;
Model octagono_M;
Model escalon_M;
//Primitivas avatar Grandpa Rick
Model cabezaRick_M;
Model torax_M;
Model cadera_M;
Model entrepierna_M;
Model piernaIzq_M;
Model piernaDer_M;
Model brazoIzq_M;
Model brazoDer_M;
Model manoIzq_M;
Model manoDer_M;

//Primitivas avatar Ryougi Shiki
Model RScabeza_M;
Model RStorax_M;
Model RScadera_M;
Model RSentrepierna_M;
Model RSpiernaIzq_M;
Model RSpiernaDer_M;
Model RSbrazoIzq_M;
Model RSbrazoDer_M;
Model RSmanoIzq_M;
Model RSmanoDer_M;


Skybox skybox;
Skybox skybox_Dia;
Skybox skybox_Noche;

Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat uniTime = 0.0f;
GLfloat luzSol = 0.0f;
static double limitFPS = 1.0 / 60.0;

//void my_input(GLFWwindow *window);
void inputKeyframes(bool* keys);

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}




void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f,	0.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	30.0f,	0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f,	30.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		30.0f,	30.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int jardinAindices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat jardinAvertices[] = {
		-49.0f, 0.0f, -176.0f,	0.0f,	0.0f,	0.0f, -1.0f, 0.0f,
		17.0f, 0.0f, -176.0f,	3.4f,	0.0f,	0.0f, -1.0f, 0.0f,
		-49.0f, 0.0f, -160.0f,	0.0f,	0.8f,	0.0f, -1.0f, 0.0f,
		17.0f, 0.0f, -160.0f,	3.4f,	0.8f,	0.0f, -1.0f, 0.0f
	};

	unsigned int jardinA2indices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat jardinA2vertices[] = {
		-49.0f, 0.0f, -160.0f,	0.0f,	0.0f,	0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -160.0f,	2.45f,	0.0f,	0.0f, -1.0f, 0.0f,
		-49.0f, 0.0f, -140.0f,	0.0f,	1.0f,	0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -140.0f,	2.45f,	1.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int jardinCindices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat jardinCvertices[] = {
		-49.0f, 0.0f, -131.0f,	0.0f,	0.0f,	0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -131.0f,	2.45f,	0.0f,	0.0f, -1.0f, 0.0f,
		-49.0f, 0.0f, -95.0f,	0.0f,	1.85f,	0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -95.0f,		2.45f,	1.85f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(jardinAvertices, jardinAindices, 32, 6);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(jardinA2vertices, jardinA2indices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(jardinCvertices, jardinCindices, 32, 6);
	meshList.push_back(obj6);

	/*calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);*/

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;

//NEW// Keyframes
float posXModelo = 10.0, posYModelo = 0.0, posZModelo = 2.0;
float movModelo_x = 0.0f, movModelo_y = 0.0f, movModelo_z = 0.0f;
float giroModeloX = 0.0f;
float giroModeloY = 0.0f;
float giroModeloZ = 0.0f;

#define MAX_FRAMES 100
int i_max_steps = 30;
int i_curr_steps = 88;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movModelo_x;		//Variable para PosicionX
	float movModelo_y;		//Variable para PosicionY
	float movModelo_z;
	float movModelo_xInc;		//Variable para IncrementoX
	float movModelo_yInc;		//Variable para IncrementoY
	float movModelo_zInc;
	float giroModeloX;
	float giroModeloY;
	float giroModeloZ;
	float giroModeloX_Inc;
	float giroModeloY_Inc;
	float giroModeloZ_Inc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 88;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movModelo_x = movModelo_x;
	KeyFrame[FrameIndex].movModelo_y = movModelo_y;
	KeyFrame[FrameIndex].movModelo_z = movModelo_z;
	KeyFrame[FrameIndex].giroModeloX = giroModeloX;
	KeyFrame[FrameIndex].giroModeloY = giroModeloY;
	KeyFrame[FrameIndex].giroModeloZ = giroModeloZ;

	FrameIndex++;
}

void resetElements(void)
{

	movModelo_x = KeyFrame[0].movModelo_x;
	movModelo_y = KeyFrame[0].movModelo_y;
	movModelo_z = KeyFrame[0].movModelo_z;
	giroModeloX = KeyFrame[0].giroModeloX;
	giroModeloY = KeyFrame[0].giroModeloY;
	giroModeloZ = KeyFrame[0].giroModeloZ;
}

void interpolation(void)
{
	KeyFrame[playIndex].movModelo_xInc = (KeyFrame[playIndex + 1].movModelo_x - KeyFrame[playIndex].movModelo_x) / i_max_steps;
	KeyFrame[playIndex].movModelo_yInc = (KeyFrame[playIndex + 1].movModelo_y - KeyFrame[playIndex].movModelo_y) / i_max_steps;
	KeyFrame[playIndex].movModelo_zInc = (KeyFrame[playIndex + 1].movModelo_z - KeyFrame[playIndex].movModelo_z) / i_max_steps;
	KeyFrame[playIndex].giroModeloX_Inc = (KeyFrame[playIndex + 1].giroModeloX - KeyFrame[playIndex].giroModeloX) / i_max_steps;
	KeyFrame[playIndex].giroModeloY_Inc = (KeyFrame[playIndex + 1].giroModeloY - KeyFrame[playIndex].giroModeloY) / i_max_steps;
	KeyFrame[playIndex].giroModeloZ_Inc = (KeyFrame[playIndex + 1].giroModeloZ - KeyFrame[playIndex].giroModeloZ) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movModelo_x += KeyFrame[playIndex].movModelo_xInc;
			movModelo_y += KeyFrame[playIndex].movModelo_yInc;
			movModelo_z += KeyFrame[playIndex].movModelo_zInc;
			giroModeloX += KeyFrame[playIndex].giroModeloX_Inc;
			giroModeloY += KeyFrame[playIndex].giroModeloY_Inc;
			giroModeloZ += KeyFrame[playIndex].giroModeloZ_Inc;
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();


	//PlaySound(TEXT("Silence.wav"), NULL, SND_LOOP | SND_ASYNC);


	camera = Camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 25.0f, 0.5);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();
	Tverde = Texture("Textures/Verde_basura.tga");
	Tverde.LoadTextureA();
	Tgris = Texture("Textures/Gris_humo.tga");
	Tgris.LoadTextureA();
	//tPisoAlameda = Texture("Textures/Piso_alameda.tga");
	tPisoAlameda = Texture("Textures/legoTextureGray.tga");
	tPisoAlameda.LoadTextureA();
	tPisoJardin = Texture("Textures/legoTextureGreen.tga");
	tPisoJardin.LoadTextureA();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/heli.obj");
	Helices_M = Model();
	Helices_M.LoadModel("Models/mainRotor.obj");
	rotorTras_M = Model();
	rotorTras_M.LoadModel("Models/tailRotor.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");
	Brick_M = Model();
	Brick_M.LoadModel("Models/brick_2_x_4.obj");
	Reja_M = Model();
	Reja_M.LoadModel("Models/Reja.obj");
	Banca_M = Model();
	Banca_M.LoadModel("Models/Banca.obj");
	Luminaria_M = Model();
	Luminaria_M.LoadModel("Models/Luminaria.obj");
	Tree_M = Model();
	Tree_M.LoadModel("Models/Tree.obj");
	Bush_M = Model();
	Bush_M.LoadModel("Models/Bush.obj");
	shortTree_M = Model();
	shortTree_M.LoadModel("Models/ShortTree.obj");
	greenBin_M = Model();
	greenBin_M.LoadModel("Models/Basura_Verde.3ds");
	grayBin_M = Model();
	grayBin_M.LoadModel("Models/Basura_Gris.obj");
	esquinaRejas_M = Model();
	esquinaRejas_M.LoadModel("Models/EsquinaRejas.obj");
	esquinaRejasExt_M = Model();
	esquinaRejasExt_M.LoadModel("Models/EsquinaRejasExt.obj");
	esquinaRejasInv_M = Model();
	esquinaRejasInv_M.LoadModel("Models/EsquinaRejasInvertido.obj");
	nave_M = Model();
	nave_M.LoadModel("Models/Nave.obj");
	pickle_M = Model();
	pickle_M.LoadModel("Models/Pepinillo.obj");
	fuente_M = Model();
	fuente_M.LoadModel("Models/Fuente.obj");
	cabina_M = Model();
	cabina_M.LoadModel("Models/Portable_Restroom.obj");
	Colibri = Model();
	Colibri.LoadModel("Models/Colibri.obj");
	Bicicleta = Model();
	Bicicleta.LoadModel("Models/Bicicleta.obj");
	//Kiosco
	base_M = Model();
	base_M.LoadModel("Models/Base.obj");
	columna_M = Model();
	columna_M.LoadModel("Models/Columna.obj");
	barandalRecto_M = Model();
	barandalRecto_M.LoadModel("Models/BarandalRecto.obj");
	triangulo_M = Model();
	triangulo_M.LoadModel("Models/Triangulo.obj");
	octagono_M = Model();
	octagono_M.LoadModel("Models/Tapa.obj");
	escalon_M = Model();
	escalon_M.LoadModel("Models/Escalon.obj");
	//Avatar Grandpa Rick
	cabezaRick_M = Model();
	cabezaRick_M.LoadModel("Models/CabezaRick.obj");
	torax_M = Model();
	torax_M.LoadModel("Models/ToraxText.obj");
	cadera_M = Model();
	cadera_M.LoadModel("Models/Cadera.obj");
	entrepierna_M = Model();
	entrepierna_M.LoadModel("Models/EntrePierna.obj");
	piernaIzq_M = Model();
	piernaIzq_M.LoadModel("Models/PiernaIzq.obj");
	piernaDer_M = Model();
	piernaDer_M.LoadModel("Models/PiernaDer.obj");
	brazoIzq_M = Model();
	brazoIzq_M.LoadModel("Models/BrazoIzq.obj");
	brazoDer_M = Model();
	brazoDer_M.LoadModel("Models/BrazoDer.obj");
	manoIzq_M = Model();
	manoIzq_M.LoadModel("Models/ManoIzq.obj");
	manoDer_M = Model();
	manoDer_M.LoadModel("Models/ManoDer.obj");

	//Avatar Ryougi Shiki
	RScabeza_M = Model();
	RScabeza_M.LoadModel("Models/CabezaRS.obj");
	RStorax_M = Model();
	RStorax_M.LoadModel("Models/RSToraxText.obj");
	RScadera_M = Model();
	RScadera_M.LoadModel("Models/RSCadera.obj");
	RSentrepierna_M = Model();
	RSentrepierna_M.LoadModel("Models/RSEntrePierna.obj");
	RSpiernaIzq_M = Model();
	RSpiernaIzq_M.LoadModel("Models/RSPiernaIzq.obj");
	RSpiernaDer_M = Model();
	RSpiernaDer_M.LoadModel("Models/RSPiernaDer.obj");
	RSbrazoIzq_M = Model();
	RSbrazoIzq_M.LoadModel("Models/RSBrazoIzq.obj");
	RSbrazoDer_M = Model();
	RSbrazoDer_M.LoadModel("Models/RSBrazoDer.obj");
	RSmanoIzq_M = Model();
	RSmanoIzq_M.LoadModel("Models/RSManoIzq.obj");
	RSmanoDer_M = Model();
	RSmanoDer_M.LoadModel("Models/RSManoDer.obj");

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.3f, 1.0f,
		4.0f, 7.0f, 16.7f,
		0.0f, 0.05f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		0.3f, 1.0f,
		4.0f, 7.0f, -25.3f,
		0.0f, 0.05f, 0.1f);
	pointLightCount++;
	pointLights[2] = PointLight(1.0f, 1.0f, 1.0f,
		0.3f, 1.0f,
		4.0f, 7.0f, -69.7f,
		0.0f, 0.05f, 0.1f);
	pointLightCount++;
	pointLights[3] = PointLight(1.0f, 1.0f, 1.0f,
		0.3f, 1.0f,
		4.0f, 7.0f, -114.9f,
		0.0f, 0.05f, 0.1f);
	pointLightCount++;
	pointLights[4] = PointLight(1.0f, 1.0f, 1.0f,
		0.3f, 1.0f,
		4.0f, 7.0f, -158.8f,
		0.0f, 0.05f, 0.1f);
	pointLightCount++;
	//
	//pointLights[5] = PointLight(1.0f, 1.0f, 1.0f,
	//	0.3f, 1.0f,
	//	42.4f, 7.0f, 17.2f,
	//	0.0f, 0.05f, 0.1f);
	//pointLightCount++;
	//pointLights[6] = PointLight(1.0f, 1.0f, 1.0f,
	//	0.3f, 1.0f,
	//	42.4f, 7.0f, -25.3f,
	//	0.0f, 0.05f, 0.1f);
	//pointLightCount++;
	//pointLights[7] = PointLight(1.0f, 1.0f, 1.0f,
	//	0.3f, 1.0f,
	//	42.4f, 7.0f, -69.7f,
	//	0.0f, 0.05f, 0.1f);
	//pointLightCount++;
	//pointLights[8] = PointLight(1.0f, 1.0f, 1.0f,
	//	0.3f, 1.0f,
	//	42.4f, 7.0f, -114.9f,
	//	0.0f, 0.05f, 0.1f);
	//pointLightCount++;
	//pointLights[9] = PointLight(1.0f, 1.0f, 1.0f,
	//	0.3f, 1.0f,
	//	42.4f, 7.0f, -158.8f,
	//	0.0f, 0.05f, 0.1f);
	//pointLightCount++;
	/*4.0f, 0.0f, (-6.4f - 1 * (3.2f))*/
	unsigned int spotLightCount = 0;
	unsigned int spotLightCountTemp = 0;



	//linterna
	spotLights[0] = SpotLight(0.3f, 0.3f, 0.3f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//luz de fiesta 1
	spotLights[1] = SpotLight(1.0f, 0.0f, 0.0f,
		0.0f, 0.3f,
		22.5f, 13.5f, -69.0f,
		0.0f, -1.0f, 0.0f,
		0.3f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	//luz de fiesta 2
	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 0.3f,
		23.5f, 13.5f, -69.0f,
		0.0f, -1.0f, 0.0f,
		0.3f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	//luz de fiesta 3
	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 0.3f,
		24.5f, 13.5f, -69.0f,
		0.0f, -1.0f, 0.0f,
		0.3f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;



	spotLightCountTemp = spotLightCount - 1;
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");



	skybox = Skybox(skyboxFaces);

	// ===============================================================================
	//Cambio de dia y noche
	std::vector<std::string> skyboxFaces_Dia;
	skyboxFaces_Dia.push_back("Textures/SkyboxDia/TropicalSunnyDayLeft.tga");
	skyboxFaces_Dia.push_back("Textures/SkyboxDia/TropicalSunnyDayRight.tga");
	skyboxFaces_Dia.push_back("Textures/SkyboxDia/TropicalSunnyDayDown.tga");
	skyboxFaces_Dia.push_back("Textures/SkyboxDia/TropicalSunnyDayUp.tga");
	skyboxFaces_Dia.push_back("Textures/SkyboxDia/TropicalSunnyDayFront.tga");
	skyboxFaces_Dia.push_back("Textures/SkyboxDia/TropicalSunnyDayBack.tga");

	skybox_Dia = Skybox(skyboxFaces_Dia);




	std::vector<std::string> skyboxFaces_Noche;
	skyboxFaces_Noche.push_back("Textures/SkyboxNoche/DarkStormyLeft.tga");
	skyboxFaces_Noche.push_back("Textures/SkyboxNoche/DarkStormyRight.tga");
	skyboxFaces_Noche.push_back("Textures/SkyboxNoche/DarkStormyDown.tga");
	skyboxFaces_Noche.push_back("Textures/SkyboxNoche/DarkStormyUp.tga");
	skyboxFaces_Noche.push_back("Textures/SkyboxNoche/DarkStormyFront.tga");
	skyboxFaces_Noche.push_back("Textures/SkyboxNoche/DarkStormyBack.tga");

	skybox_Noche = Skybox(skyboxFaces_Noche);
	// ==============================================================================


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	movCoche = 0.0f;
	movOffset = 5.0f;
	avanza = 1;

	//KEYFRAMES DECLARADOS INICIALES

	KeyFrame[0].movModelo_x = 0.0f;
	KeyFrame[0].movModelo_y = 0.0f;
	KeyFrame[0].movModelo_z = 0.0f;
	KeyFrame[0].giroModeloX = 0.0f;
	KeyFrame[0].giroModeloY = 0.0f;
	KeyFrame[0].giroModeloZ = 0.0f;


	KeyFrame[1].movModelo_x = 0.0f;
	KeyFrame[1].movModelo_y = 2.0f;
	KeyFrame[1].movModelo_z = 0.0f;
	KeyFrame[1].giroModeloX = -30.0f;
	KeyFrame[1].giroModeloY = 0.0f;
	KeyFrame[1].giroModeloZ = 0.0f;


	KeyFrame[2].movModelo_x = 0.0f;
	KeyFrame[2].movModelo_y = 4.0f;
	KeyFrame[2].movModelo_z = 0.0f;
	KeyFrame[2].giroModeloX = 0.0f;
	KeyFrame[2].giroModeloY = 0.0f;
	KeyFrame[2].giroModeloZ = 0.0f;

	KeyFrame[3].movModelo_x = 0.0f;
	KeyFrame[3].movModelo_y = 6.0f;
	KeyFrame[3].movModelo_z = 0.0f;
	KeyFrame[3].giroModeloX = 30.0f;
	KeyFrame[3].giroModeloY = 0.0f;
	KeyFrame[3].giroModeloZ = 0.0f;

	KeyFrame[4].movModelo_x = 0.0f;
	KeyFrame[4].movModelo_y = 4.0f;
	KeyFrame[4].movModelo_z = 0.0f;
	KeyFrame[4].giroModeloX = 0.0f;
	KeyFrame[4].giroModeloY = 0.0f;
	KeyFrame[4].giroModeloZ = 0.0f;

	KeyFrame[5].movModelo_x = 0.0f;
	KeyFrame[5].movModelo_y = 2.0f;
	KeyFrame[5].movModelo_z = 0.0f;
	KeyFrame[5].giroModeloX = -30.0f;
	KeyFrame[5].giroModeloY = 0.0f;
	KeyFrame[5].giroModeloZ = 0.0f;

	KeyFrame[6].movModelo_x = 0.0f;
	KeyFrame[6].movModelo_y = 0.0f;
	KeyFrame[6].movModelo_z = 0.0f;
	KeyFrame[6].giroModeloX = 0.0f;
	KeyFrame[6].giroModeloY = 0.0f;
	KeyFrame[6].giroModeloZ = 0.0f;

	KeyFrame[7].movModelo_x = 0.0f;
	KeyFrame[7].movModelo_y = 2.0f;
	KeyFrame[7].movModelo_z = 0.0f;
	KeyFrame[7].giroModeloX = -30.0f;
	KeyFrame[7].giroModeloY = 0.0f;
	KeyFrame[7].giroModeloZ = 0.0f;

	KeyFrame[8].movModelo_x = 0.0f;
	KeyFrame[8].movModelo_y = 4.0f;
	KeyFrame[8].movModelo_z = 0.0f;
	KeyFrame[8].giroModeloX = 0.0f;
	KeyFrame[8].giroModeloY = 0.0f;
	KeyFrame[8].giroModeloZ = 0.0f;

	KeyFrame[9].movModelo_x = 0.0f;
	KeyFrame[9].movModelo_y = 6.0f;
	KeyFrame[9].movModelo_z = 0.0f;
	KeyFrame[9].giroModeloX = 30.0f;
	KeyFrame[9].giroModeloY = 0.0f;
	KeyFrame[9].giroModeloZ = 0.0f;

	KeyFrame[10].movModelo_x = 0.0f;
	KeyFrame[10].movModelo_y = 8.0f;
	KeyFrame[10].movModelo_z = 0.0f;
	KeyFrame[10].giroModeloX = 0.0f;
	KeyFrame[10].giroModeloY = 0.0f;
	KeyFrame[10].giroModeloZ = 0.0f;

	KeyFrame[11].movModelo_x = 0.0f;
	KeyFrame[11].movModelo_y = 6.0f;
	KeyFrame[11].movModelo_z = 0.0f;
	KeyFrame[11].giroModeloX = -30.0f;
	KeyFrame[11].giroModeloY = 0.0f;
	KeyFrame[11].giroModeloZ = 0.0f;

	KeyFrame[12].movModelo_x = 0.0f;
	KeyFrame[12].movModelo_y = 4.0f;
	KeyFrame[12].movModelo_z = 0.0f;
	KeyFrame[12].giroModeloX = 0.0f;
	KeyFrame[12].giroModeloY = 0.0f;
	KeyFrame[12].giroModeloZ = 0.0f;

	KeyFrame[13].movModelo_x = 0.0f;
	KeyFrame[13].movModelo_y = 2.0f;
	KeyFrame[13].movModelo_z = 0.0f;
	KeyFrame[13].giroModeloX = 30.0f;
	KeyFrame[13].giroModeloY = 0.0f;
	KeyFrame[13].giroModeloZ = 0.0f;

	KeyFrame[14].movModelo_x = 0.0f;
	KeyFrame[14].movModelo_y = 0.0f;
	KeyFrame[14].movModelo_z = 0.0f;
	KeyFrame[14].giroModeloX = 0.0f;
	KeyFrame[14].giroModeloY = 0.0f;
	KeyFrame[14].giroModeloZ = 0.0f;

	KeyFrame[15].movModelo_x = 0.0f;
	KeyFrame[15].movModelo_y = 0.1f;
	KeyFrame[15].movModelo_z = 0.0f;
	KeyFrame[15].giroModeloX = 0.0f;
	KeyFrame[15].giroModeloY = -30.0f;
	KeyFrame[15].giroModeloZ = 0.0f;

	KeyFrame[16].movModelo_x = 0.0f;
	KeyFrame[16].movModelo_y = 0.15f;
	KeyFrame[16].movModelo_z = 0.0f;
	KeyFrame[16].giroModeloX = 0.0f;
	KeyFrame[16].giroModeloY = -60.0f;
	KeyFrame[16].giroModeloZ = 0.0f;

	KeyFrame[17].movModelo_x = 0.0f;
	KeyFrame[17].movModelo_y = 0.2f;
	KeyFrame[17].movModelo_z = 0.0f;
	KeyFrame[17].giroModeloX = 0.0f;
	KeyFrame[17].giroModeloY = -90.0f;
	KeyFrame[17].giroModeloZ = 0.0f;

	KeyFrame[18].movModelo_x = 0.0f;
	KeyFrame[18].movModelo_y = 0.25f;
	KeyFrame[18].movModelo_z = 0.0f;
	KeyFrame[18].giroModeloX = 0.0f;
	KeyFrame[18].giroModeloY = -90.0f;
	KeyFrame[18].giroModeloZ = 0.0f;

	KeyFrame[19].movModelo_x = 0.0f;
	KeyFrame[19].movModelo_y = 0.25f;
	KeyFrame[19].movModelo_z = -5.0f;
	KeyFrame[19].giroModeloX = -500.0f;
	KeyFrame[19].giroModeloY = -90.0f;
	KeyFrame[19].giroModeloZ = 0.0f;

	KeyFrame[20].movModelo_x = 0.0f;
	KeyFrame[20].movModelo_y = 0.25f;
	KeyFrame[20].movModelo_z = -10.0f;
	KeyFrame[20].giroModeloX = -1000.0f;
	KeyFrame[20].giroModeloY = -90.0f;
	KeyFrame[20].giroModeloZ = 0.0f;

	KeyFrame[21].movModelo_x = 0.0f;
	KeyFrame[21].movModelo_y = 0.25f;
	KeyFrame[21].movModelo_z = -15.0f;
	KeyFrame[21].giroModeloX = -1500.0f;
	KeyFrame[21].giroModeloY = -90.0f;
	KeyFrame[21].giroModeloZ = 0.0f;

	KeyFrame[22].movModelo_x = 0.0f;
	KeyFrame[22].movModelo_y = 0.25f;
	KeyFrame[22].movModelo_z = -20.0f;
	KeyFrame[22].giroModeloX = -2000.0f;
	KeyFrame[22].giroModeloY = -90.0f;
	KeyFrame[22].giroModeloZ = 0.0f;

	KeyFrame[23].movModelo_x = 0.0f;
	KeyFrame[23].movModelo_y = 0.25f;
	KeyFrame[23].movModelo_z = -25.0f;
	KeyFrame[23].giroModeloX = -2500.0f;
	KeyFrame[23].giroModeloY = -90.0f;
	KeyFrame[23].giroModeloZ = 0.0f;

	KeyFrame[24].movModelo_x = 0.0f;
	KeyFrame[24].movModelo_y = 0.25f;
	KeyFrame[24].movModelo_z = -30.0f;
	KeyFrame[24].giroModeloX = -3000.0f;
	KeyFrame[24].giroModeloY = -90.0f;
	KeyFrame[24].giroModeloZ = 0.0f;

	KeyFrame[25].movModelo_x = 0.0f;
	KeyFrame[25].movModelo_y = 0.25f;
	KeyFrame[25].movModelo_z = -35.0f;
	KeyFrame[25].giroModeloX = -3500.0f;
	KeyFrame[25].giroModeloY = -90.0f;
	KeyFrame[25].giroModeloZ = 0.0f;

	KeyFrame[26].movModelo_x = 0.0f;
	KeyFrame[26].movModelo_y = 0.25f;
	KeyFrame[26].movModelo_z = -40.0f;
	KeyFrame[26].giroModeloX = -4000.0f;
	KeyFrame[26].giroModeloY = -90.0f;
	KeyFrame[26].giroModeloZ = 0.0f;

	KeyFrame[27].movModelo_x = 0.0f;
	KeyFrame[27].movModelo_y = 0.25f;
	KeyFrame[27].movModelo_z = -45.0f;
	KeyFrame[27].giroModeloX = -4500.0f;
	KeyFrame[27].giroModeloY = -90.0f;
	KeyFrame[27].giroModeloZ = 0.0f;

	KeyFrame[28].movModelo_x = 0.0f;
	KeyFrame[28].movModelo_y = 0.25f;
	KeyFrame[28].movModelo_z = -50.0f;
	KeyFrame[28].giroModeloX = -5000.0f;
	KeyFrame[28].giroModeloY = -90.0f;
	KeyFrame[28].giroModeloZ = 0.0f;

	KeyFrame[29].movModelo_x = 0.0f;
	KeyFrame[29].movModelo_y = 0.25f;
	KeyFrame[29].movModelo_z = -55.0f;
	KeyFrame[29].giroModeloX = -5500.0f;
	KeyFrame[29].giroModeloY = -90.0f;
	KeyFrame[29].giroModeloZ = 0.0f;

	KeyFrame[30].movModelo_x = 0.0f;
	KeyFrame[30].movModelo_y = 0.25f;
	KeyFrame[30].movModelo_z = -60.0f;
	KeyFrame[30].giroModeloX = -6000.0f;
	KeyFrame[30].giroModeloY = -90.0f;
	KeyFrame[30].giroModeloZ = 0.0f;

	KeyFrame[31].movModelo_x = 0.0f;
	KeyFrame[31].movModelo_y = 0.25f;
	KeyFrame[31].movModelo_z = -65.0f;
	KeyFrame[31].giroModeloX = -6500.0f;
	KeyFrame[31].giroModeloY = -90.0f;
	KeyFrame[31].giroModeloZ = 0.0f;

	KeyFrame[32].movModelo_x = 0.0f;
	KeyFrame[32].movModelo_y = 0.25f;
	KeyFrame[32].movModelo_z = -70.0f;
	KeyFrame[32].giroModeloX = -7000.0f;
	KeyFrame[32].giroModeloY = -90.0f;
	KeyFrame[32].giroModeloZ = 0.0f;

	KeyFrame[33].movModelo_x = 0.0f;
	KeyFrame[33].movModelo_y = 0.25f;
	KeyFrame[33].movModelo_z = -75.0f;
	KeyFrame[33].giroModeloX = -7500.0f;
	KeyFrame[33].giroModeloY = -90.0f;
	KeyFrame[33].giroModeloZ = 0.0f;

	KeyFrame[34].movModelo_x = 0.0f;
	KeyFrame[34].movModelo_y = 0.25f;
	KeyFrame[34].movModelo_z = -80.0f;
	KeyFrame[34].giroModeloX = -8000.0f;
	KeyFrame[34].giroModeloY = -90.0f;
	KeyFrame[34].giroModeloZ = 0.0f;

	KeyFrame[35].movModelo_x = 0.0f;
	KeyFrame[35].movModelo_y = 0.25f;
	KeyFrame[35].movModelo_z = -85.0f;
	KeyFrame[35].giroModeloX = -8500.0f;
	KeyFrame[35].giroModeloY = -90.0f;
	KeyFrame[35].giroModeloZ = 0.0f;

	KeyFrame[36].movModelo_x = 0.0f;
	KeyFrame[36].movModelo_y = 0.25f;
	KeyFrame[36].movModelo_z = -90.0f;
	KeyFrame[36].giroModeloX = -9000.0f;
	KeyFrame[36].giroModeloY = -90.0f;
	KeyFrame[36].giroModeloZ = 0.0f;

	KeyFrame[37].movModelo_x = 0.0f;
	KeyFrame[37].movModelo_y = 0.25f;
	KeyFrame[37].movModelo_z = -95.0f;
	KeyFrame[37].giroModeloX = -9500.0f;
	KeyFrame[37].giroModeloY = -90.0f;
	KeyFrame[37].giroModeloZ = 0.0f;

	KeyFrame[38].movModelo_x = 0.0f;
	KeyFrame[38].movModelo_y = 0.25f;
	KeyFrame[38].movModelo_z = -100.0f;
	KeyFrame[38].giroModeloX = -10000.0f;
	KeyFrame[38].giroModeloY = -90.0f;
	KeyFrame[38].giroModeloZ = 0.0f;

	KeyFrame[39].movModelo_x = 0.0f;
	KeyFrame[39].movModelo_y = 0.25f;
	KeyFrame[39].movModelo_z = -105.0f;
	KeyFrame[39].giroModeloX = -10500.0f;
	KeyFrame[39].giroModeloY = -90.0f;
	KeyFrame[39].giroModeloZ = 0.0f;

	KeyFrame[40].movModelo_x = 0.0f;
	KeyFrame[40].movModelo_y = 0.25f;
	KeyFrame[40].movModelo_z = -110.0f;
	KeyFrame[40].giroModeloX = -11000.0f;
	KeyFrame[40].giroModeloY = -90.0f;
	KeyFrame[40].giroModeloZ = 0.0f;

	KeyFrame[41].movModelo_x = 0.0f;
	KeyFrame[41].movModelo_y = 0.25f;
	KeyFrame[41].movModelo_z = -115.0f;
	KeyFrame[41].giroModeloX = -11500.0f;
	KeyFrame[41].giroModeloY = -90.0f;
	KeyFrame[41].giroModeloZ = 0.0f;

	KeyFrame[42].movModelo_x = 0.0f;
	KeyFrame[42].movModelo_y = 0.25f;
	KeyFrame[42].movModelo_z = -120.0f;
	KeyFrame[42].giroModeloX = -12000.0f;
	KeyFrame[42].giroModeloY = -90.0f;
	KeyFrame[42].giroModeloZ = 0.0f;

	KeyFrame[43].movModelo_x = 0.0f;
	KeyFrame[43].movModelo_y = 0.25f;
	KeyFrame[43].movModelo_z = -125.0f;
	KeyFrame[43].giroModeloX = -12500.0f;
	KeyFrame[43].giroModeloY = -90.0f;
	KeyFrame[43].giroModeloZ = 0.0f;

	KeyFrame[44].movModelo_x = 0.0f;
	KeyFrame[44].movModelo_y = 0.25f;
	KeyFrame[44].movModelo_z = -130.0f;
	KeyFrame[44].giroModeloX = -13000.0f;
	KeyFrame[44].giroModeloY = -90.0f;
	KeyFrame[44].giroModeloZ = 0.0f;

	KeyFrame[45].movModelo_x = 0.0f;
	KeyFrame[45].movModelo_y = 0.25f;
	KeyFrame[45].movModelo_z = -135.0f;
	KeyFrame[45].giroModeloX = -13500.0f;
	KeyFrame[45].giroModeloY = -90.0f;
	KeyFrame[45].giroModeloZ = 0.0f;

	KeyFrame[45].movModelo_x = 0.0f;
	KeyFrame[45].movModelo_y = 0.25f;
	KeyFrame[45].movModelo_z = -140.0f;
	KeyFrame[45].giroModeloX = -14000.0f;
	KeyFrame[45].giroModeloY = -90.0f;
	KeyFrame[45].giroModeloZ = 0.0f;

	KeyFrame[46].movModelo_x = 0.0f;
	KeyFrame[46].movModelo_y = 0.25f;
	KeyFrame[46].movModelo_z = -145.0f;
	KeyFrame[46].giroModeloX = -14500.0f;
	KeyFrame[46].giroModeloY = -90.0f;
	KeyFrame[46].giroModeloZ = 0.0f;

	KeyFrame[47].movModelo_x = 0.0f;
	KeyFrame[47].movModelo_y = 0.25f;
	KeyFrame[47].movModelo_z = -150.0f;
	KeyFrame[47].giroModeloX = -15000.0f;
	KeyFrame[47].giroModeloY = -90.0f;
	KeyFrame[47].giroModeloZ = 0.0f;

	KeyFrame[48].movModelo_x = 0.0f;
	KeyFrame[48].movModelo_y = 0.25f;
	KeyFrame[48].movModelo_z = -150.0f;
	KeyFrame[48].giroModeloX = -15000.0f;
	KeyFrame[48].giroModeloY = -60.0f;
	KeyFrame[48].giroModeloZ = 0.0f;

	KeyFrame[49].movModelo_x = 0.0f;
	KeyFrame[49].movModelo_y = 0.25f;
	KeyFrame[49].movModelo_z = -150.0f;
	KeyFrame[49].giroModeloX = -15500.0f;
	KeyFrame[49].giroModeloY = -30.0f;
	KeyFrame[49].giroModeloZ = 0.0f;

	KeyFrame[50].movModelo_x = 0.0f;
	KeyFrame[50].movModelo_y = 0.25f;
	KeyFrame[50].movModelo_z = -150.0f;
	KeyFrame[50].giroModeloX = -15000.0f;
	KeyFrame[50].giroModeloY = 0.0f;
	KeyFrame[50].giroModeloZ = 0.0f;

	KeyFrame[51].movModelo_x = 0.0f;
	KeyFrame[51].movModelo_y = 4.25f;
	KeyFrame[51].movModelo_z = -150.0f;
	KeyFrame[51].giroModeloX = -15000.0f;
	KeyFrame[51].giroModeloY = 0.0f;
	KeyFrame[51].giroModeloZ = 0.0f;

	KeyFrame[52].movModelo_x = 0.0f;
	KeyFrame[52].movModelo_y = 8.25f;
	KeyFrame[52].movModelo_z = -150.0f;
	KeyFrame[52].giroModeloX = -15000.0f;
	KeyFrame[52].giroModeloY = 0.0f;
	KeyFrame[52].giroModeloZ = 0.0f;

	KeyFrame[53].movModelo_x = 0.0f;
	KeyFrame[53].movModelo_y = 4.25f;
	KeyFrame[53].movModelo_z = -150.0f;
	KeyFrame[53].giroModeloX = -15000.0f;
	KeyFrame[53].giroModeloY = 0.0f;
	KeyFrame[53].giroModeloZ = 0.0f;

	KeyFrame[54].movModelo_x = 0.0f;
	KeyFrame[54].movModelo_y = 0.25f;
	KeyFrame[54].movModelo_z = -150.0f;
	KeyFrame[54].giroModeloX = -15000.0f;
	KeyFrame[54].giroModeloY = 0.0f;
	KeyFrame[54].giroModeloZ = 0.0f;

	KeyFrame[55].movModelo_x = 0.0f;
	KeyFrame[55].movModelo_y = 0.25f;
	KeyFrame[55].movModelo_z = -150.0f;
	KeyFrame[55].giroModeloX = -15000.0f;
	KeyFrame[55].giroModeloY = 30.0f;
	KeyFrame[55].giroModeloZ = 0.0f;

	KeyFrame[56].movModelo_x = 0.0f;
	KeyFrame[56].movModelo_y = 0.25f;
	KeyFrame[56].movModelo_z = -150.0f;
	KeyFrame[56].giroModeloX = -15000.0f;
	KeyFrame[56].giroModeloY = 60.0f;
	KeyFrame[56].giroModeloZ = 0.0f;

	KeyFrame[57].movModelo_x = 0.0f;
	KeyFrame[57].movModelo_y = 0.25f;
	KeyFrame[57].movModelo_z = -150.0f;
	KeyFrame[57].giroModeloX = -15000.0f;
	KeyFrame[57].giroModeloY = 90.0f;
	KeyFrame[57].giroModeloZ = 0.0f;

	KeyFrame[58].movModelo_x = 0.0f;
	KeyFrame[58].movModelo_y = 0.25f;
	KeyFrame[58].movModelo_z = -145.0f;
	KeyFrame[58].giroModeloX = -14500.0f;
	KeyFrame[58].giroModeloY = 90.0f;
	KeyFrame[58].giroModeloZ = 0.0f;

	KeyFrame[59].movModelo_x = 0.0f;
	KeyFrame[59].movModelo_y = 0.25f;
	KeyFrame[59].movModelo_z = -140.0f;
	KeyFrame[59].giroModeloX = -14000.0f;
	KeyFrame[59].giroModeloY = 90.0f;
	KeyFrame[59].giroModeloZ = 0.0f;

	KeyFrame[60].movModelo_x = 0.0f;
	KeyFrame[60].movModelo_y = 0.25f;
	KeyFrame[60].movModelo_z = -135.0f;
	KeyFrame[60].giroModeloX = -13500.0f;
	KeyFrame[60].giroModeloY = 90.0f;
	KeyFrame[60].giroModeloZ = 0.0f;

	KeyFrame[61].movModelo_x = 0.0f;
	KeyFrame[61].movModelo_y = 0.25f;
	KeyFrame[61].movModelo_z = -130.0f;
	KeyFrame[61].giroModeloX = -13000.0f;
	KeyFrame[61].giroModeloY = 90.0f;
	KeyFrame[61].giroModeloZ = 0.0f;

	KeyFrame[62].movModelo_x = 0.0f;
	KeyFrame[62].movModelo_y = 0.25f;
	KeyFrame[62].movModelo_z = -125.0f;
	KeyFrame[62].giroModeloX = -12500.0f;
	KeyFrame[62].giroModeloY = 90.0f;
	KeyFrame[62].giroModeloZ = 0.0f;

	KeyFrame[63].movModelo_x = 0.0f;
	KeyFrame[63].movModelo_y = 0.25f;
	KeyFrame[63].movModelo_z = -120.0f;
	KeyFrame[63].giroModeloX = -12000.0f;
	KeyFrame[63].giroModeloY = 90.0f;
	KeyFrame[63].giroModeloZ = 0.0f;

	KeyFrame[64].movModelo_x = 0.0f;
	KeyFrame[64].movModelo_y = 0.25f;
	KeyFrame[64].movModelo_z = -115.0f;
	KeyFrame[64].giroModeloX = -11500.0f;
	KeyFrame[64].giroModeloY = 90.0f;
	KeyFrame[64].giroModeloZ = 0.0f;

	KeyFrame[65].movModelo_x = 0.0f;
	KeyFrame[65].movModelo_y = 0.25f;
	KeyFrame[65].movModelo_z = -110.0f;
	KeyFrame[65].giroModeloX = -11000.0f;
	KeyFrame[65].giroModeloY = 90.0f;
	KeyFrame[65].giroModeloZ = 0.0f;

	KeyFrame[66].movModelo_x = 0.0f;
	KeyFrame[66].movModelo_y = 0.25f;
	KeyFrame[66].movModelo_z = -105.0f;
	KeyFrame[66].giroModeloX = -10500.0f;
	KeyFrame[66].giroModeloY = 90.0f;
	KeyFrame[66].giroModeloZ = 0.0f;

	KeyFrame[67].movModelo_x = 0.0f;
	KeyFrame[67].movModelo_y = 0.25f;
	KeyFrame[67].movModelo_z = -100.0f;
	KeyFrame[67].giroModeloX = -10000.0f;
	KeyFrame[67].giroModeloY = 90.0f;
	KeyFrame[67].giroModeloZ = 0.0f;

	KeyFrame[68].movModelo_x = 0.0f;
	KeyFrame[68].movModelo_y = 0.25f;
	KeyFrame[68].movModelo_z = -95.0f;
	KeyFrame[68].giroModeloX = -9500.0f;
	KeyFrame[68].giroModeloY = 90.0f;
	KeyFrame[68].giroModeloZ = 0.0f;

	KeyFrame[69].movModelo_x = 0.0f;
	KeyFrame[69].movModelo_y = 0.25f;
	KeyFrame[69].movModelo_z = -90.0f;
	KeyFrame[69].giroModeloX = -9000.0f;
	KeyFrame[69].giroModeloY = 90.0f;
	KeyFrame[69].giroModeloZ = 0.0f;

	KeyFrame[70].movModelo_x = 0.0f;
	KeyFrame[70].movModelo_y = 0.25f;
	KeyFrame[70].movModelo_z = -85.0f;
	KeyFrame[70].giroModeloX = -8500.0f;
	KeyFrame[70].giroModeloY = 90.0f;
	KeyFrame[70].giroModeloZ = 0.0f;

	KeyFrame[71].movModelo_x = 0.0f;
	KeyFrame[71].movModelo_y = 0.25f;
	KeyFrame[71].movModelo_z = -80.0f;
	KeyFrame[71].giroModeloX = -8000.0f;
	KeyFrame[71].giroModeloY = 90.0f;
	KeyFrame[71].giroModeloZ = 0.0f;

	KeyFrame[72].movModelo_x = 0.0f;
	KeyFrame[72].movModelo_y = 0.25f;
	KeyFrame[72].movModelo_z = -75.0f;
	KeyFrame[72].giroModeloX = -7500.0f;
	KeyFrame[72].giroModeloY = 90.0f;
	KeyFrame[72].giroModeloZ = 0.0f;

	KeyFrame[73].movModelo_x = 0.0f;
	KeyFrame[73].movModelo_y = 0.25f;
	KeyFrame[73].movModelo_z = -70.0f;
	KeyFrame[73].giroModeloX = -7000.0f;
	KeyFrame[73].giroModeloY = 90.0f;
	KeyFrame[73].giroModeloZ = 0.0f;

	KeyFrame[74].movModelo_x = 0.0f;
	KeyFrame[74].movModelo_y = 0.25f;
	KeyFrame[74].movModelo_z = -65.0f;
	KeyFrame[74].giroModeloX = -6500.0f;
	KeyFrame[74].giroModeloY = 90.0f;
	KeyFrame[74].giroModeloZ = 0.0f;

	KeyFrame[75].movModelo_x = 0.0f;
	KeyFrame[75].movModelo_y = 0.25f;
	KeyFrame[75].movModelo_z = -60.0f;
	KeyFrame[75].giroModeloX = -6000.0f;
	KeyFrame[75].giroModeloY = 90.0f;
	KeyFrame[75].giroModeloZ = 0.0f;

	KeyFrame[76].movModelo_x = 0.0f;
	KeyFrame[76].movModelo_y = 0.25f;
	KeyFrame[76].movModelo_z = -55.0f;
	KeyFrame[76].giroModeloX = -5500.0f;
	KeyFrame[76].giroModeloY = 90.0f;
	KeyFrame[76].giroModeloZ = 0.0f;

	KeyFrame[77].movModelo_x = 0.0f;
	KeyFrame[77].movModelo_y = 0.25f;
	KeyFrame[77].movModelo_z = -50.0f;
	KeyFrame[77].giroModeloX = -5000.0f;
	KeyFrame[77].giroModeloY = 90.0f;
	KeyFrame[77].giroModeloZ = 0.0f;

	KeyFrame[78].movModelo_x = 0.0f;
	KeyFrame[78].movModelo_y = 0.25f;
	KeyFrame[78].movModelo_z = -45.0f;
	KeyFrame[78].giroModeloX = -4500.0f;
	KeyFrame[78].giroModeloY = 90.0f;
	KeyFrame[78].giroModeloZ = 0.0f;

	KeyFrame[79].movModelo_x = 0.0f;
	KeyFrame[79].movModelo_y = 0.25f;
	KeyFrame[79].movModelo_z = -40.0f;
	KeyFrame[79].giroModeloX = -4000.0f;
	KeyFrame[79].giroModeloY = 90.0f;
	KeyFrame[79].giroModeloZ = 0.0f;

	KeyFrame[80].movModelo_x = 0.0f;
	KeyFrame[80].movModelo_y = 0.25f;
	KeyFrame[80].movModelo_z = -35.0f;
	KeyFrame[80].giroModeloX = -3500.0f;
	KeyFrame[80].giroModeloY = 90.0f;
	KeyFrame[80].giroModeloZ = 0.0f;

	KeyFrame[81].movModelo_x = 0.0f;
	KeyFrame[81].movModelo_y = 0.25f;
	KeyFrame[81].movModelo_z = -30.0f;
	KeyFrame[81].giroModeloX = -3000.0f;
	KeyFrame[81].giroModeloY = 90.0f;
	KeyFrame[81].giroModeloZ = 0.0f;

	KeyFrame[82].movModelo_x = 0.0f;
	KeyFrame[82].movModelo_y = 0.25f;
	KeyFrame[82].movModelo_z = -25.0f;
	KeyFrame[82].giroModeloX = -2500.0f;
	KeyFrame[82].giroModeloY = 90.0f;
	KeyFrame[82].giroModeloZ = 0.0f;

	KeyFrame[83].movModelo_x = 0.0f;
	KeyFrame[83].movModelo_y = 0.25f;
	KeyFrame[83].movModelo_z = -20.0f;
	KeyFrame[83].giroModeloX = -2000.0f;
	KeyFrame[83].giroModeloY = 90.0f;
	KeyFrame[83].giroModeloZ = 0.0f;

	KeyFrame[84].movModelo_x = 0.0f;
	KeyFrame[84].movModelo_y = 0.25f;
	KeyFrame[84].movModelo_z = -15.0f;
	KeyFrame[84].giroModeloX = -1500.0f;
	KeyFrame[84].giroModeloY = 90.0f;
	KeyFrame[84].giroModeloZ = 0.0f;

	KeyFrame[85].movModelo_x = 0.0f;
	KeyFrame[85].movModelo_y = 0.25f;
	KeyFrame[85].movModelo_z = -10.0f;
	KeyFrame[85].giroModeloX = -1000.0f;
	KeyFrame[85].giroModeloY = 90.0f;
	KeyFrame[85].giroModeloZ = 0.0f;

	KeyFrame[86].movModelo_x = 0.0f;
	KeyFrame[86].movModelo_y = 0.25f;
	KeyFrame[86].movModelo_z = -5.0f;
	KeyFrame[86].giroModeloX = -500.0f;
	KeyFrame[86].giroModeloY = 90.0f;
	KeyFrame[86].giroModeloZ = 0.0f;

	KeyFrame[87].movModelo_x = 0.0f;
	KeyFrame[87].movModelo_y = 0.25f;
	KeyFrame[87].movModelo_z = 0.0f;
	KeyFrame[87].giroModeloX = 0.0f;
	KeyFrame[87].giroModeloY = 90.0f;
	KeyFrame[87].giroModeloZ = 0.0f;

	//Para animaciones basicas
	float Bicicleta_TX = 0.0f;
	float Bicicleta_TY = 0.0f;
	float Bicicleta_TZ = 0.0f;
	float Bicicleta_RX = 0.0f;
	float Bicicleta_RY = 0.0f;
	float Bicicleta_RZ = 0.0f;
	int Estado_Bicicleta = 0.0;
	//Variable contadora para el cambio dia y noche
	float tiempo_Dia = 0.0f;
	//Variables para recorrer bancas
	float bancasX_1 = (-0.8f + 12.25 * 3.2f);
	float bancasX_2 = (-0.8f + 10.25 * 3.2f);
	float bancasX_3 = (-0.8f + 2.25 * 3.2f);
	//Variables para botes de basura
	float botesX_1 = (-0.8f + 12.25 * 3.2f);
	//Variables Para recorrer jardines
	//+Z
	float JardinZ_1 = 0 * (-6.4f - 12 * (3.2f));
	float JardinZ_2 = 1 * (-6.4f - 12 * (3.2f));
	float JardinZ_3 = 2 * (-6.4f - 12 * (3.2f));
	float JardinZ_4 = 3 * (-6.4f - 12 * (3.2f));
	float JardinZ_5 = -1 * (-6.4f - 12 * (3.2f));
	//X
	float JardinX_1 = 2 * (-0.8f + 15.25 * 3.2f);
	float JardinX_2 = 3 * (-0.8f + 15.25 * 3.2f);
	float JardinX_3 = 4 * (-0.8f + 15.25 * 3.2f);
	float JardinX_4 = 5 * (-0.8f + 15.25 * 3.2f);
	float JardinX_5 = -1 * (-0.8f + 15.25 * 3.2f);
	float JardinX_10 = 1 * (-0.8f + 15.25 * 3.2f) - 1.6f;
	//Auxiliares
	float recorrerEntrada = 5.5 * 3.2f;
	float recorrerEntradaZ = -(-0.8f + 5 * 3.2f) - 0.8f;
	//float recorrerEntradaZ = -(-0.8f + 5 * 3.2f) -0.8f;
	int Estado_Colibri = 0;
	posHeli = -10.0f;
	posHeliX = 40.0f;
	posHeliY = 0.2f;
	posHeliZ = 0.0f;
	rotHeliX = 0.0f;
	rotHeliY = 0.0f;
	rotHeliZ = 0.0f;
	movOffset = 2.0f;
	tiempoOffset = 2.0f;
	rotHelices = 0.0f;
	incHelices = 0.001f;
	retardo = 0.0f;
	srand(time(NULL));
	//Loop mientras no se cierra la ventana
	GLfloat inicio;
	spotLightCount = 1;
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		uniTime += tiempoOffset * deltaTime;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//
		luzSol = ((sin(5 * uniTime * toRadians) + 1.0f) / 2) * 0.6f + 0.07f; // factor de 15

		mainLight = DirectionalLight(luzSol, luzSol, luzSol,
			0.9f, 0.9f,
			0.0f, -1.0f, 0.0f);

		if (luzSol <= 0.3)
		{
			pointLightCount = 4;
			spotLights[0] = SpotLight(0.3f, 0.3f, 0.3f,
				0.0f, 2.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				20.0f);
			skybox_Noche.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else
		{
			skybox_Dia.DrawSkybox(camera.calculateViewMatrix(), projection);
			pointLightCount = 0;
			spotLights[0] = SpotLight(0.0f, 0.0f, 0.0f,
				0.0f, 2.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, -1.0f, 0.0f,
				1.0f, 0.0f, 0.0f,
				20.0f);
		}

		if (espectaculo && siguiente)
		{
			spotLightCount = rand() % (4) + 1;
			printf("Random es: %d", spotLightCount);
			spotLights[1].SetFlash(glm::vec3(21.5f, 13.5f, -69.0f), glm::vec3(((rand() % 40) - 20.0f) / 100.0f, -1.0f, ((rand() % 40) - 20.0f) / 100.0f));
			spotLights[2].SetFlash(glm::vec3(23.5f, 13.5f, -69.0f), glm::vec3(((rand() % 40) - 20.0f) / 100.0f, -1.0f, ((rand() % 40) - 20.0f) / 100.0f));
			spotLights[3].SetFlash(glm::vec3(25.5f, 13.5f, -69.0f), glm::vec3(((rand() % 40) - 20.0f) / 100.0f, -1.0f, ((rand() % 40) - 20.0f) / 100.0f));
			spotLightCountTemp = spotLightCount;

			inicio = lastTime;
			siguiente = false;
		}
		if (!siguiente)
		{
			if (lastTime - inicio >= 0.5)
				siguiente = true;
		}
		if (!espectaculo)
		{
			spotLightCount = 1;
		}
		else
		{
			spotLightCount = spotLightCountTemp;
		}


		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		uniformColor = shaderList[0].GetColorLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);
		glm::mat4 aux(1.0);
		glm::vec3 color(1.0);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoAlameda.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//Jardines en L
		//A
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.15f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//B
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(78.6f, 0.15f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//I
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.15f, 199.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		//J
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(78.6f, 0.15f, 199.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();



		//A
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.15f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//B
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(96.2f, 0.15f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//I
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.15f, 163.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//J
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(96.2f, 0.15f, 163.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		//Jardines rectangulares
		//C
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.15f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//D
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(96.2f, 0.15f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//E
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.15f, 44.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//F
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(96.2f, 0.15f, 44.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//G
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.15f, 89.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//H
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(96.2f, 0.15f, 89.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoJardin.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		// Baños
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(19.0f, 0.0f, -182.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoAlameda.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		cabina_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(28.0f, 0.0f, -182.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tPisoAlameda.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		cabina_M.RenderModel();


		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXModelo + movModelo_x, posYModelo + movModelo_y, posZModelo + movModelo_z);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));//0.05f
		model = glm::rotate(model, (-90 + giroModeloX)* toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (90 + giroModeloY) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, (90 + giroModeloZ) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pickle_M.RenderModel();
		/*spotLights[3].SetPos(posblackhawk);*/






		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		//Jardin Cuadrado

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.4f + 0.8f));
		//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 1 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 2 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 3 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 4 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 5 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 6 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 7 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 8 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 9 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 10 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 10 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 9 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 8 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 7 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 6 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 5 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 4 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 3 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 2 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 1 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// Segundo jardin Cuadrado
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 12 * (3.2f))));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 13 * (3.2f))));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		// + (-6.4f - 13 * (3.2f))

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// Tercer jardin Cuadrado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// Cuarto jardin Cuadrado



		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 1 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 2 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 3 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 4 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 5 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 6 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 7 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 8 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 9 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 10 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 11 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 12 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 13 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-0.8f + 14 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Reja_M.RenderModel();
		//
		//
		//
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, -6.4f + 0.8f));
		//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 1 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 2 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 3 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 4 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 5 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 6 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 7 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 8 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 9 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 10 * (3.2f))));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 10 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 9 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 8 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 7 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 6 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 5 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 4 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 3 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 2 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 1 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f));
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// Quinto jardin Cuadrado

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_2));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_2));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_2));
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();


		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// Sexto jardin Cuadrado

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_3));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_3));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f + JardinX_1, 0.0f, -6.4f + 0.8f + JardinZ_3));
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		//////////////////////////////////////////////////////////////////////////////////////////////////xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// Jardin esquina superior izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.4f + 0.8f + JardinZ_4));//<-
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f + 4 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));///////////////////////////////////////////////////////////////<---
		model = glm::rotate(model, -270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f + 3 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));//<-
		model = glm::rotate(model, -270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f + 2 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));//<-
		model = glm::rotate(model, -270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f + 1 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));//<-
		model = glm::rotate(model, -270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f + 0 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));//<-
		model = glm::rotate(model, -270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 0 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_4));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_4));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_4));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.4f + 0.8f + JardinZ_4 + recorrerEntradaZ - 4.8f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasInv_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4 * 3.2f + 0.8f, 0.0f, -6.4f + JardinZ_4 + recorrerEntradaZ - 4.0f));
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3 * 3.2f + 0.8f, 0.0f, -6.4f + JardinZ_4 + recorrerEntradaZ - 4.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2 * 3.2f + 0.8f, 0.0f, -6.4f + JardinZ_4 + recorrerEntradaZ - 4.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1 * 3.2f + 0.8f, 0.0f, -6.4f + JardinZ_4 + recorrerEntradaZ - 4.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.4f + 0.8f + JardinZ_4 + recorrerEntradaZ + 2.4f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_4));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(recorrerEntrada, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_4 + 2.4f));
		model = glm::scale(model, glm::vec3(-0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasExt_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(recorrerEntrada, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_4));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(recorrerEntrada, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_4));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(recorrerEntrada, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_4));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(recorrerEntrada, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(-0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasExt_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// Jardin esquina superior derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, -6.4f + 0.8f + JardinZ_4));//<-
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f + 4 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));///////////////////////////////////////////////////////////////<---
		model = glm::rotate(model, -270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f + 3 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));//<-
		model = glm::rotate(model, -270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f + 2 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));//<-
		model = glm::rotate(model, -270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f + 1 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));//<-
		model = glm::rotate(model, -270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f + 0 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));//<-
		model = glm::rotate(model, -270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 0 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 1 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 2 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 3 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 4 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 5 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 6 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 7 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 8 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 9 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 10 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 11 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 12 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 13 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 14 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, -6.4f + 0.8f + JardinZ_4 + recorrerEntradaZ - 4.8f));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasInv_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(4 * 3.2f + 0.8f), 0.0f, -6.4f + JardinZ_4 + recorrerEntradaZ - 4.0f));
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(3 * 3.2f + 0.8f), 0.0f, -6.4f + JardinZ_4 + recorrerEntradaZ - 4.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(2 * 3.2f + 0.8f), 0.0f, -6.4f + JardinZ_4 + recorrerEntradaZ - 4.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(1 * 3.2f + 0.8f), 0.0f, -6.4f + JardinZ_4 + recorrerEntradaZ - 4.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, -6.4f + 0.8f + JardinZ_4 + recorrerEntradaZ + 2.4f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(recorrerEntrada), 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_4 + 2.4f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasExt_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(recorrerEntrada), 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(recorrerEntrada), 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(recorrerEntrada), 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(recorrerEntrada), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasExt_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 1 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 2 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 3 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 4 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 5 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 6 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 7 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 8 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 9 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 10 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 11 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 12 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 13 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 14 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_4));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_4));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();



		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// Jardin esquina inferior izquierda

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(recorrerEntrada, 0.0f, -6.4f + 0.8f + JardinZ_5));
		//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasExt_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f + 4 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f + 3 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f + 2 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f + 1 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 0 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(recorrerEntrada, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_5));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(recorrerEntrada, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_5));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(recorrerEntrada, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_5));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasInv_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4 * 3.2f + 0.8f, 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3 * 3.2f + 0.8f, 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2 * 3.2f + 0.8f, 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1 * 3.2f + 0.8f, 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(recorrerEntrada, 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasExt_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_5));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_5));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_5));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_5));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_5));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 1 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 2 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 3 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 4 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 5 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 6 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 7 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 8 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 9 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 10 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 11 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 12 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 13 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 14 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f - 15.25 * 3.2f, 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();


		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// Jardin esquina inferior derecha

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(recorrerEntrada), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(-0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasExt_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f + 4 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f + 3 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f + 2 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f + 1 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 0 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 1 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 2 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 3 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 4 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 5 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 6 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 7 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 8 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 9 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 10 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 11 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 12 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 13 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 14 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(recorrerEntrada), 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(recorrerEntrada), 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(recorrerEntrada), 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasInv_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(4 * 3.2f + 0.8f), 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(3 * 3.2f + 0.8f), 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(2 * 3.2f + 0.8f), 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(1 * 3.2f + 0.8f), 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(recorrerEntrada), 0.0f, -6.4f + 0.8f + JardinZ_5 + recorrerEntradaZ));
		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(-0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejasExt_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(0.0f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 1 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 2 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 3 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 4 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 5 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 6 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 7 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 8 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 9 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 10 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 11 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 12 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 13 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 14 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();
		//
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 11.25 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 10 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 9 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 8 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 7 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 6 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 5 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 4 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 3 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 2 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, (-6.4f - 1 * (3.2f)) + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Reja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(JardinX_10 + (-1)*(-0.8f - 15.25 * 3.2f), 0.0f, -6.4f + 0.8f + JardinZ_5));
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		esquinaRejas_M.RenderModel();

		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================

		//Bancas
		//Izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f + 6 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 2 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 8 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 16 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 22 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 30 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 36 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 44 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		//Derecha

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_1, 0.0f, -5.0f + (-6.4f + 6 * (3.2f))));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_1, 0.0f, (-6.4f - 2 * (3.2f))));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_1, 0.0f, (-6.4f - 8 * (3.2f))));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_1, 0.0f, (-6.4f - 16 * (3.2f))));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_1, 0.0f, (-6.4f - 22 * (3.2f))));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_1, 0.0f, (-6.4f - 30 * (3.2f))));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_1, 0.0f, (-6.4f - 36 * (3.2f))));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_1, 0.0f, (-6.4f - 44 * (3.2f))));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();


		//Extremos

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_2, 0.0f, (-6.4f - 47 * (3.2f))));
		//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_3, 0.0f, (-6.4f - 47 * (3.2f))));
		//model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_2, 0.0f, (-6.4f + 8 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + bancasX_3, 0.0f, (-6.4f + 8 * (3.2f))));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Banca_M.RenderModel();

		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================

		//Botes de basura y luminarias
		//Izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f + 8 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tverde.UseTexture();
		greenBin_M.RenderModel();
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 5.75 * (3.2f) + 2.0f)));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tgris.UseTexture();
		greenBin_M.RenderModel();
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 19.0 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tverde.UseTexture();
		greenBin_M.RenderModel();
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 33.75 * (3.2f) + 2.0f)));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tgris.UseTexture();
		greenBin_M.RenderModel();
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 47.0 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tverde.UseTexture();
		greenBin_M.RenderModel();
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();

		//Derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + botesX_1, 0.0f, (-6.4f + 8 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tgris.UseTexture();
		greenBin_M.RenderModel();
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + botesX_1, 0.0f, (-6.4f - 5.75 * (3.2f) + 2.0f)));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tverde.UseTexture();
		greenBin_M.RenderModel();
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + botesX_1, 0.0f, (-6.4f - 19.0 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tgris.UseTexture();
		greenBin_M.RenderModel();
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + botesX_1, 0.0f, (-6.4f - 33.75 * (3.2f) + 2.0f)));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tverde.UseTexture();
		greenBin_M.RenderModel();
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + botesX_1, 0.0f, 0.5f + (-6.4f - 47.0 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tgris.UseTexture();
		greenBin_M.RenderModel();
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();

		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================

		//Jardín A ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -160.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -160.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, -160.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -160.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -160.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, -163.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -170.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -147.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		//Jardín B ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(75.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(92.0f, 0.0f, -160.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(73.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(61.0f, 0.0f, -160.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(35.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, 0.0f, -160.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(93.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(46.5f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(77.0f, 0.0f, -160.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(55.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(57.0f, 0.0f, -160.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(59.0f, 0.0f, -150.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(61.0f, 0.0f, -170.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(63.0f, 0.0f, -163.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(82.0f, 0.0f, -170.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(94.0f, 0.0f, -147.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		//Jardín C ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, 0.0f, -128.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, -127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-26.0f, 0.0f, -126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, -126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 0.0f, -115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-36.0f, 0.0f, -116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, -114.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-17.0f, 0.0f, -115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, -116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, 0.0f, -100.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, -104.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-26.0f, 0.0f, -101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, -102.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, -101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -124.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-21.0f, 0.0f, -123.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-11.0f, 0.0f, -122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-39.0f, 0.0f, -109.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-28.0f, 0.0f, -111.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-19.0f, 0.0f, -110.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, -112.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, -130.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, -101.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -115.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -106.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		//Jardín D ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 46.0f, 0.0f, -128.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, -127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 26.0f, 0.0f, -126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 15.0f, 0.0f, -127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 6.0f, 0.0f, -126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 45.0f, 0.0f, -115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 36.0f, 0.0f, -116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 25.0f, 0.0f, -114.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 17.0f, 0.0f, -115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 7.0f, 0.0f, -116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 46.0f, 0.0f, -100.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, -104.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 26.0f, 0.0f, -101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 15.0f, 0.0f, -102.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 6.0f, 0.0f, -101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 40.0f, 0.0f, -124.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 30.0f, 0.0f, -122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 21.0f, 0.0f, -123.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 11.0f, 0.0f, -122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 39.0f, 0.0f, -109.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 28.0f, 0.0f, -111.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 19.0f, 0.0f, -110.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 8.0f, 0.0f, -112.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 40.0f, 0.0f, -130.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, -101.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 10.0f, 0.0f, -115.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 5.0f, 0.0f, -106.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		//Jardín E ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, 0.0f, 45.0f - 128.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 45.0f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-26.0f, 0.0f, 45.0f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 45.0f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 45.0f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 0.0f, 45.0f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-36.0f, 0.0f, 45.0f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, 45.0f - 114.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-17.0f, 0.0f, 45.0f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 45.0f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, 0.0f, 45.0f - 100.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 45.0f - 104.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-26.0f, 0.0f, 45.0f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 45.0f - 102.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 45.0f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 45.0f - 124.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 45.0f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-21.0f, 0.0f, 45.0f - 123.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-11.0f, 0.0f, 45.0f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-39.0f, 0.0f, 45.0f - 109.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-28.0f, 0.0f, 45.0f - 111.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-19.0f, 0.0f, 45.0f - 110.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 45.0f - 112.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 45.0f - 130.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 45.0f - 101.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 45.0f - 115.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 45.0f - 106.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		//Jardín F ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 46.0f, 0.0f, 45.0f - 128.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, 45.0f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 26.0f, 0.0f, 45.0f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 15.0f, 0.0f, 45.0f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 6.0f, 0.0f, 45.0f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 45.0f, 0.0f, 45.0f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 36.0f, 0.0f, 45.0f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 25.0f, 0.0f, 45.0f - 114.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 17.0f, 0.0f, 45.0f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 7.0f, 0.0f, 45.0f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 46.0f, 0.0f, 45.0f - 100.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, 45.0f - 104.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 26.0f, 0.0f, 45.0f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 15.0f, 0.0f, 45.0f - 102.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 6.0f, 0.0f, 45.0f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 40.0f, 0.0f, 45.0f - 124.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 30.0f, 0.0f, 45.0f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 21.0f, 0.0f, 45.0f - 123.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 11.0f, 0.0f, 45.0f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 39.0f, 0.0f, 45.0f - 109.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 28.0f, 0.0f, 45.0f - 111.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 19.0f, 0.0f, 45.0f - 110.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 8.0f, 0.0f, 45.0f - 112.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 40.0f, 0.0f, 45.0f - 130.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, 45.0f - 101.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 10.0f, 0.0f, 45.0f - 115.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 5.0f, 0.0f, 45.0f - 106.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		//Jardín G ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, 0.0f, 89.5f - 128.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 89.5f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-26.0f, 0.0f, 89.5f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 89.5f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 89.5f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 0.0f, 89.5f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-36.0f, 0.0f, 89.5f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, 89.5f - 114.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-17.0f, 0.0f, 89.5f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 89.5f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, 0.0f, 89.5f - 100.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 89.5f - 104.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-26.0f, 0.0f, 89.5f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 89.5f - 102.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 89.5f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 89.5f - 124.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 89.5f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-21.0f, 0.0f, 89.5f - 123.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-11.0f, 0.0f, 89.5f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-39.0f, 0.0f, 89.5f - 109.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-28.0f, 0.0f, 89.5f - 111.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-19.0f, 0.0f, 89.5f - 110.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 89.5f - 112.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 89.5f - 130.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 89.5f - 101.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 89.5f - 115.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 89.5f - 106.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		//Jardín H ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 46.0f, 0.0f, 89.5f - 128.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, 89.5f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 26.0f, 0.0f, 89.5f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 15.0f, 0.0f, 89.5f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 6.0f, 0.0f, 89.5f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 45.0f, 0.0f, 89.5f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 36.0f, 0.0f, 89.5f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 25.0f, 0.0f, 89.5f - 114.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 17.0f, 0.0f, 89.5f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 7.0f, 0.0f, 89.5f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 46.0f, 0.0f, 89.5f - 100.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, 89.5f - 104.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 26.0f, 0.0f, 89.5f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 15.0f, 0.0f, 89.5f - 102.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 6.0f, 0.0f, 89.5f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 40.0f, 0.0f, 89.5f - 124.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 30.0f, 0.0f, 89.5f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 21.0f, 0.0f, 89.5f - 123.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 11.0f, 0.0f, 89.5f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 39.0f, 0.0f, 89.5f - 109.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 28.0f, 0.0f, 89.5f - 111.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 19.0f, 0.0f, 89.5f - 110.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 8.0f, 0.0f, 89.5f - 112.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 40.0f, 0.0f, 89.5f - 130.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, 89.5f - 101.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 10.0f, 0.0f, 89.5f - 115.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 5.0f, 0.0f, 89.5f - 106.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();


		//Jardín I ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, 0.0f, 134.0f - 128.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 134.0f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-26.0f, 0.0f, 134.0f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 134.0f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 134.0f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-45.0f, 0.0f, 134.0f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-36.0f, 0.0f, 134.0f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-25.0f, 0.0f, 134.0f - 114.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-17.0f, 0.0f, 134.0f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 134.0f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-46.0f, 0.0f, 134.0f - 100.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 134.0f - 104.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-26.0f, 0.0f, 134.0f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 134.0f - 102.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 134.0f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(37.0f - 29.0f, 0.0f, 134.0f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 134.0f - 124.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 134.0f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-21.0f, 0.0f, 134.0f - 123.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-11.0f, 0.0f, 134.0f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-39.0f, 0.0f, 134.0f - 109.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-28.0f, 0.0f, 134.0f - 111.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-19.0f, 0.0f, 134.0f - 110.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.0f, 0.0f, 134.0f - 112.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(42.0f - 29.0f, 0.0f, 30.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 134.0f - 130.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 134.0f - 101.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 134.0f - 115.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 134.0f - 106.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		//Jardín J ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 46.0f, 0.0f, 134.0f - 128.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, 134.0f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 26.0f, 0.0f, 134.0f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 15.0f, 0.0f, 134.0f - 127.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 6.0f, 0.0f, 134.0f - 126.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 45.0f, 0.0f, 134.0f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 36.0f, 0.0f, 134.0f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 25.0f, 0.0f, 134.0f - 114.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 17.0f, 0.0f, 134.0f - 115.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 7.0f, 0.0f, 134.0f - 116.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 46.0f, 0.0f, 134.0f - 100.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, 134.0f - 104.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 26.0f, 0.0f, 134.0f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 15.0f, 0.0f, 134.0f - 102.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 6.0f, 0.0f, 134.0f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(37.0f, 0.0f, 134.0f - 101.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 40.0f, 0.0f, 134.0f - 124.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 30.0f, 0.0f, 134.0f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 21.0f, 0.0f, 134.0f - 123.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 11.0f, 0.0f, 134.0f - 122.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 39.0f, 0.0f, 134.0f - 109.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 28.0f, 0.0f, 134.0f - 111.0f));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 19.0f, 0.0f, 134.0f - 110.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 8.0f, 0.0f, 134.0f - 112.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(42.0f, 0.0f, 30.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 40.0f, 0.0f, 134.0f - 130.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 35.0f, 0.0f, 134.0f - 101.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 10.0f, 0.0f, 134.0f - 115.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(97.5f - 5.0f, 0.0f, 134.0f - 106.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();


		// Fuentes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7 * 3.2f, 0.0f, -7 * 1.6f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		fuente_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7 * 3.2f, 0.0f, -80 * 1.6f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		fuente_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + Colibri_TX, 20.0f + Colibri_TY, 0.0f + Colibri_TZ));
		//model = glm::rotate(model, (-90 + Colibri_RX)* toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (90 + Colibri_RY)* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, (Colibri_RZ)* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Colibri.RenderModel();

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(-10.0f + Bicicleta_TX, 0.0f + Bicicleta_TY, 0.0f + Bicicleta_TZ));
		//model = glm::rotate(model, (-90 + Bicicleta_RX)* toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, ( 90 + Bicicleta_RY)* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, (Bicicleta_RZ)* toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bicicleta.RenderModel();

		//Animación compleja
		//tramo inicial
		if (t1)
		{
			posCarroZ -= movOffset * deltaTime * 4.0f;
			posCarroY = 5.0f*sin(lastTime * toRadians * 60.0f);
			if (posCarroZ <= -185.0f)
			{
				t1 = false;
				t2 = true;
			}
		}
		//primer giro a la derecha
		if (t2)
		{
			rotCarroY += movOffset * deltaTime * 16.0f;
			posCarroZ -= movOffset * deltaTime * 3.2f * cos(rotCarroY * toRadians);
			posCarroX += movOffset * deltaTime * 3.2f * sin(rotCarroY * toRadians);
			posCarroY = 5.0f * sin(lastTime * toRadians * 60.0f);
			if (rotCarroY >= 90.0f)
			{
				t2 = false;
				t4 = true;
			}
		}

		//continua
		if (t4)
		{
			posCarroX += movOffset * deltaTime * 4.0f;
			posCarroY = 5.0f * sin(lastTime * toRadians * 60.0f);
			if (posCarroX >= 100.0f)
			{
				t4 = false;
				t8 = true;
			}
		}
		//segundo giro a la derecha
		if (t8)
		{
			rotCarroY += movOffset * deltaTime * 16.0f;
			posCarroZ -= movOffset * deltaTime * 3.2f * cos(rotCarroY * toRadians);
			posCarroX += movOffset * deltaTime * 3.2f * sin(rotCarroY * toRadians);
			posCarroY = 5.0f * sin(lastTime * toRadians * 60.0f);
			if (rotCarroY >= 180.0)
			{
				t8 = false;
				t9 = true;
			}
		}
		//continua
		if (t9)
		{
			posCarroZ += movOffset * deltaTime * 4.0f;
			posCarroY = 5.0f * sin(lastTime * toRadians * 60.0f);
			if (posCarroZ >= 45.0f)
			{
				t9 = false;
				t10 = true;
			}
		}
		//tercer giro a la derecha
		if (t10)
		{
			rotCarroY += movOffset * deltaTime * 16.0f;
			posCarroZ -= movOffset * deltaTime * 3.2f * cos(rotCarroY * toRadians);
			posCarroX += movOffset * deltaTime * 3.2f * sin(rotCarroY * toRadians);
			posCarroY = 5.0f * sin(lastTime * toRadians * 60.0f);
			if (rotCarroY >= 270.0)
			{
				t10 = false;
				t11 = true;
			}
		}

		//continua
		if (t11)
		{
			posCarroX -= movOffset * deltaTime * 4.0f;
			posCarroY = 5.0f * sin(lastTime * toRadians * 60.0f);
			if (posCarroX <= -60.0f)
			{
				t11 = false;
				t16 = true;
			}
		}
		//cuarto giro a la derecha
		if (t16)
		{
			rotCarroY += movOffset * deltaTime * 16.0f;
			posCarroZ -= movOffset * deltaTime * 3.2f * cos(rotCarroY * toRadians);
			posCarroX += movOffset * deltaTime * 3.2f * sin(rotCarroY * toRadians);
			posCarroY = 5.0f * sin(lastTime * toRadians * 60.0f);
			if (rotCarroY >= 360.0f)
			{
				rotCarroY = 0.0f;
				t16 = false;
				t1 = true;
			}
		}



		//Colibri
		//Avanza
		if (Estado_Colibri == 0 && Colibri_TX < 40.0f) {
			Colibri_TX += 0.1;
		}
		//Gira
		if (Colibri_TX >= 40.0f) {
			Estado_Colibri = 3;
		}
		if (Estado_Colibri == 3 && Colibri_TX > 30.0f) {
			if (Colibri_RZ <= 0.0) {
				Colibri_RZ += 10;
				Colibri_TZ -= 0.3;
				Colibri_TX += 0.3;
			}
			if (Colibri_RZ > 0.0 && Colibri_RZ < 180.0) {
				Colibri_RZ += 10;
				Colibri_TZ -= 0.3;
				Colibri_TX -= 0.3;
			}
		}
		if (Estado_Colibri == 3 && Colibri_RZ >= 180.0f) {
			Estado_Colibri = 4;
			Colibri_RZ = 180;
		}
		//Retrocede
		if (Estado_Colibri == 4 && (Colibri_TX > -8.5f)) {
			Colibri_TX -= 0.1;
		}
		if (Colibri_TX <= -8.5f) {
			Estado_Colibri = 5;
		}
		//Gira otra vez
		if (Estado_Colibri == 5 && Colibri_RZ < 360.0f) {
			if (Colibri_RZ >= 0.0) {
				Colibri_RZ += 10;
				Colibri_TZ += 0.3;
				Colibri_TX -= 0.3;
			}
			if (Colibri_RZ < 0.0 && Colibri_RZ > 1270.0) {
				Colibri_RZ += 10;
				Colibri_TZ -= 0.3;
				Colibri_TX += 0.3;
			}
		}
		if (Estado_Colibri == 5 && Colibri_RZ >= 360.0f) {
			Estado_Colibri = 0;
		}



		//
		if (Bicicleta_TX < 10.0) {
			Bicicleta_TX += 0.2;
		}
		if (Bicicleta_TX >= 10.0) {
			if (Bicicleta_RZ <= 10) {
				Bicicleta_RZ += 2;
				Bicicleta_TY += 0.1;
			}
			Bicicleta_TX += 0.2;
		}
		if (Bicicleta_TX >= 15 && Bicicleta_TX < 25) {
			if (Bicicleta_RZ >= 0) {
				Bicicleta_RZ -= 2;
				Bicicleta_TY -= 0.1;
			}
			else {
				Estado_Bicicleta = 1;
			}
			Bicicleta_TX += 0.2;
		}
		if (Estado_Bicicleta == 1) {
			if (Bicicleta_TX < 30.0) {
				Bicicleta_TX += 0.2;
			}
		}
		//

		model = glm::mat4(1.0);
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(posCarroX, 7.0f + posCarroY, posCarroZ));
		model = glm::rotate(model, (-90.0f + rotCarroY) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::rotate(model, rotCarroX * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		color = glm::vec3(0.1f, 0.1f, 0.1f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		nave_M.RenderModel();

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////Modelado Jerárquico///////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(23.5f, 0.0f, -69.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		aux = model;
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		base_M.RenderModel();


		model = aux;
		model = glm::translate(model, glm::vec3(11.0f, 15.0f, 26.5f));
		//model = glm::scale(model, glm::vec3(0.f, 0.075f, 0.075f));
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		columna_M.RenderModel();

		model = glm::rotate(model, -45.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.0f, 0.5f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		barandalRecto_M.RenderModel();

		model = aux;
		model = glm::translate(model, glm::vec3(-11.0f, 15.0f, 26.5f));
		//model = glm::scale(model, glm::vec3(0.f, 0.075f, 0.075f));
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		columna_M.RenderModel();



		//
		model = aux;
		model = glm::translate(model, glm::vec3(11.0f, 15.0f, -26.5f));
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		columna_M.RenderModel();

		model = aux;
		model = glm::translate(model, glm::vec3(-11.0f, 15.0f, -26.5f));
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		columna_M.RenderModel();

		model = glm::rotate(model, 135.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.0f, 0.5f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		barandalRecto_M.RenderModel();

		//
		model = aux;
		model = glm::translate(model, glm::vec3(26.5f, 15.0f, -11.0f));
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		columna_M.RenderModel();

		model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.0f, 0.5f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		barandalRecto_M.RenderModel();


		model = aux;
		model = glm::translate(model, glm::vec3(26.5f, 15.0f, 11.0f));
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		columna_M.RenderModel();

		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.0f, 0.5f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		barandalRecto_M.RenderModel();

		//
		model = aux;
		model = glm::translate(model, glm::vec3(-26.5f, 15.0f, -11.0f));
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		columna_M.RenderModel();

		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.0f, 0.5f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		barandalRecto_M.RenderModel();


		model = aux;
		model = glm::translate(model, glm::vec3(-26.5f, 15.0f, 11.0f));
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		columna_M.RenderModel();

		model = glm::rotate(model, 225.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		color = glm::vec3(0.0f, 0.5f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		barandalRecto_M.RenderModel();

		// Techo
		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 55.0f, 0.0f));
		color = glm::vec3(.5f, .16f, .16f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		octagono_M.RenderModel();

		color = glm::vec3(.6f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		triangulo_M.RenderModel();

		model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		triangulo_M.RenderModel();

		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		triangulo_M.RenderModel();

		model = glm::rotate(model, 135.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		triangulo_M.RenderModel();

		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		triangulo_M.RenderModel();

		model = glm::rotate(model, 225.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		triangulo_M.RenderModel();

		model = glm::rotate(model, 270.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		triangulo_M.RenderModel();

		model = glm::rotate(model, 315.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		triangulo_M.RenderModel();


		// Escalones Norte
		// 1ª Capa
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 29.322f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		// 2ª Capa
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, 29.322f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		// 3ª Capa
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, 29.322f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		// 4ª Capa
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 9.0f, 29.322f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		// 5ª Capa
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 12.0f, 29.322f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		// Escalones Sur
		// 1ª Capa
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -29.322f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		// 2ª Capa
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -29.322f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		// 3ª Capa
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 6.0f, -29.322f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		// 4ª Capa
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 9.0f, -29.322f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		// 5ª Capa
		color = glm::vec3(0.3f, 0.3f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 12.0f, -29.322f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		escalon_M.RenderModel();

		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////// Avatares ///////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Grandpa Rick
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f + movXAvatar1, 2.0f, 0.0f + movZAvatar1));
		model = glm::rotate(model, headingAvatar1 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		aux = model;
		/*color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		torax_M.RenderModel();

		model = aux;
		model = glm::translate(model, glm::vec3(9.5f, 5.0f, 0.0));
		model = glm::rotate(model, rotacionAvatar1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		brazoIzq_M.RenderModel();

		model = glm::translate(model, glm::vec3(5.0f, -10.0f, -3.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		manoIzq_M.RenderModel();


		model = aux;
		model = glm::translate(model, glm::vec3(-9.5f, 5.0f, 0.0));
		model = glm::rotate(model, rotacionAvatar1 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazoDer_M.RenderModel();

		model = glm::translate(model, glm::vec3(-5.0f, -10.0f, -3.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		manoDer_M.RenderModel();

		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		cabezaRick_M.RenderModel();

		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, -12.0f, 0.0));
		color = glm::vec3(0.36f, 0.2f, 0.09f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RScadera_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, -6.0f, 0.0f));
		aux = model;
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0));
		color = glm::vec3(0.36f, 0.2f, 0.09f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::rotate(model, rotacionAvatar1 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		piernaIzq_M.RenderModel();

		model = aux;
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0));
		model = glm::rotate(model, rotacionAvatar1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.36f, 0.2f, 0.09f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		piernaDer_M.RenderModel();

		// Ryougi Shiki
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f + movXAvatar2, 2.0f, 0.0f + movZAvatar2));
		model = glm::rotate(model, headingAvatar2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		aux = model;
		/*color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RStorax_M.RenderModel();

		model = aux;
		model = glm::translate(model, glm::vec3(9.5f, 5.0f, 0.0));
		model = glm::rotate(model, rotacionAvatar2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RSbrazoIzq_M.RenderModel();

		model = glm::translate(model, glm::vec3(5.0f, -10.0f, -3.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		RSmanoIzq_M.RenderModel();


		model = aux;
		model = glm::translate(model, glm::vec3(-9.5f, 5.0f, 0.0));
		model = glm::rotate(model, rotacionAvatar2 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		RSbrazoDer_M.RenderModel();

		model = glm::translate(model, glm::vec3(-5.0f, -10.0f, -3.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		RSmanoDer_M.RenderModel();

		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RScabeza_M.RenderModel();

		model = aux;
		model = glm::translate(model, glm::vec3(0.0f, -12.0f, 0.0));
		color = glm::vec3(0.36f, 0.2f, 0.09f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RScadera_M.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, -6.0f, 0.0f));
		aux = model;
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0));
		color = glm::vec3(0.36f, 0.2f, 0.09f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::rotate(model, rotacionAvatar2 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RSpiernaIzq_M.RenderModel();

		model = aux;
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0));
		model = glm::rotate(model, rotacionAvatar2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.36f, 0.2f, 0.09f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RSpiernaDer_M.RenderModel();


		//model = aux;
		//model = glm::translate(model, glm::vec3(0.0f, .5f, 0.0));
		////model = glm::scale(model, glm::vec3(0.f, 0.075f, 0.075f));
		////color = glm::vec3(0.5f, 0.5f, 0.0f);
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//cabezaRick_M.RenderModel();


		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -1.7f, -2.0f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		////blending: transparencia o traslucidez
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//Tagave.UseTexture();
		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[3]->RenderMesh();




		glDisable(GL_BLEND);
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	//if (keys[GLFW_KEY_L])
	//{
	//	if (guardoFrame < 1)
	//	{
	//		saveFrame();
	//		printf("movModelo_x es: %f\n", movModelo_x);
	//		//printf("movModelo_y es: %f\n", movModelo_y);
	//		printf("presiona P para habilitar guardar otro frame'\n");
	//		guardoFrame++;
	//		reinicioFrame = 0;
	//	}
	//}
	if (keys[GLFW_KEY_Z])
	{
		espectaculo = true;
	}

	if (keys[GLFW_KEY_X])
	{
		espectaculo = false;
	}

	// Control de Grandpa Rick
	if (keys[GLFW_KEY_UP])
	{
		if (cambia_sentido1)
		{
			rotacionAvatar1 += 13.0f;
			if (rotacionAvatar1 >= 60.0f)
			{
				cambia_sentido1 = false;
			}
		}
		else
		{
			rotacionAvatar1 -= 13.0f;
			if (rotacionAvatar1 <= -60.0f)
			{
				cambia_sentido1 = true;
			}
		}
		headingAvatar1 = 180.0f;
		movZAvatar1 -= 0.3f;

	}

	if (keys[GLFW_KEY_RIGHT])
	{
		if (cambia_sentido1)
		{
			rotacionAvatar1 += 13.0f;
			if (rotacionAvatar1 >= 60.0f)
			{
				cambia_sentido1 = false;
			}
		}
		else
		{
			rotacionAvatar1 -= 13.0f;
			if (rotacionAvatar1 <= -60.0f)
			{
				cambia_sentido1 = true;
			}
		}
		headingAvatar1 = 90.0f;
		movXAvatar1 += 0.3f;

	}

	if (keys[GLFW_KEY_DOWN])
	{
		if (cambia_sentido1)
		{
			rotacionAvatar1 += 13.0f;
			if (rotacionAvatar1 >= 60.0f)
			{
				cambia_sentido1 = false;
			}
		}
		else
		{
			rotacionAvatar1 -= 13.0f;
			if (rotacionAvatar1 <= -60.0f)
			{
				cambia_sentido1 = true;
			}
		}
		headingAvatar1 = 0.0f;
		movZAvatar1 += 0.3f;

	}

	if (keys[GLFW_KEY_LEFT])
	{
		if (cambia_sentido1)
		{
			rotacionAvatar1 += 13.0f;
			if (rotacionAvatar1 >= 60.0f)
			{
				cambia_sentido1 = false;
			}
		}
		else
		{
			rotacionAvatar1 -= 13.0f;
			if (rotacionAvatar1 <= -60.0f)
			{
				cambia_sentido1 = true;
			}
		}
		headingAvatar1 = 270.0f;
		movXAvatar1 -= 0.3f;

	}
	// Control de Ryougi Shiki
	if (keys[GLFW_KEY_I])
	{
		if (cambia_sentido2)
		{
			rotacionAvatar2 += 13.0f;
			if (rotacionAvatar2 >= 60.0f)
			{
				cambia_sentido2 = false;
			}
		}
		else
		{
			rotacionAvatar2 -= 13.0f;
			if (rotacionAvatar2 <= -60.0f)
			{
				cambia_sentido2 = true;
			}
		}
		headingAvatar2 = 180.0f;
		movZAvatar2 -= 0.3f;

	}

	if (keys[GLFW_KEY_L])
	{
		if (cambia_sentido2)
		{
			rotacionAvatar2 += 13.0f;
			if (rotacionAvatar2 >= 60.0f)
			{
				cambia_sentido2 = false;
			}
		}
		else
		{
			rotacionAvatar2 -= 13.0f;
			if (rotacionAvatar2 <= -60.0f)
			{
				cambia_sentido2 = true;
			}
		}
		headingAvatar2 = 90.0f;
		movXAvatar2 += 0.3f;

	}

	if (keys[GLFW_KEY_K])
	{
		if (cambia_sentido2)
		{
			rotacionAvatar2 += 13.0f;
			if (rotacionAvatar2 >= 60.0f)
			{
				cambia_sentido2 = false;
			}
		}
		else
		{
			rotacionAvatar2 -= 13.0f;
			if (rotacionAvatar2 <= -60.0f)
			{
				cambia_sentido2 = true;
			}
		}
		headingAvatar2 = 0.0f;
		movZAvatar2 += 0.3f;

	}

	if (keys[GLFW_KEY_J])
	{
		if (cambia_sentido2)
		{
			rotacionAvatar2 += 13.0f;
			if (rotacionAvatar2 >= 60.0f)
			{
				cambia_sentido2 = false;
			}
		}
		else
		{
			rotacionAvatar2 -= 13.0f;
			if (rotacionAvatar2 <= -60.0f)
			{
				cambia_sentido2 = true;
			}
		}
		headingAvatar2 = 270.0f;
		movXAvatar2 -= 0.3f;

	}
	//////////////////////

	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movModelo_x es: %f\n", movModelo_x);
			movModelo_x += 1.0f;
			printf("movModelo_x es: %f\n", movModelo_x);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}

}
