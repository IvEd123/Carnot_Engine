#version 460 core

out vec4 outColor;

in vec2 Texcoord;

in VS_OUT {
    vec3 FragPos;
    vec4 FragPosLightSpace;
    mat4 depthMVP;
} fs_in;
in vec3 ViewDir;

float atmosphereRadius = 6471e3;
float planetRadius = 6371e3;
vec3 planetCenter = vec3(0, 0, 0);

float Hr = 8e3;
float Hm = 1.5e3;

float heightAbsorption = 30e3;
float absorptionFalloff = 4e3;

uniform vec3 eyepos;
uniform vec3 light;

const int NUM_OF_SAMPLES = 12;
const int NUM_OF_SAMPLES_SECOND_PASS = 12;
const float DENSITY_FALLOFF = 20;

vec3 waveLengths = vec3(680, 550, 440);
float scaterringStrength = 0.0020;

float power = 4.0;
float coeff = 400; 

vec3 absorptionRay = vec3(5.5e-6, 13.0e-6, 22.4e-6);
vec3 absorptionBeta = vec3(2.04e-5, 4.97e-5, 1.95e-6);
vec3 ambientBeta = vec3(0);


vec3 absorptionMie = vec3(21e-6);

float G = 0.85;

vec2 intersectSphere(vec3 origin, vec3 dir, float radius, vec3 center){
	//vec3 k = origin - center;
	float a = dot(dir, dir);
	float b = 2.0 * dot(origin, dir);
	float c = dot(origin, origin) - radius*radius;
	float d = b*b - 4 * a * c;

	if (d < 0)
		return vec2(1e5,-1e5);;

	float sqrtd = sqrt(d);

	float t1 = (-b - sqrtd) / (a * 2.0f);
	float t2 = (-b + sqrtd) / (a * 2.0f);
	 


	return vec2(t1, t2);

}

float getAtmosphereDensity(vec3 p){
	float H = length( p - planetCenter);
	float h = (H - atmosphereRadius) / planetRadius;
	return exp(-h * DENSITY_FALLOFF) * (1 - h);
}

float opticalDepth(vec3 pos, vec3 dir, float rayLength){
	float _step = rayLength / (NUM_OF_SAMPLES_SECOND_PASS-1);
	float res = 0;

	vec3 rayPos = pos;
	for(int i = 0; i < NUM_OF_SAMPLES_SECOND_PASS; i++){
		float localDensity = getAtmosphereDensity(rayPos);
		res += localDensity * _step;
		rayPos += dir * rayLength;
	}

	return res;
}

vec3 calculate_scattering(
	vec3 start, 				
    vec3 dir, 					
    float max_dist, 			
    vec3 scene_color,			
    vec3 light_dir, 			
    vec3 light_intensity,		
    vec3 planet_position, 		
    float planet_radius, 		
    float atmo_radius, 			
    vec3 beta_ray, 				
    vec3 beta_mie, 				
    vec3 beta_absorption,   	
    vec3 beta_ambient,			
    float g, 					
    float height_ray, 			
    float height_mie, 			
    float height_absorption,	
    float absorption_falloff			
) {
	start -= planet_position;
	vec2 rayLength = intersectSphere(start, dir, atmo_radius, planet_position);

	rayLength.y = min(rayLength.y, max_dist);
	rayLength.x = max(rayLength.x, 0.0);

	float stepSize_i = (rayLength.y - rayLength.x) / float(NUM_OF_SAMPLES);

	float RayPos_i = rayLength.x + stepSize_i * 0.5;

	vec3 totalRay = vec3(0);
	vec3 totalMie = vec3(0);

	vec3 opticalDepth_i = vec3(0);

	vec2 scaleHeight = vec2(height_ray, height_mie);

	vec3 prevDensity = vec3(0);

	float mu = dot(dir, light_dir);
	float mumu = mu * mu;
	float gg = g * g;
	float phaseRay = 3.0 / (50.2654824574 /* (16 * pi) */) * (1.0 + mumu);
	float phaseMie =  3.0 / (25.1327412287 /* (8 * pi) */) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

	for (int i = 0; i < NUM_OF_SAMPLES; ++i){
		vec3 pos_i = start + dir * RayPos_i;

		float height_i = length(pos_i) - planet_radius;

		vec3 density = vec3( exp( -height_i / scaleHeight), 0);

		float denom = (height_absorption - height_i) / absorption_falloff;

		density.z = (1.0 / (denom * denom + 1.0) * density.x);

		density *= stepSize_i;

		opticalDepth_i +=	max (density + (prevDensity - density) * 0.5, 0);
		
		prevDensity = density;

		float stepSize_l = intersectSphere(pos_i, light_dir, atmo_radius, planet_position).y; // !!!!!MAY BE BUG

		float RayPos_l = stepSize_l * 0.5;

		vec3 opticalDepth_l = vec3(0), prevDensity_l = vec3(0);

		for(int l = 0; l < NUM_OF_SAMPLES_SECOND_PASS; ++l){
			vec3 pos_l = pos_i + light_dir * RayPos_l;
			float height_l = length(pos_l) - planet_radius;
			vec3 density_l = vec3(exp(-height_l / scaleHeight), 0);
			float denom = (height_absorption - height_l) / absorption_falloff;
			density_l.z = (1.0 / (denom * denom + 1.0)) * density_l.x;

			density_l *= stepSize_l;

			opticalDepth_l += max(density_l + (prevDensity_l - density_l) * 0.5, 0);

			prevDensity_l = density_l;
			RayPos_l += stepSize_l;
		}

		vec3 attenuation = exp( -beta_ray * (opticalDepth_i.x + opticalDepth_l.x) - beta_mie * (opticalDepth_i.y  + opticalDepth_l.y) - beta_absorption	* (opticalDepth_i.z + opticalDepth_l.z));

		totalRay += density.x * attenuation;
		totalMie += density.y * attenuation;

		RayPos_i += stepSize_i;

	}

	vec3 opacity = exp(- (beta_mie * opticalDepth_i.y + beta_ray * opticalDepth_i.x + beta_absorption * opticalDepth_i.z));

	//return opacity;

	return (
		phaseRay * beta_ray * totalRay 
		+ phaseMie * beta_mie * totalMie
		+ opticalDepth_i.x * beta_ambient
		) * light_intensity + scene_color * opacity; 
		//return vec3(0);*/

}

/*void main(){
	vec3 lightDir =  normalize(vec3(1, -1.25, 0));
    vec3 viewDir = normalize(fs_in.FragPos);
	
	vec3 rayPos = vec3(0);
	float atmosphereHeight = planetRadius;

	float distToEndOfAtmosphere = intersectSphere(rayPos, viewDir, atmosphereHeight);
	float stepSize = distToEndOfAtmosphere / float(NUM_OF_SAMPLES);
	vec3 res = vec3(0);
	vec3 sumR = vec3(0), sumM = vec3(0);
	float opticalDepthR = 0, opticalDepthM = 0;
	float mu = dot(viewDir, lightDir);
	float phaseR = 0.05968 * (1.f + mu * mu);
	float g = 0.76;
	float phaseM = 0.119366 * ((1.f - g * g) * ( 1.f + mu * mu)) / ((2.f + g * g) * pow(1.f + g * g - 2.f * g * mu, 1.5f)); 

	
	for(int i = 0; i < NUM_OF_SAMPLES - 1; i++){
		rayPos += viewDir * stepSize;
		float height = length(rayPos - planetCenter);

		float hr = exp( - height / Hr) * stepSize;
		float hm = exp( - height / Hm) * stepSize;
		opticalDepthR += hr;
		opticalDepthM += hm;

		float tLight = intersectSphere(rayPos, lightDir, atmosphereHeight);

		float stepLight = tLight / float(NUM_OF_SAMPLES_SECOND_PASS), tCurrentLight = 0;
		float opticalDepthLightR = 0, opticalDepthLightM = 0;

		for(int j = 0; j < NUM_OF_SAMPLES_SECOND_PASS; j++){
			vec3 rayPosLight = rayPos + (tCurrentLight + stepLight * 0.5) * lightDir;
			float heightLight = length(rayPosLight - planetCenter);
			opticalDepthLightR += exp(-heightLight / Hr) * stepLight;
			opticalDepthLightM += exp(-heightLight / Hm) * stepLight;
			tCurrentLight += stepLight;
		}

		vec3 tau = absorptionRay * (opticalDepthLightR + opticalDepthLightR) + absorptionMie * 1.1f * (opticalDepthLightM + opticalDepthLightM);
		vec3 attenuation = vec3(exp(-tau));
		sumR += attenuation * hr;
		sumM += attenuation * hm;
		
	}

	float sun = pow(max(dot(viewDir, -lightDir), 0), 64.0);

	
	res = (sumR * absorptionRay * phaseR + sumM * absorptionMie * phaseM) * 200.f;
	outColor = vec4(vec3(res), 1);
	//outColor = vec4(vec3(intersectSphere(rayPos, lightDir, atmosphereHeight)/7f), 1);
	//outColor = vec4( normalize(vec3(ViewDir)), 1);
}*/


void main(){
	vec3 lightDir =  normalize(light);
    vec3 viewDir = normalize(fs_in.FragPos);
	viewDir.y += 0.5;
	viewDir = normalize(viewDir);

	vec3 camera_position = vec3(0.0, planetRadius + 200.0, 0.0);
	
	vec3 color = vec3(0);

	color = calculate_scattering(
		camera_position,
		viewDir,
		1e12,
		vec3(0),
		lightDir,
		vec3(40.0),
		planetCenter,
		planetRadius,
		atmosphereRadius,
		absorptionRay,
		absorptionMie, 
		absorptionBeta, 
		ambientBeta, 
		G, 
		Hr,
		Hm, 
		heightAbsorption, 
		absorptionFalloff
	);

	//color /= 1.f;

	color = 1.0 - exp (- color);
outColor = vec4(color, 1);
	
}