#version 460 core

in vec3 position;
in vec2 texcoord;
in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightSpaceMatrix;
uniform vec3 size;
uniform float texture_repeat;

out VS_OUT {
    vec3 FragPos;
    vec4 FragPosLightSpace;
    mat4 depthMVP;
} vs_out;

out vec2 Texcoord;
out vec3 Normal;
out vec3 Vertex_pos;


void main(){
    vs_out.FragPos = vec3(model  * vec4(position * size, 1.0));
    vs_out.FragPosLightSpace = lightSpaceMatrix *  vec4(vs_out.FragPos, 1.0);
    vs_out.depthMVP = lightSpaceMatrix;

    Normal = normal;
    Texcoord = texcoord * texture_repeat;
    gl_Position = proj * view *  vec4(vs_out.FragPos, 1);

    Vertex_pos = vs_out.FragPos.xyz;
}