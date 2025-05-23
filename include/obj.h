/* Copyright (c) 2005 Robert Kooima                                           */
/*                                                                            */
/* Permission is hereby granted, free of charge, to any person obtaining a    */
/* copy of this software and associated documentation files (the "Software"), */
/* to deal in the Software without restriction, including without limitation  */
/* the rights to use, copy, modify, merge, publish, distribute, sublicense,   */
/* and/or sell copies of the Software, and to permit persons to whom the      */
/* Software is furnished to do so, subject to the following conditions:       */
/*                                                                            */
/* The above copyright notice and this permission notice shall be included in */
/* all copies or substantial portions of the Software.                        */
/*                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    */
/* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    */
/* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        */
/* DEALINGS IN THE SOFTWARE.                                                  */

#ifndef UTIL3D_OBJ_H
#define UTIL3D_OBJ_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/

enum {
	OBJ_KD,						//Diffuse
	OBJ_KA,						//Ambient
	OBJ_KE,						//Emmisive (for PBR)
	OBJ_KS,						//Specular
	OBJ_NS,						//Specular Exponent (for Phong)
	OBJ_KN,
	OBJ_PROP_COUNT
};

#define OBJ_OPT_CLAMP  1

#define MAXSTR 1024

/*============================================================================*/

#define index_t       unsigned int
#define GL_INDEX_T GL_UNSIGNED_INT

struct obj_prop
{
    char* str;
    int          opt;   //Material Options
    unsigned int map;

    float c[4];         //Material Colour
    float o[3];         //Material Offset
    float s[3];         //Material Scale
};

struct obj_mtrl
{
    char* name;

    struct obj_prop kv[OBJ_PROP_COUNT];
};

struct obj_vert
{
    float u[3];         //Tangent Vector
    float n[3];         //Normal Vector
    float t[2];         //Texture Coordinates
    float v[3];         //Vertex Coordinates
};

struct obj_poly
{
    index_t vi[3];      //Vertex Index
};

struct obj_line
{
    index_t vi[2];      //Vertex Index
};

struct obj_surf
{
    int materialIndex;  //Material Index

    int polygonCount;   //Polygon Count   
    int pm;             //Polygon Mem Size
    int lineCount;      //Line Count
    int lm;             //Line Mem Size

    unsigned int pibo;  //Polygon IBO
    unsigned int libo;  //Line IBO

    struct obj_poly* polyVertexArr;//Polygon Verticies
    struct obj_line* lineVertexArr;//Line Vertices
};

struct obj
{
    unsigned int vao;   //VAO
    unsigned int vbo;   //VBO

    int materialCount;  //Material Count
    int mm;             //Material Mem Size
    int vertexCount;    //Vertex Count
    int vm;             //Vertex Mem Size
    int surfaceCount;   //Sureface Count
    int sm;             //Surface Mem Size

    int uloc;           //Tangent Vectors Location      (Shader)
    int nloc;           //Normal Vectors Location       (Shader)
    int tloc;           //Texture Corrdinates Location  (Shader)
    int vloc;           //Vertex Corrdinates Location   (Shader)

    int cloc[OBJ_PROP_COUNT];   //Material Colour Locations (Shader)
    int oloc[OBJ_PROP_COUNT];   //Material Texture Locations(Shader)    
    int Mloc[OBJ_PROP_COUNT];   //Material Matrix Locations (Shader: texture scale and offset transform matrix)

    struct obj_mtrl* materialArr;        //Material Array
    struct obj_vert* vertexArr;        //Vertex Array
    struct obj_surf* surfaceArr;        //Surface Array
};


struct vec2
{
    float v[2];
    int _ii;
};

struct vec3
{
    float v[3];
    int _ii;
};

struct iset
{
    int vi;
    int gi;

    int _vi;
    int _ti;
    int _ni;
    int _ii;
};

/*----------------------------------------------------------------------------*/

typedef struct obj obj;

obj *obj_create(const char *);
void obj_render(obj *);
void obj_delete(obj *);

/*----------------------------------------------------------------------------*/

int  obj_add_mtrl(obj *);
int  obj_add_vert(obj *);
int  obj_add_poly(obj *, int);
int  obj_add_line(obj *, int);
int  obj_add_surf(obj *);

int  obj_num_mtrl(const obj *);
int  obj_num_vert(const obj *);
int  obj_num_poly(const obj *, int);
int  obj_num_line(const obj *, int);
int  obj_num_surf(const obj *);

void obj_del_mtrl(obj *, int);
void obj_del_vert(obj *, int);
void obj_del_poly(obj *, int, int);
void obj_del_line(obj *, int, int);
void obj_del_surf(obj *, int);

/*----------------------------------------------------------------------------*/

void obj_set_mtrl_name(obj *, int,      const char *);
void obj_set_mtrl_map (obj *, int, int, const char *);
void obj_set_mtrl_opt (obj *, int, int, unsigned int);
void obj_set_mtrl_c   (obj *, int, int, const float *);
void obj_set_mtrl_o   (obj *, int, int, const float *);
void obj_set_mtrl_s   (obj *, int, int, const float *);

void obj_set_vert_v(obj *, int, const float *);
void obj_set_vert_t(obj *, int, const float *);
void obj_set_vert_n(obj *, int, const float *);
void obj_set_vert_u(obj *, int, const float *);

void obj_set_poly(obj *, int, int, const int *);
void obj_set_line(obj *, int, int, const int *);
void obj_set_surf(obj *, int, int);

void obj_set_vert_loc(obj *, int, int, int, int);
void obj_set_prop_loc(obj *, int, int, int, int);

/*----------------------------------------------------------------------------*/

const char  *obj_get_mtrl_name(const obj *, int);
unsigned int obj_get_mtrl_map (const obj *, int, int);
unsigned int obj_get_mtrl_opt (const obj *, int, int);
void         obj_get_mtrl_c   (const obj *, int, int, float *);
void         obj_get_mtrl_o   (const obj *, int, int, float *);
void         obj_get_mtrl_s   (const obj *, int, int, float *);

void obj_get_vert_v(const obj *, int, float *);
void obj_get_vert_t(const obj *, int, float *);
void obj_get_vert_n(const obj *, int, float *);

void obj_get_poly(const obj *, int, int, int *);
void obj_get_line(const obj *, int, int, int *);
int  obj_get_surf(const obj *, int);

/*----------------------------------------------------------------------------*/

void  obj_render_mtrl(const obj *, int);
void  obj_render_surf(const obj *, int);
void  obj_render_file(const obj *);

/*----------------------------------------------------------------------------*/

unsigned int obj_load_image(const char *);

void  obj_mini(obj *);
void  obj_norm(obj *);
void  obj_proc(obj *);
void  obj_uniq(obj *, float, float, int);
void  obj_sort(obj *, int);
float obj_acmr(obj *, int);

void  obj_bound(const obj *, float *);
void  obj_write(const obj *, const char *, const char *, int);

/*======================================================================+=====*/

#ifdef __cplusplus
}
#endif
#endif
