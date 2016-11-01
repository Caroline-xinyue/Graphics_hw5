#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "common.h"
#include "file.h"

using namespace std;

#define BOID_RADIUS 0.5
#define CUBE_SIZE 1
#define TIME_DURATION 0.1
#define STEP 0.1

const int numPoints = 0;
GLuint vao, vbos[2], program, pos, norm;
GLfloat start = 0.0f;
GLfloat last = 0.0f;
GLfloat angle = 0.0f;
glm::mat4 Projection;
glm::mat4 Model;
glm::mat4 View;
bool animateState = false;
Mesh newMesh;
Faces newFace;
Camera camera;
int vertex_num=0;
int face_num=0;
int numMeshes = 0;

/*
static const GLfloat points[24][6] = {
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, 0.33, 0.33}, // 0:0
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, 0.33, 0.33}, // 1:0
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, 0.33, 0.33}, // 2:0
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, 0.33, 0.33}, // 3:1
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, 0.33, 0.33}, // 4:1
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, 0.33, 0.33}, // 5:1
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, 0.33, -0.33}, // 6:2
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, 0.33, -0.33}, // 7:2
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, 0.33, -0.33}, // 8:2
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, 0.33, -0.33}, // 9:3
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, 0.33, -0.33}, // 10:3
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, 0.33, -0.33}, // 11:3
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, -0.33, -0.33}, // 12:4
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, -0.33, -0.33}, // 13:4
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, -0.33, -0.33}, // 14:4
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, -0.33, -0.33}, // 15:5
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, -0.33, -0.33}, // 16:5
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, -0.33, -0.33}, // 17:5
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, -0.33, 0.33}, // 18:6
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, -0.33, 0.33}, // 19:6
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, -0.33, 0.33}, // 20:6
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, -0.33, 0.33}, // 21:7
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, -0.33, 0.33}, // 22:7
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, -0.33, 0.33}, // 23:7
};
*/

std::vector<Vertex> vertices;
std::vector<GLfloat> all_vertices;
std::vector<GLuint> all_indices;

/*
std::vector<GLfloat> points = {
    -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, 0.33, 0.33, // 0:0
    -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, 0.33, 0.33, // 1:0
    -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, 0.33, 0.33, // 2:0
    CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, 0.33, 0.33, // 3:1
    CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, 0.33, 0.33, // 4:1
    CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, 0.33, 0.33, // 5:1
    CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, 0.33, -0.33, // 6:2
    CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, 0.33, -0.33, // 7:2
    CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, 0.33, -0.33, // 8:2
    -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, 0.33, -0.33, // 9:3
    -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, 0.33, -0.33, // 10:3
    -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, 0.33, -0.33, // 11:3
    -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, -0.33, -0.33, // 12:4
    -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, -0.33, -0.33, // 13:4
    -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, -0.33, -0.33, // 14:4
    CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, -0.33, -0.33, // 15:5
    CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, -0.33, -0.33, // 16:5
    CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, -0.33, -0.33, // 17:5
    CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, -0.33, 0.33, // 18:6
    CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, -0.33, 0.33, // 19:6
    CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, -0.33, 0.33, // 20:6
    -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, -0.33, 0.33, // 21:7
    -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, -0.33, 0.33, // 22:7
    -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, -0.33, 0.33, // 23:7
};
*/

/*
std::vector<GLuint> elements {
    1, 10, 4,
    10, 7, 4,
    3, 6, 18,
    6, 15, 18,
    5, 2, 23,
    20, 5, 23,
    12, 21, 9,
    21, 0, 9,
    16, 19, 13,
    19, 22, 13,
    11, 14, 8,
    14, 17, 8,
};
*/

/*
static const GLuint elements[] = {
    1, 10, 4,
    10, 7, 4,
    3, 6, 18,
    6, 15, 18,
    5, 2, 23,
    20, 5, 23,
    12, 21, 9,
    21, 0, 9,
    16, 19, 13,
    19, 22, 13,
    11, 14, 8,
    14, 17, 8,
};
*/

/*
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0},
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0},
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0},
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0},
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0},
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0},
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0},
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0}

    0, 3, 1,
    3, 2, 1,
    1, 2, 6,
    2, 5, 6,
    1, 0, 7,
    6, 1, 7,
    4, 7, 3,
    7, 0, 3,
    5, 6, 4,
    6, 7, 4,
    3, 4, 2,
    4, 5, 2,

0: 0 1 2
1: 3 4 5
2: 6 7 8
3: 9 10 11
4: 12 13 14
5: 15 16 17
6: 18 19 20
7: 21 22 23
*/

/*
static const GLfloat points[8][6] = {
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, 0.33, 0.33},
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, 0.33, 0.33},
    {CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, 0.33, -0.33},
    {-CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, 0.33, -0.33},
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -0.33, -0.33, -0.33},
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, 0.33, -0.33, -0.33},
    {CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, 0.33, -0.33, 0.33},
    {-CUBE_SIZE / 2.0, -CUBE_SIZE / 2.0, CUBE_SIZE / 2.0, -0.33, -0.33, 0.33}
};

static const GLuint elements[] = {
    0, 3, 1,
    3, 2, 1,
    1, 2, 6,
    2, 5, 6,
    1, 0, 7,
    6, 1, 7,
    4, 7, 3,
    7, 0, 3,
    5, 6, 4,
    6, 7, 4,
    3, 4, 2,
    4, 5, 2,
};
*/

/*
static const GLfloat pnormal[][3] = {
    {0, 1, 0},
    //{0, 1, 0},
    {1, 0, 0},
    //{1, 0, 0},
    {0, 0, 1},
    //{0, 0, 1},
    {-1, 0, 0},    
    //{-1, 0, 0},    
    {0, -1, 0},
    //{0, -1, 0},
    {0, 0, -1},
    //{0, 0, -1}       
};
*/

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
    GLuint bufnum;
    glGenBuffers(1, &bufnum);
    glBindBuffer(type, bufnum);
    glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
    return bufnum;
}
void makeFlatDuplicate (glm::vec3 *position1,glm::vec3 *position2,glm::vec3 *position3, glm::vec3 *corss){

    Vertex_ref temp1;
    Vertex_ref temp2;
    Vertex_ref temp3;
    /*
    temp1.position =newMesh.vertices[index1].position;
    temp1.normal = corss;
    temp2.position =newMesh.vertices[index2].position;
    temp2.normal = corss;
    temp3.position =newMesh.vertices[index3].position;
    temp3.normal = corss;*/
    temp1.position = position1;
    temp1.normal = corss;
    temp2.position = position2;
    temp2.normal = corss;
    temp3.position = position3;
    temp3.normal = corss;
    cout<<"!!!!!!"<<glm::to_string(*(temp1.normal))<<endl;
    cout<<"!!!!!!"<<glm::to_string(*(temp2.normal))<<endl;
    cout<<"!!!!!!"<<glm::to_string(*(temp3.normal))<<endl;
    cout << "BEFORE&&&&&&&&&&&&&&" << newMesh.vertices_for_flat.size() << endl;
    newMesh.vertices_for_flat.push_back(temp1);
    newMesh.vertices_for_flat.push_back(temp2);
    newMesh.vertices_for_flat.push_back(temp3);
    cout << "SIZE!!!!!!!!" << newMesh.vertices_for_flat.size() << endl;
    cout<<"#####"<<glm::to_string(*((newMesh.vertices_for_flat)[2].normal))<<endl;
    cout<<"#####"<<glm::to_string(*((newMesh.vertices_for_flat)[2].normal))<<endl;
    cout<<"#####"<<glm::to_string(*((newMesh.vertices_for_flat)[2].normal))<<endl;
}
void makeLineDuplicate(glm::vec3 *position1,glm::vec3 *position2, glm::vec3 *corss){
    cout<<"here"<<endl;
    Vertex_ref temp1;
    
    temp1.normal = corss;
    temp1.position = position1;
    Vertex_ref temp2;
    temp2.normal = corss;
    temp2.position = position2;
    cout<<"@@@@@@"<<glm::to_string(*(temp1.normal))<<endl;
    cout<<"@@@@@@"<<glm::to_string(*(temp2.normal))<<endl;
    newMesh.vertices_for_line.push_back(temp1);
    newMesh.vertices_for_line.push_back(temp2);
    cout<<"******"<<glm::to_string(*((newMesh.vertices_for_line)[0].normal))<<endl;
    cout<<"specularID"<<glm::to_string(*((newMesh.vertices_for_line)[1].normal))<<endl;
}
glm::vec3 crossByCounterClock(int index1,int index2,int index3){
    glm::vec3 crossVe1 = glm::vec3(newMesh.vertices[index1].position.x,newMesh.vertices[index1].position.y,newMesh.vertices[index1].position.z);
    glm::vec3 crossVe2 = glm::vec3(newMesh.vertices[index2].position.x,newMesh.vertices[index2].position.y,newMesh.vertices[index2].position.z);
    glm::vec3 crossVe3 = glm::vec3(newMesh.vertices[index3].position.x,newMesh.vertices[index3].position.y,newMesh.vertices[index3].position.z);
    glm::vec3 crossproduct = glm::cross(crossVe2-crossVe1,crossVe3-crossVe2);
    return crossproduct;

}

void readfile(string name){
    std::ifstream file(name);
    std::string str;
    std::getline(file, str);
    if(str != "OFF"){
        cout<<"File is not in right format. Please use an OFF file."<<endl;
        exit(0);
    }
    std::getline(file, str);
    stringstream fs(str);
    fs>>vertex_num>>face_num;
    newMesh.numVertices =vertex_num;
    newMesh.numFaces =face_num;
    vector<vector<glm::vec3>>mapping(vertex_num);
    vector<glm::vec4>mapping_sum(vertex_num);
    for(int i = 0; i < vertex_num; i++){
        std::getline(file, str);
        stringstream vs(str);
        GLfloat vertexx;
        GLfloat vertexy;
        GLfloat vertexz;
        vs>>vertexx>>vertexy>>vertexz;
        Vertex temp;
        temp.position =glm::vec3(vertexx,vertexy,vertexz);
        temp.normal = glm::vec3();
        newMesh.vertices.push_back(temp);
    }
    for(int i = 0; i <face_num; i++){
        int num_vertices_per;
        int index1;
        int index2;
        int index3;
        int index4 = 0;
        std::getline(file, str);
        stringstream vs(str);
        vs>>num_vertices_per>>index1>>index2>>index3>>index4;
        if(num_vertices_per < 3 || num_vertices_per > 4) {
            cout << "bad input line" << endl;
        }
        glm::vec3 corss = crossByCounterClock(index1,index2,index3);

        corss = glm::normalize(corss);
        //cout<<glm::to_string(corss)<<endl;;
        cout<<"index1_"<< glm::to_string(corss)<<endl;
        mapping[index1].push_back(corss);
        mapping[index2].push_back(corss);
        mapping[index3].push_back(corss);
        mapping_sum[index1]=mapping_sum[index1]+glm::vec4(corss,1);
        mapping_sum[index2]=mapping_sum[index2]+glm::vec4(corss,1);
        mapping_sum[index3]=mapping_sum[index3]+glm::vec4(corss,1);
        newFace.normal_list.push_back(corss);
        newFace.num_vertices_list.push_back(num_vertices_per);
        newFace.triangulated_indices_list.push_back(index1);
        newFace.triangulated_indices_list.push_back(index2);
        newFace.triangulated_indices_list.push_back(index3);

         makeLineDuplicate(&(newMesh.vertices[index1].position),&(newMesh.vertices[index2].position),&(newFace.normal_list.back()));
         makeLineDuplicate(&(newMesh.vertices[index2].position),&(newMesh.vertices[index3].position),&(newFace.normal_list.back()));
         makeFlatDuplicate (&(newMesh.vertices[index1].position),&(newMesh.vertices[index2].position),&(newMesh.vertices[index3].position),&(newFace.normal_list.back()));
        if(num_vertices_per==4){
            mapping[index4].push_back(corss);
            mapping_sum[index4]=mapping_sum[index4]+glm::vec4(corss,1);
            cout<<"index4_"<< mapping[index4].size()<<endl;
            newFace.triangulated_indices_list.push_back(index4);
            newFace.triangulated_indices_list.push_back(index1);
            newFace.triangulated_indices_list.push_back(index3); 
            
            makeLineDuplicate(&(newMesh.vertices[index3].position),&(newMesh.vertices[index4].position),&(newFace.normal_list.back()));
            makeLineDuplicate(&(newMesh.vertices[index4].position),&(newMesh.vertices[index1].position),&(newFace.normal_list.back()));
            makeFlatDuplicate (&(newMesh.vertices[index4].position),&(newMesh.vertices[index1].position),&(newMesh.vertices[index3].position),&(newFace.normal_list.back()));
        }else{
            makeLineDuplicate(&(newMesh.vertices[index3].position),&(newMesh.vertices[index1].position),&(newFace.normal_list.back()));
        }
    }
    for(int i = 0; i < vertex_num;i++){
        glm::vec3 meannorm = glm::vec3(mapping_sum[i].x/mapping_sum[i].w,mapping_sum[i].y/mapping_sum[i].w,mapping_sum[i].z/mapping_sum[i].w);
        newMesh.vertices[i].normal =  glm::normalize(meannorm);
    }
    for(int i =0; i<newMesh.vertices_for_flat.size();i++){
        //cout<<"index"<<i<<endl;
        cout<<glm::to_string(*(newMesh.vertices_for_flat[i].position))<<endl;
        cout<<glm::to_string(*(newMesh.vertices_for_flat[i].normal))<<endl;
        //cout<<glm::to_string(newMesh.vertices[i].normal)<<endl;
    }
    file.close();
}

void init_view() {
    //Projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100000.0f);
    //Projection = glm::ortho(camera.ortho_left, camera.ortho_right, camera.ortho_bottom, camera.ortho_top, camera.near, camera.far);
    Projection = glm::perspective(glm::radians(camera.perspective_angle), camera.perspective_ratio, camera.near, camera.far);

    GLuint projectionID = glGetUniformLocation(program, "vProjection");
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(Projection));
    // Camera matrix
    /*
    View = glm::lookAt(
                                 glm::vec3(400,300,600), // Camera is at (4,3,3), in World Space
                                 glm::vec3(0,0,0), // and looks at the origin
                                 glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                      );
    */
    View = glm::lookAt(camera.eye, camera.center, camera.up);
    // Model matrix : an identity matrix (model will be at the origin)
    Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices

    /*
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around
    GLuint MatrixID = glGetUniformLocation(program, "vModelViewProjection");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    */

    //Modelview = View * Model; // Remember, matrix multiplication is the other way around
    GLuint modelID = glGetUniformLocation(program, "vModel");
    glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(Model));
    GLuint viewID = glGetUniformLocation(program, "vView");
    glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(View));
}

void init_vao() {

    /*
    for(int i = 0; i < 24; i++) {
        Vertex temp;
        temp.position = glm::vec3(points[i][0], points[i][1], points[i][2]);
        temp.normal = glm::vec3(points[i][3], points[i][4], points[i][5]);
        vertices.push_back(temp);
    }

    for(int i = 0; i < 24; i++) {
        std::cout << "position" << glm::to_string(vertices[i].position) << " normal" << glm::to_string(vertices[i].normal) << std::endl;  
    }

    for(int i = 0; i < 24; i++) {
            all_vertices.push_back(vertices[i].position[0]);
            all_vertices.push_back(vertices[i].position[1]);
            all_vertices.push_back(vertices[i].position[2]);
            all_vertices.push_back(vertices[i].normal[0]);
            all_vertices.push_back(vertices[i].normal[1]);
            all_vertices.push_back(vertices[i].normal[2]);
    }
*/


// draw faces and points
/*
    for(int i = 0; i < newMesh.numVertices; i++) {
        all_vertices.push_back(newMesh.vertices[i].position[0]);
        all_vertices.push_back(newMesh.vertices[i].position[1]);
        all_vertices.push_back(newMesh.vertices[i].position[2]);
        all_vertices.push_back(newMesh.vertices[i].normal[0]);
        all_vertices.push_back(newMesh.vertices[i].normal[1]);
        all_vertices.push_back(newMesh.vertices[i].normal[2]);
    }
    all_indices = newFace.triangulated_indices_list;
    */

    // draw edges

/*
    std::cout << "#############################################" << std::endl;
    for(int i = 0; i < newMesh.vertices_for_line.size(); i++) {
        all_vertices.push_back((*(newMesh.vertices_for_line[i].position))[0]);
        all_vertices.push_back((*(newMesh.vertices_for_line[i].position))[1]);
        all_vertices.push_back((*(newMesh.vertices_for_line[i].position))[2]);
        all_vertices.push_back((*(newMesh.vertices_for_line[i].normal))[0]);
        all_vertices.push_back((*(newMesh.vertices_for_line[i].normal))[1]);
        all_vertices.push_back((*(newMesh.vertices_for_line[i].normal))[2]);
        std::cout << glm::to_string(*(newMesh.vertices_for_line[i].position)) << std::endl;
    }
    */

    for(int i = 0; i < newMesh.vertices_for_flat.size(); i++) {
        all_vertices.push_back((*(newMesh.vertices_for_flat[i].position))[0]);
        all_vertices.push_back((*(newMesh.vertices_for_flat[i].position))[1]);
        all_vertices.push_back((*(newMesh.vertices_for_flat[i].position))[2]);
        all_vertices.push_back((*(newMesh.vertices_for_flat[i].normal))[0]);
        all_vertices.push_back((*(newMesh.vertices_for_flat[i].normal))[1]);
        all_vertices.push_back((*(newMesh.vertices_for_flat[i].normal))[2]);
       // std::cout << glm::to_string(*(newMesh.vertices_for_flat[i].normal)) << std::endl;
    }
}

void init_shader() {

    glEnable(GL_DEPTH_TEST);
    // Load shaders and use the resulting shader program
    program = initshader( "gouraud_vs.glsl", "gouraud_fs.glsl");
    //program = initshader( "phong_vs.glsl", "phong_fs.glsl");
    glUseProgram(program);
    
    // Create a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Create and initialize a buffer object
    //vbos[0] = make_bo(GL_ARRAY_BUFFER, &points[0], points.size() * sizeof(GLfloat));
    /*
    vbos[0] = make_bo(GL_ARRAY_BUFFER, &vertices[0], vertices.size() * sizeof(GLfloat));
    vbos[1] = make_bo(GL_ELEMENT_ARRAY_BUFFER, &elements[0], elements.size() * sizeof(GLuint));
    */

    vbos[0] = make_bo(GL_ARRAY_BUFFER, &all_vertices[0], all_vertices.size() * sizeof(GLfloat));
    //vbos[1] = make_bo(GL_ELEMENT_ARRAY_BUFFER, &elements[0], elements.size() * sizeof(GLuint));
    vbos[1] = make_bo(GL_ELEMENT_ARRAY_BUFFER, &all_indices[0], all_indices.size() * sizeof(GLuint));

    // Initialize the vertex position attribute from the vertex shader
    pos = glGetAttribLocation(program, "vPosition");
    //must have vbo bound
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) 0); // input to vs.glsl vPos

    norm = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(norm);
    glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // input to vs.glsl vColor

/*
    vbos[2] = make_bo(GL_ARRAY_BUFFER, pnormal, sizeof(pnormal));
    norm = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(norm);
    glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0); // input to vs.glsl vColor
 */

    glClearColor( 0.0, 0.0, 0.0, 1.0); // black background

    glm::vec4 ambient = glm::vec4(0.0215, 0.1745, 0.0215, 1.0);
    GLuint ambientID = glGetUniformLocation(program, "vAmbientProduct");
    glUniform4fv(ambientID, 1, glm::value_ptr(ambient));

    glm::vec4 diffuse = glm::vec4(0.07568, 0.61424, 0.07568, 1.0);
    GLuint diffuseID = glGetUniformLocation(program, "vDiffuseProduct");
    if(diffuseID != -1) {
         glUniform4fv(diffuseID, 1, glm::value_ptr(diffuse));       
    }

    glm::vec4 specular = glm::vec4(0.633, 0.727811, 0.633, 1.0);
    GLuint specularID = glGetUniformLocation(program, "vSpecularProduct");
    if(specularID != -1) {
        glUniform4fv(specularID, 1, glm::value_ptr(specular));        
    }

    GLuint shininessID = glGetUniformLocation(program, "vShininess");
    if(shininessID != -1) {
        glUniform1f(shininessID, 0.6);
    }
}

void init_modelViewProjection() {
    // Projection matrix
    camera.ortho_left = -1;
    camera.ortho_right = 1;
    camera.ortho_bottom = -1;
    camera.ortho_top = 1;
    camera.perspective_angle = 45;
    camera.perspective_ratio = 1.0f;
    camera.near = 1.0f;
    camera.far = 100000.0f;
    camera.eye = glm::vec3(4,3,6);
    camera.center = glm::vec3(0,0,0);
    camera.up = glm::vec3(0,1,0);
}

void init() {
    glPointSize(2);
    init_vao();
    init_shader();
    init_modelViewProjection();
    init_view();
    start = glfwGetTime();
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
    if(action == GLFW_RELEASE || action == GLFW_REPEAT) {
        switch (key) {
            case 'a':
            case 'A':
                animateState = !animateState;
                std::cout << animateState << std::endl;
                break;
            case GLFW_KEY_ESCAPE:
            case 'q':
            case 'Q':
                glfwSetWindowShouldClose(w, GL_TRUE);
                break;
        }
    }
}

void update_animate() {
    last = glfwGetTime();
    if(last - start > TIME_DURATION) {
        angle += STEP;
        if(angle > 360) {
            angle -= 360;
        }
        start = last;
    }
}

void update_camera() {
    Model = glm::mat4(1.0f);
    Model = glm::rotate(Model, angle, glm::vec3(0, 0, 1));
    //Modelview = View * Model; // Remember, matrix multiplication is the other way around
    GLuint modelID = glGetUniformLocation(program, "vModel");
    glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(Model));
    GLuint viewID = glGetUniformLocation(program, "vView");
    glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(View));

    glm::vec4 lightPos = glm::vec4(0.0, 0.0, 5.0, 1.0);
    lightPos = View * lightPos;
    GLuint lightPosID = glGetUniformLocation(program, "vLightPosition");
    if(lightPosID != -1) {
        glUniform4fv(lightPosID, 1, glm::value_ptr(lightPos));
    }
}

/*
void update_vertices_position(std::vector<Vertex>& vertices, GLuint vertex_num, GLuint ind){
    glm::vec3 range = max_pos-min_pos;
    print3v(max_pos, "max_pos");
    print3v(min_pos, "min_pos");
    glm::vec3 ratio = glm::vec3(1.0f, 1.0f, 1.0f);
    ratio[1] = range[1]/range[0];
    ratio[2] = range[2]/range[0];
    for(int i=0; i<vertex_num; i++){
        for(int j=0; j<3; j++){
            GLfloat pos_value = vertices[i].position[j];
            vertices[i].position[j] = ratio[j]*2*(pos_value - min_pos[j])/range[j] - 1 + 5*ind;
        }
    }
}
*/

void update() {
    //std::cout << animateState << std::endl;
    if(animateState) {
        update_animate();       
    }
    update_camera();
}

void draw() {
    //glDrawElements(GL_TRIANGLES, numPoints, GL_UNSIGNED_INT, 0);
    // real ones
    //glDrawElements(GL_TRIANGLES, newFace.triangulated_indices_list.size(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_POINTS, 0, newMesh.numVertices);
    //glDrawArrays(GL_LINES, 0, newMesh.vertices_for_line.size());

    glDrawArrays(GL_TRIANGLES, 0, newMesh.vertices_for_flat.size());
}

int main(int argc, char* argv[]) {
    if(argc<2){
        cout<<"File not found, please input file"<<endl;
         exit(0);
    }
    string name = argv[1];
    readfile(name); 

    // start GL context and O/ window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow *window = glfwCreateWindow(512, 512, "Hello Triangle", NULL, NULL);
    if(!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    // will segfault otherwise in init()
    glewExperimental = GL_TRUE;
    glewInit();
    
    printf("%s\n", glGetString(GL_VERSION));
    
    init();
    glfwSetKeyCallback(window, keyboard);

    while(!glfwWindowShouldClose(window)) {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw();
        update();
        //glDrawArrays(GL_TRIANGLES, 0, numPoints);
        glfwSwapBuffers(window);
        // update other events like input handling
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
