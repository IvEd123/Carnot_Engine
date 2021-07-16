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
uniform vec3 light;

vec3 LightColor = vec3(1, 1, 1);

const int num_of_steps = 20;
float DensityThreshold = 0.75;
float DensityMultiplier = 50;
float lightAbsorptionThroughCloud = 0.85;

const int num_of_steps_inside = 20;
float lightAbsorptionTowardSun = 1;
float darknessThreshold  = 0.07;

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

float lightmarch(vec3 position){
	vec3 dirToLight = normalize(light - eyepos);
	float dstInsideBox = intersect(position, dirToLight).y;

	float stepSize = dstInsideBox / num_of_steps_inside;
	float totalDensity = 0;

	for (int step = 0; step < num_of_steps_inside; step++){
		position += dirToLight * stepSize;
		totalDensity += max(0, GetSample(position) * stepSize);
	}

	float transmittance = exp(-totalDensity * lightAbsorptionTowardSun);
	return darknessThreshold + transmittance * (1 - darknessThreshold);
}



void main() {
	vec3 texCoord = TexCoord;
	texCoord.z = round(TexCoord.z*100)*.01 + 0.5;
	vec3 dirToLight = normalize(light - eyepos);

	vec3 origin = eyepos;
	vec3 dir = normalize(FragPos - eyepos);

	vec2 t = intersect(origin, dir);


	float dstInsideBox = t.y;
	float dstToBox = t.x;
	float dstTravelled = 0;
	float step = dstInsideBox / num_of_steps;
	float totalDensity  = 0;
	float transmittance = 1;
	vec3 lightEnergy = vec3(0);

	while(dstTravelled < dstInsideBox){

		vec3 rayPos = origin + dir * (dstToBox +  dstTravelled);
		float density = GetSample(rayPos);

		if(density > 0){
			float lightTransmittance = lightmarch(rayPos);
			lightEnergy += density * step * lightTransmittance * transmittance;
			transmittance *= exp( - density * step * lightAbsorptionThroughCloud);
			if (transmittance < 0.01)
				break;
		}
		/*
		float currentDensity = 0;
		if(totalDensity > 0){
			vec3 dir_in = normalize(light - rayPos);
			float inside2 = intersect(rayPos, dir_in).y;
			float step_inside = inside2 / num_of_steps_inside;
			float dstTravelled2 = 0;

			while (dstTravelled2 < inside2){
				 vec3 rayPos2 = rayPos + dir_in * dstTravelled2;
				 currentDensity += GetSample(rayPos2) / num_of_steps_inside;
				 dstTravelled2 +=step_inside;
			}

		}
		transmittance += currentDensity;
		*/
		dstTravelled += step;
	}

	vec3 cloudCol = lightEnergy * LightColor;
	vec3 color = cloudCol + vec3(1) * transmittance;

	outColor = vec4(color, 1 - transmittance);
	//outColor = vec4(vec3(dot(dir, normalize(light - FragPos)  )  ), 1);
	//outColor = vec4(vec3(GetSample(FragPos)), 1);
	//outColor = vec4( toLocal(FragPos) , 1);
	//outColor = vec4(vec3(1), 1  - exp(-t.y));  
	//outColor = vec4(vec3(t.x + t.y)*0.3, 1);
	//outColor = texture(tex, vec2(texCoord.x + texCoord.z, texCoord.y));
	//outColor = texture(tex, vec2(texCoord.x + texCoord.z, texCoord.y));
}