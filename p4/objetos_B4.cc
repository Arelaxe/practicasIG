//**************************************************************************
// Práctica 4 usando objetos
// Noelia Escalera Mejías
//**************************************************************************

#include "objetos_B4.h"


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPointSize(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawArrays(GL_POINTS,0,vertices.size()); 

/*int i;
glPointSize(grosor);
glColor3f(r,g,b);
glBegin(GL_POINTS);
for (i=0;i<vertices.size();i++){
	glVertex3fv((GLfloat *) &vertices[i]);
	}
glEnd();*/
}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
	ambiente_difusa = {0.0, 0.5, 0.5, 0.1};
	brillo = 10;
	especular = {0.5, 0.1, 0.1, 1};
}

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawElements(GL_TRIANGLES,caras.size()*3,GL_UNSIGNED_INT,&caras[0]);

/*int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();*/
}

//*************************************************************************
// dibujar en modo sólido
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
int i;

glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar en modo sólido con apariencia de ajedrez
//*************************************************************************

void _triangulos3D::draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	if (i%2==0) glColor3f(r1,g1,b1);
	else glColor3f(r2,g2,b2);
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

void _triangulos3D::calcular_normales_caras(){
	if (!b_normales_caras){
		for (int i=0; i<caras.size(); i++){
			_vertex3f p0 = vertices[caras[i]._0];
			_vertex3f p1 = vertices[caras[i]._1];
			_vertex3f p2 = vertices[caras[i]._2];
			_vertex3f normal = (p1-p0).cross_product(p2-p0);
			float modulo = sqrt(pow(normal.x,2)+pow(normal.y,2)+pow(normal.z,2));
			normal.x = normal.x/modulo;
			normal.y = normal.y/modulo;
			normal.z = normal.z/modulo;
			normales_caras.push_back(normal);
		}
		b_normales_caras = true;
	}
}

void _triangulos3D::calcular_normales_vertices(){
	if (!b_normales_vertices){
		for (int j=0; j<vertices.size(); j++){
			vector<_vertex3f> aux;
			for (int i=0; i<caras.size(); i++){
				if (vertices[caras[i]._0]==vertices[j] || vertices[caras[i]._1]==vertices[j] || vertices[caras[i]._2]==vertices[j]){
					aux.push_back(normales_caras[i]);
				}
			}
			_vertex3f normal;
			normal.x = 0;
			normal.y = 0;
			normal.z = 0;
			for (int k=0; k<aux.size(); k++){
				normal.x += aux[k].x;
				normal.y += aux[k].y;
				normal.z += aux[k].z;
			}
			normal.x = normal.x/aux.size();
			normal.y = normal.y/aux.size();
			normal.z = normal.z/aux.size();
			float modulo = sqrt(pow(normal.x,2)+pow(normal.y,2)+pow(normal.z,2));
			normal.x = normal.x/modulo;
			normal.y = normal.y/modulo;
			normal.z = normal.z/modulo;
			normales_vertices.push_back(normal);
		}
		b_normales_vertices = true;
	}	
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw_sombreado_plano(){
	int i;
	calcular_normales_caras();
	glShadeModel(GL_FLAT);
	glBegin(GL_TRIANGLES);
	for (i=0;i<caras.size();i++){
		glNormal3f(normales_caras[i].x,normales_caras[i].y,normales_caras[i].z);
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
		}
	glEnd();
}

void _triangulos3D::draw_sombreado_gouraud(){
	int i;
	calcular_normales_caras();
	calcular_normales_vertices();
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLES);
	for (i=0;i<caras.size();i++){
		glNormal3f(normales_vertices[caras[i]._0].x,normales_vertices[caras[i]._0].y,normales_vertices[caras[i]._0].z);
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		glNormal3f(normales_vertices[caras[i]._1].x,normales_vertices[caras[i]._1].y,normales_vertices[caras[i]._1].z);
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		glNormal3f(normales_vertices[caras[i]._2].x,normales_vertices[caras[i]._2].y,normales_vertices[caras[i]._2].z);
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
		}
	glEnd();
}

void _triangulos3D::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
switch (modo){
	case POINTS:draw_puntos(r1, g1, b1, grosor);break;
	case EDGES:draw_aristas(r1, g1, b1, grosor);break;
	case SOLID_CHESS:draw_solido_ajedrez(r1, g1, b1, r2, g2, b2);break;
	case SOLID:draw_solido(r1, g1, b1);break;
	case SUAVIZADO_PLANO:draw_sombreado_plano();break;
	case SUAVIZADO_GOURAUD:draw_sombreado_gouraud();break;
	}
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
//vertices
vertices.resize(8);
vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=tam;
vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=tam;
vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;
vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=-tam;
vertices[7].x=-tam;vertices[7].y=tam;vertices[7].z=-tam;

// triangulos
caras.resize(12);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
caras[3]._0=5;caras[3]._1=6;caras[3]._2=2;
caras[4]._0=5;caras[4]._1=4;caras[4]._2=6;
caras[5]._0=4;caras[5]._1=7;caras[5]._2=6;
caras[6]._0=0;caras[6]._1=7;caras[6]._2=4;
caras[7]._0=0;caras[7]._1=3;caras[7]._2=7;
caras[8]._0=3;caras[8]._1=2;caras[8]._2=7;
caras[9]._0=2;caras[9]._1=6;caras[9]._2=7;
caras[10]._0=0;caras[10]._1=1;caras[10]._2=4;
caras[11]._0=1;caras[11]._1=5;caras[11]._2=4;  
}


//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{

//vertices 
vertices.resize(5); 
vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

caras.resize(6);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;
}

//*************************************************************************
// clase objeto ply
//*************************************************************************


_objeto_ply::_objeto_ply() 
{
   // leer lista de coordenadas de vértices y lista de indices de vértices
 
}



int _objeto_ply::parametros(char *archivo)
{
int n_ver,n_car;

vector<float> ver_ply ;
vector<int>   car_ply ;
   
_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

//if (n_ver<3 || n_car<1){
//	printf("Error %d %d\n",__FILE__,__LINE__);
//	exit(-1);
//	}

vertices.resize(n_ver);
caras.resize(n_car);

_vertex3f ver_aux;
_vertex3i car_aux;

for (int i=0;i<n_ver;i++)
	{ver_aux.x=ver_ply[i*3];
	 ver_aux.y=ver_ply[i*3+1];
	 ver_aux.z=ver_ply[i*3+2];
	 vertices[i]=ver_aux;
	}

for (int i=0;i<n_car;i++)
	{car_aux.x=car_ply[i*3];
	 car_aux.y=car_ply[i*3+1];
	 car_aux.z=car_ply[i*3+2];
	 caras[i]=car_aux;
	}

return(0);
}


//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{

}


void _rotacion::parametros(vector<_vertex3f> perfil, int num, _figura f, int al)
{
int i,j;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;

if (f==_figura::PLY){ // Quitamos la cara que hemos puesto para que nos lea el ply
	caras.clear();
}

// tratamiento de los vértices

num_aux=perfil.size();

if (perfil[0].y>perfil[num_aux-1].y){ // Damos la vuelta si nos dan el perfil al revés
	for (int c=0;c<num_aux;c++)
		perfil[c]=perfil[num_aux-1-c];
}

vertices.resize(num_aux*num);

if (f!=_figura::CILINDROX){
	for (j=0;j<num;j++){
		for (i=0;i<num_aux;i++)
    	{
      		vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
      		vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
      		        perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
      		vertice_aux.y=perfil[i].y;
      		vertices[i+j*num_aux]=vertice_aux;
     }
  }
}
else{
	for (j=0;j<num;j++){
		for (i=0;i<num_aux;i++)
    	{
      		vertice_aux.x=perfil[i].x;
      		vertice_aux.z=-perfil[i].y*sin(2.0*M_PI*j/(1.0*num))+
      		        perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
      		vertice_aux.y=perfil[i].y*cos(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
      		vertices[i+j*num_aux]=vertice_aux;
     }
  }
}

// Caras laterales

if (f!=_figura::CONO){
	int num_caras = (perfil.size()-1)*2*num;
	caras.resize(num_caras);
	int num_cara = 0;

	for (i=0; i<num; i++){
		for(j=0; j<num_aux-1; j++){
			cara_aux._0 = (i*num_aux+j)%(num_aux*num);
			cara_aux._1 = ((i+1)*num_aux+j)%(num_aux*num);
			cara_aux._2 = (i*num_aux+(j+1))%(num_aux*num);
			caras[num_cara] = cara_aux;
			num_cara++;

			cara_aux._0 = (i*num_aux+(j+1))%(num_aux*num);
			cara_aux._1 = ((i+1)*num_aux+j)%(num_aux*num);
			cara_aux._2 = ((i+1)*num_aux+(j+1))%(num_aux*num);
			caras[num_cara] = cara_aux;
			num_cara++;
		}
	}
}

    
 // tapa inferior
if (fabs(perfil[0].x)>0.0)
{
	if(f!=_figura::CONO_SIN_TAPA && f!=_figura::CILINDRO_SIN_TAPA){
	// Calcular centro
	if(f!=_figura::ESFERA && f!=_figura::CILINDROX){
		vertice_aux.x = 0;
		vertice_aux.y = perfil[0].y;
		vertice_aux.z = 0;
	}
	else if(f==_figura::CILINDROX){
		vertice_aux.x = perfil[0].x;
		vertice_aux.y = 0;
		vertice_aux.z = 0;
	}
	else{
		vertice_aux.x = 0;
		vertice_aux.y = -al;
		vertice_aux.z = 0;
	}
	
	vertices.push_back(vertice_aux);

	if(f==_figura::CILINDRO || f==_figura::CILINDROX){
		for (i=0; i<num*2; i+=2){
		  cara_aux._0 = i;
		  cara_aux._1 = (i+2)%(num*num_aux);
		  cara_aux._2 = vertices.size()-1;
		  caras.push_back(cara_aux);
		}
	}
	else if(f==_figura::CONO){
		for (i=0; i<num; i++){
		  cara_aux._0 = i;
		  cara_aux._1 = (i+1)%(num*num_aux);
		  cara_aux._2 = vertices.size()-1;
		  caras.push_back(cara_aux);
		}
	}
	else if(f==_figura::ESFERA || f==_figura::PLY){
		for(i=0; i<num*num_aux; i+=num_aux){
			cara_aux._0 = (i)%(num*num_aux);
			cara_aux._1 = vertices.size()-1;
			cara_aux._2 = (i+num_aux)%(num*num_aux);
			caras.push_back(cara_aux);
		}
	}
}
}

 // tapa superior
 if (fabs(perfil[num_aux-1].x)>0.0)
  {
	  // Calcular centro
	  if(f==_figura::CILINDRO){
	  	vertice_aux.x = 0;
	  	vertice_aux.y = perfil[num_aux-1].y;
	  	vertice_aux.z = 0;

		vertices.push_back(vertice_aux); 

		for (i=1; i<num*2; i+=2){
	  		cara_aux._0 = i;
	  		cara_aux._1 = (i+2)%(num*num_aux);
	  		cara_aux._2 = vertices.size()-1;
	  		caras.push_back(cara_aux);
	  	}
	  }
	  else if(f==_figura::CILINDROX){
	  	vertice_aux.x = perfil[num_aux-1].x;
	  	vertice_aux.y = 0;
	  	vertice_aux.z = 0;

		vertices.push_back(vertice_aux); 

		for (i=1; i<num*2; i+=2){
	  		cara_aux._0 = i;
	  		cara_aux._1 = (i+2)%(num*num_aux);
	  		cara_aux._2 = vertices.size()-1;
	  		caras.push_back(cara_aux);
	  	}
	  }
	  else if (f==_figura::CONO || f==_figura::CONO_SIN_TAPA){
		vertice_aux.x = 0;
		vertice_aux.y = vertices[0].y+al;
		vertice_aux.z = 0;

		vertices.push_back(vertice_aux); 

		for (i=0; i<num; i++){
		  cara_aux._0 = i;
		  cara_aux._1 = (i+1)%(num*num_aux);
		  cara_aux._2 = vertices.size()-1;
		  caras.push_back(cara_aux);
		}
	  }
	  else if (f==_figura::ESFERA){
		vertice_aux.x = 0;
		vertice_aux.y = al;
		vertice_aux.z = 0;

		vertices.push_back(vertice_aux);

		for(i=num_aux-1; i<num*num_aux; i+=num_aux){
			cara_aux._0 = (i)%(num*num_aux);
			cara_aux._1 = vertices.size()-1;
			cara_aux._2 = (i+num_aux)%(num*num_aux);
			caras.push_back(cara_aux);
		}
	  }
	  else if(f==_figura::PLY){
		vertice_aux.x = 0;
	  	vertice_aux.y = perfil[num_aux-1].y;
	  	vertice_aux.z = 0;

		vertices.push_back(vertice_aux); 

		for(i=num_aux-1; i<num*num_aux; i+=num_aux){
			cara_aux._0 = (i)%(num*num_aux);
			cara_aux._1 = vertices.size()-1;
			cara_aux._2 = (i+num_aux)%(num*num_aux);
			caras.push_back(cara_aux);
		}
	  }	
	}
	
	if (f==_figura::ESFERA)
	{
		for (int i=0; i<vertices.size(); i++){
			_vertex3f normal;
			normal = vertices[i]; 
			float modulo = sqrt(pow(normal.x,2)+pow(normal.y,2)+pow(normal.z,2));
			normal.x = normal.x/modulo;
			normal.y = normal.y/modulo;
			normal.z = normal.z/modulo;
			normales_vertices.push_back(normal);
		}
	    b_normales_vertices = true;
	}
}
//************************************************************************
// objeto articulado: tanque
//************************************************************************

_chasis::_chasis()
{
// perfil para un cilindro
vector<_vertex3f> perfil;
_vertex3f aux;
aux.x=0.107;aux.y=-0.5;aux.z=0.0;
perfil.push_back(aux);
aux.x=0.107;aux.y=0.5;aux.z=0.0;
perfil.push_back(aux);
rodamiento.parametros(perfil,12,_figura::CILINDRO,1);
altura=0.22;
};

void _chasis::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
glScalef(1.0,0.22,0.95);
base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.25,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.5,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.25,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.5,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}

//************************************************************************

_torreta::_torreta()
{
altura=0.18;
anchura=0.65;
};

void _torreta::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
glScalef(0.65,0.18,0.6);
base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.325,0,0);
glRotatef(90.0,0,0,1);
glScalef(0.18,0.16,0.6);
parte_trasera.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}

//************************************************************************

_tubo::_tubo()
{
// perfil para un cilindro
vector<_vertex3f> perfil;
_vertex3f aux;
aux.x=0.04;aux.y=-0.4;aux.z=0.0;
perfil.push_back(aux);
aux.x=0.04;aux.y=0.4;aux.z=0.0;
perfil.push_back(aux);
tubo_abierto.parametros(perfil,12,_figura::CILINDRO_SIN_TAPA,0);
};

void _tubo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

glPushMatrix();
glTranslatef(0.4,0,0);
glRotatef(90.0,0,0,1);
tubo_abierto.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}


//************************************************************************

_tanque::_tanque()
{
giro_tubo=2.0;
giro_torreta=0.0;
giro_tubo_min=-9;
giro_tubo_max=20;
};

void _tanque::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
chasis.draw(modo, r1, g1, b1, r2, g2, b2, grosor);

glRotatef(giro_torreta,0,1,0);
glPushMatrix();
glTranslatef(0.0,(chasis.altura+torreta.altura)/2.0,0.0);
torreta.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(torreta.anchura/2.0,(chasis.altura+torreta.altura)/2.0,0.0);
glRotatef(giro_tubo,0,0,1);
tubo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
glPopMatrix();

};

//************************************************************************
// objeto articulado: flexo
//************************************************************************

_junta::_junta()
{
	//Perfil para una esfera
	vector<_vertex3f> perfil_junta;
	_vertex3f aux;

	for(float angulo=M_PI-0.3;angulo>0.15;angulo-=M_PI/10){
		aux.z=0.0;
    	aux.x=(1)*((float) sin(angulo))*((float)sin(1.5));
    	aux.y=(1)*((float) cos(angulo));
		perfil_junta.push_back(aux);
	}

	junta.parametros(perfil_junta,20,_figura::ESFERA,1);
}

void _junta::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	glPushMatrix();
	glScalef(0.1,0.1,0.1);
	junta.draw(modo,r1,g1,b1,r2,g2,b2,grosor);
	glPopMatrix();
}

_base_flexo::_base_flexo()
{
}

void _base_flexo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	glPushMatrix();
	glScalef(1,0.25,1);
	base.draw(modo,r1,g1,b1,r2,g2,b2,grosor);
	glPopMatrix();
}

_semibrazo::_semibrazo()
{
	vector<_vertex3f> perfil_c;
	_vertex3f aux;

	//Perfil para un cilindro
	
	aux.x=0.5;aux.y=0.0;aux.z=0.0;
	perfil_c.push_back(aux);
	aux.x=0.5;aux.y=1.0;aux.z=0.0;
	perfil_c.push_back(aux);

	semi.parametros(perfil_c,20,_figura::CILINDRO,1);
}

void _semibrazo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	glPushMatrix();
	glScalef(0.25,1.25,0.25);
	semi.draw(modo,r1,g1,b1,r2,g2,b2,grosor);
	glPopMatrix();
}


_cabeza::_cabeza()
{
	//Perfil para una esfera
	vector<_vertex3f> perfil_bombilla, perfil_cilindro, perfil_cono;
	_vertex3f aux;
	for(float angulo=M_PI-0.3;angulo>0.15;angulo-=M_PI/10){
		aux.z=0.0;
    	aux.x=(1)*((float) sin(angulo))*((float)sin(1.5));
    	aux.y=(1)*((float) cos(angulo));
		perfil_bombilla.push_back(aux);
	}

	bombilla.parametros(perfil_bombilla,20,_figura::ESFERA,1);

	//Perfil para un cilindro
	aux.x=0.5;aux.y=0.0;aux.z=0.0;
	perfil_cilindro.push_back(aux);
	aux.x=0.5;aux.y=1.0;aux.z=0.0;
	perfil_cilindro.push_back(aux);

	cilindro.parametros(perfil_cilindro,20,_figura::CILINDRO,2);

	//Perfil para un cono
	aux.x=0.5;aux.y=0.0;aux.z=0,0;
	perfil_cono.push_back(aux);

	cono.parametros(perfil_cono,20,_figura::CONO_SIN_TAPA,1);
}

void _cabeza::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, 
				float r3, float g3, float b3, float r4, float g4, float b4, float grosor)
{
	glPushMatrix();
	glTranslatef(0.0,0.75,0.0);
	glScalef(0.5,0.5,0.5);
	cilindro.draw(modo,r3,g3,b3,r4,g4,b4,grosor);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,0.25,0.0);
	cono.draw(modo,r3,g3,b3,r4,g4,b4,grosor);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,0.25,0.0);
	glScalef(0.25,0.25,0.25);
	bombilla.draw(modo,r1,g1,b1,r2,g2,b2,grosor);
	glPopMatrix();
}


_flexo::_flexo()
{
	giro_cabeza_z = -0.0;
	giro_cabeza_z_min = -15;
	giro_cabeza_z_max = 15;

	giro_cabeza_y = 0.0;
	giro_cabeza_y_min = -90;
	giro_cabeza_y_max = 90;

	giro_cabeza_semibrazo = -80;
	giro_cabeza_semibrazo_max = 0;
	giro_cabeza_semibrazo_min = -100;

	giro_semibrazo_base = 40;
	giro_semibrazo_base_min = 0;
	giro_semibrazo_base_max = 80;

	giro_flexo = 0.0;

	salto = 0.0;
}

void  _flexo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, 
				float r3, float g3, float b3, float r4, float g4, float b4,
				float r5, float g5, float b5, float r6, float g6, float b6, float grosor)
{
	glTranslatef(0.0,salto,0.0);
	glTranslatef(0.0,-1.5,0.0);
	glPushMatrix();
	glTranslatef(0,0.125,0);
	base.draw(modo,r3,g3,b3,r4,g4,b4,grosor);
	glPopMatrix();
	glTranslatef(0,0.35,0);
	glPushMatrix();
	glRotatef(giro_flexo,0,1,0);
	junta3.draw(modo,r5,g5,b5,r6,g6,b6,grosor);
	glRotatef(giro_semibrazo_base,0,0,1);
	glTranslatef(0.0,0.1,0.0);
	semibrazo2.draw(modo,r5,g5,b5,r6,g6,b6,grosor);
	glPushMatrix();
	glTranslatef(0.0,1.35,0.0);
	junta2.draw(modo,r5,g5,b5,r6,g6,b6,grosor);
	glRotatef(giro_cabeza_semibrazo,0,0,1);
	glTranslatef(0.0,0.1,0.0);
	semibrazo1.draw(modo,r5,g5,b5,r6,g6,b6,grosor);
	glTranslatef(0,1.35,0);
	junta1.draw(modo,r5,g5,b5,r6,g6,b6,grosor);
	glPushMatrix();
	glRotatef(giro_cabeza_y,0,1,0);
	glTranslatef(0.0,0.35,0.0);
	glRotatef(giro_cabeza_z,0,0,1);
	glTranslatef(0.25,0.0,0.0);
	glRotatef(90,0,0,1);
	glTranslatef(0.0,-0.75,0.0);
	cabeza.draw(modo,r1,g1,b1,r2,g2,b2,r3,g3,b3,r4,g4,b4,grosor);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}
