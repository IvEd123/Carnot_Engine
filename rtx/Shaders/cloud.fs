#version 460 core


vec3 hash33(vec3 p3)
{
	p3 = fract(p3 * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yxz+33.33);
    return fract((p3.xxy + p3.yxx)*p3.zyx);

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

float worley(vec3 st){
    float freq = 1;
    
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


   
  min_dist = 1 - min_dist;
  return min_dist;
}



void main(){      
    vec3 pos ;
    pos.x = mod( gl_FragCoord.x, layers);
    pos.y = gl_FragCoord.y;
    pos.z = floor(gl_FragCoord.x / layers);

    pos /= float(layers);

    

    
    outColor = vec4(worley(pos),worley(pos * 0.8) , worley(pos * 3) , 1);
   // outColor = vec4(vec3(st.z), 1);
}