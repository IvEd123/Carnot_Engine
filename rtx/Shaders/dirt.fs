#version 460 core

in vec2 Texcoord;
in vec3 Normal;

uniform vec3 light;
uniform sampler2D tex;
uniform sampler2D shadowMap;

in VS_OUT {
    vec3 FragPos;
    vec4 FragPosLightSpace;
    mat4 depthMVP;
} fs_in;

out vec4 outColor;

float ShadowCalculation(vec4 fragPosLightSpace){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.001);
    float shadow = 1.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 0.0 : 1.0;        
        }    
    }
    shadow /= 9.0;
    
    if(projCoords.z > 1.0 || projCoords.x > 1.0  || projCoords.y > 1.0 || projCoords.x < 0.0  || projCoords.y < 0.0)
        shadow = 1.0;
        
    return shadow;
}


void main(){

    vec3 color = texture(tex, Texcoord).rgb;

    vec3 ambient = 0.2 * color;

    float shadow =  ShadowCalculation(fs_in.FragPosLightSpace);
    
    outColor = vec4(color * max(shadow, 0.2) + ambient, 1);
    //vec3 projCoords = fs_in.FragPosLightSpace.xyz / fs_in.FragPosLightSpace.w;
    //projCoords = projCoords * 0.5 + 0.5;
    //outColor = fs_in.depthMVP*vec4( fs_in.FragPos, 1);//texture(shadowMap, projCoords.xy)*2;//
}
