#version 460 core

in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 size;
uniform vec3 pos;

out vec3 TexCoord;
out vec3 PosOnCube;

out CUBE_PARAM{
    vec3 min;
    vec3 max;
}cube_param_out;

out VS_OUT {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 FragPos;
} vs_out;

vec3 mult (vec3 v1, vec3 v2){
    return vec3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

vec3 div (vec3 v1, vec3 v2){
    return vec3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);
}

void main(){
    gl_Position = proj * view * model * vec4( mult(position, size), 1.0);

    

    PosOnCube =position;
    //TexCoord = vec3((position.x)*0.01+0.5, position.y, position.z+0.5);
    TexCoord = vec3((position.x+0.5)*0.01, position.y+0.5, position.z +0.5 );
    //TexCoord = div(TexCoord,size);

    vs_out.model = model;
    vs_out.view = view;
    vs_out.proj = proj;
    vs_out.FragPos = vec3( model *  vec4(mult(position, size), 1.0));

    cube_param_out.min = vec3( model *  vec4(mult(vec3(-0.5), size), 1.0));
    cube_param_out.max = vec3( model *  vec4(mult(vec3(0.5), size), 1.0));
}