#include "../Headers/GeometricObject.h"

Player& _pl = Player::Get();

#define M_PI 3.1415926535897932384626433832795
#define LOD 4

void printVec(sf::Vector3f v) {
    std::cout << v.x << '_' << v.y << '_' << v.z;
}

inline sf::Vector3f Normalize(sf::Vector3f v) {
    float length =sqrt( v.x * v.x + v.y * v.y + v.z * v.z);
    return v / length;
}

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
    material.sun_rot = (glm::vec3*)source->GetDirPtr();
    material.sun_dist = source->GetDistance();
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());
    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "shadowMap"), 1);
    glUseProgram(0);
    glBindVertexArray(0);
}



void GeometricObject::decrementIndex(){
    array_index--;
}

void GeometricObject::deleteArrays() {
    vertices.clear();
    
    std::vector<std::vector<glm::vec3>>::iterator _vert_iter = vert_vec3->begin() + array_index;
    std::vector<std::vector<glm::vec3>>::iterator _norm_iter = norm_vec3->begin() + array_index;
    std::vector<std::vector<glm::vec2>>::iterator _uv_iter = uv_vec2->begin() + array_index;

    vert_vec3->erase(_vert_iter);
    norm_vec3->erase(_norm_iter);
    uv_vec2->erase(_uv_iter);
    
}

void GeometricObject::SetName(std::string _name) {
    name = _name;
}

std::string GeometricObject::GetName(void) {
    return name;
}

std::string * GeometricObject::GetNamePtr(void) {
    return &name;
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

void GeometricObject::SetSize(sf::Vector3f s) {
    size = s;
}

sf::Vector3f GeometricObject::GetSize() {
    return size;
}

sf::Vector3f* GeometricObject::GetSizePtr() {
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

Cube::Cube(sf::Vector3f _size){
    size = _size;
    pos = sf::Vector3f(0, 0, 0);
    rot = sf::Vector3f(0, 0, 0);


    material = Material();
    
}

Cube::Cube(sf::Vector3f _pos, sf::Vector3f _rot, sf::Vector3f _size, GLuint* _texture){
    pos = _pos;
    rot = _rot;
    size = _size;
    material.bindTexture(*_texture);
}

Cube::~Cube() {
    vertices.clear();
    material.~Material();
}

Cube::Cube() {
    size = sf::Vector3f(1, 1, 1);
    pos = sf::Vector3f(0, 0, 0);
    rot = sf::Vector3f(0, 0, 0);


    material = *(new Material());
}



void Cube::CreateVerticesLegacy() {
    std::string path = ".\\Meshes\\cube.obj";
    setModel(path);

    //material.createVAO_VBO_mesh((*vert_vec3)[array_index], (*uv_vec2)[array_index], (*norm_vec3)[array_index]);
    material.createVAO_VBO(vertices);
}



void Cube::setModel(std::string & path) {
    model_path = path;
    OBJLoader_v(path, this);
}

void Cube::Draw(){
    UpdateModelMatrix();
    material.updateUniforms();
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());
    
    material.setModel(glm::mat4(1.0f));

      // glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_3D, material.getTexture()); 


    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eyepos");
    glUniform3f(uniEye, _pl.GetPos().x, _pl.GetPos().y, _pl.GetPos().z);

    GLuint uniPos = glGetUniformLocation(material.getShaderProgram(), "pos");
    glUniform3f(uniPos, pos.x, pos.y, pos.z);

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform3f(uniSize,size.x, size.y,  size.z);

    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, material.sun_rot->x, material.sun_rot->y, material.sun_rot->z);

    uniLight = glGetUniformLocation(material.getShaderProgram(), "lightDistance");
    glUniform1f(uniLight, material.sun_dist);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::Draw(sf::Vector3f cameraPos){
    UpdateModelMatrix();
    material.updateUniforms();
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());
    
    material.setModel(glm::mat4(1.0f));

      // glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_3D, material.getTexture()); 


    //GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eyepos");
    //glUniform3f(uniEye, _pl.GetPos().x, _pl.GetPos().y, _pl.GetPos().z);

    GLuint uniPos = glGetUniformLocation(material.getShaderProgram(), "pos");
    glUniform3f(uniPos, pos.x, pos.y, pos.z);

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform3f(uniSize,size.x, size.y,  size.z);

    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, material.sun_rot->x, material.sun_rot->y, material.sun_rot->z);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::Delete(){
    deleteArrays();
    

    material.Delete();
}

//  .d88b. .d88b 888b. 8888 8888 8888 8b  8 
//  YPwww. 8P    8  .8 8www 8www 8www 8Ybm8 
//      d8 8b    8wwK' 8    8    8    8  "8 
//  `Y88P' `Y88P 8  Yb 8888 8888 8888 8   8 
//   

void Screen::Delete(){
    deleteArrays();
    material.Delete();
    glDeleteBuffers(1, &frameBuffer);
    glDeleteBuffers(1, &depth_stencil_buff);
    glDeleteTextures(1, &tex);
}

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

    glm::mat4 mat = glm::translate(glm::mat4(1.0), *material.sun_rot);
        
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

Mesh::Mesh(std::string& path){
    pos = sf::Vector3f(0, 0, 0);
    rot = sf::Vector3f(0, 0, 0);
    OBJLoader(path, this);

    size = sf::Vector3f( 1, 1, 1);

    material = Material();
}

void Mesh::Delete(){
    deleteArrays();
    material.Delete();
    model_path.erase();
}

void Mesh::CreateVertices() {
    material.createVAO_VBO_mesh((*vert_vec3)[array_index], (*uv_vec2)[array_index],(* norm_vec3)[array_index]);
}

void Mesh::setModel(std::string& path) {
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
    glUniform3f(uniSize, size.x, size.y, size.z);

    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eye");
    glUniform3f(uniEye, _pl.GetPos().x, _pl.GetPos().y, _pl.GetPos().z);

    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, -material.sun_rot->x, -material.sun_rot->y, -material.sun_rot->z);
    
    uniLight = glGetUniformLocation(material.getShaderProgram(), "lightDistance");
    glUniform1f(uniLight, material.sun_dist);
    
    

    glActiveTexture(GL_TEXTURE1);
    unsigned int sm = *material.shadowmap;
    glBindTexture(GL_TEXTURE_2D, sm);

    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "skybox"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, material.getEnvironmentMap());

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

void Plane::Delete(){
    deleteArrays();
    material.Delete();
}

void Plane::Draw(){
    UpdateModelMatrix();
    material.updateUniforms();
    glUseProgram(material.getShaderProgram());
    glBindVertexArray(material.getVAO());
    material.setModel(glm::mat4(1.0f));

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform3f(uniSize, size.x, size.y, size.z);
    
    GLuint uniRepeat = glGetUniformLocation(material.getShaderProgram(), "repeat");
    glUniform1f(uniRepeat, texture_repeat);

    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eye");
    glUniform3f(uniEye, _pl.GetPos().x, _pl.GetPos().y, _pl.GetPos().z);
    
    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, -material.sun_rot->x, -material.sun_rot->y, -material.sun_rot->z);

    uniLight = glGetUniformLocation(material.getShaderProgram(), "lightDistance");
    glUniform1f(uniLight, material.sun_dist);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.getTexture());


    glActiveTexture(GL_TEXTURE1);
    unsigned int sm = *material.shadowmap;
    glBindTexture(GL_TEXTURE_2D, sm);

    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "skybox"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, material.getEnvironmentMap());


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

   // SetDir();
    updatePos();

    lightView = glm::lookAt(ConvertSFML2GLM(pos), ConvertSFML2GLM(sf::Vector3f(0, 0, 0)), glm::vec3(0.0f, 1.0f, 0.0f));

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
            glUniform3f(size_loc, obj_list[i]->GetSize().x, obj_list[i]->GetSize().y, obj_list[i]->GetSize().z);

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


 void LightSource::updatePos() {

     pos = - Normalize(dir) * distance;


 }

 void LightSource::SetDir() {
     
     glm::vec4 _dir = glm::vec4(1, 0, 0, 1);
     glm::mat4 rot_mat = glm::mat4(1);

     rot_mat = glm::rotate(rot_mat, glm::radians(rot.x), glm::vec3(1, 0, 0));
     rot_mat = glm::rotate(rot_mat, glm::radians(rot.y), glm::vec3(0, 1, 0));
     rot_mat = glm::rotate(rot_mat, glm::radians(rot.z), glm::vec3(0, 0, 1));

     _dir = rot_mat * _dir;

     dir = sf::Vector3f(_dir.x, _dir.y, _dir.z);
 }

 void LightSource::SetDir(sf::Vector3f vec) {
     dir = Normalize(vec);
 }


 void LightSource::Delete() {
     name.clear();
     vertexShader_source.clear();
     fragmentShader_source.clear();
     glDeleteBuffers(1, &depthMapFBO);
     glDeleteTextures(1, &depthMap);
     glDeleteProgram(ShaderProgram);
 }

 //     _____ _                 _ ____            
//    / ____| |               | |  _ \           
//   | |    | | ___  _   _  __| | |_) | _____  __
//   | |    | |/ _ \| | | |/ _` |  _ < / _ \ \/ /
//   | |____| | (_) | |_| | (_| | |_) | (_) >  < 
//    \_____|_|\___/ \__,_|\__,_|____/ \___/_/\_\
//                                               

 Cloudbox::Cloudbox(sf::Vector3f _pos, sf::Vector3f _res, sf::Vector3f _size) {
    pos = _pos;
    rot = sf::Vector3f(0, 0, 0);
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

 void Cloudbox::RenderCloud(float innerRadius, float outerRadius, sf::Vector3f center) {
     uniforms();
     glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);
     glEnable(GL_TEXTURE_3D);

     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_3D, cloudtex);

     material.attachUniform("r", innerRadius);
     material.attachUniform("R", outerRadius);
     material.attachUniform("center", ConvertSFML2GLM( center));
        

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
 

 //     _____ _  ____     __
 //    / ____| |/ /\ \   / /
 //   | (___ | ' /  \ \_/ / 
 //    \___ \|  <    \   /  
 //    ____) | . \    | |   
 //   |_____/|_|\_\   |_|   
 //                         
 //                        


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

     centerPos = sf::Vector3f(0, -200, 0);
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