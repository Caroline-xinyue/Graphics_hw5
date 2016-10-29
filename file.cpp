
#include <stdafx.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;
int main(int argc,  char *argv)
{
    for (int i = 1; i < argc; i++) {
        
        String name = *argv[i]; //e.g. Goku
        String type = *argv[i]; //e.g. Saiyan, Human, etc
        String match = * argv[i]; //Goku Piccolo
        //I don't think any of the statements above would be correct.
        //I'm just searching for how to use string values of txt files in such a way
        
        cout << i << " " << endl; //I'd like to show names, types or matchs inside the double quotation mark.
    }
}
