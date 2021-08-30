#version 460 core

in vec2 Texcoord;

uniform vec3 light;
uniform vec3 eye;

uniform samplerCube skybox;

out vec4 outColor;

in VS_OUT {
    vec3 FragPos;
    vec4 FragPosLightSpace;
    mat4 depthMVP;
} fs_in;

in vec3 ViewDir;

float random (in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))*43758.5453123);
}

float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

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

#define OCTAVES 6
float fbm (in vec2 st) {
    // Initial values
    float value = 0.0;
    float amplitude = .5;
    float frequency = 0.;
    //
    // Loop of octaves
    for (int i = 0; i < OCTAVES; i++) {
        value += amplitude * noise(st);
        st *= 2.;
        amplitude *= .5;
    }
    return value;
}

void main(){
    float x = fs_in.FragPos.x;
    float y = fs_in.FragPos.y;
    float z = fs_in.FragPos.z;

    vec3 light_dir = normalize(eye - light);
    vec3 view_dir = normalize(eye - fs_in.FragPos);

    //outColor = vec4(0.0, 0.0, 0.2, 1.0) + texture(tex, Texcoord);
    float tint =  y / 80;
    vec3 sky = vec3(0.53, 0.8, 0.92);
    
    float len = sqrt(x*x+y*y+z*z);

    float k = sqrt(x*x+z*z) / y ;

    float noise = fbm(vec2(x, z) * k / 50);

    float cloud = fbm(vec2(x, z) * k /200);
    if (cloud < 0.4)
        cloud *= 0.0;

    cloud *= noise;

    

    

     outColor = vec4(sky +  vec3(1-  tint, 1- tint, 0.0)*0.2 + vec3(cloud * (tint)) * 0.2 , 1.0);
     outColor.rgb += pow(max(dot(view_dir, light_dir), 0), 64.0);

     //outColor = vec4( normalize(fs_in.FragPos), 1);
     outColor = texture(skybox, normalize(ViewDir));
}