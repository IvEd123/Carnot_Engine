#version 460 core

in vec3 position;
in vec3 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 TexCoord;
out vec3 PosOnCube;

out VS_OUT {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 FragPos;
} vs_out;

void main(){
    
    
    //gl_Position =  vec4( position, 1.0);
    gl_Position = proj * view * model * vec4( position, 1.0);

    

    PosOnCube =position;
    TexCoord = vec3(position.x+0.5, position.y+0.5, position.z+0.5);

    vs_out.model = model;
    vs_out.view = view;
    vs_out.proj = proj;
    vs_out.FragPos = vec3( model *  vec4(position, 1.0));
}