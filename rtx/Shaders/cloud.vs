#version 460 core

in vec3 position;
in vec2 texcoord;

uniform float layer;

out vec3 TexCoord;


void main(){
    
    
    gl_Position =  vec4( position, 1.0);

    TexCoord = vec3( texcoord.x, layer, texcoord.y);

}