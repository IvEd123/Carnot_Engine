#version 460 core

in vec2 Texcoord;

uniform sampler2D tex;

out vec4 outColor;

void main(){
    outColor = vec4(0.0, 0.0, 0.2, 1.0) + texture(tex, Texcoord);
}