#version 430
precision mediump float;
in vec3 normalInterp;
in vec3 vertPos;
in vec3 newLightPos;

out vec4 frag_color;

uniform float Ka;
uniform float Kd;
uniform float Ks;
uniform float shinessVal;


void main() {
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
    frag_color = vec4(Ka * vec3(0, 0.2, 0.2) + Kd * lamber * vec3(0, 0.5, 0.5) + Ks * spec * vec3(0, 0.8, 0.8), 1.0);
}
