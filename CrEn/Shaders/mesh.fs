#version 460 core

in vec2 Texcoord;
in vec3 Normal;

in VS_OUT {
    vec3 FragPos;
    vec4 FragPosLightSpace;
    mat4 depthMVP;
} fs_in;

uniform samplerCube skybox;
in vec3 ViewDir;

uniform sampler2D shadowMap;
uniform sampler2D tex;
uniform vec3 light;
uniform mat4 model;
uniform float lightDistance;

layout (location = 0) out vec4 outAlbedo;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outPos;
layout (location = 3) out vec4 outColor;

vec3 lightPos = - light * lightDistance;

float ShadowCalculation(vec4 fragPosLightSpace){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-light);
    float bias =   0.001;
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
    vec3 _normal = Normal;
    _normal.y = max(Normal.y, 0);
    
    float shadow = 1- ShadowCalculation(  fs_in.depthMVP *  vec4(fs_in.FragPos, 1.0) );
    vec3 texColor = texture(tex, Texcoord).rgb;
    vec3 ambient = texture(skybox, _normal, 3).rgb;
    float diffuse = dot(Normal, normalize(light))*0.5 + 0.5;

    //outColor.rgb = mix(texture(tex, Texcoord).rgb * shadow * max( dot(Normal, vec3(transpose(model) * vec4(normalize(lightPos), 1.0) )), 0.05), texture(tex, Texcoord).rgb, 0.2) + vec3(0.1, 0.1, 0.11);
    outColor.rgb =  vec3(shadow);

    //outColor.rgb =  vec3(gl_FragCoord.z);


    outColor.a = 1.0;
    outPos = vec4(fs_in.FragPos, 1);
    outNormal = vec4(Normal, 1);
    outAlbedo = vec4((ambient + shadow * diffuse) * texColor, 1);//vec4(texColor, 1);
}