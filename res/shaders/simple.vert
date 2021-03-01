#version 330 core

#define UP 0
#define DOWN 1
#define NORTH 2
#define SOUTH 3
#define EAST 4
#define WEST 5

layout(location = 0) in vec3 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
  gl_Position = projection * view * model * vec4(vertexPosition_modelspace,1);
}

