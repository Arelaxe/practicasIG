//**************************************************************************
// Práctica 2 usando objetos
//**************************************************************************

#include "objetos_B2.h"
#include "file_ply_stl.hpp"


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
int i;
glPointSize(grosor);
glColor3f(r,g,b);
glBegin(GL_POINTS);
for (i=0;i<vertices.size();i++){
	glVertex3fv((GLfloat *) &vertices[i]);
	}
glEnd();
}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
}


//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
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
// dibujar en modo sólido
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{

glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (int i=0;i<caras.size();i++){
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
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glColor3f(r1,g1,b1);
glBegin(GL_TRIANGLES);
for (int i=0;i<caras.size();i+=2){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
}
glEnd();

glColor3f(r2,g2,b2);
glBegin(GL_TRIANGLES);
for (int i=1;i<caras.size();i+=2){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
}
glEnd();
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
switch (modo){
	case POINTS:draw_puntos(r1, g1, b1, grosor);break;
	case EDGES:draw_aristas(r1, g1, b1, grosor);break;
	case SOLID_CHESS:draw_solido_ajedrez(r1, g1, b1, r2, g2, b2);break;
	case SOLID:draw_solido(r1, g1, b1);break;
	}
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
//vertices
vertices.resize(8);
vertices[0].x=-(tam*2);vertices[0].y=0;vertices[0].z=tam*2;
vertices[1].x=tam*2;vertices[1].y=0;vertices[1].z=tam*2;
vertices[2].x=tam*2;vertices[2].y=0;vertices[2].z=-(tam*2);
vertices[3].x=-(tam*2);vertices[3].y=0;vertices[3].z=-(tam*2);
vertices[4].x=tam*2;vertices[4].y=4*tam;vertices[4].z=tam*2;
vertices[5].x=-(tam*2);vertices[5].y=4*tam;vertices[5].z=tam*2;
vertices[6].x=tam*2;vertices[6].y=4*tam;vertices[6].z=-(tam*2);
vertices[7].x=-(tam*2);vertices[7].y=4*tam;vertices[7].z=-(tam*2);

// triangulos
caras.resize(12);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=5;
caras[1]._0=1;caras[1]._1=4;caras[1]._2=5;
caras[2]._0=1;caras[2]._1=2;caras[2]._2=4;
caras[3]._0=2;caras[3]._1=6;caras[3]._2=4;
caras[4]._0=3;caras[4]._1=2;caras[4]._2=6;
caras[5]._0=3;caras[5]._1=6;caras[5]._2=7;
caras[6]._0=0;caras[6]._1=3;caras[6]._2=7;
caras[7]._0=0;caras[7]._1=7;caras[7]._2=5;
caras[8]._0=0;caras[8]._1=1;caras[8]._2=2;
caras[9]._0=0;caras[9]._1=2;caras[9]._2=3;
caras[10]._0=5;caras[10]._1=4;caras[10]._2=7;
caras[11]._0=4;caras[11]._1=6;caras[11]._2=7;
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
int n_ver,n_car,cont_ver=0,cont_car=0;

vector<float> ver_ply ;
vector<int>   car_ply ;
 
_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

vertices.resize(n_ver);
caras.resize(n_car);

for (int i=0; i<n_ver; i++){
	vertices[i].x = ver_ply[cont_ver];
	cont_ver++;
	vertices[i].y = ver_ply[cont_ver];
	cont_ver++;
	vertices[i].z = ver_ply[cont_ver];
	cont_ver++;
}

for (int i=0; i<n_car; i++){
	caras[i]._0 = car_ply[cont_car];
	cont_car++;
	caras[i]._1 = car_ply[cont_car];
	cont_car++;
	caras[i]._2 = car_ply[cont_car];
	cont_car++;
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
	  else if (f==_figura::CONO){
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
}