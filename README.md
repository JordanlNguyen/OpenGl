# OpenGl
studying and practicing OpenGL and low level developemnt

## compiling 
- to compile code on Mac
` clang++ src/main.cpp src/glad.c -std=c++17 -Iinclude -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw -framework OpenGL -o app`

## shaders
### shader concept
shaders are instructions givento openGL for execution, shaders are required
### fragment and vertex shaders
- fragment shaders : decide what color pixels are 
- vertex shaders : decide where vertices go (screen positions)
### GPU pipline
1. C++ : create the data
2. Vertex Shaders : screen position
3. fragement shaders : pixel color
4. GPU : drawing on screen
