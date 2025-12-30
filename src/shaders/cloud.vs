#version 460 core

in vec3 position;

uniform float layer;

out vec3 TexCoord;
out vec3 FragPos;

void main(){
    
    
    gl_Position =  vec4( position*2, 1.0);
    FragPos =  position;

    TexCoord = vec3(position.x+0.5, position.y+0.5, layer);

}