#version 330 core

in float alpha;
in float vColor;
in vec2 uv;
out vec4 color;

uniform sampler2D u_texture;

void main() {
  // color = vColor * vec3(uv, 0.0f);
  color = vec4(texture(u_texture, uv).rgb * vColor, 1.0f);
  color.rgb = mix(vec3(0.9f, 0.9f, 0.9f), color.rgb, alpha);
}

