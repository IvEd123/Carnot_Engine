#version 460 core

in vec3 TexCoord;

layout(location = 0)  out vec4 outColor;

uniform vec3 point_pos [10];


void main(){      
    float min_dist = 10000f;
    for (int i = 0; i < 10; i++){
        float dist = length(point_pos[i] - TexCoord);
        if (dist < min_dist){
            min_dist = dist;
        }
    }
    
    outColor = vec4(min_dist, min_dist, min_dist, 1);
}