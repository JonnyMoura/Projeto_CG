/*

Projeto CG

João Miguel Fernandes Moura-2020235800


*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "RgbImage.h"



#define PI		 3.14159

//--------------------------------- Definir cores
#define BLUE     0.0, 0.0, 1.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 0.5, 0.2, 1.0
#define ORANGE   0.8, 0.2, 0.1, 1.0
#define CYAN     0.0, 1.0, 1.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.3, 0.3, 0.3, 1.0



//----------------------------------------------Material
GLint     material = 16;

//================================================================================
//------------------------------------------------------------ Sistema Coordenadas 
GLint		wScreen = 700, hScreen = 600;		//.. janela - pixeis


//========================================================= Animacao Avião
GLfloat	  rotacaoHelice = 0;
GLfloat   movAAsa = 0;
GLfloat   rotAAsa = 0;
GLint     giraHelice = 0;
GLint     abreAsa = 0;

//========================================================= Movimentação Aviao
GLfloat theta = 0.;
GLfloat vel = 1.;
GLfloat pos[] = { 0., 0., 0. };

//============================================================= Observador
GLfloat  rVisao = 8, aVisao = -0.5 * PI, incVisao = 1;
GLfloat  obsP[] = { rVisao * cos(aVisao), 2.0, rVisao * sin(aVisao) };


GLfloat	 anguloZ = 35;

//============================================================= Timer
GLint     msec = 50;

//============================================================= Materiais
void initMaterials(int material);

char Materiais[18][30] = {
	"Esmerald",  "Jade",  "obsidian",    "Pearl",        "Ruby",
	"Turquoise", "Brass", "Bronze",      "Chrome",       "Copper",
	"Gold",      "Silver","blackPlastic","cyankPlastic", "greenPlastic",
	"redPlastic", "whitePlastic","yellowPlastic" };



//=======================================================================Material transparente

GLfloat transp[] = {1,1,1,0.3};

//============================================================= Luz Ambiente

GLint   Dia = 0;     //:::   'D'  
GLfloat intensidadeDia = 0.2;
GLfloat luzGlobalCorAmb[4] = { intensidadeDia, intensidadeDia,intensidadeDia, 1.0 };   

//============================================================= Luz pontual no TETO
GLint   ligaTeto = 1;		 //:::   'T'  
GLfloat intensidadeT = 0.3;  //:::   'I'  
GLint   luzR = 1;		 	 //:::   'R'  
GLint   luzG = 1;			 //:::   'G'  
GLint   luzB = 1;			 //:::   'B'  
GLfloat localPos[4] = { 0.0, 3, 0.0, 0.0 };
GLfloat localCorAmb[4] = { 0, 0, 0, 0.0 };
GLfloat localCorDif[4] = { luzR, luzG, luzB, 1.0 };
GLfloat localCorEsp[4] = { luzR, luzG, luzB, 1.0 };


//…………………………………………………………………………………………………………………………………………………………………………………………
GLfloat Pos1[] = { 2.4f, 0.4f,  0.0f, 1.0f };   // Foco 1
GLfloat Pos2[] = { 2.4f, 0.7f,  0.0f, 1.0f };   // Foco 2 


bool 		Focos[] = { 1,1 };		//.. Dois Focos ligados ou desligados
GLfloat		aberturaFoco = 20.0;		//.. angulo inicial do foco
GLfloat		anguloINC = 3.0;		//.. incremento
GLfloat		anguloMIN = 3.0;		//.. minimo
GLfloat		anguloMAX = 70.0;		//.. maximo

//…………………………………………………………………………………………………………………………………
GLint	  dim = 128;   //numero divisoes da grelha
bool malha = 1;   //ativa malha



//==================================================================== VERTEX ARRAY
//------------------------------------------- coordenadas + normais + cores

//...........................................Paralelepípedo
GLfloat tam = 0.5;
GLfloat comp = tam * (4 / 3);
GLfloat larg = tam / 2;
//......................cilindro

GLfloat raio = 0.125;
GLfloat cilindroComp = 0.100;

//.......................Prisma triangular
GLfloat tamP = 0.2;




static GLfloat vertices[] = {
	//............Faces..............................
	//…………………………………………………………………………………………………… Esquerda
		-comp,  -larg,  larg,	// 0 
		-comp,   larg,  larg,	// 1 
		-comp,   larg, -larg,	// 2 
		-comp,  -larg, -larg,	// 3 
		//…………………………………………………… Direita
				 comp,  -larg,  larg,	// 4 
				 comp,   larg,  larg,	// 5 
				 comp,   larg, -larg,	// 6 
				 comp,  -larg, -larg,	// 7 
				 //……………………………………………………… Cima
						 -comp,  larg,  larg,	// 8 
						 -comp,  larg, -larg,	// 9 
						  comp,  larg, -larg,	// 10 
						  comp,  larg,  larg,	// 11 
};


static GLfloat normais[] = {
	//…………………………………………………………………………………………………… x=Esquerda
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  //…………………………………………………………………………………………………… x=Direita
	  1.0,  0.0,  0.0,
	  1.0,  0.0,  0.0,
	  1.0,  0.0,  0.0,
	  1.0,  0.0,  0.0,
	  //…………………………………………………………………………………………………… y=Cima
	  0.0,  1.0,  0.0,
	  0.0,  1.0,  0.0,
	  0.0,  1.0,  0.0,
	  0.0,  1.0,  0.0,
	  //..........................................y=baixo
	  0.0,  -1.0,  0.0,
	  0.0,  -1.0,  0.0,
	  0.0,  -1.0,  0.0,
	  0.0,  -1.0,  0.0,
	  //..........................................z=frente
	   0.0,  0.0,  1.0,
	   0.0,  0.0,  1.0,
	   0.0,  0.0,  1.0,
	   0.0,  0.0,  1.0,
	   //..........................................z=tras

		0.0,  0.0,  -1.0,
		0.0,  0.0,  -1.0,
		0.0,  0.0,  -1.0,
		0.0,  0.0,  -1.0,


};


static GLfloat cores[] = {
	1., 0., 0.,
	1., 1., 0.,
	1., 1., 0.,
	1., 0., 0.,
	1., 0., 1.,
	0., 1., 0.,
	0., 0., 1.,
	1., 0., 1.,
	1., 1., 0.,
	1., 1., 0.,
	0., 0., 1.,
	0., 1., 0.,
};


//......................Faces da base...............
static GLuint cima[] = { 8, 11, 10, 9 };
static GLuint esquerda[] = { 0, 1, 2, 3 };
static GLuint direita[] = { 4, 7, 6, 5 };
static GLuint frente[] = { 0, 1, 5, 4 };
static GLuint tras[] = { 3, 2, 6, 7 };
static GLuint baixo[] = { 0, 3, 7, 4 };


//..............Texturas.............................

static GLfloat texturas[] = {
0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		0, 1,
		1, 1,
		1, 0,
		1, 0,
		1, 1,
		0, 1,
		0, 0, };

//-------------------------------------------------------Texturas
RgbImage imag;

GLuint   texture[5];
//------------------------------------------------------------ Skybox
GLUquadricObj* esfera = gluNewQuadric();

void initTexturas()
{
	//asas e cauda
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("3253472.bmp");
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	glBindTexture(GL_TEXTURE_2D, 0);


	//CORPO DO AVIAO
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	imag.LoadBmpFile("carbono.bmp");
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
	glBindTexture(GL_TEXTURE_2D, 0);

	//Hélices
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	imag.LoadBmpFile("metal_helix.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	glBindTexture(GL_TEXTURE_2D, 0);

	//--------------------------------------Sky
	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	imag.LoadBmpFile("sky.bmp");
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	glBindTexture(GL_TEXTURE_2D, 0);


	//……………………………………………………………………………………………………………………………  Textura
	glGenTextures(1, &texture[4]);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	imag.LoadBmpFile("metal_malha.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	glBindTexture(GL_TEXTURE_2D, 0);
}


void initLights(void) {
	//…………………………………………………………………………………………………………………………………………… Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
	//…………………………………………………………………………………………………………………………………………… Teto
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);



	GLfloat Foco_direccao[] = { 0, 0, -1, 0 };	//……… -Z
	GLfloat Foco1_cor[] = { 0, 1.5,  0, 1 };	//……… Cor da luz 1
	GLfloat Foco2_cor[] = { 1.5, 0,  0, 1 };	//……… Cor da luz 2
	GLfloat Foco_ak = 1.0;
	GLfloat Foco_al = 0.05f;
	GLfloat Foco_aq = 0.0f;
	GLfloat Foco_Expon = 2.0;		// Foco, SPOT_Exponent

	//……………………………………………………………………………………………………………………………Foco Esquerda
	glLightfv(GL_LIGHT1, GL_POSITION, Pos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Foco1_cor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, Foco_ak);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, Foco_al);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Foco_aq);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, aberturaFoco);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Foco_direccao);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, Foco_Expon);

	//……………………………………………………………………………………………………………………………Foco Direita
	glLightfv(GL_LIGHT2, GL_POSITION, Pos2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Foco2_cor);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, Foco_ak);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, Foco_al);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, Foco_aq);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, aberturaFoco);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, Foco_direccao);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, Foco_Expon);

}


void updateLuz() {
	localCorAmb[0] = luzR * intensidadeT;
	localCorAmb[1] = luzG * intensidadeT;
	localCorAmb[2] = luzB * intensidadeT;
	localCorDif[0] = luzR * intensidadeT;
	localCorDif[1] = luzG * intensidadeT;
	localCorDif[2] = luzB * intensidadeT;
	localCorEsp[0] = luzR * intensidadeT;
	localCorEsp[1] = luzG * intensidadeT;
	localCorEsp[2] = luzB * intensidadeT;;
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
}
//================================================================================
//=========================================================================== INIT

void initialize(void)
{
	glClearColor(WHITE);		//………………………………………………………………………………Apagar
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	

	glVertexPointer(3, GL_FLOAT, 0, vertices);	//………………………………………Posicoes
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);		//………………………………………Normais
	glEnableClientState(GL_NORMAL_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, cores);
	glEnableClientState(GL_COLOR_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texturas);   // coordenadas textura
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//==================================Luzes
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1); //fOCO 1
	glEnable(GL_LIGHT2); //fOCO 2
	

	initLights();
	
	
}

void iluminacao() {
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
	if (ligaTeto)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
}


void drawEixos()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo
	glColor4f(ORANGE);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.5 * 10, 0, 0);
	glEnd();
	glColor4f(GREEN);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.5 * 10, 0);
	glEnd();
	glColor4f(BLUE);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 0.5 * 10);
	glEnd();

}

void desenha() {
	glPushMatrix();
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	glPopMatrix();

	glPushMatrix();
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
	glPopMatrix();

	glPushMatrix();
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glPopMatrix();

	glPushMatrix();
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
	glPopMatrix();

	glPushMatrix();
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glPopMatrix();

	glPushMatrix();
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	glPopMatrix();

}
void drawEsfera()
{
	//------------------------- Esfera
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	gluQuadricDrawStyle(esfera, GLU_FILL);
	gluQuadricNormals(esfera, GLU_SMOOTH);
	gluQuadricTexture(esfera, GL_TRUE);
	gluSphere(esfera, 60.0, 100, 100);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

// prisma triangular(Asas e cauda)
void triangularPrism()
{



	
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);   glVertex3f(tamP, 0, tamP);
	glTexCoord2f(1.0f, 0.0f); 	glVertex3f(tamP, 0, -tamP);
	glTexCoord2f(1.0f, 1.0f);   glVertex3f(-tamP, 0, -tamP);
	glTexCoord2f(0.0f, 1.0f);  	glVertex3f(-tamP, 0, tamP);




	glTexCoord2f(0.0f, 0.0f);   glVertex3f(tamP, 0, tamP);
	glTexCoord2f(1.0f, 0.0f);	glVertex3f(tamP, 1, -tamP);
	glTexCoord2f(1.0f, 1.0f);	glVertex3f(-tamP, 1, -tamP);
	glTexCoord2f(0.0f, 1.0f);	glVertex3f(-tamP, 0, tamP);



	glTexCoord2f(0.0f, 0.0f);   glVertex3f(tamP, 1, -tamP);
	glTexCoord2f(1.0f, 0.0f);   glVertex3f(-tamP, 1, -tamP);
	glTexCoord2f(1.0f, 1.0f);   glVertex3f(-tamP, 0, -tamP);
	glTexCoord2f(0.0f, 1.0f);   glVertex3f(tamP, 0, -tamP);



	glEnd();
	



	
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(tamP, 0, tamP);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(tamP, 1, -tamP);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(tamP, 0, -tamP);

	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-tamP, 0, tamP);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-tamP, 1, -tamP);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-tamP, 0, -tamP);
	glEnd();




}


// ..............................Desenha cilindro (suporte das hélices)

void cylinder()
{
	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat angulo = 0.0;
	GLfloat angulo_stepsize = 0.1;

	glBegin(GL_QUAD_STRIP);
	angulo = 0.0;
	while (angulo < 2 * PI) {
		x = raio * cos(angulo);
		y = raio * sin(angulo);
		glVertex3f(x, y, cilindroComp);
		glVertex3f(x, y, 0.0);
		angulo = angulo + angulo_stepsize;
	}
	glVertex3f(raio, 0.0, cilindroComp);
	glVertex3f(raio, 0.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	angulo = 0.0;
	while (angulo < 2 * PI) {
		x = raio * cos(angulo);
		y = raio * sin(angulo);
		glVertex3f(x, y, cilindroComp);
		angulo = angulo + angulo_stepsize;
	}
	glVertex3f(raio, 0.0, cilindroComp);
	glEnd();
}

//......desenha hélices

void drawHelix() {
	glScalef(0.75, 0.25, 0.15);

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	

	glRotatef(90, 0.0, 0.0, 1.0);
	glScalef(3, 0.25, 1);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);



}



//======================================Desenho do veículo completo===========================
void drawScene() {

	int				i, j;
	float			med_dim = (float)dim / 2;



	if (giraHelice) {//........rotacao das helices(animação)
		rotacaoHelice += 25;
	}
	if (abreAsa) {//........abertura das asas
		movAAsa = larg;
		rotAAsa = -90;

	}

	//============================================Esferasverde e vermelha
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	//…………………………………………………………………………………Verde
	if (Focos[0]) {
		glPushMatrix();
		glColor3f(0.0f, 1.0f, 0.0f);
		glTranslatef(Pos1[0], Pos1[1], Pos1[2]);
		glutSolidSphere(0.1f, 100, 100);
		glPopMatrix();
	}
	//…………………………………………………………………………………Vermelha
	if (Focos[1]) {
		glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(Pos2[0], Pos2[1], Pos2[2]);
		glutSolidSphere(0.1f, 100, 100);
		glPopMatrix();
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glColor4f(GRAY);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	//==================================== Post de luz
	glPushMatrix();
	glTranslatef(2.7, 0.6, 0);
	glScalef(0.4, 6.5, 1);
	desenha();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(2.4,2.4,0);
	glScalef(1, 1, 1);
	desenha();
	glPopMatrix();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glPopMatrix();

	if (malha == 1) {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else {
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glPushMatrix();

		glNormal3f(0, 1, 0);          //normal 
		glScalef(1.5, 1, 1.5);
		glBegin(GL_QUADS);
		for (i = 0; i < dim; i++)
			for (j = 0; j < dim; j++) {
				glTexCoord2f((float)j / dim, (float)i / dim);
				glVertex3d((float)i / med_dim, -1, (float)j / med_dim);
				glTexCoord2f((float)(j + 1) / dim, (float)i / dim);
				glVertex3d(-(float)(i + 1) / med_dim, -1, (float)j / med_dim);
				glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);
				glVertex3d(-(float)(i + 1) / med_dim, -1, -(float)(j + 1) / med_dim);
				glTexCoord2f((float)j / dim, (float)(i + 1) / dim);
				glVertex3d((float)i / med_dim, -1, -(float)(j + 1) / med_dim);
			}
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D,0);
	
	

	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glRotatef(theta, 0, 1, 0);
	glScalef(2.5, 2.0, 2.5);
	glColor4f(BLACK);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	initMaterials(material);
	glPushMatrix();
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glColor4f(ORANGE);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glTranslatef(0.0, larg, 0.0);
	initMaterials(material);
	glPushMatrix();
	if (abreAsa) {
		glTranslatef(movAAsa, 0.0, 0.0);
		glRotatef(rotAAsa, 0.0, 0.0, 1.0);
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	triangularPrism();
	glPopMatrix();

	glColor4f(ORANGE);
	glRotatef(90, 1.0, 0.0, 0.0);
	glRotatef(-180, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0.0, tam, 0.0);
	initMaterials(material);
	glPushMatrix();
	if (abreAsa) {
		glTranslatef(-movAAsa, 0.0, 0.0);
		glRotatef(-rotAAsa, 0.0, 0.0, 1.0);

	}
	triangularPrism();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glTranslatef(0.0, -larg, comp);

	glColor4f(BLUE);
	initMaterials(material);
	glPushMatrix();
	cylinder();
	glPopMatrix();
	glTranslatef(0, 0, larg - 0.125);
	glRotatef(45, 0.0, 0, 1);
	initMaterials(material);
	glPushMatrix();
	glRotatef(rotacaoHelice, 0.0, 0.0, 1.0);
	drawHelix();
	glPopMatrix();

	glTranslatef(-larg / 1.5, larg / 1.5, 0);
	glTranslatef(0, 0, -comp * 2);
	glRotatef(45, 0, 0, 1);
	glScalef(1, 0.50, 1);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, transp);
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	triangularPrism();
	glPopMatrix();
	glDisable(GL_BLEND);
	

	glPopMatrix();
	



	

	








}




//====================================================== 
//====================================================== 
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//================================================================= Viewport 1
	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(85, (float)wScreen / hScreen, 0.1, 9999);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);



	//…………………………………………………………………………………………………………………………………………………………Objectos
	iluminacao();
	drawEixos();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	drawEsfera();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	drawScene();
	//…………………………………………………………………………………………………………………………………………………………



	//================================================================= Viewport 2
	glViewport(0, 0, wScreen * 0.5, hScreen * 0.5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos[0], 5, pos[2], pos[0], 0, pos[2], obsP[0], 0, obsP[2]);

	//…………………………………………………………………………………………………………………………………………………………Objectos
	iluminacao();
	drawEixos();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	drawEsfera();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	drawScene();
	//…………………………………………………………………………………………………………………………………………………………

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
	glutSwapBuffers();
}


void Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y) {//========= movimento do observador e inicio das animações



		switch (key) {
			//===================================== ILUMINACAO
		case 'O':
		case 'o':
			Focos[0] = !Focos[0];
			if (Focos[0] == 0)
				glDisable(GL_LIGHT1);
			else
				glEnable(GL_LIGHT1);
			glutPostRedisplay();
			break;
		case 'P':
		case 'p':
			Focos[1] = !Focos[1];
			if (Focos[1] == 0)
				glDisable(GL_LIGHT2);
			else
				glEnable(GL_LIGHT2);
			glutPostRedisplay();
			break;
//==========================================Ativar malha
		case 'H':
		case 'h':
			malha = !malha;
			glutPostRedisplay();
			break;

//=================================================Movimento eixo x
		case 'K': case 'k':
			localPos[0] -= 0.1;
				glutPostRedisplay();
				break;
		case 'L': case 'l':
			localPos[0] += 0.1;
			glutPostRedisplay();
			break;
//===================================================Movimento luz de teto eixo z
		case 'J': case 'j':
			localPos[2] += 0.1;
			glutPostRedisplay();
			break;
				
		case 'N': case 'n':
			localPos[2] -= 0.1;
			glutPostRedisplay();
			break;
			//===================================== ILUMINACAO
		case 't':case 'T':
			ligaTeto = !ligaTeto;
			updateLuz();
			glutPostRedisplay();
			break;
			//--------------------------- Iluminacaoda sala
		case 'i': case 'I':
			intensidadeT = intensidadeT + 0.1;
			if (intensidadeT > 1.1) intensidadeT = 0;
			updateLuz();
			glutPostRedisplay();
			break;
		case 'r':case 'R':
			luzR = (luzR + 1) % 2;
			updateLuz();
			glutPostRedisplay();
			break;
		case 'g':case 'G':
			luzG = (luzG + 1) % 2;
			updateLuz();
			glutPostRedisplay();
			break;
		case 'b':case 'B':
			luzB = (luzB + 1) % 2;
			updateLuz();
			glutPostRedisplay();
			break;

			//--------------------------- MAterial
		case 'm': case 'M':
			material = (material + 1) % 18;
			initMaterials(material);
			glutPostRedisplay();

			//---------------------------Animação

	case 'F': case 'f':
		giraHelice = !giraHelice;
		glutPostRedisplay();
		break;

	case 'V': case 'v':
		abreAsa = !abreAsa;
		glutPostRedisplay();
		break;

	case 'A': case 'a':
		aVisao += incVisao;
		obsP[0] = rVisao * cos(aVisao * PI / 180.);
		obsP[2] = rVisao * sin(aVisao * PI / 180.);
		glutPostRedisplay();
		break;
	case 'D': case'd':
		aVisao -= incVisao;
		obsP[0] = rVisao * cos(aVisao * PI / 180.);
		obsP[2] = rVisao * sin(aVisao * PI / 180.);
		glutPostRedisplay();
		break;

	case 'W': case 'w':
		obsP[1] += 0.5;
		glutPostRedisplay();
		break;

	case 'S': case 's':
		obsP[1] -= 0.5;
		glutPostRedisplay();
		break;


	case 27:
		exit(0);
		break;
	}

}



void teclasNotAscii(int key, int x, int y) {//======movimentação do veículo

	if (key == GLUT_KEY_UP) {
		pos[0] = pos[0] + vel * cos(theta * PI / 180.);
		pos[2] = pos[2] - vel * sin(theta * PI / 180.);
	}
	if (key == GLUT_KEY_DOWN) {
		pos[0] = pos[0] - vel * cos(theta * PI / 180.);
		pos[2] = pos[2] + vel * sin(theta * PI / 180.);
	}
	if (key == GLUT_KEY_DOWN || key == GLUT_KEY_UP) {//============sempre que o veículo se mexe as hélices rodam
		giraHelice = !giraHelice;
	}
	else {
		giraHelice = 0;
	}
	if (key == GLUT_KEY_LEFT)
		theta -= 3.;
	if (key == GLUT_KEY_RIGHT)
		theta += 3.;


	glutPostRedisplay();
}


//======================================================= MAIN
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("JMoura-Aviao  |Observador:'wasd'| |Movimento:'SETAS'| |RotacaoHelices:'f'| |Abrir/fechar asas:'v'| |Intensidade luz pontual:'i'| |Alterar RGB luz pontual:'r','g,'b'| |Alterar posicao eixo x luz pontual:'k'/'l'| |Alterar posicao eixo z luz pontual:'j'/'n'| |Ativar luz pontual:'t'| |Ativar malha:'h'| |Ativar cada foco:'o'/'p'| |Alterar materiais avião:'m'|");

	initialize();
	initTexturas();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}


