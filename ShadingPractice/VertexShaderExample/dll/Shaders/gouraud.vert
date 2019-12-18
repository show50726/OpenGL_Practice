#version 430
precision mediump float;
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

out vec4 color;

uniform mat4 Projection;
uniform mat4 ModelView;
uniform vec3 lightPos;

uniform float Ka;
uniform float Kd;
uniform float Ks;
uniform float shinessVal;

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
    
    vec3 N = normalize(normalInterp);
    vec3 L = normalize(newLightPos-vertPos);

    float lamber = max(dot(N, L), 0.0);
    float spec = 0.0;
    //if(lamber > 0.0){
    	vec3 R = reflect(-L, N);
    	vec3 V = normalize(-vertPos);
    	vec3 H = normalize(L+V);

    	float specAngle = max(dot(N, H), 0.0);
    	spec = pow(specAngle, shinessVal);
    //}

    color = vec4(Ka * vec3(0.0, 0.2, 0.2) + Kd * lamber * vec3(0.0, 0.5, 0.5) + Ks * spec * vec3(0.0, 0.8, 0.8), 1.0);
}
