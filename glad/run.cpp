#include <cstdlib>
#include <iostream>
using namespace std;

int main(){
    /* ------ user inputs ------*/
    float mass1;
    float mass2;
    float r1;
    float r2;
    float x1;
    float x2;
    float y1;
    float y2;
    float z1;
    float z2;
    float vx1;
    float vx2;
    float vy1;
    float vy2;
    float vz1;
    float vz2;
    int inputVariables;
    
    cout << "do you want to input starting variables? \n yes(0) | no(1)" << endl;
    cin >> inputVariables;
    if (inputVariables == 0){
        cout << "mass 1 | ";
        cin >> mass1;
        cout << endl;

        cout << "mass 2 | ";
        cin >> mass2;
        cout << endl;
        
        cout << "radius 1 | ";
        cin >> r1;
        cout << endl;

        cout << "radius 2 | ";
        cin >> r2;
        cout << endl;

        cout << "position x 1 | ";
        cin >> x1;
        cout << endl;

        cout << "position x 2 | ";
        cin >> x2;
        cout << endl;

        cout << "position y 1 | ";
        cin >> y1;
        cout << endl;

        cout << "position y 2 | ";
        cin >> y2;
        cout << endl;

        cout << "position z1 | ";
        cin >> z1;
        cout << endl;

        cout << "position z2 | ";
        cin >> z2;
        cout << endl;

        cout << "velocity x 1 | ";
        cin >> vx1;
        cout << endl;

        cout << "velocity x 2 | ";
        cin >> vx2;
        cout << endl;

        cout << "velocity y 1 | ";
        cin >> vy1;
        cout << endl;

        cout << "velocity y2 | ";
        cin >> vy2;
        cout << endl;

        cout << "velocity z 1 | ";
        cin >> vz1;
        cout << endl;

        cout << "velocity z 2 | ";
        cin >> vz2;
        cout << endl;
    }
    else {
        x1 = 5.0f; y1 = 0.0f; z1 = 4.0f; vx1 = 0; vy1 = 0; vz1 = 5.0f; r1 = 1.0f; mass1 = 500.0f;
        x2 = -5.0f; y2 = 0.0f; z2 = -4.0f; vx2 = 0; vy2 = 0; vz2 = -5.0f; r2 = 1.0f; mass2 = 500.0f;
    }
    system("clang++ src/3D.cpp src/glad.c src/objects/Sphere.cpp src/objects/Grid.cpp -std=c++17 -Iinclude -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw -framework OpenGL -o app");
    string command = "./app ";
    command += to_string(mass1) + " " + to_string(mass2) + " ";
    command += to_string(r1) + " " + to_string(r2) + " ";
    command += to_string(x1) + " " + to_string(x2) + " ";
    command += to_string(y1) + " " + to_string(y2) + " ";
    command += to_string(z1) + " " + to_string(z2) + " ";
    command += to_string(vx1) + " " + to_string(vx2) + " ";
    command += to_string(vy1) + " " + to_string(vy2) + " ";
    command += to_string(vz1) + " " + to_string(vz2);
    system(command.c_str());
    return 0;
}   