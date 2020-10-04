//**************************************************************************
// Práctica 5
// Noelia Escalera Mejías
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include "file_ply_stl.hpp"

using namespace std;

const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID_CHESS,SOLID} _modo;
typedef enum{CILINDRO,CONO,ESFERA,PLY,CILINDROX,CONO_SIN_TAPA, CILINDRO_SIN_TAPA} _figura;
//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor, int cara_escogida=-1, bool color_normal=true, bool es_base=false);

vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor, int cara_escogida=-1, bool color_normal=true, bool es_base=false);
void    draw_solido(float r, float g, float b, int cara_escogida=-1, bool color_normal=true, bool es_base=false);
void 	draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2, int cara_escogida=-1, bool color_normal=true, bool es_base=false);
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor, int cara_escogida=-1, bool color_normal=true, bool es_base=false);
void draw_seleccion_color(float r, float g, float b);
void draw_seleccion_triangulos(float r, float g, float b);
// void draw sel triangulo. Cambia porque hay que meter el glcolor dentro del for. Los parámetros tendrán un cierto incremento

vector<_vertex3i> caras;
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
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor, bool es_base);
void   draw_sel(float r, float g, float b);

protected:
       _rotacion junta;
};

class _base_flexo: public _triangulos3D
{
public:
       _base_flexo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor, int cara_escogida, bool color_normal, bool es_base);
void   draw_sel(float r, float g, float b);

protected:
       _cubo base;
};

class _semibrazo: public _triangulos3D
{
public:
       _semibrazo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor, bool es_base);
void   draw_sel(float r, float b, float g);

protected:
       _rotacion semi;
};


class _cabeza: public _triangulos3D
{
public:
       _cabeza();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, 
              float r3, float g3, float b3, float r4, float g4, float b4, float grosor, bool es_base);
void   draw_sel(float r1, float g1, float b1, float r2, float g2, float b2);

protected:
       _rotacion bombilla;
       _rotacion cono;
       _rotacion cilindro;
};


class _flexo: public _triangulos3D
{
public:
       _flexo();
void   draw(_modo modo, float r1_cabeza, float g1_cabeza, float b1_cabeza,
                     float r2_cabeza, float g2_cabeza, float b2_cabeza, 
                     float r1_bombilla, float g1_bombilla, float b1_bombilla,
                     float r2_bombilla, float g2_bombilla, float b2_bombilla,
			float r1_base, float g1_base, float b1_base, 
                     float r2_base, float g2_base, float b2_base, 
                     float r1_semi1, float g1_semi1, float b1_semi1,
                     float r2_semi1, float g2_semi1, float b2_semi1,
                     float r1_semi2, float g1_semi2, float b1_semi2,
                     float r2_semi2, float g2_semi2, float b2_semi2,
			float r1_junta1, float g1_junta1, float b1_junta1,
                     float r2_junta1, float g2_junta1, float b2_junta1,
                     float r1_junta2, float g1_junta2, float b1_junta2,
                     float r2_junta2, float g2_junta2, float b2_junta2, 
                     float r1_junta3, float g1_junta3, float b1_junta3,
                     float r2_junta3, float g2_junta3, float b2_junta3,float grosor, int cara_escogida, bool color_normal, bool es_base);

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

//protected:
       _cabeza cabeza;
       _semibrazo semibrazo1, semibrazo2;
       _base_flexo base;
       _junta junta1, junta2, junta3;
};
