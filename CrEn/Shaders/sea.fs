#version 460 core

in vec2 Texcoord;
in vec3 Normal;


in VS_OUT {
    vec3 FragPos;
    vec4 FragPosLightSpace;
    mat4 depthMVP;
} fs_in;

in vec3 ViewDir;

uniform samplerCube skybox;
uniform sampler2D shadowMap;
uniform float time;

uniform vec3 eye;
uniform vec3 light;

layout (location = 0) out vec4 outAlbedo;
layout (location = 1) out vec4 outNormal;
layout (location = 2) out vec4 outPos;
layout (location = 3) out vec4 outColor;


float random (in vec2 _st) {
    return fract(sin(dot(_st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

float noise (in vec2 _st) {
    vec2 i = floor(_st);
    vec2 f = fract(_st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

#define NUM_OCTAVES 5

float fbm ( in vec2 _st) {
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100.0);
    // Rotate to reduce axial bias
    mat2 rot = mat2(cos(0.5), sin(0.5),
                    -sin(0.5), cos(0.50));
    for (int i = 0; i < NUM_OCTAVES; ++i) {
        v += a * noise(_st);
        _st = rot * _st * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}

float sigmoid(in float x){
    float res = 1 + pow(2.17, x);
    res = 1 / res;
    return res;
}

float ShadowCalculation(vec4 fragPosLightSpace){
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-light);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.001);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0 || projCoords.x > 1.0  || projCoords.y > 1.0 || projCoords.x < 0.0  || projCoords.y < 0.0)
        shadow = 0.0;

        
    return shadow;
}


void main(){    
   const float speed1 = -0.005;
   const float speed2 = 0.005;
   const float scale1 = 2; 
   const float scale2 = 5; 

   vec2 _xy1 = scale1 * vec2(fs_in.FragPos.x + time*speed1, fs_in.FragPos.z + time*speed1);
   vec2 _xy2 = scale2 * vec2(fs_in.FragPos.x + time*speed2, fs_in.FragPos.z + time*speed2);

   float _noise1 = ( fbm(vec2(fbm(_xy1)) + _xy1 + time*speed1));
   float _noise2 =( fbm(vec2(fbm(_xy2)) + _xy2 + time*speed2));

   float noiseComb = (_noise1 + _noise2 - 1) * 0.1; 

    vec3 normal = normalize( vec3(noiseComb, 1, noiseComb));
    vec3 viewDir = normalize(eye - fs_in.FragPos);
   
   
  //  vec3 reflectDir = reflect(normalize(viewDir), normal);

  // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8)*2;

   float transparency = sqrt(1-abs(dot( viewDir, vec3(0, 1, 0))))*1.5;

   float shadow =1- ShadowCalculation(fs_in.FragPosLightSpace);

   // outColor =  vec4(max(col + spec*shadow, vec3(8, 64, 176)/255) + 0.2*shadow,  max(0.5, transparency + spec));
   vec3 reflection  = texture(skybox, reflect(-viewDir, normal)).rgb;
   outColor.rgb = max(vec3(0.2, 0.2, 0.4), reflection * shadow);
   outColor.a = transparency;

   outPos = vec4(fs_in.FragPos, 1);
   outAlbedo = outColor;//vec4(0.2, 0.2, 0.4, 1);
   outNormal = vec4(normal, 1);

}