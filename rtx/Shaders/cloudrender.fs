#version 460 core

in vec2 TexCoord;

uniform sampler2D tex;
out vec4 outColor;

void main(){      
    //outColor = vec4(0.50);   
    outColor = texture(tex, TexCoord);
    //outColor = texture(tex, vec2(0.5));
 }