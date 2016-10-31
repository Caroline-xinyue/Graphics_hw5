#ifndef HW5_H
#define HW5_H

#include <iostream>
#include <fstream>
#include <vector>
#include "math.h"
#include "file.h"
#include "common.h"

#define DEFAULT_NUM_MESHES 10

typedef enum {VERTEX, EDGE, FACE} drawMode;
typedef enum {SMOOTH, FLAT, PHONG} shadeMode;

std::vector<Mesh> meshes;
Material material;
GLfloat* all_vertices;
GLuint* all_indices;
int numMeshes = 0;
bool parallelView = false;
bool animateState = false;
GLfloat start = 0.0f;
GLfloat last = 0.0f;
GLfloat angle = 0.0f;

#endif
