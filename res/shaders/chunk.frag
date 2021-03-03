#version 330 core

in float alpha;
in float vColor;
in vec2 uv;
out vec4 color;

uniform sampler2D u_texture;

void main() {
  // color = vColor * vec3(uv, 0.0f);
  color = texture(u_texture, uv);
  color.rgb = mix(vec3(0.9f, 0.9f, 0.9f), color.rgb * vColor, alpha);
}

