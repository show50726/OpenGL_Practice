#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 Projection;
uniform mat4 ModelView;
uniform vec3 lightPos;

out vec3 normalInterp;
out vec3 vertPos;
out vec3 newLightPos;

void main() {
 	mat4 normalMat = transpose(inverse(ModelView));
	vec3 normal = normalize(position);
	vec4 vertPos4 = ModelView * vec4(position, 1.0);
	vertPos = vec3(vertPos4) / vertPos4.w;
	normalInterp = vec3(normalMat * vec4(normal, 0.0));
	vec4 _newLightPos = ModelView * vec4(lightPos, 1.0);
	newLightPos = vec3(_newLightPos) / _newLightPos.w;
	gl_Position = Projection * vertPos4;
}
