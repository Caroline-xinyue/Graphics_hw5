#ifndef HW5_H
#define HW5_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include "math.h"
#include "common.h"
#include "file.h"

using namespace std;

#define GLM_SWIZZLE
#define DEFAULT_NUM_MESHES 10
#define TIME_DURATION 0.01
#define STEP 0.01
#define VBO_SIZE 6
#define ZOOM 0.9

typedef enum {VERTEX, EDGE, FACE} DrawMode;
typedef enum {GOURAUD, FLAT, PHONG} ShadeMode;

const int numPoints = 0;
GLuint vao, vbos[2], program, pos, norm;
std::vector<Mesh> meshes;
std::vector<GLfloat> vertices;
std::vector<GLuint> indices;
std::vector<GLfloat> line_vertices;
std::vector<GLfloat> flat_vertices;
GLuint numMeshes = 0;
GLfloat start = 0.0f;
GLfloat last = 0.0f;
GLfloat angle = 0.0f;
glm::mat4 Projection;
glm::mat4 Model;
glm::mat4 View;
//Mesh newMesh; // change to meshes[i]
//Faces newFace; // change to meshes[i].face
Camera camera;
Material material;
Light light;
DrawMode drawMode = FACE;
ShadeMode shadeMode = GOURAUD;
bool parallelView = false;
bool animateState = false;
bool key_Up;
bool key_Down;

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size);
void makeFlatDuplicate (glm::vec3 *position1,glm::vec3 *position2,glm::vec3 *position3, glm::vec3 corss,int MeshIndex);
void multipleFileHandle(int argc, char* argv[]);
bool checkValidIndex(int a,int max);
void makeLineDuplicate(glm::vec3 *position1,glm::vec3 *position2, glm::vec3 corss,int MeshIndex);
glm::vec3 crossByCounterClock(int index1,int index2,int index3,int MeshIndex);
void readfile(string name, int MeshIndex);

void init_modelViewProjection();
void init_vao();
void init_shader();
void init_view();
void init();
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
void update_zoom_in();
void update_animate();
void update_mesh();
void update_shader();
void update();
void draw();
int main(int argc, char* argv[]);
#endif
