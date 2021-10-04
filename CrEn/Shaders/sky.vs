#version 460 core

in vec3 position;
in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 size;

out vec2 Texcoord;

void main(){
    Texcoord = texcoord;
    gl_Position = proj * view * model * vec4(position * size, 1.0);
}