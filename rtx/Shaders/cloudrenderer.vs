#version 460 core

in vec3 position;
in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform float size;

out vec2 TexCoord;


void main(){
    
    
    //gl_Position =  vec4( position, 1.0);
    gl_Position = proj * view * model * vec4( size * position, 1.0);

    TexCoord = texcoord;

}