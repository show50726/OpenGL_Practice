#version 430
in vec2 Texcoord;
out vec4 frag_color;

uniform sampler2D Texture;

void main() {
    frag_color = texture(Texture, Texcoord);
}
