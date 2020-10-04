//**************************************************************************
// Práctica 5
// Noelia Escalera Mejías
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_5.h"


using namespace std;

// tipos
typedef enum{CUBO, PIRAMIDE, OBJETO_PLY, ROTACION, ARTICULADO, PROPIO} _tipo_objeto;
typedef enum{PERSPECTIVA, PARALELO, VISTAS} _tipo_proyeccion;
_tipo_objeto t_objeto=PROPIO;
_modo   modo=SOLID_CHESS;
bool anim_activa = false;
float grado_libertad = 0.02;
int cara_escogida = -1;
bool color_normal = false, es_base = false;
_tipo_proyeccion proyeccion = _tipo_proyeccion::PERSPECTIVA;
float r1_cabeza = 0.0, g1_cabeza = 0.9, b1_cabeza = 0.9, 
        r2_cabeza = 0.0, g2_cabeza = 0.8, b2_cabeza = 0.8, 
        r1_base = 0.0, g1_base = 0.9, b1_base = 0.9, 
        r2_base = 0.0, g2_base = 0.8, b2_base = 0.8, 
        r1_junta1 = 0.0, g1_junta1 = 0.0, b1_junta1 = 0.0, 
        r2_junta1 = 0.1, g2_junta1 = 0.1, b2_junta1 = 0.1, 
        r1_junta2 = 0.0, g1_junta2 = 0.0, b1_junta2 = 0.0, 
        r2_junta2 = 0.1, g2_junta2 = 0.1, b2_junta2 = 0.1, 
        r1_junta3 = 0.0, g1_junta3 = 0.0, b1_junta3 = 0.0,
        r2_junta3 = 0.1, g2_junta3 = 0.1, b2_junta3 = 0.1,
        r1_semi1 = 0.0, g1_semi1 = 0.0, b1_semi1 = 0.0, 
        r2_semi1 = 0.1, g2_semi1 = 0.1, b2_semi1 = 0.1, 
        r1_semi2 = 0.0, g1_semi2 = 0.0, b1_semi2 = 0.0, 
        r2_semi2 = 0.1, g2_semi2 = 0.1, b2_semi2 = 0.1, 
        r1_bombilla = 0.9, g1_bombilla = 0.9, b1_bombilla = 0.9,
        r2_bombilla = 0.8, g2_bombilla = 0.8, b2_bombilla = 0.8;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x,Size_y,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=50,Window_y=50,Window_width=450,Window_high=450;

int estado_raton[3], xc, yc, modo_color[8], cambio=0, modo_color_t[12];
int ancho=450, alto=450;
float factor=1.0;

void pick_color(int x, int y);


// objetos
_cubo cubo;
_piramide piramide(0.85,1.3);
_objeto_ply  ply; 
_rotacion rotacion; 
_tanque tanque;
_flexo flexo;

void anim1();
void anim2();
void anim3();
void anim4();

// _objeto_ply *ply1;


//**************************************************************************
//
//***************************************************************************

void clean_window()
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection_perspectiva()
{

glMatrixMode(GL_PROJECTION);
glLoadIdentity();

// formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
//  plano_delantero>0  plano_trasero>PlanoDelantero)
glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane); //glortho para paralelo params: -minx*escala,maxx*escala,-miny*escala,maxy*escala,-100,100
}//Se puede poner abajo un glScale en vez de poner minx*escala

void change_projection_paralelo()
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(-2*factor,2*factor,-2*factor,2*factor,-100,100);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer_perspectiva()
{
// posicion del observador
glViewport(0,0,ancho,alto); // cambiar ancho y alto para que se vea solo en un trozo
change_projection_perspectiva(); //if (mejor duplicar este también a parte del change projection y poner el if en draw)
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0,0,-Observer_distance); // Esto habría que quitarlo en el paralelo
glRotatef(Observer_angle_x,1,0,0);
glRotatef(Observer_angle_y,0,1,0);
}

void change_observer_paralelo()
{
// posicion del observador
glViewport(0,0,ancho,alto); 
change_projection_paralelo();
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glRotatef(Observer_angle_x,1,0,0);
glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
	
glDisable(GL_LIGHTING);
glLineWidth(2);
glBegin(GL_LINES);
// eje X, color rojo
glColor3f(1,0,0);
glVertex3f(-AXIS_SIZE,0,0);
glVertex3f(AXIS_SIZE,0,0);
// eje Y, color verde
glColor3f(0,1,0);
glVertex3f(0,-AXIS_SIZE,0);
glVertex3f(0,AXIS_SIZE,0);
// eje Z, color azul
glColor3f(0,0,1);
glVertex3f(0,0,-AXIS_SIZE);
glVertex3f(0,0,AXIS_SIZE);
glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects()
{

switch (t_objeto){
	case CUBO: cubo.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
	case PIRAMIDE: piramide.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
        case OBJETO_PLY: ply.draw(modo,1.0,0.6,0.0,0.0,1.0,0.3,2);break;
        case ROTACION: rotacion.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
        case ARTICULADO: tanque.draw(modo,0.5,0.7,0.2,0.3,0.6,0.3,2);break;
        case PROPIO: flexo.draw(modo, r1_cabeza, g1_cabeza, b1_cabeza, r2_cabeza, g2_cabeza, b2_cabeza, 
                                r1_bombilla, g1_bombilla, b1_bombilla, r2_bombilla, g2_bombilla, b2_bombilla,
				r1_base, g1_base, b1_base, r2_base, g2_base, b2_base, 
                                r1_semi1, g1_semi1, b1_semi1, r2_semi1, g2_semi1, b2_semi1,
                                r1_semi2, g1_semi2, b1_semi2, r2_semi2, g2_semi2, b2_semi2,
				r1_junta1, g1_junta1, b1_junta1, r2_junta1, g2_junta1, b2_junta1,
                                r1_junta2, g1_junta2, b1_junta2, r2_junta2, g2_junta2, b2_junta2, 
                                r1_junta3, g1_junta3, b1_junta3, r2_junta3, g2_junta3, b2_junta3,2,cara_escogida,color_normal,es_base);break;
	}
}

void procesar_color (unsigned char color[3]){
        switch(color[0]){
                case 100:
                if (modo_color_t[0]==0) 
                {
                modo_color_t[0]=1;
                cara_escogida=0;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[0]=0;
                cara_escogida=0;
                color_normal = true;
                es_base = true;
                }
                break;
                case 101:
                if (modo_color_t[1]==0) 
                {
                modo_color_t[1]=1;
                cara_escogida=1;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[1]=0;
                cara_escogida=1;
                color_normal = true;
                es_base = true;
                }
                break;
                case 102:
                if (modo_color_t[2]==0) 
                {
                modo_color_t[2]=1;
                cara_escogida=2;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[2]=0;
                cara_escogida=2;
                color_normal = true;
                es_base = true;
                }
                break;
                case 103:
                if (modo_color_t[3]==0) 
                {
                modo_color_t[3]=1;
                cara_escogida=3;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[3]=0;
                cara_escogida=3;
                color_normal = true;
                es_base = true;
                }
                break;
                case 104:
                if (modo_color_t[4]==0) 
                {
                modo_color_t[4]=1;
                cara_escogida=4;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[4]=0;
                cara_escogida=4;
                color_normal = true;
                es_base = true;
                }
                break;
                case 105:
                if (modo_color_t[5]==0) 
                {
                modo_color_t[5]=1;
                cara_escogida=5;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[5]=0;
                cara_escogida=5;
                color_normal = true;
                es_base = true;
                }
                break;
                case 106:
                if (modo_color_t[6]==0) 
                {
                modo_color_t[6]=1;
                cara_escogida=6;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[6]=0;
                cara_escogida=6;
                color_normal = true;
                es_base = true;
                }
                break;
                case 107:
                if (modo_color_t[7]==0) 
                {
                modo_color_t[7]=1;
                cara_escogida=7;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[7]=0;
                cara_escogida=7;
                color_normal = true;
                es_base = true;
                }
                break;
                case 108:
                if (modo_color_t[8]==0) 
                {
                modo_color_t[8]=1;
                cara_escogida=8;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[8]=0;
                cara_escogida=8;
                color_normal = true;
                es_base = true;
                }
                break;
                case 109:
                if (modo_color_t[9]==0) 
                {
                modo_color_t[9]=1;
                cara_escogida=9;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[9]=0;
                cara_escogida=9;
                color_normal = true;
                es_base = true;
                }
                break;
                case 110:
                if (modo_color_t[10]==0) 
                {
                modo_color_t[10]=1;
                cara_escogida=10;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[10]=0;
                cara_escogida=10;
                color_normal = true;
                es_base = true;
                }
                break;
                case 111:
                if (modo_color_t[11]==0) 
                {
                modo_color_t[11]=1;
                cara_escogida=11;
                color_normal = false;
                es_base = true;
                }
                else 
                {
                modo_color_t[11]=0;
                cara_escogida=11;
                color_normal = true;
                es_base = true;
                }
                break;
                case 120:
                if (modo_color[1]==0) 
                {
                modo_color[1]=1;
                r1_junta3 = 0.75;
                g1_junta3 = 0.75;
                b1_junta3 = 0.75;
                r2_junta3 = 0.65;
                g2_junta3 = 0.65;
                b2_junta3 = 0.65;
                cara_escogida=-1;
                es_base = false;
                }
                else 
                {
                modo_color[1]=0;
                r1_junta3 = 0.0;
                g1_junta3 = 0.0;
                b1_junta3 = 0.0;
                r2_junta3 = 0.1;
                g2_junta3 = 0.1;
                b2_junta3 = 0.1;
                cara_escogida=-1;
                es_base = false;
                }
                break;
                case 140:
                if (modo_color[2]==0) 
                {
                modo_color[2]=1;
                r1_semi2 = 0.75;
                g1_semi2 = 0.75;
                b1_semi2 = 0.75;
                r2_semi2 = 0.65;
                g2_semi2 = 0.65;
                b2_semi2 = 0.65;
                cara_escogida=-1;
                es_base = false;
                }
                else 
                {
                modo_color[2]=0;
                r1_semi2 = 0.0;
                g1_semi2 = 0.0;
                b1_semi2 = 0.0;
                r2_semi2 = 0.1;
                g2_semi2 = 0.1;
                b2_semi2 = 0.1;
                cara_escogida=-1;
                es_base = false;
                }
                break;
                case 160:
                if (modo_color[3]==0) 
                {
                modo_color[3]=1;
                r1_junta2 = 0.75;
                g1_junta2 = 0.75;
                b1_junta2 = 0.75;
                r2_junta2 = 0.65;
                g2_junta2 = 0.65;
                b2_junta2 = 0.65;
                cara_escogida=-1;
                es_base = false;
                }
                else 
                {
                modo_color[3]=0;
                r1_junta2 = 0.0;
                g1_junta2 = 0.0;
                b1_junta2 = 0.0;
                r2_junta2 = 0.1;
                g2_junta2 = 0.1;
                b2_junta2 = 0.1;
                cara_escogida=-1;
                es_base = false;
                }
                break;
                case 180:
                if (modo_color[4]==0) 
                {
                modo_color[4]=1;
                r1_semi1 = 0.75;
                g1_semi1 = 0.75;
                b1_semi1 = 0.75;
                r2_semi1 = 0.65;
                g2_semi1 = 0.65;
                b2_semi1 = 0.65;
                cara_escogida=-1;
                es_base = false;
                }
                else 
                {
                modo_color[4]=0;
                r1_semi1 = 0.0;
                g1_semi1 = 0.0;
                b1_semi1 = 0.0;
                r2_semi1 = 0.0;
                g2_semi1 = 0.0;
                b2_semi1 = 0.0;
                cara_escogida=-1;
                es_base = false;
                }
                break;
                case 200:
                if (modo_color[5]==0) 
                {
                modo_color[5]=1;
                r1_junta1 = 0.75;
                g1_junta1 = 0.75;
                b1_junta1 = 0.75;
                r2_junta1 = 0.65;
                g2_junta1 = 0.65;
                b2_junta1 = 0.65;
                cara_escogida=-1;
                es_base = false;
                }
                else 
                {
                modo_color[5]=0;
                r1_junta1 = 0.0;
                g1_junta1 = 0.0;
                b1_junta1 = 0.0;
                r2_junta1 = 0.1;
                g2_junta1 = 0.1;
                b2_junta1 = 0.1;
                cara_escogida=-1;
                es_base = false;
                }
                break;
                case 220:
                if (modo_color[6]==0) 
                {
                modo_color[6]=1;
                r1_cabeza = 0.3;
                g1_cabeza = 0.9;
                b1_cabeza = 0.3;
                r2_cabeza = 0.4;
                g2_cabeza = 0.9;
                b2_cabeza = 0.4;
                cara_escogida=-1;
                es_base = false;
                }
                else 
                {
                modo_color[6]=0;
                r1_cabeza = 0.0; 
                g1_cabeza = 0.9; 
                b1_cabeza = 0.9; 
                r2_cabeza = 0.0; 
                g2_cabeza = 0.8; 
                b2_cabeza = 0.8;
                cara_escogida=-1;
                es_base = false;
                }
                break;
                case 240: 
                if (modo_color[7]==0){
                modo_color[7]=1;
                r1_bombilla = 1.0;
                g1_bombilla = 0.5;
                b2_bombilla = 0.0;
                r2_bombilla = 0.9;
                g2_bombilla = 0.4;
                b2_bombilla = 0.0;
                cara_escogida=-1;
                es_base = false;
                }
                else{
                modo_color[7]=0;
                r1_bombilla = 0.9;
                g1_bombilla = 0.9;
                b1_bombilla = 0.9;
                r2_bombilla = 0.8;
                g2_bombilla = 0.8;
                b2_bombilla = 0.8;
                cara_escogida=-1;
                es_base = false;
                }
                break;
        }
}

void draw_objects_seleccion()
{
        if (t_objeto==_tipo_objeto::PROPIO){
                int inc=20;
                glTranslatef(0.0,flexo.salto,0.0);
	        glTranslatef(0.0,-1.5,0.0);
	        glPushMatrix();
	        glTranslatef(0,0.125,0);
	        flexo.base.draw_sel(100,100,100);
	        glPopMatrix();
	        glTranslatef(0,0.35,0);
	        glPushMatrix();
	        glRotatef(flexo.giro_flexo,0,1,0);
	        flexo.junta3.draw_sel(100+inc,100+inc,100+inc);
                inc+=20;
	        glRotatef(flexo.giro_semibrazo_base,0,0,1);
	        glTranslatef(0.0,0.1,0.0);
	        flexo.semibrazo2.draw_sel(100+inc,100+inc,100+inc);
                inc+=20;
	        glPushMatrix();
	        glTranslatef(0.0,1.35,0.0);
	        flexo.junta2.draw_sel(100+inc,100+inc,100+inc);
                inc+=20;
	        glRotatef(flexo.giro_cabeza_semibrazo,0,0,1);
	        glTranslatef(0.0,0.1,0.0);
	        flexo.semibrazo1.draw_sel(100+inc,100+inc,100+inc);
                inc+=20;
	        glTranslatef(0,1.35,0);
	        flexo.junta1.draw_sel(100+inc,100+inc,100+inc);
                inc+=20;
	        glPushMatrix();
	        glRotatef(flexo.giro_cabeza_y,0,1,0);
	        glTranslatef(0.0,0.35,0.0);
	        glRotatef(flexo.giro_cabeza_z,0,0,1);
	        glTranslatef(0.25,0.0,0.0);
	        glRotatef(90,0,0,1);
	        glTranslatef(0.0,-0.75,0.0);
	        flexo.cabeza.draw_sel(100+inc,100+inc,100+inc,120+inc,120+inc,120+inc);
	        glPopMatrix();
	        glPopMatrix();
	        glPopMatrix();
        }
}


//**************************************************************************
//
//***************************************************************************

void draw(void)
{
clean_window();

 //aquí va el if del modo cámara, meter dentro change observer draw axis y draw objects. Abajo también hay que hacerlo
               //para hacer varias vistas, varios change projection. Rotate en change projection para cambiar la vista
if (proyeccion==_tipo_proyeccion::PERSPECTIVA){
        change_observer_perspectiva();
        draw_axis();
        draw_objects();
}
else if (proyeccion==_tipo_proyeccion::PARALELO){
        change_observer_paralelo();
        draw_axis();
        draw_objects();
}
glDrawBuffer(GL_BACK);
clean_window();
if (proyeccion==_tipo_proyeccion::PERSPECTIVA){
        change_observer_perspectiva();
        draw_axis();
        draw_objects_seleccion();
}
else if (proyeccion==_tipo_proyeccion::PARALELO){
        change_observer_paralelo();
        draw_axis();
        draw_objects_seleccion();
}

glDrawBuffer(GL_FRONT);

glFlush();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
float Aspect_ratio;

Aspect_ratio=(float) Alto1/(float )Ancho1;
Size_y=Size_x*Aspect_ratio;
change_projection_perspectiva();
glViewport(0,0,Ancho1,Alto1);
glutPostRedisplay();
}


//**********-o*****************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_keys(unsigned char Tecla1,int x,int y)
{
switch (toupper(Tecla1)){
	case 'Q':exit(0);
	case '1':modo=POINTS;break;
	case '2':modo=EDGES;break;
	case '3':modo=SOLID;break;
	case '4':modo=SOLID_CHESS;break;
        case 'P':t_objeto=PIRAMIDE;break;
        case 'C':t_objeto=CUBO;break;
        case 'O':t_objeto=OBJETO_PLY;break;	
        case 'R':t_objeto=ROTACION;break;
        case 'A':t_objeto=ARTICULADO;break;
        case 'B':t_objeto=PROPIO;break;
        case 'M':if(anim_activa==false){anim_activa=true;}
                else {anim_activa=false;}break;
        case ',':flexo.giro_semibrazo_base+=5;
                        if (flexo.giro_semibrazo_base>flexo.giro_semibrazo_base_max) flexo.giro_semibrazo_base=flexo.giro_semibrazo_base_max;
                        break;break;
        case '.':flexo.giro_semibrazo_base-=5;
                        if (flexo.giro_semibrazo_base<flexo.giro_semibrazo_base_min) flexo.giro_semibrazo_base=flexo.giro_semibrazo_base_min;
                        break;break;
        case '8':grado_libertad-=0.005;
                if(grado_libertad<=0) grado_libertad = 0.001;
                break;break;
        case '9':grado_libertad+=0.005;
                if (grado_libertad>=1) grado_libertad = 0.0;
                break;break;
        case 'Z':proyeccion = _tipo_proyeccion::PERSPECTIVA;break;
        case 'X':proyeccion = _tipo_proyeccion::PARALELO;break;
	}
glutPostRedisplay();
}

//**************************************************************************
// Funcion l-olamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1,int x,int y)
{

switch (Tecla1){
	case GLUT_KEY_LEFT:Observer_angle_y--;break;
	case GLUT_KEY_RIGHT:Observer_angle_y++;break;
	case GLUT_KEY_UP:Observer_angle_x--;break;
	case GLUT_KEY_DOWN:Observer_angle_x++;break;
	case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;factor*=1.2;break;
	case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;factor/=1.2;break;
        case GLUT_KEY_F1:tanque.giro_tubo+=1;
                         if (tanque.giro_tubo>tanque.giro_tubo_max) tanque.giro_tubo=tanque.giro_tubo_max;
                         break;
        case GLUT_KEY_F2:tanque.giro_tubo-=1;
                         if (tanque.giro_tubo<tanque.giro_tubo_min) tanque.giro_tubo=tanque.giro_tubo_min;
                         break;break;
        case GLUT_KEY_F3:tanque.giro_torreta+=5;break;
        case GLUT_KEY_F4:tanque.giro_torreta-=5;break;
        case GLUT_KEY_F5:flexo.giro_cabeza_z+=5;
                         if (flexo.giro_cabeza_z>flexo.giro_cabeza_z_max) flexo.giro_cabeza_z=flexo.giro_cabeza_z_max;
                         break;break;
        case GLUT_KEY_F6:flexo.giro_cabeza_z-=5;
                         if (flexo.giro_cabeza_z<flexo.giro_cabeza_z_min) flexo.giro_cabeza_z=flexo.giro_cabeza_z_min;
                         break;break;
        case GLUT_KEY_F7:flexo.giro_cabeza_y+=5;
                        if (flexo.giro_cabeza_y>flexo.giro_cabeza_y_max) flexo.giro_cabeza_y=flexo.giro_cabeza_y_max;
                        break;break;
        case GLUT_KEY_F8:flexo.giro_cabeza_y-=5;
                        if (flexo.giro_cabeza_y<flexo.giro_cabeza_y_min) flexo.giro_cabeza_y=flexo.giro_cabeza_y_min;
                        break;break;
        case GLUT_KEY_F9:flexo.giro_cabeza_semibrazo+=5;
                        if (flexo.giro_cabeza_semibrazo>flexo.giro_cabeza_semibrazo_max) flexo.giro_cabeza_semibrazo=flexo.giro_cabeza_semibrazo_max;
                        break;break;
        case GLUT_KEY_F10:flexo.giro_cabeza_semibrazo-=5;
                        if (flexo.giro_cabeza_semibrazo<flexo.giro_cabeza_semibrazo_min) flexo.giro_cabeza_semibrazo=flexo.giro_cabeza_semibrazo_min;
                        break;break;
        case GLUT_KEY_F11:flexo.giro_flexo+=5;break;
        case GLUT_KEY_F12:flexo.giro_flexo-=5;break;
	}
glutPostRedisplay();
}

void clickRaton( int boton, int estado, int x, int y )
{
if (boton== GLUT_RIGHT_BUTTON) {
   if (estado == GLUT_DOWN) {
      estado_raton[2] = 1;
      xc=x;
      yc=y;
     } 
   else estado_raton[2] = 1;
   }
if(boton== GLUT_LEFT_BUTTON) {
  if( estado == GLUT_DOWN) {
      estado_raton[2] = 2;
      xc=x;
      yc=y;
      pick_color(xc, yc);
    }
  }
if(boton==4){
        Observer_distance*=1.2;
        factor*=1.2;  
        glutPostRedisplay();
}
if(boton==3){
        Observer_distance/=1.2;
        factor/=1.2;  
        glutPostRedisplay();
}
}


/*************************************************************************/

void getCamara (GLfloat *x, GLfloat *y)
{
*x=Observer_angle_x;
*y=Observer_angle_y;
}

/*************************************************************************/

void setCamara (GLfloat x, GLfloat y)
{
Observer_angle_x=x;
Observer_angle_y=y;
}

void RatonMovido( int x, int y )
{
float x0, y0, xn, yn; 
if(estado_raton[2]==1) 
    {getCamara(&x0,&y0);
     yn=y0+(y-yc);
     xn=x0-(x-xc);
     setCamara(xn,yn);
     xc=x;
     yc=y;
     glutPostRedisplay();
    }
}


void pick_color(int x, int y)
{
GLint viewport[4];
unsigned char pixel[3];

glGetIntegerv(GL_VIEWPORT, viewport);
glReadBuffer(GL_BACK);
glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(GLubyte *) &pixel[0]);
printf(" valor x %d, valor y %d, color %d, %d, %d \n",x,y,pixel[0],pixel[1],pixel[2]);
procesar_color(pixel);
glutPostRedisplay();
}

void anim4(){
        if(anim_activa){
                if(flexo.salto>0.0){
                        flexo.salto-=0.0005;
                }
                else{
                        glutIdleFunc(anim1);
                }
                glutPostRedisplay();
        }
}

void anim3(){
        if(anim_activa){
                if(flexo.salto<0.4){
                        flexo.salto+=0.0005;
                }
                else{
                        glutIdleFunc(anim4);
                }
        }
        glutPostRedisplay();
}

void anim2(){
        if(anim_activa){
                if (flexo.giro_cabeza_semibrazo<-60){
                flexo.giro_cabeza_semibrazo+=grado_libertad;
                flexo.giro_semibrazo_base-=grado_libertad;
        }
        else{
                glutIdleFunc(anim3);
        }
        glutPostRedisplay();
        }
}

void anim1(){
        if (anim_activa){
                if(flexo.giro_cabeza_semibrazo>-110){
                        flexo.giro_cabeza_semibrazo-=grado_libertad;
                        flexo.giro_semibrazo_base+=grado_libertad;
                }
                else{
                        glutIdleFunc(anim2);
                }
        glutPostRedisplay();
        }
}



//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{

// se inicalizan la ventana y los planos de corte
Size_x=0.5;
Size_y=0.5;
Front_plane=1;
Back_plane=1000;

// se incia la posicion del observador, en el eje z
Observer_distance=4*Front_plane;
Observer_angle_x=0;
Observer_angle_y=0;

// se indica cua*ply1l sera el color para limpiar la ventana	(r,v,a,al)
// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
glClearColor(1,1,1,1);

// se habilita el z-bufer
glEnable(GL_DEPTH_TEST);
change_projection_perspectiva();
glViewport(0,0,Window_width,Window_high);



}


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************


int main(int argc, char **argv)
{
 

// creación del objeto ply

ply.parametros(argv[1]);


// perfil 

vector<_vertex3f> perfil2;
_vertex3f aux;
aux.x=1.0;aux.y=-1.4;aux.z=0.0;
perfil2.push_back(aux);
aux.x=1.0;aux.y=-1.1;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.5;aux.y=-0.7;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.4;aux.y=-0.4;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.4;aux.y=0.5;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.5;aux.y=0.6;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.3;aux.y=0.6;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.5;aux.y=0.8;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.55;aux.y=1.0;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.5;aux.y=1.2;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.3;aux.y=1.4;aux.z=0.0;
perfil2.push_back(aux);
rotacion.parametros(perfil2,6,_figura::PLY,1);


// se llama a la inicialización de glut
glutInit(&argc, argv);

// se indica las caracteristicas que se desean para la visualización con OpenGL
// Las posibilidades son:
// GLUT_SIMPLE -> memoria de imagen simple
// GLUT_DOUBLE -> memoria de imagen doble
// GLUT_INDEX -> memoria de imagen con color indizado
// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
// GLUT_DEPTH -> memoria de profundidad o z-bufer
// GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

// posicion de la esquina inferior izquierdad de la ventana
glutInitWindowPosition(Window_x,Window_y);

// tamaño de la ventana (ancho y alto)
glutInitWindowSize(Window_width,Window_high);

// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
// al bucle de eventos)
glutCreateWindow("PRACTICA - 5");

// asignación de la funcion llamada "dibujar" al evento de dibujo
glutDisplayFunc(draw);
// asignación de la funcion llamada "change_window_size" al evento correspondiente
glutReshapeFunc(change_window_size);
// asignación de la funcion llamada "normal_key" al evento correspondiente
glutKeyboardFunc(normal_keys);
// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
glutSpecialFunc(special_key);
// función idle
glutIdleFunc(anim1);
// eventos del ratón
glutMouseFunc(clickRaton);
glutMotionFunc(RatonMovido);

// funcion de inicialización
initialize();

// inicio del bucle de eventos
glutMainLoop();
return 0;
}
