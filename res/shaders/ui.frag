#version 330 core

uniform sampler2D u_texture;
in vec2 uv;
out vec4 color;

void main() {
  color = texture(u_texture, uv);
  //color = vec4(1.0f, 1.0f, 1.0f, 0.5f);
}
