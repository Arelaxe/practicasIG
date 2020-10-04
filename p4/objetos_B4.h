//**************************************************************************
// Práctica 4
// Noelia Escalera Mejías
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include "file_ply_stl.hpp"

using namespace std;

const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID_CHESS,SOLID,SUAVIZADO_PLANO,SUAVIZADO_GOURAUD} _modo;
typedef enum{CILINDRO,CONO,ESFERA,PLY,CILINDROX,CONO_SIN_TAPA, CILINDRO_SIN_TAPA} _figura;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor);
void   draw_solido(float r, float g, float b);
void 	draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2);
void   draw_sombreado_plano();
void   draw_sombreado_gouraud();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

void	calcular_normales_caras();
void 	calcular_normales_vertices();

vector<_vertex3i> caras;

vector<_vertex3f> normales_caras;
vector<_vertex3f> normales_vertices;

bool b_normales_caras = false;
bool b_normales_vertices = false;

_vertex4f ambiente_difusa;     //coeficientes ambiente y difuso
_vertex4f especular;           //coeficiente especular
float brillo;   
};


//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=0.75);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

int   parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _rotacion: public _triangulos3D
{
public:
       _rotacion();
void  parametros(vector<_vertex3f> perfil1, int num1, _figura f, int al=2);

vector<_vertex3f> perfil; 
int num;
};


//************************************************************************
// objeto articulado: tanque
//************************************************************************

class _chasis: public _triangulos3D
{
public:
       _chasis();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float altura;

protected:
_rotacion  rodamiento;
_cubo  base;
};

//************************************************************************

class _torreta: public _triangulos3D
{
public:
       _torreta();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float altura;
float anchura;

protected:
_cubo  base;
_piramide parte_trasera;
};

//************************************************************************

class _tubo: public _triangulos3D
{
public:
       _tubo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
_rotacion tubo_abierto; // caña del cañón
};

//************************************************************************

class _tanque: public _triangulos3D
{
public:
       _tanque();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float giro_tubo;
float giro_torreta;

float giro_tubo_min;
float giro_tubo_max;

protected:
_chasis  chasis;
_torreta  torreta;
_tubo     tubo;
};

class _junta: public _triangulos3D
{
public:
       _junta();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
       _rotacion junta;
};

class _base_flexo: public _triangulos3D
{
public:
       _base_flexo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
       _cubo base;
};

class _semibrazo: public _triangulos3D
{
public:
       _semibrazo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
       _rotacion semi;
};


class _cabeza: public _triangulos3D
{
public:
       _cabeza();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, 
              float r3, float g3, float b3, float r4, float g4, float b4, float grosor);

protected:
       _rotacion bombilla;
       _rotacion cono;
       _rotacion cilindro;
};


class _flexo: public _triangulos3D
{
public:
       _flexo();
void   draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, 
			float r3, float g3, float b3, float r4, float g4, float b4,
			float r5, float g5, float b5, float r6, float g6, float b6, float grosor);

float giro_cabeza_z;
float giro_cabeza_z_min;
float giro_cabeza_z_max;

float giro_cabeza_y;
float giro_cabeza_y_min;
float giro_cabeza_y_max;

float giro_cabeza_semibrazo;
float giro_cabeza_semibrazo_min;
float giro_cabeza_semibrazo_max;

float giro_semibrazo_base;
float giro_semibrazo_base_min;
float giro_semibrazo_base_max;

float giro_flexo;

float salto;

protected:
       _cabeza cabeza;
       _semibrazo semibrazo1, semibrazo2;
       _base_flexo base;
       _junta junta1, junta2, junta3;
};
