#version 460 core

    in vec2 position_screen;
    in vec2 texcoord;

    out vec2 Texcoord;

    void main(){
        Texcoord = texcoord;


        gl_Position = vec4(position_screen, 0.0, 1.0);
    }