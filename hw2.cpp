#include"surface.h"
#include<iostream>
using namespace std;

#define T(x) (model->triangles[(x)])

#include <cfloat>

#define WIDTH 512
#define HEIGHT 512


Vertex Ia(1);
Vertex I(1);
Light light(Vertex(1.0, 1.0, -1.0), Ia, I);
Vertex lookAt(0.0,0.0,-1);

GLvoid
myGlmDraw(GLMmodel* model, bool flat, bool gouraud,bool zbuffer)
{
    static GLuint i;
    static GLMgroup* group;
    static GLMtriangle* triangle;
    static GLMmaterial* material;
    
    GLdouble model0[4*4];
    GLdouble proj0[4*4];
    GLint view0[4];
    
    GLdouble   win_x = 0.0;
    GLdouble   win_y = 0.0;
    GLdouble   win_z = 0.0;
    
    glGetDoublev(GL_MODELVIEW_MATRIX, model0);
    glGetDoublev(GL_PROJECTION_MATRIX, proj0);
    glGetIntegerv(GL_VIEWPORT, view0);
    
    Clear(1, 1, 1);
    group = model->groups;
    while (group) {
        for (i = 0; i < group->numtriangles; i++) {
            triangle = &T(group->triangles[i]);
            material = &model->materials[group->material];
            
            Vertex *nflat =new Vertex(model->facetnorms[3 * triangle->findex],model->facetnorms[3 * triangle->findex +1],model->facetnorms[3 * triangle->findex +2]) ;
            Vertex temp = nflat->normalize();
            nflat = &temp;
            
            gluProject(model->vertices[3 * triangle->vindices[0]] , model->vertices[3 * triangle->vindices[0]+1], model->vertices[3 * triangle->vindices[0]+2],
                       model0, proj0, view0,
                       &win_x, &win_y, &win_z);
            
            Vertex *p1 = new Vertex(win_x,win_y,win_z);
            Vertex *p1n = new Vertex(model->normals[3 * triangle->nindices[0]],model->normals[3 * triangle->nindices[0]+1],model->normals[3 * triangle->nindices[0]+2]);
            
            
            gluProject(model->vertices[3 * triangle->vindices[1]] , model->vertices[3 * triangle->vindices[1]+1], model->vertices[3 * triangle->vindices[1]+2],
                       model0, proj0, view0,
                       &win_x, &win_y, &win_z);
            Vertex *p2 = new Vertex(win_x,win_y,win_z);
            Vertex *p2n = new Vertex(model->normals[3 * triangle->nindices[1]],model->normals[3 * triangle->nindices[1]+1],model->normals[3 * triangle->nindices[1]+2]);
      
            gluProject(model->vertices[3 * triangle->vindices[2]] , model->vertices[3 * triangle->vindices[2]+1], model->vertices[3 * triangle->vindices[2]+2],
                       model0, proj0, view0,
                       &win_x, &win_y, &win_z);
            
            Vertex *p3 = new Vertex(win_x,win_y,win_z);
            Vertex *p3n = new Vertex(model->normals[3 * triangle->nindices[2]],model->normals[3 * triangle->nindices[2]+1],model->normals[3 * triangle->nindices[2]+2]);

            DrawTriangle(p1,p2,p3,flat,nflat,gouraud,p1n,p2n,p3n,material,zbuffer);
            
        }
        group = group->next;
    }
    
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, image);
    glFlush();
}

void swapV(Vertex &a, Vertex &b){
    Vertex temp = b;
    b = a;
    a = temp;
}
// between 0 and 1
float Clamp(float value,float min,float max ){
    return  std::__1::max(min, std::__1::min(value, max));
}

float Interpolate(float min,float max,float gradient){
    return min + (max - min) *Clamp(gradient);
}

GLvoid Clear(GLfloat r,GLfloat g,GLfloat b){
    for(int i =0;i<WIDTH;i++){
        for(int j=0;j<HEIGHT;j++){
            image[j][i][0] = r;
            image[j][i][1] = g;
            image[j][i][2] = b;
            depth[j][i] = FLT_MAX;
        }
    }
    
    
}


// scan from left to right  papb -> pcpd
GLvoid scanline(int y, Vertex *pa, Vertex *pb, Vertex *pc, Vertex *pd,bool flat, Vertex *nflat, bool gouraud,Vertex *p1n,Vertex *p2n,Vertex *p3n,GLMmaterial* material,bool zbuffer){
    
    //assert(material != NULL);
    if (material == NULL) {
        //cout<<" no material defined";
        return;
    }
    
    Vertex color(0.0,0.0,0.0);
    float gradient1 = pa->y != pb->y ? (y-pa->y) / (pb->y-pa->y) : 1;
    float gradient2 = pc->y != pd->y ? (y-pc->y) / (pd->y-pc->y) : 1;
    int sx = (int) Interpolate(pa->x, pb->x, gradient1);
    int ex = (int) Interpolate(pc->x, pd->x, gradient2);
    
    
    float z1 = Interpolate(pa->z, pb->z, gradient1);
    float z2 = Interpolate(pc->z, pd->z, gradient2);

    for(int x = sx;x<ex;x++){
        
        float gradient = (x - sx) / (float)(ex-sx);
        float z = Interpolate(z1, z2, gradient);

        if(zbuffer && depth[y][x] < z) continue;
        if(x >= 0 && y>=0 && x< WIDTH && y<HEIGHT) {
            depth[y][x] = z;
            if (flat) {
                color = calColor(flat,nflat,gouraud,p1n,material);
                image[y][x][0] = color.x;
                image[y][x][1] = color.y;
                image[y][x][2] = color.z;
            }else if (gouraud){
                
        
                float xx1 = Interpolate(pa->color->x, pb->color->x, gradient1);
                float xx2 = Interpolate(pc->color->x, pd->color->x, gradient1);
                
                float yy1 = Interpolate(pa->color->y, pb->color->y, gradient1);
                float yy2 = Interpolate(pc->color->y, pd->color->y, gradient1);
                
                float zz1 = Interpolate(pa->color->z, pb->color->z, gradient1);
                float zz2 = Interpolate(pc->color->z, pd->color->z, gradient1);
                
                
                image[y][x][0] = Interpolate(xx1, xx2, gradient);
                image[y][x][1] = Interpolate(yy1, yy2, gradient);
                image[y][x][2] = Interpolate(zz1, zz2, gradient);
            }
            
        }
    }
}

Vertex calColor(bool flat, Vertex *nflat, bool gouraud,Vertex *pn,GLMmaterial* material){
    if(material == NULL) return *new Vertex(1,1,1);
    Vertex ambient(material->ambient[0]*light.Ia.x,material->ambient[1]*light.Ia.y,material->ambient[2]*light.Ia.z);
    
    Vertex *N = nflat;
    if(gouraud){
        N = pn;
    }
    Vertex L = light.direction.normalize() * -1;
    Vertex V = lookAt.normalize() * -1;
    Vertex H = (L + V).normalize();
    
    Vertex diffuse(material->diffuse[0]*light.I.x,material->diffuse[1]*light.I.y,material->diffuse[2]*light.I.z);
    diffuse = diffuse * max(0.0f,N->dot(L));
    
    Vertex specular(material->specular[0]*light.I.x,material->specular[1]*light.I.y,material->specular[2]*light.I.z);
    specular = specular * powf(max(0.0f, N->dot(H)),material->shininess);
    
    
    return ambient + diffuse + specular;
}





GLvoid DrawTriangle(Vertex *p1,Vertex *p2, Vertex *p3,bool flat, Vertex *nflat, bool gouraud,Vertex *p1n,Vertex *p2n,Vertex *p3n,GLMmaterial* material,bool zbuffer){
    

    if(p1->y > p2->y) {
        swapV(*p1, *p2);
        swapV(*p1n, *p2n);
    }
    if(p1->y > p3->y) {
        swapV(*p1, *p3);
        swapV(*p1n, *p3n);
    }
    if(p2->y > p3->y){
        swapV(*p2, *p3);
        swapV(*p2n, *p3n);
    }
    
    
    //inverse slope
    float dp1p2,dp1p3;
    if(p2->y - p1->y >0){
        dp1p2 = (p2->x - p1->x) / (p2->y - p1->y);
    }else{
        dp1p2 = 0;
    }
    
    if(p3->y - p1->y >0){
        dp1p3 = (p3->x - p1->x) / (p3->y - p1->y);
    }else{
        dp1p3 = 0;
    }
    
    Vertex color1 = calColor(flat, nflat, gouraud, p1n,  material);
    Vertex color2 = calColor(flat, nflat, gouraud, p2n,  material);
    Vertex color3 = calColor(flat, nflat, gouraud, p3n,  material);
    
    p1->color = &color1;
    p2->color = &color2;
    p3->color = &color3;
    // P1
    // -
    // --
    // - -
    // -  -
    // -   - P2
    // -  -
    // - -
    // -
    // P3
    
    if(dp1p2 > dp1p3){
        for(int y =(int)p1->y;y<=(int)p3->y;y++){
            if(y < p2->y){
                scanline(y,p1,p3,p1,p2,flat, nflat, gouraud, p1n, p2n, p3n, material,zbuffer);
            }else{
                scanline(y,p1,p3,p2,p3,flat, nflat, gouraud, p1n, p2n, p3n, material,zbuffer);
            }
        }
    }
    
    
    //       P1
    //        -
    //       --
    //      - -
    //     -  -
    // P2 -   -
    //     -  -
    //      - -
    //        -
    //       P3
    
    else{
        for(int y =(int)p1->y;y<=(int)p3->y;y++){
            if(y < p2->y){
                scanline(y,p1,p2,p1,p3,flat, nflat, gouraud, p1n, p2n, p3n, material,zbuffer);
            }else{
                scanline(y,p2,p3,p1,p3,flat, nflat, gouraud, p1n, p2n, p3n, material,zbuffer);
            }
        }
    }

}




