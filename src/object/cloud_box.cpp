#include "cloud_box.h"


 Cloudbox::Cloudbox(glm::vec3 _pos, glm::vec3 _res, glm::vec3 _size) {
    pos = _pos;
    rot = glm::vec3(0, 0, 0);
    cloudTexRes = _res;
    size = _size;

    CreateVerticesLegacy();
    initBuffer();
    initTexture();
    attachBuffer();
 }

 void Cloudbox::Delete() {
     deleteArrays();
     material.Delete();
     glDeleteTextures(1, &cloudtex);
     glDeleteBuffers(1, &cloudbuffer);
 }

 void Cloudbox::RenderCloud(float innerRadius, float outerRadius, glm::vec3 center) {
     uniforms();
     glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);
     glEnable(GL_TEXTURE_3D);

     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_3D, cloudtex);

     material.attachUniform("r", innerRadius);
     material.attachUniform("R", outerRadius);
     material.attachUniform("center", center);
        

     Draw();

     glDisable(GL_CULL_FACE);
     glDisable(GL_TEXTURE_3D);
 }

 void Cloudbox::renderTexture(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    glViewport(0, 0, cloudTexRes.x, cloudTexRes.y);
    glUseProgram(material.getShaderProgram());
    glBindVertexArray(material.getVAO());

    int grid_res_loc = glGetUniformLocation(material.getShaderProgram(), "grid_resolution");
    glUniform1i(grid_res_loc, pointsGrid);

    int layer_loc = glGetUniformLocation(material.getShaderProgram(), "layers");
    glUniform1i(layer_loc, cloudTexRes.z);

    for (int i = 0; i < cloudTexRes.z; ++i) {
        int layer_loc = glGetUniformLocation(material.getShaderProgram(), "layer");
        glUniform1f(layer_loc, (float)i / (float)cloudTexRes.z);
        glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, cloudtex, 0, i);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }


    glGenerateMipmap(GL_TEXTURE_3D);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
 }

 void Cloudbox::initBuffer() {
     glGenFramebuffers(1, &cloudbuffer);
     glBindFramebuffer(GL_FRAMEBUFFER, cloudbuffer);
 }

 void Cloudbox::initTexture() {
     glGenTextures(1, &cloudtex);
     glBindTexture(GL_TEXTURE_3D, cloudtex);

     glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, cloudTexRes.x, cloudTexRes.y, cloudTexRes.z, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
     

     material.bindTexture(cloudtex);
 }

 void Cloudbox::attachBuffer() {
     glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cloudtex, 0);


     if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
         std::cout << "ERRROR WITH CLOUD BUFFER " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;

     glBindFramebuffer(GL_FRAMEBUFFER, cloudbuffer);
     glDrawBuffer(GL_COLOR_ATTACHMENT0);
 }

 void Cloudbox::recreateShaders() {
     glDeleteProgram(material.getShaderProgram());
     material.loadShader(GL_VERTEX_SHADER, ".\\Shaders\\cloudrenderer.vs");
     material.loadShader(GL_FRAGMENT_SHADER, ".\\Shaders\\cloudrender.fs");
     material.CreateShaders();
 }

 void Cloudbox::uniforms() {
     material.attachUniform("DensityThreshold", cloudParams.DensityThreshold);
     material.attachUniform("lightAbsorptionTowardSun", cloudParams.lightAbsorptionTowardSun);
     material.attachUniform("darknessThreshold", cloudParams.darknessThreshold);
     material.attachUniform("DensityMultiplier", cloudParams.DensityMultiplier);
     material.attachUniform("num_of_steps", cloudParams.num_of_steps);
     material.attachUniform("num_of_steps_inside", cloudParams.num_of_steps_inside);
     material.attachUniform("lightAbsorptionThroughCloud", cloudParams.lightAbsorptionThroughCloud);
     material.attachUniform("LightColor", cloudParams.LightColor);
     material.attachUniform("cloudScale", cloudParams.cloudScale);
     material.attachUniform("cloudOffset", cloudParams.cloudOffset);
     material.attachUniform("secondLayerScale", cloudParams.secondLayerScale);
     material.attachUniform("secondLayerOffset", cloudParams.secondLayerOffset);
     material.attachUniform("thirdLayerScale", cloudParams.thirdLayerScale);
     material.attachUniform("thirdLayerOffset", cloudParams.thirdLayerOffset);
     material.attachUniform("phaseParams", cloudParams.phaseParams);

     material.attachUniform("tex_res", glm::vec3(cloudTexRes.x, cloudTexRes.y, cloudTexRes.z));
 }
 