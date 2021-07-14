#version 460 core

in vec3 position;

out vec2 TexCoord;
out vec3 FragPos;

void main(){
    
    
    gl_Position =  vec4( position*2, 1.0);
    FragPos =  position;

    TexCoord = vec2(position.x, position.y);

}