# OpenGl
studying and practicing OpenGL and low level developemnt

## compiling 
- to compile code on Mac
`clang++ src/main.cpp src/glad.c circle.cpp -std=c++17 -Iinclude -I/opt/homebrew/include -L/opt/homebrew/lib -lglfw -framework OpenGL -o app`

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

## Physics (Gravity)
### position 
x= x0 +v0t + (1/2)at^2
### velocity
v = v0 + at
### acceleration
a = (v - v0) / t
### drawing motion
- we will use the positions of the physics equations to draw the position of the circle on screen
- displacement : will be used to calculate the velocity and position at a given time
- acceleration : we will use the earths gravity constant 9.8
### gravittional pull of 2 objects
F = G((m1 * m2) / r^2)
### units
- 1 unit = 0.001f $..$ 1 m
- velolicties = 1 unit / 1 second
- acceleration = 1 unit / second^2
- mass = arbitrary 
- G = tuned constant