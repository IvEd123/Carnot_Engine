#version 460 core

    in vec2 Texcoord;

    out vec4 outColor;

    uniform sampler2D texFramebuffer;
    uniform sampler2D depthTex;

    uniform sampler2D albedo;
    uniform sampler2D normal;
    uniform sampler2D position;
    uniform sampler2D shadowMap;

    uniform mat4 lightSpaceMatrix;

    uniform float time;

    float blursize = 0.01;


    float getLen(vec4 v){
        return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    }

    float ShadowCalculation(){
        vec4 fragPosLightSpace = lightSpaceMatrix * texture(position, Texcoord);


        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        projCoords = projCoords * 0.5 + 0.5;
        float closestDepth = texture(shadowMap, projCoords.xy).r; 
        float currentDepth = projCoords.z;
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
        vec4 color;
        color.rgb = texture(albedo, Texcoord).rgb;
        //color.rgb = vec3(1 - ShadowCalculation());
        color.a = 1;

        outColor = color;
    }