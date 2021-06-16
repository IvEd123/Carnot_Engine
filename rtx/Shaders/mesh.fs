#version 460 core

in vec2 Texcoord;
in vec3 Normal;

in VS_OUT {
    vec3 FragPos;
    vec4 FragPosLightSpace;
    mat4 depthMVP;
} fs_in;

uniform sampler2D shadowMap;
uniform sampler2D tex;
uniform sampler2D norm;
uniform vec3 light;
uniform mat4 model;

out vec4 outColor;

float ShadowCalculation(vec4 fragPosLightSpace){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.001);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }

    shadow /= 9.0;
    
    if(projCoords.z > 1.0 || projCoords.x > 1.0  || projCoords.y > 1.0 || projCoords.x < 0.0  || projCoords.y < 0.0)
        shadow = 0.0;
        
    return shadow;
}


void main(){
    	
    float shadow = max( 1- ShadowCalculation(fs_in.FragPosLightSpace), 0.4);


    outColor.rgb = mix(texture(tex, Texcoord).rgb * shadow * max( dot(Normal, vec3(transpose(model) * vec4(normalize(light), 1.0) )), 0.05), texture(tex, Texcoord).rgb, 0.2) + vec3(0.1, 0.1, 0.11);
    outColor.a = 1.0;
}