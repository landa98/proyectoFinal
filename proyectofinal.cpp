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


const float toRadians = 3.14159265f / 180.0f;
float movCoche;
float movOffset;
float rotapato;
bool avanza, patoadelante;
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

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

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
float posXavion = 2.0, posYavion = 2.0, posZavion = 0;
float	movAvion_x = 0.0f, movAvion_y = 0.0f, movAvion_z = 0.0f;
float giroAvion = 0;
int numFramesActual = 24 + 1;


#define MAX_FRAMES 25
int i_max_steps = 250;
int i_curr_steps = numFramesActual;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_z;		//Variable para PosicionZ
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float movAvion_zInc;		//Variable para IncrementoZ
	float giroAvion;
	float giroAvionInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = numFramesActual;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].movAvion_z = movAvion_z;
	KeyFrame[FrameIndex].giroAvion;

	FrameIndex++;
}

void resetElements(void)
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	movAvion_z = KeyFrame[0].movAvion_z;
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].movAvion_zInc = (KeyFrame[playIndex + 1].movAvion_z - KeyFrame[playIndex].movAvion_z) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

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
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			movAvion_z += KeyFrame[playIndex].movAvion_zInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
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


	PlaySound(TEXT("Silence.wav"), NULL, SND_LOOP | SND_ASYNC);


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
	tPisoAlameda = Texture("Textures/legoTextureCafe.tga");
	tPisoAlameda.LoadTextureA();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
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
	nave_M.LoadModel("Models/nave.obj");

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.3f, 1.0f,
		4.0f, 7.0f, -19.2f,
		0.0f, 0.05f, 0.1f);
	pointLightCount++;
	//4.0f, 0.0f, (-6.4f - 1 * (3.2f))
	unsigned int spotLightCount = 0;


	//linterna
	//spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
	//	0.0f, 2.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, -1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	20.0f);
	//spotLightCount++;

	////luz fija
	//spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
	//	0.0f, 2.0f,
	//	10.0f, 0.0f, 0.0f,
	//	0.0f, -5.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	10.0f);
	//spotLightCount++;
	////luz de faro
	//spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
	//	0.0f, 1.0f,
	//	0.0f, -1.5f, 0.0f,
	//	-4.0f, -1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	10.0f);
	//spotLightCount++;
	////luz de helicoptero
	//spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
	//	0.0f, 1.0f,
	//	2.0 - movCoche, 2.0f, 0.0f,
	//	0.0f, -5.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	10.0f);
	//spotLightCount++;
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
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	movCoche = 0.0f;
	movOffset = 1.0f;
	avanza = 1;

	//KEYFRAMES DECLARADOS INICIALES

	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion = 0;


	KeyFrame[1].movAvion_x = 1.0f;
	KeyFrame[1].movAvion_y = 2.0f;
	KeyFrame[1].giroAvion = 0;


	KeyFrame[2].movAvion_x = 2.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].giroAvion = 0;


	KeyFrame[3].movAvion_x = 3.0f;
	KeyFrame[3].movAvion_y = -2.0f;
	KeyFrame[3].giroAvion = 0;

	KeyFrame[4].movAvion_x = 4.0f;
	KeyFrame[4].movAvion_y = 0.0f;
	KeyFrame[4].giroAvion = 0.0f;

	//Agregar Kefyrame[5] para que el avión regrese al inicio
	KeyFrame[5].movAvion_x = 5.0f;
	KeyFrame[5].movAvion_y = 2.0f;
	KeyFrame[5].giroAvion = 0.0f;

	KeyFrame[6].movAvion_x = 6.0f;
	KeyFrame[6].movAvion_y = 0.0f;
	KeyFrame[6].giroAvion = 0.0f;

	KeyFrame[7].movAvion_x = 7.0f;
	KeyFrame[7].movAvion_y = -2.0f;
	KeyFrame[7].giroAvion = 0.0f;

	KeyFrame[8].movAvion_x = 8.0f;
	KeyFrame[8].movAvion_y = 0.0f;
	KeyFrame[8].giroAvion = 0.0f;

	KeyFrame[9].movAvion_x = 8.0f;
	KeyFrame[9].movAvion_y = 0.0f;
	KeyFrame[9].giroAvion = 45.0f;

	KeyFrame[10].movAvion_x = 8.0f;
	KeyFrame[10].movAvion_y = 0.0f;
	KeyFrame[10].giroAvion = 90.0f;

	KeyFrame[11].movAvion_x = 8.0f;
	KeyFrame[11].movAvion_y = 0.0f;
	KeyFrame[11].giroAvion = 135.0f;

	KeyFrame[12].movAvion_x = 8.0f;
	KeyFrame[12].movAvion_y = 0.0f;
	KeyFrame[12].giroAvion = 180.0f;

	KeyFrame[13].movAvion_x = 7.0f;
	KeyFrame[13].movAvion_y = 2.0f;
	KeyFrame[13].giroAvion = 180.0f;

	KeyFrame[14].movAvion_x = 6.0f;
	KeyFrame[14].movAvion_y = 0.0f;
	KeyFrame[14].giroAvion = 180.0f;

	KeyFrame[15].movAvion_x = 5.0f;
	KeyFrame[15].movAvion_y = -2.0f;
	KeyFrame[15].giroAvion = 180.0f;

	KeyFrame[16].movAvion_x = 4.0f;
	KeyFrame[16].movAvion_y = 0.0f;
	KeyFrame[16].giroAvion = 180.0f;

	KeyFrame[17].movAvion_x = 3.0f;
	KeyFrame[17].movAvion_y = 2.0f;
	KeyFrame[17].giroAvion = 180.0f;

	KeyFrame[18].movAvion_x = 2.0f;
	KeyFrame[18].movAvion_y = 0.0f;
	KeyFrame[18].giroAvion = 180.0f;

	KeyFrame[19].movAvion_x = 1.0f;
	KeyFrame[19].movAvion_y = -2.0f;
	KeyFrame[19].giroAvion = 180.0f;

	KeyFrame[20].movAvion_x = 0.0f;
	KeyFrame[20].movAvion_y = 0.0f;
	KeyFrame[20].giroAvion = 180.0f;

	KeyFrame[21].movAvion_x = 0.0f;
	KeyFrame[21].movAvion_y = 0.0f;
	KeyFrame[21].giroAvion = 135.0f;

	KeyFrame[22].movAvion_x = 0.0f;
	KeyFrame[22].movAvion_y = 0.0f;
	KeyFrame[22].giroAvion = 90.0f;

	KeyFrame[23].movAvion_x = 0.0f;
	KeyFrame[23].movAvion_y = 0.0f;
	KeyFrame[23].giroAvion = 45.0f;

	KeyFrame[24].movAvion_x = 0.0f;
	KeyFrame[24].movAvion_y = 0.0f;
	KeyFrame[24].giroAvion = 0.0f;

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

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		uniTime += movOffset * deltaTime;

		if (avanza)
		{
			if (movCoche < 10.0f)
			{
				//				movCoche += movOffset*deltaTime;
				movCoche += movOffset;
				avanza = 1;
			}
			else
			{
				avanza = 0;
			}
		}
		else
		{
			if (movCoche > -10.0f)
			{
				movCoche -= movOffset * deltaTime;
			}
			else
			{
				avanza = 1;
			}


		}


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

		//skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		//
		//Cambio de dia y noche

		//
		luzSol = ((sin(1 * uniTime * toRadians) + 1.0f) / 2) * 0.6f + 0.07f; // factor de 15

		mainLight = DirectionalLight(luzSol, luzSol, luzSol,
			0.9f, 0.9f,
			0.0f, -1.0f, 0.0f);

		if (luzSol <= 0.3)
		{
			pointLightCount = 1;
			skybox_Noche.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else
		{
			skybox_Dia.DrawSkybox(camera.calculateViewMatrix(), projection);
			pointLightCount = 0;
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		/*tiempo_Dia++;
		printf("%f \n", tiempo_Dia);
		if (tiempo_Dia <= 5000.0f) {
			skybox_Dia.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		if (tiempo_Dia > 5000.0f) {
			skybox_Noche.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		if (tiempo_Dia >= 10000.0) {
			tiempo_Dia = 0.0f;
		}*/


		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//pisoTexture.UseTexture();
		tPisoAlameda.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(movCoche, -1.4f, 0.5f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		////model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		////model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Kitt_M.RenderModel();
		//spotLights[2].SetPos(glm::vec3(movCoche, -1.5f, 0.0f));

		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXavion+movAvion_x,posYavion+movAvion_y, posZavion);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		/*model = glm::rotate(model, -90* toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));*/
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		nave_M.RenderModel();
		spotLights[3].SetPos(posblackhawk);
		//
		//
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -2.53f, 0.0f));
		////model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Camino_M.RenderModel();






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
		model = glm::translate(model, glm::vec3(4.0f + bancasX_1, 0.0f, (-6.4f + 6 * (3.2f))));
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

		//Botes de basura
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

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 5.75 * (3.2f) + 2.0f)));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tgris.UseTexture();
		greenBin_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 19.0 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tverde.UseTexture();
		greenBin_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 33.75 * (3.2f) + 2.0f)));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tgris.UseTexture();
		greenBin_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 47.0 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tverde.UseTexture();
		greenBin_M.RenderModel();


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

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + botesX_1, 0.0f, (-6.4f - 5.75 * (3.2f) + 2.0f)));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tverde.UseTexture();
		greenBin_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + botesX_1, 0.0f, (-6.4f - 19.0 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tgris.UseTexture();
		greenBin_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + botesX_1, 0.0f, (-6.4f - 33.75 * (3.2f) + 2.0f)));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tverde.UseTexture();
		greenBin_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f + botesX_1, 0.0f, (-6.4f - 47.0 * (3.2f))));
		//model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tgris.UseTexture();
		greenBin_M.RenderModel();

		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================
		// =========================================================================================


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, (-6.4f - 4 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Luminaria_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, (-6.4f - 3 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, (-6.4f - 1 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		shortTree_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, (-6.4f - 4 * (3.2f))));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bush_M.RenderModel();

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

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
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
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_x += 1.0f;
			printf("movAvion_x es: %f\n", movAvion_x);
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
