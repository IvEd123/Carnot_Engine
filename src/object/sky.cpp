#include "sky.h"
#include "../render/render.h"


 void Sky::Camera::switchToFace(int faceIndex) {
     switch (faceIndex){
     case 0:
         pitch = 0;
         yaw = 90;
         break;
     case 1:
         pitch = 0;
         yaw = -90;
         break;
     case 2:
         pitch = -90;
         yaw = 180;
         break;
     case 3:
         pitch = 90;
         yaw = 180;
         break;
     case 4:
         pitch = 0;
         yaw = 180;
         break;
     case 5:
         pitch = 0;
         yaw = 0;
         break;
     default:
         break;
     }
 }

 void Sky::Camera::createMatrices() {
    float scale = (float)((1.f / tan(M_PI / 2.f)));
    float frustrum_length = 100;

    proj = glm::perspective(M_PI_2, 1.0, 0.1, 100.0);
    
 }

 void Sky::updateMatrices() {
     camera.view = glm::mat4(1.0f);
     
     camera.view = glm::rotate(camera.view, glm::radians(180.f ), glm::vec3(0.0, 0.0, 1.0));
     camera.view = glm::rotate(camera.view, glm::radians(camera.pitch ), glm::vec3(1.0, 0.0, 0.0));
     camera.view = glm::rotate(camera.view, glm::radians(camera.yaw ),   glm::vec3(0.0, 1.0, 0.0));
     camera.view = glm::translate(camera.view, ConvertSFML2GLM(-camera.cameraPos));
    //add from tutorial if doesn't work
 }

 void Sky::Cloud::initTexture(int res){
     glGenTextures(1, &cloudCubeMap);
     glBindTexture(GL_TEXTURE_CUBE_MAP, cloudCubeMap);

     for (int i = 0; i < 6; i++) {
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, res, res, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

     }
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
 }

 void Sky::RenderCloud() {
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_3D);

    cloudsOnSky.cloudbox->uniforms();

    //cloudsOnSky.cloudbox->material.updateUniforms();
    cloudsOnSky.cloudbox->material.attachUniform("r", innerRadius);
    cloudsOnSky.cloudbox->material.attachUniform("R", outerRadius);
    cloudsOnSky.cloudbox->material.attachUniform("center", ConvertSFML2GLM(centerPos));

    glUseProgram(cloudsOnSky.cloudbox->material.getShaderProgram());
    glBindVertexArray(cloudsOnSky.cloudbox->material.getVAO());
     
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, cloudsOnSky.cloudbox->GetTexture());

    cloudsOnSky.cloudbox->material.setModel(glm::mat4(1.0));

    GLuint uniView = glGetUniformLocation(cloudsOnSky.cloudbox->material.getShaderProgram(), "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));

    GLuint uniProj = glGetUniformLocation(cloudsOnSky.cloudbox->material.getShaderProgram(), "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));



    GLuint uniModel = glGetUniformLocation(cloudsOnSky.cloudbox->material.getShaderProgram(), "model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(cloudsOnSky.cloudbox->material.getModel()));



    GLuint uniEye = glGetUniformLocation(cloudsOnSky.cloudbox->material.getShaderProgram(), "eyepos");
    glUniform3f(uniEye, camera.cameraPos.x, camera.cameraPos.y, camera.cameraPos.z);

    GLuint uniPos = glGetUniformLocation(cloudsOnSky.cloudbox->material.getShaderProgram(), "pos");
    glUniform3f(uniPos, cloudsOnSky.cloudbox->GetPos().x, cloudsOnSky.cloudbox->GetPos().y, cloudsOnSky.cloudbox->GetPos().z);

    GLuint uniSize = glGetUniformLocation(cloudsOnSky.cloudbox->material.getShaderProgram(), "size");
    glUniform3f(uniSize, cloudsOnSky.cloudbox->GetSize().x, cloudsOnSky.cloudbox->GetSize().y, cloudsOnSky.cloudbox->GetSize().z);

    GLuint uniLight = glGetUniformLocation(cloudsOnSky.cloudbox->material.getShaderProgram(), "light");
    glUniform3f(uniLight, -cloudsOnSky.cloudbox->material.sun_rot->x, -cloudsOnSky.cloudbox->material.sun_rot->y, -cloudsOnSky.cloudbox->material.sun_rot->z);



    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glUseProgram(0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_3D);
 }

 void Sky::initTexture(){
     glGenTextures(1, &skyBoxTexture);
     glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexture);
     std::cout << "glerror " << glGetError() << std::endl;
     for (int i = 0; i < 6; i++) {
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, cubemapRes, cubemapRes, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

     }
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
 }

 Sky::Sky(Cloudbox* _cloudbox) {
     std::cout << "glerror " << glGetError() << std::endl;
     cloudsOnSky.cloudbox = _cloudbox;
     camera.cameraPos.x = cloudsOnSky.cloudbox->GetPos().x;
     camera.cameraPos.y = cloudsOnSky.cloudbox->GetPos().y - cloudsOnSky.cloudbox->GetSize().y / 2.f;
     camera.cameraPos.z = cloudsOnSky.cloudbox->GetPos().z;

     
     cloudsOnSky.initTexture(cubemapRes);
     skySphere.initTexture(cubemapRes);
     initTexture();
     skyBoxFrameBuffer = createFrameBuffer(cubemapRes, cubemapRes);
     camera.createMatrices();
     setRadius();
 }

 void Sky::setRadius() {
     /*centerPos = cloudsOnSky.cloudbox->GetPos();
     centerPos.y -= cloudsOnSky.cloudbox->GetSize().y / 2.f;
     outerRadius = cloudsOnSky.cloudbox->GetSize().y;
     innerRadius = 0.1*/

     centerPos = glm::vec3(0, -200, 0);
     innerRadius =210;
     outerRadius =250;

     /*outerRadius = std::min(cloudsOnSky.cloudbox->GetSize().x, cloudsOnSky.cloudbox->GetSize().z) / (2.f * sin(angle));

     centerPos = cloudsOnSky.cloudbox->GetPos();
     centerPos.y = cloudsOnSky.cloudbox->GetPos().y + cloudsOnSky.cloudbox->GetSize().y / 2 - outerRadius;

     innerRadius = cloudsOnSky.cloudbox->GetPos().y - cloudsOnSky.cloudbox->GetSize().y - centerPos.y + 0.2f;*/


 }

 void Sky::setCloudBoxPosition() {

 }

 void Sky::initFramebuffer() {
     glGenBuffers(1, &skyBoxFrameBuffer);
     glBindFramebuffer(GL_FRAMEBUFFER, 0);
     
 }

 void Sky::Render(int i) {
    // setRadius();

    glViewport(0, 0, cubemapRes, cubemapRes);
    glBindFramebuffer(GL_FRAMEBUFFER, skyBoxFrameBuffer);
     
    //for (int i = 0; i < 6; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skyBoxTexture, 0);
        camera.switchToFace(i);
        updateMatrices();
        //
        RenderSky();
        camera.cameraPos.y -= 0.1;
        updateMatrices();
        RenderCloud();
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        camera.cameraPos.y += 0.1;
    //}
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
 }

 GLuint Sky::GetTex() {
     return skyBoxTexture;
 }

 void Sky::SkySphere::initTexture(int res) {
     glGenTextures(1, &texture);
     glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

     for (int i = 0; i < 6; i++) {
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, res, res, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

     }
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
 }

 void Sky::attachMeshToSky(GeometricObject* sky_obj) {
     skySphere.sphereMesh = (Mesh*)sky_obj;
 }

 void Sky::SkySphere::attachShader(const std::string& fragment_shader_path) {
     vertexShader = loadShader(GL_VERTEX_SHADER, sphereMesh->material.GetVSPath().c_str());
     fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragment_shader_path.c_str());

 }

 void Sky::SkySphere::createShaderProgram() {
     shaderProgram = glCreateProgram();
     glAttachShader(shaderProgram, vertexShader);
     glAttachShader(shaderProgram, fragmentShader);

     glLinkProgram(shaderProgram);
 }

 void Sky::initSky(const std::string& path_to_fragment_shader, GeometricObject* sky_obj) {
     attachMeshToSky(sky_obj);
     skySphere.attachShader(path_to_fragment_shader);
     skySphere.createShaderProgram();
 }

 void Sky::RenderSky() {


     glUseProgram(skySphere.shaderProgram);
     glBindVertexArray(skySphere.sphereMesh->material.getVAO());

     skySphere.sphereMesh->material.setModel(glm::mat4(1.0));
/*
     GLuint unirad = glGetUniformLocation(skySphere.shaderProgram, "r");
     glUniform1f(unirad, innerRadius);
     unirad = glGetUniformLocation(skySphere.shaderProgram, "R");
     glUniform1f(unirad, outerRadius);

     GLuint uniCent = glGetUniformLocation(skySphere.shaderProgram, "center");
     glUniform3f(uniCent, ConvertSFML2GLM(centerPos).x, ConvertSFML2GLM(centerPos).y, ConvertSFML2GLM(centerPos).z);*/

     GLuint uniView = glGetUniformLocation(skySphere.shaderProgram, "view");
     glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));

     GLuint uniProj = glGetUniformLocation(skySphere.shaderProgram, "proj");
     glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));

     GLuint uniModel = glGetUniformLocation(skySphere.shaderProgram, "model");
     glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(skySphere.sphereMesh->material.getModel()));

     GLuint uniEye = glGetUniformLocation(skySphere.shaderProgram, "eyepos");
     glUniform3f(uniEye, 0, 0, 0);

     GLuint uniPos = glGetUniformLocation(skySphere.shaderProgram, "pos");
     glUniform3f(uniPos, 0, 0, 0);

     GLuint uniSize = glGetUniformLocation(skySphere.shaderProgram, "size");
     glUniform3f(uniSize, .04f, .04f, .04f);

     GLuint uniLight = glGetUniformLocation(skySphere.shaderProgram, "light");
     glUniform3f(uniLight, -skySphere.sphereMesh->material.sun_rot->x, -skySphere.sphereMesh->material.sun_rot->y, -skySphere.sphereMesh->material.sun_rot->z);

     //GLuint uniViewDir = glGetUniformLocation(skySphere.shaderProgram, "ViewDir");
     //glUniform3f(uniViewDir, )

     glDrawArrays(GL_TRIANGLES, 0, (*skySphere.sphereMesh).vert_vec3[0][skySphere.sphereMesh->array_index].size());
     glBindVertexArray(0);
     glUseProgram(0);
 }

 void Sky::Delete() {
     glDeleteTextures(1, &skySphere.texture);
     glDeleteShader(skySphere.vertexShader);
     glDeleteShader(skySphere.fragmentShader);
     glDeleteProgram(skySphere.shaderProgram);

     glDeleteTextures(1, &cloudsOnSky.cloudCubeMap);
     
     glDeleteTextures(1, &skyBoxTexture);
     glDeleteBuffers(1, &skyBoxFrameBuffer);
 }
 