#version 460

layout (location = 0) in vec3 in_position;

uniform mat4 mvpMatrix;
uniform mat4 model;

uniform float size;

void main(void){

	gl_Position = mvpMatrix * model * vec4(in_position * size/2, 1.0);

}