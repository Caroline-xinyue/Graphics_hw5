#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include "file.h"
#include "glm/ext.hpp"
#include <iostream>
#include <ctime>
#include <map>
#include <fstream>
#include <string>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

struct compare_glm_vec3 {
    bool operator()(const glm::vec3& x, const glm::vec3& y) const { return x.x < y.x && x.y < y.y && x.z < y.z; }
};

int main(int argc, char* argv[])

{
    int vertex_num=0;
    int face_num=0;
    if(argc<2){
        cout<<"File not found, please input file"<<endl;
         exit(0);
    }
    string name = argv[1];
    //cout <<name<< endl;
    
    std::ifstream file(name);
    std::string str;
    std::getline(file, str);
    if(str != "OFF"){
        cout<<"File is not in right format. Please use an OFF file."<<endl;
    }
    std::getline(file, str);
    stringstream fs(str);
    Mesh newMesh;
    Faces newFace;
    fs>>vertex_num>>face_num;
   /* cout<<"face"<<face_num<<endl;
    cout<<"vertex"<<vertex_num<<endl;*/
    newMesh.numVertices =vertex_num;
    newMesh.numFaces =face_num;
   /* cout<<"vertex number1"<<newMesh.numVertices<<endl;
    cout<<"vertex number2"<<newMesh.numFaces<<endl;
    cout<<"vertex number"<<endl;*/
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
        /*
        glm::vec3 temp = glm::vec3(vertexx,vertexy,vertexz);
        newMesh.vertices.push_back(temp);
        newMesh.vertices.push_back(glm::vec3());*/
        //cout<<newMesh.vertices.size()<<endl;
        
    }
    //cout<<"face number"<<endl;
   /* for(int i =0; i<newMesh.vertices.size();i++){
        cout<<"index"<<i<<endl;
        cout<<glm::to_string(newMesh.vertices[i].position)<<endl;
         cout<<glm::to_string(newMesh.vertices[i].normal)<<endl;
    }*/
    for(int i = 0; i <face_num; i++){
        int num_vertices_per;
        int index1;
        int index2;
        int index3;
        int index4 = 0;
        std::getline(file, str);
        stringstream vs(str);
        //cout<<str<<endl;
        vs>>num_vertices_per>>index1>>index2>>index3>>index4;
        if(num_vertices_per < 3 || num_vertices_per > 4) {
          
            cout << "bad input line" << endl;}
        
       // cout<<index1<<"  "<<index2<<"  "<<index3<<"  "<<index4<<"  ";


        glm::vec3 crossVe1 = glm::vec3(newMesh.vertices[index1].position.x,newMesh.vertices[index1].position.y,newMesh.vertices[index1].position.z);
        glm::vec3 crossVe2 = glm::vec3(newMesh.vertices[index2].position.x,newMesh.vertices[index2].position.y,newMesh.vertices[index2].position.z);
        glm::vec3 crossVe3 = glm::vec3(newMesh.vertices[index3].position.x,newMesh.vertices[index3].position.y,newMesh.vertices[index3].position.z);
        glm::vec3 corss = glm::cross(crossVe2-crossVe1,crossVe3-crossVe2);
        /* cout<<"index1_"<< glm::to_string(crossVe1)<<endl;
        cout<<"index1_"<< glm::to_string(crossVe2)<<endl;
        cout<<"index1_"<< glm::to_string(crossVe3)<<endl;*/
        corss = glm::normalize(corss);
        mapping[index1].push_back(corss);
         mapping[index2].push_back(corss);
         mapping[index3].push_back(corss);
        mapping_sum[index1]=mapping_sum[index1]+glm::vec4(corss,1);
        mapping_sum[index2]=mapping_sum[index2]+glm::vec4(corss,1);
        mapping_sum[index3]=mapping_sum[index3]+glm::vec4(corss,1);
       /* cout<<"index1_"<< glm::to_string(corss)<<endl;
        cout<<"index2_"<< mapping[index2].size()<<endl;

        cout<<"index3_"<< mapping[index3].size()<<endl;*/


        newFace.normal_list.push_back(corss);
        newFace.num_vertices_list.push_back(num_vertices_per);
        newFace.triangulated_indices_list.push_back(index1);
        newFace.triangulated_indices_list.push_back(index2);
        newFace.triangulated_indices_list.push_back(index3);
        if(num_vertices_per==4){
            mapping[index4].push_back(corss);
            mapping_sum[index4]=mapping_sum[index4]+glm::vec4(corss,1);
            cout<<"index4_"<< mapping[index4].size()<<endl;
            newFace.triangulated_indices_list.push_back(index4);
            newFace.triangulated_indices_list.push_back(index1);
            newFace.triangulated_indices_list.push_back(index3);
            
        }
 
       // cout<<num_vertices_per<<endl;
    }
   /* for(int i =0; i<newFace.triangulated_indices_list.size();i++){
        std::cout<<newFace.triangulated_indices_list[i]<<std::endl;
    }*/
    
    
    
    
    
 
   for(int i = 0; i < vertex_num;i++){
      
       glm::vec3 meannorm = glm::vec3(mapping_sum[i].x/mapping_sum[i].w,mapping_sum[i].y/mapping_sum[i].w,mapping_sum[i].z/mapping_sum[i].w);
       newMesh.vertices[i].normal =  meannorm;
       // cout<<"Average"<< glm::to_string(meannorm)<<endl;
    }
  /*  for(int i =0; i<newMesh.vertices.size();i++){
        cout<<"index"<<i<<endl;
        cout<<glm::to_string(newMesh.vertices[i].position)<<endl;
        cout<<glm::to_string(newMesh.vertices[i].normal)<<endl;
    }*/
    
    
   /* map<glm::vec3, vector<glm::vec3>, compare_glm_vec3> m;
    vector<glm::vec3> test;
     vector<glm::vec3> test2;
    //m.insert(pair<glm::vec3, vector<glm::vec3 >>(a, test));
    m[a]=test;
    m[b] = test2;
    m[c] = vector<glm::vec3>();
    m[a].push_back(a);
    //m[a].push_back(a);
    m[b].push_back(a);
    m[c].push_back(b);
    //m[a].push_back(glm::vec3(1,2,3));
    cout<<"testsize"<<m[a].size()<<endl;
    cout<<"testsize_"<<m[b].size()<<endl;*/
    file.close();
    return 0;
}
