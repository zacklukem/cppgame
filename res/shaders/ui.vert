#version 330 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 i_uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 uv;

void main() {
  gl_Position = projection * view * model * vec4(pos, 0, 1);
  uv = i_uv;

  // Output position of the vertex, in clip space
  // map [0..800][0..600] to [-1..1][-1..1]
  // vec2 vertexPosition_homoneneousspace = pos.xy - vec2(400,300);
  // vertexPosition_homoneneousspace /= vec2(400,300);
  // gl_Position =  vec4(vertexPosition_homoneneousspace,0,1);
}
