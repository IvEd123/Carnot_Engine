#version 460 core

    in vec2 Texcoord;

    out vec4 outColor;

    uniform sampler2D depthTex;
    uniform sampler2D texFramebuffer;
    uniform float time;

    float blursize = 0.01;

    vec4 blur (vec2 texcoord, float size){
        vec4 clor = vec4(0);
        clor += texture(texFramebuffer, (texcoord) );
        clor += texture(texFramebuffer, (texcoord +vec2(size/10,size/10 )));
        clor += texture(texFramebuffer, (texcoord +vec2(size/10,0 )));
        clor += texture(texFramebuffer, (texcoord +vec2(size/10,-size/10 )));
        clor += texture(texFramebuffer, (texcoord +vec2(0,-size/10 )));
        clor += texture(texFramebuffer, (texcoord +vec2(-size/10,-size/10 )));
        clor += texture(texFramebuffer, (texcoord +vec2(-size/10,0)));
        clor += texture(texFramebuffer, (texcoord +vec2(-size/10,size/10 )));
        clor += texture(texFramebuffer, (texcoord +vec2(0,size/10 )));
        clor = clor / 9;
        return clor;
    }

    vec4 sobel(vec2 texcoord, float size){
        vec4 color = texture(texFramebuffer, (texcoord)/1 );
        vec4 topright = texture(texFramebuffer, (texcoord +vec2(size/10,size/10 )));
        vec4 right = texture(texFramebuffer, (texcoord +vec2(size/10,0 )));
        vec4 bottomright = texture(texFramebuffer, (texcoord +vec2(size/10,-size/10 )));
        vec4 bottom = texture(texFramebuffer, (texcoord +vec2(0,-size/10 )));
        vec4 bottomleft = texture(texFramebuffer, (texcoord +vec2(-size/10,-size/10 )));
        vec4 left = texture(texFramebuffer, (texcoord +vec2(-size/10,0)));
        vec4 topleft = texture(texFramebuffer, (texcoord +vec2(-size/10,size/10 )));
        vec4 top = texture(texFramebuffer, (texcoord +vec2(0,size/10 )));

        vec4 sx = -topleft - 2 * left - bottomleft + topright + 2 * right + bottomright;
        vec4 sy = -topleft - 2 * top - topright + bottomleft + 2 * bottom + bottomright;

        vec4 res = sqrt(sx * sx - sy * sy);

        return res;
    }

    float getLen(vec4 v){
        return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    }

    void main(){      
        vec4 color= texture(texFramebuffer, Texcoord);
        //color.rgb = pow(color.rgb, vec3(3));



        outColor = max( color , vec4(0.1, 0.1, 0.11, 1.0));
    }