#version 430

uniform int mode;
uniform float threshold;

in vec3 normalInterp;
in vec3 vertPos;
in vec3 newLightPos;

void main() {
	vec4 color;
	float intensity = dot(normalize(newLightPos-vertPos), normalInterp);
	vec3 N = normalize(normalInterp);
	vec3 V = normalize(-vertPos);
	if(dot(N, V) < threshold && mode == 1) color = vec4(1 ,1 , 1 , 1);
 	else if (intensity > 0.9)      color = vec4(0.0, 1.0, 1.0, 1.0);
    else if (intensity > 0.75) color = vec4(0.0, 0.8, 0.8, 1.0);
    else if (intensity > 0.50) color = vec4(0.0, 0.6, 0.6, 1.0);
    else if (intensity > 0.25) color = vec4(0.0, 0.4, 0.4, 1.0);
    else                       color = vec4(0.0, 0.2, 0.2, 1.0);
    gl_FragColor = color;
}
