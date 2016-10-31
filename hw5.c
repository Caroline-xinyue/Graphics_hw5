//
//  hw5.c
//
//
//  Created by Xinyue Zhang on 10/30/16.
//
//

#include "hw5.h"

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
    GLuint bufnum;
    glGenBuffers(1, &bufnum);
    glBindBuffer(type, bufnum);
    glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
    return bufnum;
}

GLfloat degree_to_radians(GLfloat degree) {
    return degree / 180.0f * M_PI;
}

GLfloat radians_to_degree(GLfloat radians) {
    return radians / M_PI * 180.0f;
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if(action == GLFW_RELEASE || action == GLFW_REPEAT) {
    switch(key) {
        case 'v':
            drawMode = VERTEX;
            break;
        case 'e':
            drawMode = EDGE;
            break;
        case 't':
            drawMode = FACE;
            break;
        case 's':
            shadeMode = SMOOTH;
            break;
        case 'f':
            shadeMode = FLAT;
            break;
        case 'p':
            shadeMode = PHONG;
            break;
        case 'a':
            animateState = !animateState;
            break;
        case 'P':
            parallelMode = !parallelMode;
            break;
        case 'q':
        case 'Q':
            glfwSetWindowShouldClose(w, GL_TRUE);
            break;
        default:
            break;
    }
  }
}

/*
    vbos[0] = make_bo(GL_ARRAY_BUFFER, &points[0], points.size() * sizeof(GLfloat));
    vbos[1] = make_bo(GL_ELEMENT_ARRAY_BUFFER, &elements[0], elements.size() * sizeof(GLuint));
*/

/*
void init_vao() {
    // global variable number of files/meshes
    for(int i = 0; i < num_meshes; i++) {
        all_vertices.pushback(meshes[i].vertices);
        all_indices = meshes[i].faces.triangulated_indices_list;
    }
}
*/

/*
void init_vao() {
    for(int i = 0; i < num_meshes; i++) {
        all_vertices.insert(std::end(meshes[i].vertices), std::begin(all_vertices), std::end(all_vertices));
        all_indices.insert(std::end(meshes[i].faces.indices_list), std::begin(all_indices), std::end(all_indices));
    }
}
*/

void init_vao() {
    for(int i = 0; i < num_meshes; i++) {
        for(int k = 0; k < mesh.numVertices; k++) {
            all_vertices.push_back(mesh.vertices[k].position[0]);
            all_vertices.push_back(mesh.vertices[k].position[1]);
            all_vertices.push_back(mesh.vertices[k].position[2]);
            all_vertices.push_back(mesh.vertices[k].normal[0]);
            all_vertices.push_back(mesh.vertices[k].normal[1]);
            all_vertices.push_back(mesh.vertices[k].normal[2]);
        }
        all_indices.push_back(mesh.faces.triangulated_indices_list);
   }
}

void init_shader() {
    // Load shaders and use the resulting shader program
    program = initshader( "gouraud_vs.glsl", "gouraud_fs.glsl");
    glUseProgram(program);

    material.ambient = glm::vec4(0.0215, 0.1745, 0.0215, 1.0);
    material.diffuse = glm::vec4(0.07568, 0.61424, 0.07568, 1.0);
    material.specular = glm::vec4(0.633, 0.727811, 0.633, 1.0);
    material.shininess = 0.6;
    material.ambient = glm::vec4(1.0, 1.0, 1.0, 1.0);
    material.diffuse = glm::vec4(1.0, 1.0, 1.0, 1.0);
    material.specular = glm::vec4(1.0, 1.0, 1.0, 1.0);
    light.lightPos = glm::vec4(0.0, 0.0, 6.0, 1.0);
    glClearColor( 0.0, 0.0, 0.0, 1.0); // black background
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

void update_shader() {
    if(shadeMode == GOURAUD) {
        // Load shaders and use the resulting shader program
        program = initshader( "gouraud_vs.glsl", "gouraud_fs.glsl");
        glUseProgram(program);    
    } else if(shadeMode == PHONG) {
        // Load shaders and use the resulting shader program
        program = initshader( "phong_vs.glsl", "phong_fs.glsl");
        glUseProgram(program);
    }
    // Create a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object
    vbos[0] = make_bo(GL_ARRAY_BUFFER, &all_vertices[0], all_vertices.size() * sizeof(GLfloat));
    ebos[0] = make_bo(GL_ELEMENT_ARRAY_BUFFER, &all_indices[0], all_indices.size() * sizeof(GLuint));

    // Initialize the vertex position attribute from the vertex shader
    pos = glGetAttribLocation(program, "vPosition");
    //must have vbo bound
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, VBO_SIZE * sizeof(GLfloat), (void*) 0); // input to vs.glsl vPos

    norm = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(norm);
    glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, VBO_SIZE * sizeof(GLfloat), (void*)(VBO_SIZE/2 * sizeof(GLfloat))); // input to vs.glsl vColor

    GLuint ambientID = glGetUniformLocation(program, "vAmbientProduct");
    glUniform4fv(ambientID, 1, glm::value_ptr(material.ambient));

    GLuint diffuseID = glGetUniformLocation(program, "vDiffuseProduct");
    if(diffuseID != -1) {
         glUniform4fv(diffuseID, 1, glm::value_ptr(material.diffuse));
    }

    GLuint specularID = glGetUniformLocation(program, "vSpecularProduct");
    if(specularID != -1) {
        glUniform4fv(specularID, 1, glm::value_ptr(material.specular));
    }

    GLuint lightPosID = glGetUniformLocation(program, "vLightPosition");
    if(lightPosID != -1) {
        glUniform4fv(lightPosID, 1, glm::value_ptr(light.lightPos));3
    }

    GLuint shininessID = glGetUniformLocation(program, "vShininess");
    if(shininessID != -1) {
        glUniform1f(shininessID, material.shininess);
    }
    glClearColor( 0.0, 0.0, 0.0, 1.0); // black background
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
    camera.far = 100.0f;
    camera.eye = glm::vec3(4,3,6);
    camera.center = glm::vec3(0,0,0);
    camera.up = glm::vec3(0,1,0);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPointSize(2);
    init_file();
    init_vao();
    init_shader();
    init_modelViewProjection();
}

void print_debug_info() {

}

void update_modelViewProjection() {
    if(parallelMode) {
        Projection = glm::ortho(camera.ortho_left, camera.ortho_right, camera.ortho_bottom, camera.ortho_top, camera.near, camera.far);
    } else {
        Projection = glm::perspective(glm::radians(camera.perspective_angle), camera.perspective_ratio, camera.near, camera.far);
    }

    // Projection matrix
    GLuint projectionID = glGetUniformLocation(program, "vProjection");
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(Projection));

    // Camera matrix
    View = glm::lookAt(camera.eye, camera.center, camera.up);
    GLuint viewID = glGetUniformLocation(program, "vView");
    glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(View));

    // Model matrix : an identity matrix (model will be at the origin)
    Model = glm::mat4(1.0f);
    GLuint modelID = glGetUniformLocation(program, "vModel");
    glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(Model));
}

void update() {
#if DEBUG
    print_debug_info();
#endif
    if(animeState) {
        update_animate();
    }
    update_shader();
    update_modelViewProjection();
}

void draw() {
    // divide into grids
    // if flat, glDrawArray
    if(drawMode == VERTEX) {
        //glDrawElements(GL_POINTS, meshes[i].face.triangulated_indices_list.size(), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_POINTS, 0, newMesh.numVertices);
    } else if(drawMode == EDGE) {
        glDrawElements(GL_LINES, );
    } else {
        if(shadeMode == FLAT) {
            glDrawArrays(GL_TRIANGLES, void*(0), numPoints);
        } else if(shadeMode == GOURAUD || shadeMode == PHONG) {
            glDrawElements(GL_TRIANGLES, meshes[i].face.triangulated_indices_list.size(), GL_UNSIGNED_INT, 0);
    
        }
    }
}

int main(int argc, char **argv) {
    // start GL context and O/ window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    // Initializes random number generator
    srand(time(NULL));
    
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow *window = glfwCreateWindow(1000, 1000, "Graphics HW4", NULL, NULL);
    if(!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwSetWindowPos(window, 100, 100);
    glfwMakeContextCurrent(window);
    
    // will segfault otherwise in init()
    glewExperimental = GL_TRUE;
    glewInit();

    printf("%s\n", glGetString(GL_VERSION));
    
    // Assign keyboard() to be the function called whenever
    // a key is pressed or released
    glfwSetKeyCallback(window, keyboard);

    init();
    start = glfwGetTime();
    
    while(!glfwWindowShouldClose(window)) {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0);
        draw();
        if(animeState == RESUME) {
          update();
        }
        glfwSwapBuffers(window);
        // update other events like input handling
        glfwPollEvents();
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}
