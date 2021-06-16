#version 460 core

in vec2 position;
in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform sampler2D heightmap;

uniform float size;
uniform float height;
uniform float res;

uniform vec3 eye;

out vec2 Texcoord;
out vec3 Normal;

out vec3 T;
out vec3 B;

out vec3 Vertex_pos;

float h(vec2 pos){
    return texture(heightmap, pos).x;
}

void main(){
    vec3 v;

    float texScale = .1;
    vec2 P = position;
    Texcoord = texcoord * texScale;

    /*vec3 off = vec3(1.0, 1.0, 0.0);
    float hL = h(vec2(P.x/size, P.y/size) - off.xz/size);
    float hR = h(vec2(P.x/size, P.y/size) + off.xz/size);
    float hD = h(vec2(P.x/size, P.y/size) - off.zy/size);
    float hU = h(vec2(P.x/size, P.y/size) + off.zy/size);

    Normal.x = hL - hR;
    Normal.y = 2.0/size;
    Normal.z = hD - hU;*/

    

    float h1, h2, h3;
    h1 = h(vec2(P.x/size, P.y/size))*height;
    h2 = h(vec2((P.x+1)/size, P.y/size))*height;
    h3 = h(vec2(P.x/size, (P.y+1)/size))*height;

    vec3 v1, v2;
    v1 = vec3((P.x+1)/size, h2, P.y/size) -  vec3(P.x/size, h1, P.y/size);
    v2 = vec3((P.x)/size, h3, (P.y+1)/size) -  vec3(P.x/size, h1, P.y/size);

    
    v = normalize(eye - (view * model * vec4(P.x,  h(vec2(P.x/size, P.y/size)), P.y, 1.0)).xyz );

    B = normalize(v2);
    T = normalize(v1);
    Normal = normalize(cross(v2, v1));
    mat3 TBN = mat3(T, B, Normal);


    gl_Position = proj * view * model * vec4( P.x, h(vec2(P.x/size, P.y/size)) * height, P.y, 1.0);
    Vertex_pos = (model * vec4( P.x, h(vec2(P.x/size, P.y/size)) * height, P.y, 1.0)).xyz;
}