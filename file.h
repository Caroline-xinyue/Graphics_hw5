#ifndef FILE_H
#define FILE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include <vector>
#include <cstring>
using namespace std;

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
}

struct Material {
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  GLfloat shininess;
};

// single light no need
struct Light {
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  glm::vec4 lightPos;
};

struct Camera {
  GLfloat ortho_left;
  GLfloat ortho_right;
  GLfloat ortho_bottom;
  GLfloat ortho_top;
  GLfloat perspective_angle;
  GLfloat perspective_ratio;
  GLfloat near;
  GLfloat far;
  glm::vec3 eye;
  glm::vec3 center;
  glm::vec3 up;
};

struct Faces {
    vector<GLint> num_vertices_list;
    vector<GLuint> indices_list;
    vector<GLuint> triangulated_indices_list;
    vector<glm::vec3> normal_list;
};
struct Mesh{
    GLint numVertices;
    GLint numFaces;
    GLint numEdges;
    vector<Vertex> vertices;
    Faces faces;
};

#endif
