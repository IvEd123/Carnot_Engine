#version 460 core

in vec2 TexCoord;
in vec3 FragPos;

layout(location = 0)  out vec4 outColor;

uniform vec3 point_pos [20];
uniform int layers;


void main(){      
   
    vec3 pos ;
    pos.x = mod( gl_FragCoord.x, 100);
    pos.y = gl_FragCoord.y;
    pos.z = floor(gl_FragCoord.x / 100);

    pos /= 100;

    float min_dist = 10000;
    for (int i = 0; i < 10; i++){
        float dist = length(point_pos[i] - pos);
        if (dist < min_dist){
            min_dist = dist;
        }
    }
    min_dist = 1 - min_dist;
    outColor = vec4(min_dist, min_dist, min_dist, 1);
    //outColor = vec4 (1 );
}