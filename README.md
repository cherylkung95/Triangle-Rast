# Triangle-Rast

## How to use
Enter in 3 vertices of the triangle (x,y). Vertices must be between [0 - 360] for x and [0 - 480] for y.

## How it works
The program fills in a 2D buffer, which is then fed into OpenGL's glDrawPixel function and generates the triangle.

## Libraries
* OpenGL
* glut
