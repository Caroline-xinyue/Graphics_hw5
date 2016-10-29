
#include "file.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])

{
    int vertex_num=0;
    int face_num=0;
        
    string name = argv[1];
    cout <<name<< endl;
    
    std::ifstream file(name);
    std::string str;
    std::getline(file, str);
    std::getline(file, str);
   stringstream fs(str);
    Mesh newMesh;
    
    fs>>vertex_num>>face_num;
    cout<<"face"<<face_num<<endl;
    cout<<"vertex"<<vertex_num<<endl;
    newMesh.numVertices =vertex_num;
    newMesh.numFaces =face_num;
     cout<<"vertex number"<<endl;
    for(int i = 0; i < vertex_num; i++){
        std::getline(file, str);
    cout<<str<<endl;
    }
    cout<<"face number"<<endl;
    for(int i = 0; i <face_num; i++){
        std::getline(file, str);
        cout<<str<<endl;
    }

    file.close();
    return 0;
}
