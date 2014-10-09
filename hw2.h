
#include<GLUT/glut.h>
#include<cmath>
#include"glm.h"
#include<assert.h>
#define WIDTH 512
#define HEIGHT 512
static GLfloat image[WIDTH][HEIGHT][3];
static GLfloat depth[WIDTH][HEIGHT];


struct Vertex {
    GLfloat x, y, z;
    Vertex *color;
    
    Vertex(){}
    Vertex(float a):x(a),y(a),z(a) {
        color = NULL;
    }
    Vertex(GLfloat _x, GLfloat _y, GLfloat _z) :
    x(_x), y(_y), z(_z) {
        color = NULL;
    }
    
    Vertex operator+(const Vertex &vector) {
        return Vertex(x + vector.x, y + vector.y, z + vector.z);
    }
    Vertex operator-(const Vertex &vector) {
        return Vertex(x - vector.x, y - vector.y, z - vector.z);
    }
    // this is used for calculate ambient diffuse specular
    Vertex operator*(const Vertex &vector) {
        return Vertex(x*vector.x,y*vector.y,z*vector.z);
    }
    Vertex operator/(const int &n) {
        return Vertex(x/n , y/n, z/n);
    }
    
    
    Vertex normalize() {
        float mod = sqrtf(x * x + y * y + z * z);
        return Vertex(x / mod, y / mod, z / mod);
    }
    
    float dot(const Vertex &vector){
        return x * vector.x + y * vector.y + z * vector.z;
    }
    Vertex cross(const Vertex &vector){
        return Vertex(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y*vector.x);
    }
    
};

struct Light {
    Vertex direction;
    //Ia : ambient light intesity ; I : intensity of light sourse
    Vertex Ia , I;
    Light() {}
    Light(Vertex d,Vertex Ia, Vertex I):direction(d.normalize()),Ia(Ia),I(I){}
};




void swapV(Vertex &a, Vertex &b);
float Clamp(float value,float min=0,float max = 1);
float Interpolate(float min,float max,float gradient);
GLvoid Clear(float r,float g,float b);
GLvoid scanline(int y, Vertex pa, Vertex pb, Vertex pc, Vertex pd,bool flat, Vertex *nflat, bool gouraud,Vertex *p1n,Vertex *p2n,Vertex *p3n,GLMmaterial* material,bool zbuffer);
GLvoid DrawTriangle(Vertex *p1,Vertex *p2, Vertex *p3, bool flat, Vertex *nflat, bool gouraud,Vertex *p1n,Vertex *p2n,Vertex *p3n,GLMmaterial* material,bool zbuffer);
GLvoid myGlmDraw(GLMmodel* model, bool flat,bool gourand,bool zbuffer);
Vertex calColor(bool flat, Vertex *nflat, bool gouraud,Vertex *pn,GLMmaterial* material);

