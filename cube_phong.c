//
//  triangle.c
//  
//
//  Created by Xinyue Zhang on 10/7/16.
//
//
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define BOID_RADIUS 0.5
#define CUBE_SIZE 1
#define TIME_DURATION 0.1
#define STEP 0.1

const int numPoints = 36;
GLuint vao, vbos[3], program, pos, col, norm;
GLfloat start = 0.0f;
GLfloat last = 0.0f;
GLfloat angle = 0.0f;
glm::mat4 Projection;
glm::mat4 Model;
glm::mat4 View;

// Specify the vertices for a triangle
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

// reference no need
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

void init_view() {
    Projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    GLuint projectionID = glGetUniformLocation(program, "vProjection");
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(Projection));
    // Camera matrix
    View = glm::lookAt(
                                 glm::vec3(4,3,6), // Camera is at (4,3,3), in World Space
                                 glm::vec3(0,0,0), // and looks at the origin
                                 glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                      );
    
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

void init_shader() {
    glEnable(GL_DEPTH_TEST);
    // Load shaders and use the resulting shader program
    program = initshader( "phong_vs.glsl", "phong_fs.glsl");
    glUseProgram(program);
    
    // Create a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Create and initialize a buffer object
    vbos[0] = make_bo(GL_ARRAY_BUFFER, points, sizeof(points));
    vbos[1] = make_bo(GL_ELEMENT_ARRAY_BUFFER, elements, sizeof(elements));
    
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

    glm::vec4 lightPos = glm::vec4(0.0, 0.0, 6.0, 1.0);
    GLuint lightPosID = glGetUniformLocation(program, "vLightPosition");
    if(lightPosID != -1) {
        glUniform4fv(lightPosID, 1, glm::value_ptr(lightPos));
    }

    GLuint shininessID = glGetUniformLocation(program, "vShininess");
    if(shininessID != -1) {
        glUniform1f(shininessID, 0.6);
    }
}

void init() {
    init_shader();
    init_view();
    start = glfwGetTime();
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
    switch (key) {
        case GLFW_KEY_ESCAPE:
        case 'q':
        case 'Q':
            glfwSetWindowShouldClose(w, GL_TRUE);
            break;
    }
}

void update_modelview() {
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
    lightPos = View * Model * lightPos;
    GLuint lightPosID = glGetUniformLocation(program, "vLightPosition");
    if(lightPosID != -1) {
        glUniform4fv(lightPosID, 1, glm::value_ptr(lightPos));
    }
}

void update() {
    update_modelview();
    update_camera();
}

void draw() {
    glDrawElements(GL_TRIANGLES, numPoints, GL_UNSIGNED_INT, 0);
    //glDrawElements(GL_TRIANGLES, numPoints, GL_UNSIGNED_INT, 0);
}

int main(int argc, char **argv) {
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
