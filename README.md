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

# Things in 3D
## Physical Aspects
2 dimensional equations for calculating force, gravity, and trajectory are very similar calculating them in 3 dimensions (just with an added z axis)
## model, views, and projections
with 3d objects comes with aspects of how 3d objects are viewed on a 2d surface. Objects in the mathmatical 3rd dimension have 3 coordinate points. But when images are rendered on screen (which is 2d) there will only be 2 coordinate points that will be shown on screen. In this section I will discuss the 3 components needed to calculate, render, and view an object in the 3rd dimension
### Model
A model is to be thought of as an object within the 3d system, this object contains its own vertices, indices, edges, etc. Models are represented within OpenGL as a 4x4 matrix. Although our objects will have vertices of a 3d system(x,y,z), we use a 4x4 matrix to help calculate changes that occur for each vertex. These cahnges can include translations, rotations, scalings, etc.

                                        | 1  0  0  tx | +  (x)
                                        | 0  1  0  ty | +  (y)
                                        | 0  0  1  tz | +  (z)
                                        | 0  0  0  1  |

using a matrix like in the example above gives an output of the translations of the 3d model given the change in x y or z.
To summarize the reasons why use a matrix instead of just representing coordinates as a tuple of 3, matricies allow or easier calculations of the tranlsation of the object.
# views