#version 330 core

in float vColor;
in vec2 uv;
out vec3 color;

uniform sampler2D u_texture;

void main() {
  // color = vColor * vec3(uv, 0.0f);
  color = texture(u_texture, uv).rgb;
  color *= vColor;
}

