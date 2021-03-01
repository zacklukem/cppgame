#version 330 core

#define UP 0
#define DOWN 1
#define NORTH 2
#define SOUTH 3
#define EAST 4
#define WEST 5

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 uvCoord;
layout(location = 2) in uint color;

// Values that stay constant for the whole mesh.
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 u_camera;

out float vColor;
out vec2 uv;
out float alpha;

#define FALLOFF 0.05f
#define DISTANCE 50.0f

void main() {
  gl_Position = projection * view * model * vec4(vertexPosition_modelspace,1);

  alpha = clamp(-1.0f * FALLOFF * (distance(u_camera, gl_Position.xyz) - DISTANCE), 0.0f, 1.0f);

  uv = uvCoord;

  switch (color) {
    case DOWN:
      vColor = 0.5f;
      break;
    case NORTH:
      vColor = 0.75f;
      break;
    case EAST:
      vColor = 0.8f;
      break;
    case SOUTH:
      vColor = 0.85f;
      break;
    case WEST:
      vColor = 0.95f;
      break;
    default:
      vColor = 1.0f;
  }
}

