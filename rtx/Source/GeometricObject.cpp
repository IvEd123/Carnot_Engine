#include "../Headers/GeometricObject.h"


Player& _pl = Player::Get();

#define M_PI 3.1415926535897932384626433832795
#define LOD 4

void getError(std::vector<GLchar>log, GLuint shader) {
    GLint maxLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	std::vector<GLchar> errorLog(maxLength);
	glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

    for (int i = 0; i < errorLog.size(); i++) {
        std::cout << errorLog[i];
    }
    std::cout << std::endl;
	// Provide the infolog in whatever manor you deem best.
	// Exit with failure.
	glDeleteShader(shader); // Don't leak the shader.
}

typedef struct {
    GLfloat x, y, z;
    GLfloat u, v;
}Vertex;



void GeometricObject::UpdateModelMatrix() {
    material.setModel(glm::translate(material.getModel(), ConvertSFML2GLM(pos)));
    material.setModel(glm::rotate(material.getModel(), glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0)));
    material.setModel(glm::rotate(material.getModel(), glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0)));
    material.setModel(glm::rotate(material.getModel(), glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0)));
}

void GeometricObject::addLightSource(LightSource* source) {
    material.lightSpaceMatrixPtr = source->getProjMatrix();
    material.shadowmap = source->getShadowMap();
    material.sun_pos = ConvertSFML2GLM(source->GetPos());
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());
    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "shadowMap"), 1);
    glUseProgram(0);
    glBindVertexArray(0);
}

GeometricObject::~GeometricObject() {
    //vertices.clear();
    //vert_vec3->clear();
    //uv_vec2->clear();
    //norm_vec3->clear();
}

void GeometricObject::SetName(std::string _name) {
    name = _name;
}

std::string GeometricObject::GetName(void) {
    return name;
}

sf::Vector3f GeometricObject::GetPos(){
   return pos;
}

sf::Vector3f* GeometricObject::GetPosPtr(){
   return &pos;
}

void GeometricObject::SetPos(sf::Vector3f _pos){
    pos = _pos;
}

sf::Vector3f GeometricObject::GetRot(){
    return rot;
}

sf::Vector3f* GeometricObject::GetRotPtr(){
    return &rot;
}

void GeometricObject::SetRot(sf::Vector3f _rot){
    rot = _rot;
}

void GeometricObject::SetSize(float s) {
    size = s / 2;
}

float GeometricObject::GetSize() {
    return size * 2;
}

float* GeometricObject::GetSizePtr() {
    return &size;
}

void GeometricObject::SetType(int t) {
    type = t;
}
int GeometricObject::GetType() {
    return type;
}


//  .d88b 8    8 888b. 8888 
//  8P    8    8 8wwwP 8www 
//  8b    8b..d8 8   b 8    
//  `Y88P `Y88P' 888P' 8888 
//                          

Cube::Cube(float _size){
    size = _size * 2;
    pos = sf::Vector3f(0, 0, 0);
    rot = sf::Vector3f(0, 0, 0);


    material = Material();
    
}

Cube::Cube(sf::Vector3f _pos, sf::Vector3f _rot, float _size, GLuint* _texture){
    pos = _pos;
    rot = _rot;
    size = _size;
}

Cube::~Cube() {
    vertices.clear();
    material.~Material();
}

Cube::Cube() {
    size = 1;
    pos = sf::Vector3f(0, 0, 0);
    rot = sf::Vector3f(0, 0, 0);


    material = *(new Material());
}

void Cube::CreateVertices() {
    (*vert_vec3)[array_index]= {
        glm::vec3(-0.5f, -0.5f, -0.5f),     //FRONT
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),

        glm::vec3(-0.5f, -0.5f,  0.5f),    //BACK
        glm::vec3(0.5f, -0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),

        glm::vec3(-0.5f,  0.5f,  0.5f),   //LEFT
        glm::vec3(-0.5f,  0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),

        glm::vec3(0.5f,  0.5f,  0.5f),  //RIGHT
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f,  0.5f),

        glm::vec3(-0.5f, -0.5f, -0.5f),  //DOWN
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f,  0.5f),
        glm::vec3(0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),

        glm::vec3(-0.5f,  0.5f, -0.5f),   //UP
        glm::vec3(0.5f,  0.5f, -0.5f),
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f, -0.5f)
    };
    (*uv_vec2)[array_index] = {
        glm::vec2(0.25, 0.33),   //FRONT
        glm::vec2(0.50, 0.33),
        glm::vec2(0.50, 0.66),
        glm::vec2(0.50, 0.66),
        glm::vec2(0.25, 0.66),
        glm::vec2(0.25, 0.33),

        glm::vec2(1.00, 0.33),   //BACK
        glm::vec2(0.75, 0.33),
        glm::vec2(0.75, 0.66),
        glm::vec2(0.75, 0.66),
        glm::vec2(1.00, 0.66),
        glm::vec2(1.00, 0.33),

        glm::vec2(1.00, 0.66),   //LEFT
        glm::vec2(0.25, 0.66),
        glm::vec2(0.25, 0.33),
        glm::vec2(0.25, 0.33),
        glm::vec2(1.00, 0.33),
        glm::vec2(1.00, 0.66),

        glm::vec2(0.75, 0.66),   //RIGHT
        glm::vec2(0.50, 0.66),
        glm::vec2(0.50, 0.33),
        glm::vec2(0.50, 0.33),
        glm::vec2(0.75, 0.33),
        glm::vec2(0.75, 0.66),

        glm::vec2(0.25, 0.33),   //DOWN
        glm::vec2(0.50, 0.33),
        glm::vec2(0.50, 0.00),
        glm::vec2(0.50, 0.00),
        glm::vec2(0.25, 0.00),
        glm::vec2(0.25, 0.33),

        glm::vec2(0.25, 0.66),   //UP
        glm::vec2(0.50, 0.66),
        glm::vec2(0.50, 1.00),
        glm::vec2(0.50, 1.00),
        glm::vec2(0.25, 1.00),
        glm::vec2(0.25, 0.66)
    };
    material.createVAO_VBO_mesh((*vert_vec3)[array_index], (*uv_vec2)[array_index], (*norm_vec3)[array_index]);
}

void Cube::Draw(){
    UpdateModelMatrix();
    material.updateUniforms();
    glUseProgram(material.getShaderProgram());
    glBindVertexArray(material.getVAO());
    material.setModel(glm::mat4(1.0f));

    //material.setModel(glm::mat4(1.0f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.getTexture());
    
    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform1f(uniSize, size);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.getTexture());

    //glTranslatef(pos.x, pos.y, pos.y);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //glTranslatef(-pos.x, -pos.y, -pos.y);
    glBindVertexArray(0);
}

//  .d88b. .d88b 888b. 8888 8888 8888 8b  8 
//  YPwww. 8P    8  .8 8www 8www 8www 8Ybm8 
//      d8 8b    8wwK' 8    8    8    8  "8 
//  `Y88P' `Y88P 8  Yb 8888 8888 8888 8   8 
//   

Screen::Screen(){
    material = Material();
    CreateVertices();
    material.createVAO_VBO(vertices);
}

Screen::~Screen() {
    vertices.clear();
    material.~Material();
}

GLuint* Screen::getDepthSteencilBuffer(){
    return &depth_stencil_buff;
}

GLuint* Screen::getColorBuffer(){
    return &tex;
}

void Screen::CreateVertices(){
    vertices =  {
        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

         1.0f, -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        -1.0f,  1.0f,  0.0f, 1.0f
    };

}

void Screen::Draw() {

    glDisable(GL_DEPTH_TEST);

    glUseProgram(material.getShaderProgram());
    glBindVertexArray(material.getVAO());

    glm::mat4 mat = glm::translate(glm::mat4(1.0), material.sun_pos);
        
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
}


//    _____ _____ ____  ____      _    ___ _   _ 
//   |_   _| ____|  _ \|  _ \    / \  |_ _| \ | |
//     | | |  _| | |_) | |_) |  / _ \  | ||  \| |
//     | | | |___|  _ <|  _ <  / ___ \ | || |\  |
//     |_| |_____|_| \_\_| \_\/_/   \_\___|_| \_|
//                                  

Terrain::Terrain(float _size, float _res, float _height){
    material = Material();

    size = _size;
    resolution = _res;
    height = _height;
    pos = sf::Vector3f(0, 0, 0);
    rot = sf::Vector3f(0, 0, 0);

}

Terrain::Terrain() {

}

Terrain::~Terrain(){
    material = Material();
    pos = sf::Vector3f(0, 0, 0);
    rot = sf::Vector3f(0, 0, 0);
    size = 1;

    CreateVertices();
    material.createVAO_VBO(vertices);
}

void Terrain::Draw(){
    UpdateModelMatrix();
    material.updateUniforms();
    glUseProgram(material.getShaderProgram());
    glBindVertexArray(material.getVAO());
    material.setModel(glm::mat4(1.0f));

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform1f(uniSize, size);
    
    GLuint uniHeight = glGetUniformLocation(material.getShaderProgram(), "height");
    glUniform1f(uniHeight, height);
    
    GLuint uniRes = glGetUniformLocation(material.getShaderProgram(), "res");
    glUniform1f(uniRes, (float)resolution+1);

    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eye");
    glUniform3f(uniEye, _pl.GetPos().x, _pl.GetPos().y, _pl.GetPos().z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.getTexture());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, heightmap);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, ao);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, norm);

    glTranslatef(pos.x, pos.y, pos.y);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()/4);
    glTranslatef(-pos.x, -pos.y, -pos.y);
    glBindVertexArray(0);
}

void Terrain::CreateVertices(){
    float step = size / (float)resolution;
    for (int j = 0; j < resolution; j++) {
        for (int i = 0; i < resolution; i++) {
            std::vector<sf::Vector2f> vert(6);
            std::vector<sf::Vector2f> uv(6);
            vert[0] = sf::Vector2f( i,   j   );
            vert[1] = sf::Vector2f( i+1, j   );
            vert[2] = sf::Vector2f( i,   j+1 );
            vert[3] = sf::Vector2f( i,   j+1 );
            vert[4] = sf::Vector2f( i+1, j+1 );
            vert[5] = sf::Vector2f( i+1, j   );

            uv[0] = sf::Vector2f(0, 1);
            uv[1] = sf::Vector2f(1, 1);
            uv[2] = sf::Vector2f(0, 0);
            uv[3] = sf::Vector2f(0, 0);
            uv[4] = sf::Vector2f(1, 0);
            uv[5] = sf::Vector2f(1, 1);

            for (int k = 0; k < 6; k++) {
                vertices.push_back(vert[k].x*step);
                vertices.push_back(vert[k].y*step);

                vertices.push_back(uv[k].x);
                vertices.push_back(uv[k].y);
            }
        }
    }
    material.createVAO_VBO_mesh((*vert_vec3)[array_index], (*uv_vec2)[array_index], (*norm_vec3)[array_index]);
}

//    __  __ ______  _____ _    _ 
//   |  \/  |  ____|/ ____| |  | |
//   | \  / | |__  | (___ | |__| |
//   | |\/| |  __|  \___ \|  __  |
//   | |  | | |____ ____) | |  | |
//   |_|  |_|______|_____/|_|  |_|
//                                
//    

Mesh::Mesh( char* path){
    pos = sf::Vector3f(0, 0, 0);
    rot = sf::Vector3f(0, 0, 0);
    OBJLoader(path, this);

    size = 1.0;

    material = Material();
}

void Mesh::CreateVertices() {
    material.createVAO_VBO_mesh((*vert_vec3)[array_index], (*uv_vec2)[array_index],(* norm_vec3)[array_index]);
}

void Mesh::setModel( char* path) {
    model_path = path;
    OBJLoader(path, this);
    CreateVertices();
}

Mesh::~Mesh(){
}

Mesh::Mesh(){
    pos = sf::Vector3f(0, 0, 0);
    rot = sf::Vector3f(0, 0, 0);
    material = *(new Material());
}

void Mesh::Draw() {
    UpdateModelMatrix();
    material.updateUniforms();
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());

    material.setModel(glm::mat4(1.0));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.getTexture());

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform1f(uniSize, size);

    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, material.sun_pos.x, material.sun_pos.y, material.sun_pos.z);
        

    glActiveTexture(GL_TEXTURE1);
    unsigned int sm = *material.shadowmap;
    glBindTexture(GL_TEXTURE_2D, sm);

    glDrawArrays(GL_TRIANGLES, 0,  (*vert_vec3)[array_index].size());

    

    glUseProgram(0);
    glBindVertexArray(0);
}

//    _____  _               _   _ ______ 
//   |  __ \| |        /\   | \ | |  ____|
//   | |__) | |       /  \  |  \| | |__   
//   |  ___/| |      / /\ \ | . ` |  __|  
//   | |    | |____ / ____ \| |\  | |____ 
//   |_|    |______/_/    \_\_| \_|______|
//                                        
//                                        

Plane::Plane(){
    pos = sf::Vector3f(0, 0, 0);
    rot = sf::Vector3f(0, 0, 0);

    material = Material();
}

Plane::~Plane(){
    vertices.clear();
    material.~Material();
}

void Plane::Draw(){
    UpdateModelMatrix();
    material.updateUniforms();
    glUseProgram(material.getShaderProgram());
    glBindVertexArray(material.getVAO());
    material.setModel(glm::mat4(1.0f));

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform1f(uniSize, size);
    
    GLuint uniRepeat = glGetUniformLocation(material.getShaderProgram(), "repeat");
    glUniform1f(uniRepeat, texture_repeat);

    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eye");
    glUniform3f(uniEye, _pl.GetPos().x, _pl.GetPos().y, _pl.GetPos().z);
    
    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, material.sun_pos.x, material.sun_pos.y, material.sun_pos.z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.getTexture());


    glActiveTexture(GL_TEXTURE1);
    unsigned int sm = *material.shadowmap;
    glBindTexture(GL_TEXTURE_2D, sm);


    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Plane::CreateVertices(){

    (*vert_vec3)[array_index] = {
         glm::vec3(0.0, 0.0, 0.0),
         glm::vec3(1.0, 0.0, 0.0),
         glm::vec3(1.0, 0.0, 1.0),

         glm::vec3(1.0, 0.0, 1.0),
         glm::vec3(0.0, 0.0, 1.0),
         glm::vec3(0.0, 0.0, 0.0),
    };
    (*uv_vec2)[array_index] = {
         glm::vec2( 0.0, 0.0),
         glm::vec2(1.0, 0.0),
         glm::vec2(1.0, 1.0),

         glm::vec2(1.0, 1.0),
         glm::vec2(0.0, 1.0),
         glm::vec2(0.0, 0.0)
    };
    (*norm_vec3)[array_index] = {
         glm::vec3(0.0f, 1.0f,  0.0f),
         glm::vec3(0.0f, 1.0f,  0.0f),
         glm::vec3(0.0f, 1.0f,  0.0f),
 
         glm::vec3(0.0f, 1.0f,  0.0f),
         glm::vec3(0.0f, 1.0f,  0.0f),
         glm::vec3(0.0f, 1.0f,  0.0f),
    };

    material.createVAO_VBO_mesh((*vert_vec3)[array_index], (*uv_vec2)[array_index], (*norm_vec3)[array_index]);

}




//    _      _____ _____ _    _ _______ 
//   | |    |_   _/ ____| |  | |__   __|
//   | |      | || |  __| |__| |  | |   
//   | |      | || | |_ |  __  |  | |   
//   | |____ _| || |__| | |  | |  | |   
//   |______|_____\_____|_|  |_|  |_|   
//                                      
//    


LightSource::LightSource() {
    resolution = 2048;
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    pov = sf::Vector3f(0, 0, 0);

    near_plane = 1.0f, far_plane = 50.0f;
    lightProjection = glm::ortho(-fov, fov, -fov, fov, near_plane, far_plane);

   

    
}

void LightSource::setShader(GLenum type, const GLchar* path){
    switch (type){
    case GL_VERTEX_SHADER:
        vertexShader_source = std::string(path);
        break;
    case GL_FRAGMENT_SHADER:
        fragmentShader_source = std::string(path);
        break;
    default:
        break;
    }
}

int LightSource::CreateShaders(){
    return CreateShaderProgram();
}

void LightSource::Draw(std::vector <GeometricObject*> obj_list) {

    glEnable(GL_DEPTH_TEST);
    glUseProgram(ShaderProgram);
    
 
    glViewport(0, 0, resolution, resolution);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    lightView = glm::lookAt(ConvertSFML2GLM(pos), ConvertSFML2GLM(pov), glm::vec3(0.0f, 1.0f, 0.0f));

    lightSpaceMatrix = lightProjection * lightView;

    for (int i = 0; i < obj_list.size(); i++) {
       
        if (obj_list[i]->cast_shadow) {

            sf::Vector3f pos = obj_list[i]->GetPos();
            sf::Vector3f rot = obj_list[i]->GetRot();

            glm::mat4 model = glm::mat4(1.0);
            model = glm::translate(model, ConvertSFML2GLM(pos));
            
            model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
            model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
            model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));

            GLuint lightSpaceMatrixLocation = glGetUniformLocation(ShaderProgram, "mvpMatrix");
            glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

            GLuint model_loc = glGetUniformLocation(ShaderProgram, "model");
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

            GLuint size_loc = glGetUniformLocation(ShaderProgram, "size");
            glUniform1f(size_loc, obj_list[i]->GetSize());

            glBindVertexArray(obj_list[i]->material.getVAO());

            int size = (*(*obj_list[i]).vert_vec3)[i].size();
            glDrawArrays(GL_TRIANGLES, 0, size);
            glBindVertexArray(0);
        }
    }

}

unsigned int *LightSource::getShadowMap() {
    return &depthMap;
}

int LightSource::CreateShaderProgram() {

    const char* vs_str = vertexShader_source.c_str();
    const char* fs_str = fragmentShader_source.c_str();

    int vertexShader = loadShader(GL_VERTEX_SHADER, vs_str);
    if (vertexShader == -1)
        return -1;
    
    int fragmentShader = loadShader(GL_FRAGMENT_SHADER, fs_str);
    if (fragmentShader == -1)
        return -1;

    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, vertexShader);
    glAttachShader(ShaderProgram, fragmentShader);

    glLinkProgram(ShaderProgram);
}

 int LightSource::loadShader(GLenum type, const GLchar* path) {

    std::ifstream file;
    file.open(path);

    if (!file.is_open()) {
        std::cout << "error!!" << std::endl;
        return -1;
    }

    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* shaderSource = (char*)calloc(file_size, 1);

    file.read(shaderSource, file_size);
    std::vector<GLchar> log;

    unsigned int shader;
    
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    GLint status;
        
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        if (type == GL_VERTEX_SHADER)
            std::cout << "vert ";
        else if(type == GL_FRAGMENT_SHADER)
            std::cout << "frag ";
        getError(log, shader);
    }


    return shader;
}