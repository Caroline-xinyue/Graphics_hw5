#include "hw5.h"

static GLuint make_bo(GLenum type, const void *buf, GLsizei buf_size) {
    GLuint bufnum;
    glGenBuffers(1, &bufnum);
    glBindBuffer(type, bufnum);
    glBufferData(type, buf_size, buf, GL_STATIC_DRAW);
    return bufnum;
}

void makeFlatDuplicate (glm::vec3 *position1,glm::vec3 *position2,glm::vec3 *position3, glm::vec3 corss,int MeshIndex){
    //Mesh&meshes[MeshIndex] = meshes[MeshIndex];

    Vertex_ref temp1;
    Vertex_ref temp2;
    Vertex_ref temp3;
    /*
    temp1.position =meshes[MeshIndex].vertices[index1].position;
    temp1.normal = corss;
    temp2.position =meshes[MeshIndex].vertices[index2].position;
    temp2.normal = corss;
    temp3.position =meshes[MeshIndex].vertices[index3].position;
    temp3.normal = corss;*/
    temp1.position = position1;
    temp1.normal = corss;
    temp2.position = position2;
    temp2.normal = corss;
    temp3.position = position3;
    temp3.normal = corss;
   // cout<<"!!!!!!"<<glm::to_string(*(temp1.normal))<<endl;
  //  cout<<"!!!!!!"<<glm::to_string(*(temp2.normal))<<endl;
   // cout<<"!!!!!!"<<glm::to_string(*(temp3.normal))<<endl;
   // cout << "BEFORE&&&&&&&&&&&&&&" <<meshes[MeshIndex].vertices_for_flat.size() << endl;
   meshes[MeshIndex].vertices_for_flat.push_back(temp1);
   meshes[MeshIndex].vertices_for_flat.push_back(temp2);
   meshes[MeshIndex].vertices_for_flat.push_back(temp3);
   // cout << "SIZE!!!!!!!!" <<meshes[MeshIndex].vertices_for_flat.size() << endl;
   cout<<"#####"<<glm::to_string(((meshes[MeshIndex].vertices_for_flat)[2].normal))<<endl;
   cout<<"#####"<<glm::to_string(((meshes[MeshIndex].vertices_for_flat)[2].normal))<<endl;
   cout<<"#####"<<glm::to_string(((meshes[MeshIndex].vertices_for_flat)[2].normal))<<endl;
}

void makeLineDuplicate(glm::vec3 *position1,glm::vec3 *position2, glm::vec3 corss,int MeshIndex){
    //Mesh&meshes[MeshIndex] = meshes[MeshIndex];
    cout<<"here"<<endl;
    Vertex_ref temp1;
    temp1.normal = corss;
    temp1.position = position1;
    Vertex_ref temp2;
    temp2.normal = corss;
    temp2.position = position2;
    //cout<<"@@@@@@"<<glm::to_string(*(temp1.normal))<<endl;
   // cout<<"@@@@@@"<<glm::to_string(*(temp2.normal))<<endl;
   meshes[MeshIndex].vertices_for_line.push_back(temp1);
   meshes[MeshIndex].vertices_for_line.push_back(temp2);
   // cout<<"******"<<glm::to_string(*((meshes[MeshIndex].vertices_for_line)[0].normal))<<endl;
   // cout<<"specularID"<<glm::to_string(*((meshes[MeshIndex].vertices_for_line)[1].normal))<<endl;
}

glm::vec3 crossByCounterClock(int index1,int index2,int index3,int MeshIndex){
       // Mesh&meshes[MeshIndex] = meshes[MeshIndex];
    glm::vec3 crossVe1 = glm::vec3(meshes[MeshIndex].vertices[index1].position.x,meshes[MeshIndex].vertices[index1].position.y,meshes[MeshIndex].vertices[index1].position.z);
    glm::vec3 crossVe2 = glm::vec3(meshes[MeshIndex].vertices[index2].position.x,meshes[MeshIndex].vertices[index2].position.y,meshes[MeshIndex].vertices[index2].position.z);
    glm::vec3 crossVe3 = glm::vec3(meshes[MeshIndex].vertices[index3].position.x,meshes[MeshIndex].vertices[index3].position.y,meshes[MeshIndex].vertices[index3].position.z);
    glm::vec3 crossproduct = glm::cross(crossVe2-crossVe1,crossVe3-crossVe2);
    return crossproduct;

}
void multipleFileHandle(int argc, char* argv[]){
    if(argc < 2) {
        cout<<"File not found, please input file"<<endl;
        exit(0);
    }
    Mesh tempMesh;
    map<string,int> m;
    cout << argc << endl;
    for(int i = 1; i < argc; i++) {
        cout << "argv[" << i << "] = " << argv[i] << endl;
        
        map<string,int>::iterator b = m.find(argv[i]);
        
        if (b == m.end()) {
            cout<<"test2"<<endl;
            meshes.push_back(tempMesh);
            meshes.back().numRepeats = 1;
            m[argv[i]]=meshes.size()-1;
            readfile(argv[i],m[argv[i]]);
        } else {int index = m[argv[i]];
            meshes[m[argv[i]]].numRepeats ++;
        }
    }
}
bool checkValidIndex(int a,int max) {
    if(a < 0 && a > max) {
        return false;
    } else {
        return true;
    }
}
void readfile(string name, int MeshIndex) {
    GLuint face_num = 0;
    GLuint vertex_num = 0;
    glm::vec3 maximum = glm::vec3(FLT_MIN,FLT_MIN,FLT_MIN);//how can you use this? global??
    glm::vec3 minimum = glm::vec3(FLT_MAX,FLT_MAX,FLT_MAX);
    glm::vec3 center = glm::vec3(0,0,0);
    //Mesh&meshes[MeshIndex] = meshes[MeshIndex];
    //Faces&meshes[MeshIndex].faces =meshes[MeshIndex].faces;
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
   meshes[MeshIndex].numVertices =vertex_num;
   meshes[MeshIndex].numFaces =face_num;
    vector<vector<glm::vec3>>mapping(vertex_num);
    vector<glm::vec4>mapping_sum(vertex_num);
    for(int i = 0; i < vertex_num; i++){
        std::getline(file, str);
        stringstream vs(str);
        GLfloat vertexx;
        GLfloat vertexy;
        GLfloat vertexz;
        vs>>vertexx>>vertexy>>vertexz;
        maximum.x = max(vertexx, maximum.x);
        maximum.y = max(vertexy, maximum.y);
        maximum.z = max(vertexz, maximum.z);
        minimum.x = min(vertexx, minimum.x);
        minimum.y = min(vertexy, minimum.y);
        minimum.z = min(vertexz, maximum.z);
        Vertex temp;
        temp.position =glm::vec3(vertexx,vertexy,vertexz);
        temp.normal = glm::vec3();
       meshes[MeshIndex].vertices.push_back(temp);
    }
    center = (maximum + minimum)/2;
    for(int i = 0; i <face_num; i++){
        int num_vertices_per;
        int index1;
        int index2;
        int index3;
        int index4 = 0;
        std::getline(file, str);
        stringstream vs(str);
        vs>>num_vertices_per>>index1>>index2>>index3>>index4;
        if(!checkValidIndex(index1,vertex_num) || !checkValidIndex(index2,vertex_num) || !checkValidIndex(index3,vertex_num) || !checkValidIndex(index4,vertex_num)){
            //cout << index1 << " " << index2 << " " << index3 << " " << index4 << " vertex_num" << vertex_num << endl;
            exit(0);
        }
        if(num_vertices_per < 3 || num_vertices_per > 4) {
            cout << "bad input line" << endl;
            exit(0);
        }
        glm::vec3 corss = crossByCounterClock(index1,index2,index3, MeshIndex);

        corss = glm::normalize(corss);
        //cout<<glm::to_string(corss)<<endl;;
        cout<<"index1_"<< glm::to_string(corss)<<endl;
        mapping[index1].push_back(corss);
        mapping[index2].push_back(corss);
        mapping[index3].push_back(corss);
        mapping_sum[index1]=mapping_sum[index1]+glm::vec4(corss,1);
        mapping_sum[index2]=mapping_sum[index2]+glm::vec4(corss,1);
        mapping_sum[index3]=mapping_sum[index3]+glm::vec4(corss,1);
       meshes[MeshIndex].faces.normal_list.push_back(corss);
       meshes[MeshIndex].faces.num_vertices_list.push_back(num_vertices_per);
       meshes[MeshIndex].faces.triangulated_indices_list.push_back(index1);
       meshes[MeshIndex].faces.triangulated_indices_list.push_back(index2);
       meshes[MeshIndex].faces.triangulated_indices_list.push_back(index3);

         makeLineDuplicate(&(meshes[MeshIndex].vertices[index1].position),&(meshes[MeshIndex].vertices[index2].position),corss,MeshIndex);
         makeLineDuplicate(&(meshes[MeshIndex].vertices[index2].position),&(meshes[MeshIndex].vertices[index3].position),corss,MeshIndex);
         makeFlatDuplicate (&(meshes[MeshIndex].vertices[index1].position),&(meshes[MeshIndex].vertices[index2].position),&(meshes[MeshIndex].vertices[index3].position),corss,MeshIndex);
        if(num_vertices_per==4){
            mapping[index4].push_back(corss);
            mapping_sum[index4]=mapping_sum[index4]+glm::vec4(corss,1);
            cout<<"index4_"<< mapping[index4].size()<<endl;
           meshes[MeshIndex].faces.triangulated_indices_list.push_back(index4);
           meshes[MeshIndex].faces.triangulated_indices_list.push_back(index1);
           meshes[MeshIndex].faces.triangulated_indices_list.push_back(index3); 
            
            makeLineDuplicate(&(meshes[MeshIndex].vertices[index3].position),&(meshes[MeshIndex].vertices[index4].position),corss,MeshIndex);
            makeLineDuplicate(&(meshes[MeshIndex].vertices[index4].position),&(meshes[MeshIndex].vertices[index1].position),corss,MeshIndex);
            makeFlatDuplicate (&(meshes[MeshIndex].vertices[index4].position),&(meshes[MeshIndex].vertices[index1].position),&(meshes[MeshIndex].vertices[index3].position),corss,MeshIndex);
        }else{
            makeLineDuplicate(&(meshes[MeshIndex].vertices[index3].position),&(meshes[MeshIndex].vertices[index1].position),corss,MeshIndex);
        }
    }
    for(int i = 0; i < vertex_num;i++){
        glm::vec3 meannorm = glm::vec3(mapping_sum[i].x/mapping_sum[i].w,mapping_sum[i].y/mapping_sum[i].w,mapping_sum[i].z/mapping_sum[i].w);
       meshes[MeshIndex].vertices[i].normal =  glm::normalize(meannorm);
    }
    for(int i =0; i<meshes[MeshIndex].vertices_for_flat.size();i++){
        //cout<<"index"<<i<<endl;
        cout<<glm::to_string(*(meshes[MeshIndex].vertices_for_flat[i].position))<<endl;
        cout<<glm::to_string(meshes[MeshIndex].vertices_for_flat[i].normal)<<endl;
        //cout<<glm::to_string(meshes[MeshIndex].vertices[i].normal)<<endl;
    }
    file.close();
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

void init_vao() {
    for(int k = 0; k < numMeshes; k++) {
        for(int i = 0; i < meshes[k].numVertices; i++) {
            vertices.push_back(meshes[k].vertices[i].position[0]);
            vertices.push_back(meshes[k].vertices[i].position[1]);
            vertices.push_back(meshes[k].vertices[i].position[2]);
            vertices.push_back(meshes[k].vertices[i].normal[0]);
            vertices.push_back(meshes[k].vertices[i].normal[1]);
            vertices.push_back(meshes[k].vertices[i].normal[2]);
        }
        indices = meshes[k].faces.triangulated_indices_list;

        for(int i = 0; i < meshes[k].vertices_for_line.size(); i++) {
            line_vertices.push_back((*(meshes[k].vertices_for_line[i].position))[0]);
            line_vertices.push_back((*(meshes[k].vertices_for_line[i].position))[1]);
            line_vertices.push_back((*(meshes[k].vertices_for_line[i].position))[2]);
            line_vertices.push_back(((meshes[k].vertices_for_line[i].normal))[0]);
            line_vertices.push_back(((meshes[k].vertices_for_line[i].normal))[1]);
            line_vertices.push_back(((meshes[k].vertices_for_line[i].normal))[2]);
        }

        for(int i = 0; i < meshes[k].vertices_for_flat.size(); i++) {
            flat_vertices.push_back((*(meshes[k].vertices_for_flat[i].position))[0]);
            flat_vertices.push_back((*(meshes[k].vertices_for_flat[i].position))[1]);
            flat_vertices.push_back((*(meshes[k].vertices_for_flat[i].position))[2]);
            flat_vertices.push_back(((meshes[k].vertices_for_flat[i].normal))[0]);
            flat_vertices.push_back(((meshes[k].vertices_for_flat[i].normal))[1]);
            flat_vertices.push_back(((meshes[k].vertices_for_flat[i].normal))[2]);
        }
    }
}

void init_shader() {
    glEnable(GL_DEPTH_TEST);
    
    // Create a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    vbos[0] = make_bo(GL_ARRAY_BUFFER, &vertices[0], vertices.size() * sizeof(GLfloat));
    vbos[1] = make_bo(GL_ELEMENT_ARRAY_BUFFER, &indices[0], indices.size() * sizeof(GLuint));

    // Initialize the vertex position attribute from the vertex shader
    pos = glGetAttribLocation(program, "vPosition");
    //must have vbo bound
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) 0); // input to vs.glsl vPos

    norm = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(norm);
    glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // input to vs.glsl vColor

    glClearColor( 0.0, 0.0, 0.0, 1.0); // black background
}

void init_view() {
    Projection = glm::perspective(glm::radians(camera.perspective_angle), camera.perspective_ratio, camera.near, camera.far);
    // Camera matrix
    View = glm::lookAt(camera.eye, camera.center, camera.up);
    // Model matrix : an identity matrix (model will be at the origin)
    Model = glm::mat4(1.0f);
    GLuint projectionID = glGetUniformLocation(program, "vProjection");
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(Projection));
    GLuint modelID = glGetUniformLocation(program, "vModel");
    glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(Model));
    GLuint viewID = glGetUniformLocation(program, "vView");
    glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(View));
}

void init() {
    // Load shaders and use the resulting shader program
    program = initshader( "gouraud_vs.glsl", "gouraud_fs.glsl");
    glUseProgram(program);
    glPointSize(2);
    numMeshes = meshes.size();
    init_vao();
    init_shader();
    init_modelViewProjection();
    init_view();
    start = glfwGetTime();
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if(action == GLFW_RELEASE || action == GLFW_REPEAT) {
    if(key == GLFW_KEY_UP) {
        if(action == GLFW_RELEASE) {
            key_Up = false;
        } else {
            key_Up = true;
        }
    }
    if(key == GLFW_KEY_DOWN) {
        if(action == GLFW_RELEASE) {
            key_Down = false;
        } else {
            key_Down = true;
        }
    }
    switch(key) {
        case 'v':
        case 'V':
            drawMode = VERTEX;
            break;
        case 'e':
        case 'E':
            drawMode = EDGE;
            break;
        case 't':
        case 'T':
            drawMode = FACE;
            break;
        case 's':
        case 'S':
            shadeMode = GOURAUD;
            break;
        case 'f':
        case 'F':
            shadeMode = FLAT;
            break;
        case 'p':
        case 'P':
            shadeMode = PHONG;
            break;
        case 'a':
        case 'A':
            animateState = !animateState;
            break;
        case 'l':
        case 'L':
            parallelView = !parallelView;
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

void update_modelViewProjection() {
    if(key_Up) {
        camera.eye = camera.eye * ZOOM;
    }
    if(key_Down) {
        camera.eye = camera.eye / ZOOM;
    }
}

void update_mesh() {
    if(parallelView) {
        Projection = glm::ortho(camera.ortho_left, camera.ortho_right, camera.ortho_bottom, camera.ortho_top, camera.near, camera.far);
    } else {
        Projection = glm::perspective(glm::radians(camera.perspective_angle), camera.perspective_ratio, camera.near, camera.far);
    }
    // Camera matrix
    View = glm::lookAt(camera.eye, camera.center, camera.up);
    // Model matrix : an identity matrix (model will be at the origin)
    Model = glm::mat4(1.0f);
    Model = glm::rotate(Model, angle, glm::vec3(0, -1, 0));
    // Projection matrix
    GLuint projectionID = glGetUniformLocation(program, "vProjection");
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(Projection));
    GLuint modelID = glGetUniformLocation(program, "vModel");
    glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(Model));
    GLuint viewID = glGetUniformLocation(program, "vView");
    glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(View));
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

void update_shader() {
    glEnable(GL_DEPTH_TEST);
    if(shadeMode == GOURAUD) {
        cout << "grouraud" << endl;
    }
    if(shadeMode == PHONG) {
        cout << "phong" << endl;
    }
    if(shadeMode == GOURAUD) {
        // Load shaders and use the resulting shader program
        program = initshader( "gouraud_vs.glsl", "gouraud_fs.glsl");
        glUseProgram(program);    
    } else if(shadeMode == PHONG) {
        // Load shaders and use the resulting shader program
        program = initshader( "phong_vs.glsl", "phong_fs.glsl");
        glUseProgram(program);
    } else {
        program = initshader( "gouraud_vs.glsl", "gouraud_fs.glsl");
        glUseProgram(program);
    }
    // Create a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    if(drawMode == FACE && shadeMode == FLAT) {
        vbos[0] = make_bo(GL_ARRAY_BUFFER, &flat_vertices[0], flat_vertices.size() * sizeof(GLfloat));
    } else if(drawMode == VERTEX || drawMode == FACE) {
        vbos[0] = make_bo(GL_ARRAY_BUFFER, &vertices[0], vertices.size() * sizeof(GLfloat));
        vbos[1] = make_bo(GL_ELEMENT_ARRAY_BUFFER, &indices[0], indices.size() * sizeof(GLuint));
    } else if(drawMode == EDGE) {
        vbos[0] = make_bo(GL_ARRAY_BUFFER, &line_vertices[0], line_vertices.size() * sizeof(GLfloat));       
    } else {
        vbos[0] = make_bo(GL_ARRAY_BUFFER, &vertices[0], vertices.size() * sizeof(GLfloat));
        vbos[1] = make_bo(GL_ELEMENT_ARRAY_BUFFER, &indices[0], indices.size() * sizeof(GLuint));    
    }
    // Initialize the vertex position attribute from the vertex shader
    pos = glGetAttribLocation(program, "vPosition");
    //must have vbo bound
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) 0); // input to vs.glsl vPos

    norm = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(norm);
    glVertexAttribPointer(norm, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); // input to vs.glsl vColor

    glClearColor( 0.0, 0.0, 0.0, 1.0); // black background
}

void update() {
    update_shader();
    if(animateState) {
        update_animate();       
    }
    update_modelViewProjection();
    update_mesh();
}

int row_col(int meshNum){
    if(pow(sqrt(meshNum),2)==meshNum){
        return sqrt(meshNum);
    } else {
        return floor(sqrt(meshNum))+1;
    }
}

glm::vec3 translatePos(int meshNum,int index, Glfloat width,Glfloat height){
    int num = row_col(meshNum);
    int row = k % num;
    int col = k / num;
    GLfloat gridWidth = width/((GLfloat)num*2);
    GLfloat gridHeight= height/((GLfloat)num*2);
    glm::vec3 translateVec;
    translateVec.x = 2*gridWidth*row - gridWidth;
    translateVec.y = 2*gridHeight*col- gridHeight;
    translateVec.z = 0;
}

/*
void draw_mesh(int k) {
    GLuint num = row_col(num_Meshes);
    GLfloat grid_size = WIDTH/(GLfloat)num;
    Model = glm::mat4(1.0f);
    Model = glm::translate(Model, glm::vec3(-meshes[k].center.x, -meshes[k].center.y, -meshes[k].center.z));
    Model = glm::scale(Model, glm::vec3(grid_size/xLen, grid_size/yLen, grid_size/zLen);
    Model = glm::translate(Model, translatePos(numMeshes, k, WIDTH, HEIGHT));
    GLuint modelID = glGetUniformLocation(program, "vModel");
    glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(Model));
	if(drawMode == VERTEX) {
        glDrawArrays(GL_POINTS, 0, meshes[k].numVertices);
    } else if(drawMode == EDGE) {
        glDrawArrays(GL_LINES, 0, meshes[k].vertices_for_line.size());
    } else {
        if(shadeMode == FLAT) {
            glDrawArrays(GL_TRIANGLES, 0, meshes[k].vertices_for_flat.size());
        } else if(shadeMode == GOURAUD || shadeMode == PHONG) {
            glDrawElements(GL_TRIANGLES, meshes[k].faces.triangulated_indices_list.size(), GL_UNSIGNED_INT, 0);    
        }
    }
}

void draw() {
	for(int k = 0; k < numMeshes; k++) {
		draw_mesh(k);
	}
}
*/

void draw() {
    /*
    if(drawMode == VERTEX) {
        glDrawArrays(GL_POINTS, 0, meshes[0].numVertices);
    } else if(drawMode == EDGE) {
        glDrawArrays(GL_LINES, 0, meshes[0].vertices_for_line.size());
    } else {
        if(shadeMode == FLAT) {
            glDrawArrays(GL_TRIANGLES, 0, meshes[0].vertices_for_flat.size());
        } else if(shadeMode == GOURAUD || shadeMode == PHONG) {
            glDrawElements(GL_TRIANGLES, meshes[0].faces.triangulated_indices_list.size(), GL_UNSIGNED_INT, 0);    
        }
    }
    */
    glDrawElements(GL_TRIANGLES, meshes[1].faces.triangulated_indices_list.size(), GL_UNSIGNED_INT, (void*)(meshes[0].faces.triangulated_indices_list.size() * sizeof(GLuint)));   
}

int main(int argc, char* argv[]) {
    multipleFileHandle(argc, argv);
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
        glfwSwapBuffers(window);
        // update other events like input handling
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
