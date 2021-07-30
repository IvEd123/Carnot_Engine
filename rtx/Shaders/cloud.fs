#version 460 core

#define UI0 1597334673U
#define UI1 3812015801U
#define UI2 uvec2(UI0, UI1)
#define UI3 uvec3(UI0, UI1, 2798796415U)
#define UIF (1.0 / float(0xffffffffU))

vec3 hash33(vec3 p)
{
	uvec3 q = uvec3(ivec3(p)) * UI3;
	q = (q.x ^ q.y ^ q.z)*UI3;
	return -1. + 2. * vec3(q) * UIF;
}


in vec2 TexCoord;
in vec3 FragPos;

layout(location = 0)  out vec4 outColor;


uniform int grid_resolution;
uniform int layers;

int num_of_cells = grid_resolution * grid_resolution * grid_resolution;


vec3 random( vec3 p ) {
    return fract(sin(vec3(dot(p,vec3(0.250,0.350, 0.546)),dot(p,vec3(269.5,183.3, 0.354)),dot(p,vec3(269.5, 0.347,183.3)) ))*43758.5453);
}



float simple_interpolate(in float a, in float b, in float x){
   return a + smoothstep(0.0,1.0,x) * (b-a);
}

float noise (vec3 st){
    vec3 i = floor(st);
    vec3 f = fract(st);

    float a = random(i).r;
    float b = random(i + vec3(1.0, 0.0, 0.0)).r;
    float c = random(i + vec3(0.0, 1.0, 0.0)).r;
    float d = random(i + vec3(1.0, 1.0, 0.0)).r;
    float a1 = random(i + vec3(0.0, 0.0, 1.0)).r;
    float b1 = random(i + vec3(1.0, 0.0, 1.0)).r;
    float c1 = random(i + vec3(0.0, 1.0, 1.0)).r;
    float d1 = random(i + vec3(1.0, 1.0, 1.0)).r;

    float i1 = simple_interpolate(a, a1, f.z);
    float i2 = simple_interpolate(b, b1, f.z);
    float i3 = simple_interpolate(c, c1, f.z);
    float i4 = simple_interpolate(d, d1, f.z);

    float ii1 = simple_interpolate(i1, i2, f.x);
    float ii2 = simple_interpolate(i3, i4, f.x);

    return simple_interpolate(ii1, ii2, f.y);
}

#define OCTAVES 6

float perlin(vec3 st){
    float value = 0.0;
    
    float amplitude = 0.5;
    float freq = 0.0;

    for (int i = 0; i < OCTAVES; i++){
        value += amplitude * noise(st);
        st *= 2.0;
        amplitude *= 0.5;
    }

    return value;
}



float worley(vec3 st, float scale){
    float freq = 1;

    float _grid = float(grid_resolution) * scale;
    
    vec3 cell_size = vec3(1.0 / _grid);

    float min_dist = 100;
    
    vec3 f_st = mod(st, cell_size)*_grid;
    vec3 cellID = st - f_st/_grid;
       
    for (float x = -1; x <= 1; ++x){
       for(float y = -1; y <= 1; ++y){
           for(float z = -1; z <= 1; ++z){
                vec3 offset = vec3(x, y, z);
                vec3 point_cell = cellID + offset * cell_size;

               point_cell = abs(mod(point_cell, vec3(1.0)));

                vec3 point = hash33(mod(point_cell, vec3(freq))*100.0 )*0.5 + 0.5;
                point +=  offset;
                float dist =length(  point-f_st );   
                min_dist = min(min_dist, dist);
             }
        }
    } 


   
  min_dist = 1 - min_dist;
  return min_dist;
}



void main(){      
    vec3 pos ;
    pos.x = mod( gl_FragCoord.x, layers);
    pos.y = gl_FragCoord.y;
    pos.z = floor(gl_FragCoord.x / layers);

    pos /= float(layers);

    

    
    outColor = vec4(worley(pos, 1), worley(pos, 2), worley(pos, 4) , 1);
}