#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 Projection;
uniform mat4 ModelView;

out vec2 Texcoord;

void main() {
	gl_Position = Projection * ModelView * vec4(position, 1.0);
	Texcoord = texcoord;
}
