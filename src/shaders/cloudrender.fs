#version 460 core

in vec3 TexCoord;
in vec3 FragPos;
in vec3 vert_min;
in vec3 vert_max;
in vec3 translation;
in mat4 model_out;

uniform vec3 tex_res;

uniform sampler3D tex;
uniform vec3 eyepos;
uniform float time;
uniform vec3 light;

uniform vec3 LightColor;

uniform vec3 cloudScale;
uniform vec3 cloudOffset;

uniform vec3 secondLayerScale;
uniform vec3 secondLayerOffset;

uniform vec3 thirdLayerScale;
uniform vec3 thirdLayerOffset;

uniform vec4 phaseParams;


uniform float r;
uniform float R;
uniform vec3 center;

uniform  int num_of_steps;
uniform float DensityThreshold;
uniform float DensityMultiplier;
uniform float lightAbsorptionThroughCloud;

uniform  int num_of_steps_inside;
uniform float lightAbsorptionTowardSun;
uniform float darknessThreshold;

float k = sin(time*0.01);
const float scale_coeff = 0.1;

out vec4 outColor;

vec3 toLocal(vec3 v){
    return v - translation;
}


vec2 posToUVW(vec3 v){
	//v -= vert_min;
	//v = mod(v, vec3(1.0));
	//return  mod(vec2(floor(v.z*tex_res.z)/tex_res.z + (v.x) / tex_res.z + 0.5, v.y), vec2(1.0));
    return mod(vec2((v.x +0.5)*0.01 +floor(v.z*100)*.01, v.y ), vec2(1.0));
}

vec3 random( vec3 p ) {
    return fract(sin(vec3(dot(p,vec3(0.250,0.350, 0.546)),dot(p,vec3(269.5,183.3, 0.354)),dot(p,vec3(269.5, 0.347,183.3)) ))*43758.5453);
}



 float hg(float a, float g) {
    float g2 = g*g;
    return (1-g2) / (4*3.1415*pow(1+g2-2*g*(a), 1.5));
}

float phase(float a) {
    float blend = .5;
    float hgBlend = hg(a,phaseParams.x) * (1-blend) + hg(a,-phaseParams.y) * blend;
    return phaseParams.z + hgBlend*phaseParams.w;
}


vec2 intersectBox(vec3 origin, vec3 dir){
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

vec2 intersectSpheres(vec3 origin, vec3 dir){
	vec3 k = origin - center;
	float b = dot(k, dir);
	float c = dot(k, k) - r*r;
	float C = dot(k, k) - R*R;
	float d = b*b - c;
	float D = b*b - C;

	float t1 =-b + sqrt(d);
	float t2 =-b + sqrt(D);


	return vec2(t1, t2);

}

float GetSample(vec3 pos){
	vec3 uvw1 = mod(pos * cloudScale * scale_coeff + cloudOffset, vec3(1.0));
	vec3 uvw2 = mod(pos * secondLayerScale * scale_coeff + secondLayerOffset, vec3(1.0));
	vec3 uvw3 = mod(pos * thirdLayerScale * scale_coeff + thirdLayerOffset, vec3(1.0));

	vec3 col = vec3(texture(tex, uvw1).r,
					texture(tex, uvw2).g,
					texture(tex, uvw3).b);

	vec3 detail = vec3( texture(tex, uvw1, 3).r,
						texture(tex, uvw1, 3).g,
						texture(tex, uvw1, 3).b);

	float detail_res = detail.r * 0.625 + detail.g * 0.250 + detail.b*0.125;

	vec3 erode = 1 - col;
	erode = vec3( pow(erode.x, 4), pow(erode.y, 4), pow(erode.z, 4));
	float erode_res = (erode.r * 0.625 + erode.g * 0.250 + erode.b * 0.125);
	float col_res = col.r * 0.750 + col.g * 0.125 + col.b*0.125;

	float res = col_res - detail_res * erode_res;

	res = (res - DensityThreshold) * DensityMultiplier;

	return res;

}

float lightmarch(vec3 position){
	vec3 dirToLight = normalize(light - eyepos);
	float dstInsideBox = intersectBox(position, dirToLight).y;

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
	//texCoord.z = round(TexCoord.z*tex_res.z)/tex_res.z + 0.5;
	vec3 dirToLight = normalize(light);

	vec3 origin = eyepos;
	vec3 dir = normalize(FragPos - eyepos);

	vec2 t = intersectBox(origin, dir);
	vec2 ts = intersectSpheres(origin, dir);

	float dstInsideBox = t.y - t.x;
	float dstToBox = t.x;
	float dstTravelled = 0;
	float step = dstInsideBox / num_of_steps;
	float totalDensity  = 0;
	float transmittance = 1;
	vec3 lightEnergy = vec3(0);

	float phaseVal = phase(dot(dir, dirToLight));
	dstTravelled += step;
	while(dstTravelled < dstInsideBox){
		
		
		vec3 rayPos = origin + dir * (dstToBox +  dstTravelled);


			float density = GetSample(rayPos);

			

			if(density > 0){
				float lightTransmittance = lightmarch(rayPos);
				lightEnergy += density * step * lightTransmittance * transmittance * phaseVal;
				transmittance *= exp( - density * step * lightAbsorptionThroughCloud);
				if (transmittance < 0.01)
					break;
			}
		
	
		dstTravelled += step;
	}

	vec3 cloudCol = lightEnergy * LightColor;
	vec3 color = cloudCol + vec3(1) * transmittance;

	

	outColor = vec4(color + dstToBox/2, (1 - transmittance) * (1.2 - dstToBox));
	//outColor = vec4(vec3(lightEnergy), 1);
}
