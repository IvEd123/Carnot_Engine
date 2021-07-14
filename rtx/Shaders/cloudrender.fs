#version 460 core

in vec3 TexCoord;

in VS_OUT {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 FragPos;
} fs_in;

in CUBE_PARAM{
    vec3 min;
    vec3 max;
}cube_param;

uniform sampler2D tex;
uniform vec3 eyepos;
uniform float time;

out vec4 outColor;

const int samples = 20;
 float DensityThreshold = 0.7;
 float DensityMultiplier = 20;

float k = sin(time*0.01)*0.5+1;

vec3 toLocal(vec3 v){
    return (transpose(fs_in.model) * vec4( v, 1.0)).xyz;
}

vec2 posToUVW(vec3 v){
    return vec2(v.x + v.z, v.y);
}

float GetSample(vec3 pos){
    
    vec2 uv = posToUVW(toLocal( pos)) ;

    float col = texture(tex,   uv).r;
    col = max (0, col - DensityThreshold) * DensityMultiplier ;
    return col;
}

vec3 min_point = cube_param.min;
vec3 max_point = cube_param.max;


vec2 IntersectCube(vec3 origin, vec3 dir){
    vec3 tMin = (min_point - origin) / dir;
    vec3 tMax = (max_point - origin) / dir;

    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);

    float dstA = max(max(t1.x, t1.y), t1.z);
    //float dstA = length(t1);
    //float dstB = length(t2);
    float dstB = min(min(t2.x, t2.y), t2.z);

    float dstToBox = max(0, dstA);
    float dstInsideBox = max(0, dstB - dstToBox);
    
    

    return vec2(dstToBox, dstInsideBox);
}



void main(){   
    vec3 texCoord = TexCoord;
    texCoord.z = round((TexCoord.z-0.5) * 100)/100+0.5;

    vec3 dir = normalize(fs_in.FragPos - eyepos);
    
    vec3 origin = eyepos;
 
    vec2 t = IntersectCube(origin, dir);

    float dstToBox = t.x;
    float dstInsideBox = t.y;
    float step = dstInsideBox /samples ;

    float dstTravelled = 0;
    
    float totalDensity = 0; 
    vec3 rayPos =  fs_in.FragPos;
    /*while(dstTravelled < dstInsideBox){
        rayPos += dir * step;
        totalDensity += GetSample(rayPos) / samples;
        dstTravelled += step;
    }*/

    totalDensity = GetSample(rayPos + dir * step*0) ;
    

   //outColor = vec4(vec3(1-  exp(-totalDensity)), 1);
   //outColor = vec4(vec3(1), 1-  exp(-totalDensity));
    
   //outColor = vec4((dir+vec3(1)/2), 1);
     
   //outColor = vec4( origin , 1);
   //outColor =  vec4(vec3( (dstInsideBox/2)), 1);
   outColor = texture(tex, posToUVW(texCoord));
 
 }