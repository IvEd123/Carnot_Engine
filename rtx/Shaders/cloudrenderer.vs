#version 460 core

in vec3 position;

uniform vec3 tex_res;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 size;
uniform vec3 pos;

out vec3 TexCoord;

out vec3 FragPos;

out vec3 vert_min;
out vec3 vert_max;
out vec3 translation;
out mat4 model_out;

void main(){
	gl_Position =  proj * view * model  *  vec4(position*size , 1);
	
	TexCoord = vec3((position.x )/tex_res.z, position.y + 0.5, position.z);

	FragPos = (model  *  vec4(position*size , 1)).xyz;
	translation = FragPos -  position*size;
	vert_min = (model  *  vec4(vec3(-0.5)*size , 1)).xyz;
	vert_max = (model  *  vec4(vec3( 0.5)*size , 1)).xyz;
	model_out = model;
}