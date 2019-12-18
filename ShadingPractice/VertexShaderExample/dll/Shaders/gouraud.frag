#version 430

in vec4 color;
in vec3 normalInterp;
in vec3 vertPos;
in vec3 newLightPos;

uniform int mode;
uniform float threshold;

void main() {
	vec3 N = normalize(normalInterp);
	vec3 V = normalize(-vertPos);
	if(dot(N, V) < threshold && mode == 1){
		gl_FragColor = vec4(1 ,1 , 1 , 1);
	}
 	else {
 		gl_FragColor = color;
 	}
}
