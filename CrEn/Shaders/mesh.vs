#version 460 core

in vec3 position;
in vec2 texcoord;
in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightSpaceMatrix;
uniform vec3 size;



out VS_OUT {
    vec3 FragPos;
    vec4 FragPosLightSpace;
    mat4 depthMVP;
} vs_out;

out vec2 Texcoord;
out vec3 Normal;
out vec3 ViewDir;

void main(){
    vs_out.FragPos = vec3(model  * vec4(position * 0.5 * size, 1.0));
    vs_out.FragPosLightSpace = lightSpaceMatrix *  vec4(vs_out.FragPos, 1.0);
    vs_out.depthMVP = lightSpaceMatrix;


    Texcoord = texcoord;

    Normal = normalize((model * vec4(normal, 0)).xyz);
    //Normal = normal;
    
    gl_Position = proj * view * model * vec4( position * 0.5 * size, 1.0);


    ViewDir = normalize(position);
}