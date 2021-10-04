#version 460 core

in vec2 Texcoord;
in vec3 Normal;
in vec3 T;
in vec3 B;


uniform sampler2D tex;
uniform sampler2D norm;
uniform sampler2D ao;

out vec4 outColor;

void main(){
   vec4 col = texture(tex, Texcoord) * texture(ao, Texcoord);

   mat3 TBN = mat3(T, B, Normal);

   vec3 normal = texture(norm, Texcoord).rgb;
   // перевод вектора нормали в интервал [-1,1]
   normal = normalize(normal * 2.0 - 1.0);   

   normal = normalize( normal); 

   //normal = normalize(TBN * Normal);
   //normal = normalize(transpose(TBN) * Normal + normal);
   normal = normalize(Normal + TBN * normal);

   col.rgb = mix(col.rgb, col.rgb*dot(normalize(vec3(0.0, 1, 0)), normal), 0.2);


   outColor =  col;
}