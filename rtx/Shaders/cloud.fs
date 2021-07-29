#version 460 core

uint UI0 = 1597334673U;
uint UI1 = 3812015801U;
uvec2 UI2 =  uvec2(UI0, UI1);
uvec3 UI3 = uvec3(UI0, UI1, 2798796415U);
	

vec3 hash33_2(vec3 p)
{
	uvec3 q = uvec3(ivec3(p*100.0)) * UI3;
	q = (q.x ^ q.y ^ q.z)*UI3;
	return 1.0 + 2.0 * vec3(q)/ float(0xffffffffU);
}

vec3 hash33(vec3 p3)
{
	p3 = fract(p3 * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yxz+33.33);
    return fract((p3.xxy + p3.yxx)*p3.zyx);

}


in vec2 TexCoord;
in vec3 FragPos;

layout(location = 0)  out vec4 outColor;

const int MAX_POINTS_NUM = int(pow(100, 3));

uniform int grid_resolution;
uniform int points_num;
uniform vec3 point_pos [1000];
uniform int layers;

int num_of_cells = grid_resolution * grid_resolution * grid_resolution;

 const ivec3 offsets[27] ={
    // centre
    ivec3(0,0,0),
    // front face
    ivec3(0,0,1),
    ivec3(-1,1,1),
    ivec3(-1,0,1),
    ivec3(-1,-1,1),
    ivec3(0,1,1),
    ivec3(0,-1,1),
    ivec3(1,1,1),
    ivec3(1,0,1),
    ivec3(1,-1,1),
    // back face
    ivec3(0,0,-1),
    ivec3(-1,1,-1),
    ivec3(-1,0,-1),
    ivec3(-1,-1,-1),
    ivec3(0,1,-1),
    ivec3(0,-1,-1),
    ivec3(1,1,-1),
    ivec3(1,0,-1),
    ivec3(1,-1,-1),
    // ring around centre
    ivec3(-1,1,0),
    ivec3(-1,0,0),
    ivec3(-1,-1,0),
    ivec3(0,1,0),
    ivec3(0,-1,0),
    ivec3(1,1,0),
    ivec3(1,0,0),
    ivec3(1,-1,0)
};

int min_component(ivec3 vec){
     return min(vec.x, min(vec.y, vec.z));
}

int max_component(ivec3 vec){
     return max(vec.x, max(vec.y, vec.z));
}

ivec3 wrap(ivec3 v){
    ivec3 res = v;
    res.x = (grid_resolution + v.x) % grid_resolution;
    res.y = (grid_resolution + v.y) % grid_resolution;
    res.z = (grid_resolution + v.z) % grid_resolution;
    return res;
}

vec3 random( vec3 p ) {
    return fract(sin(vec3(dot(p,vec3(0.250,0.350, 0.546)),dot(p,vec3(269.5,183.3, 0.354)),dot(p,vec3(269.5, 0.347,183.3)) ))*43758.5453);
}

void main(){      
    float freq =1;
    vec3 pos ;
    pos.x = mod( gl_FragCoord.x, layers);
    pos.y = gl_FragCoord.y;
    pos.z = floor(gl_FragCoord.x / layers);

    pos /= float(layers);

    vec3 st = pos * 1;

    vec3 cell_size = vec3(1.0 / float(grid_resolution));

    float min_dist = 100;
    
    vec3 f_st = mod(st, cell_size)*grid_resolution;
    vec3 cellID = st - f_st/grid_resolution;
       
    for (float x = -grid_resolution; x <= grid_resolution; ++x){
       for(float y = -grid_resolution; y <= grid_resolution; ++y){
           for(float z = -grid_resolution; z <= grid_resolution; ++z){
                vec3 offset = vec3(x, y, z);
                vec3 point_cell = cellID + offset * cell_size;

                if (point_cell.x < 0.0){
                point_cell.x = mod(point_cell.x + 1.0, 1.0);
                }
            if (point_cell.x > 1.0){
                point_cell.x = point_cell.x - 1.0;
                
                }
            
            if (point_cell.y < 0.0){
                point_cell.y = mod(point_cell.y + 1.0, 1.0);
                
                }
            if (point_cell.y > 1.0){
                point_cell.y = point_cell.y - 1.0;
                
                }

                vec3 point = hash33(mod(point_cell, vec3(freq)) )*0.5 + 0.5;
                point +=  offset;
                float dist =length(  point-f_st );   
                min_dist = min(min_dist, dist);
             }
        }
    } 


   {/*// working but not seamless
   for(int x = -1; x <= 1; x++){
         for(int y = -1; y <= 1; y++){
            for(int z = -1; z <= 1; z++){
                vec3 neighbor = vec3(float(x), float (y), float (z));
                vec3 neighbor_pos = cellID + neighbor;

                if (neighbor_pos.x < 0)
                    neighbor_pos.x = 1 + neighbor_pos.x;
                else if (neighbor_pos.x > 1)
                    neighbor_pos.x =  neighbor_pos.x - 1;

                 if (neighbor_pos.y < 0)
                    neighbor_pos.y = 1 + neighbor_pos.y;
                else if (neighbor_pos.y > 1)
                    neighbor_pos.y =  neighbor_pos.y - 1;

                 if (neighbor_pos.z < 0)
                    neighbor_pos.z = 1 + neighbor_pos.z;
                else if (neighbor_pos.z > 1)
                    neighbor_pos.z =  neighbor_pos.z - 1;

                vec3 point = random(neighbor_pos + cellID );

               vec3 diff = neighbor + point - f_st;
               float dist = length(diff);
               min_dist = min(min_dist, dist);
            }
        }
   }*/}

  //min_dist *= grid_resolution;
  min_dist = 1 - min_dist;
    outColor = vec4(vec3(min_dist), 1);
   // outColor = vec4(vec3(st.z), 1);
}