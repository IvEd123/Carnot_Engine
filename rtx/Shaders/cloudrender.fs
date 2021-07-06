#version 460 core

in vec3 TexCoord;

in VS_OUT {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 FragPos;
} fs_in;


uniform sampler3D tex;
uniform vec3 eyepos;

out vec4 outColor;

const int samples = 10;

vec3 toLocal(vec3 v){
    return (transpose(fs_in.model) * vec4( v, 1.0)).xyz;
}

vec3 posToUVW(vec3 v){
    return vec3(v.x + 0.5f, v.y + 0.5f, v.z + 0.5f);
}

void main(){      
    //outColor = vec4(0.50);   
    //outColor = texture(tex, TexCoord);
    //outColor = texture(tex, vec2(0.5));
    vec3 ray = normalize(fs_in.FragPos - eyepos)/samples;
    
    float s = 0;
    
    for (float i = 0; i< samples; i++){
        vec3 ray2 = vec3(i);
        s += texture(tex,   TexCoord +ray *i ).r/samples;
    }

   outColor =  vec4(0, 1, 0, 1-s);
   //outColor = vec4( normalize(fs_in.FragPos), 1);
 }