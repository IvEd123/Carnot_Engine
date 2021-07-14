#version 460 core

in vec3 TexCoord;
in vec3 FragPos;
in vec3 vert_min;
in vec3 vert_max;
in vec3 translation;
in mat4 model_out;

uniform sampler2D tex;
uniform vec3 eyepos;
uniform float time;

const int num_of_steps = 10;
float DensityThreshold = 0.7;
float DensityMultiplier = 50;

float k = sin(time*0.01)*0.5;

out vec4 outColor;

vec3 toLocal(vec3 v){
    return v - translation;
}


vec2 posToUVW(vec3 v){
    return vec2((v.x + 0.5)*0.01 +round(v.z*100)*.01 + 0.5, v.y + 0.5);
}

vec2 intersect (vec3 origin, vec3 dir){
	vec3 t0 = (vert_min - origin) / dir;
	vec3 t1 = (vert_max - origin) / dir;
	vec3 tmin = min(t0, t1);
	vec3 tmax = max(t0, t1);

	float dstA = max( max(tmin.x, tmin.y), tmin.z );
	float dstB = min( min(tmax.x, tmax.y), tmax.z );

	float dstToBox = max (0, dstA);
	float dstInsideBox = max (0, dstB - dstToBox);
	return vec2(dstToBox, dstInsideBox);
}

float GetSample(vec3 pos){
    
    vec2 uv = posToUVW(toLocal( pos)) ;

    float col = texture(tex,   uv).r;
    col = max (0, col - DensityThreshold) * DensityMultiplier ;
    return col;
}


void main() {
	vec3 texCoord = TexCoord;
	texCoord.z = round(TexCoord.z*100)*.01 + 0.5;

	vec3 origin = eyepos;
	vec3 dir = normalize(FragPos - eyepos);

	vec2 t = intersect(origin, dir);

	float dstInsideBox = t.y;
	float dstToBox = t.x;
	float dstTravelled = 0;
	float step = dstInsideBox / num_of_steps;
	float totalDensity  = 0;

	while(dstTravelled < dstInsideBox){
		vec3 rayPos = origin + dir * (dstToBox +  dstTravelled);
		totalDensity += GetSample(rayPos) / num_of_steps;
		dstTravelled += step;
	}

	outColor = vec4(vec3(1),1 - exp(-totalDensity));

	//outColor = vec4(vec3(GetSample(FragPos)), 1);
	//outColor = vec4( toLocal(FragPos) , 1);
	//outColor = vec4(vec3(1), 1  - exp(-t.y));  
	//outColor = vec4(vec3(t.x + t.y)*0.3, 1);
	//outColor = texture(tex, vec2(texCoord.x + texCoord.z, texCoord.y));
	//outColor = texture(tex, vec2(texCoord.x + texCoord.z, texCoord.y));
}