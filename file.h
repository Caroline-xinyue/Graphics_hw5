#ifndef __FILE.H__
#define __FILE.H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include <vector>
#include <cstring>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
};

struct Material {
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  GLfloat shininess;
};

// single light no need
struct Light {
  //glm::vec4 ambient;
  //glm::vec4 diffuse;
  //glm::vec4 specular;
  //glm::vec4 lightPos;
};

struct Camera {
  GLfloat perspective_angle;
  GLfloat perspective_ratio;
  GLfloat perspective_near;
  GLfloat perspective_far;
 // glm::vec3 eye;
  //glm::vec3 center;
  //glm::vec3 up;
};

struct Faces {
    vector<GLint> num_vertices_list;
    vector<GLuint> indices_list;
    vector<GLuint> triangulated_indices_list;
    //vector<glm::vec3> triangulated_normal_list;
};

/*
struct Face {
    GLint face_numV;
    vector<GLuint> indices;
    vector<GLuint> triangulated_indices;
    glm::vec3 face_normal;
};
*/

class Mesh {
public:
  GLint numVertices;
  GLint numFaces;
  GLint numEdges;
  vector<Vertex> vertices;
  Faces faces;
  //vector<Face> faces;
public:
  GLuint VAO, VBO, EBO;
  Mesh();
  void bind();
  void calculate_normal();
  void draw();
};
