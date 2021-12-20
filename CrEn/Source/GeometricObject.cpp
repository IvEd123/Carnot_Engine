#include "../Headers/GeometricObject.h"

Player& player_entity = Player::Get();

#define M_PI 3.1415926535897932384626433832795
#define LOD 4

inline void printVec(sf::Vector3f v) {
    std::cout << v.x << '_' << v.y << '_' << v.z;
}

inline sf::Vector3f Normalize(sf::Vector3f v) {
    float length = sqrt( v.x * v.x + v.y * v.y + v.z * v.z);
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
    material.setModel(glm::translate(material.getModel(), ConvertSFML2GLM(m_position)));
    material.setModel(glm::rotate(material.getModel(), glm::radians(m_rotation.x), glm::vec3(1.0, 0.0, 0.0)));
    material.setModel(glm::rotate(material.getModel(), glm::radians(m_rotation.y), glm::vec3(0.0, 1.0, 0.0)));
    material.setModel(glm::rotate(material.getModel(), glm::radians(m_rotation.z), glm::vec3(0.0, 0.0, 1.0)));
}

void GeometricObject::AddLightSource(LightSource* source) {
    material.lightSpaceMatrixPtr = source->GetProjMatrix();
    material.shadowmap = source->GetShadowMap();
    material.sun_rot = (glm::vec3*)source->GetDirPtr();
    material.sun_dist = source->GetDistance();

    GLint previous_vao, previous_program;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vao);
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);

    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());
    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "shadowMap"), 3);
    glUseProgram(previous_program);
    glBindVertexArray(previous_vao);
}



void GeometricObject::DecrementIndex(){
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
    m_name = _name;
}

std::string GeometricObject::GetName() {
    return m_name;
}

std::string * GeometricObject::GetNamePtr() {
    return &m_name;
}

sf::Vector3f GeometricObject::GetPos(){
   return m_position;
}

sf::Vector3f* GeometricObject::GetPosPtr(){
   return &m_position;
}

void GeometricObject::SetPos(sf::Vector3f _pos){
    m_position = _pos;
}

sf::Vector3f GeometricObject::GetRot(){
    return m_rotation;
}

sf::Vector3f* GeometricObject::GetRotPtr(){
    return &m_rotation;
}

void GeometricObject::SetRot(sf::Vector3f _rot){
    m_rotation = _rot;
}

void GeometricObject::SetSize(sf::Vector3f s) {
    m_size = s;
}

sf::Vector3f GeometricObject::GetSize() {
    return m_size;
}

sf::Vector3f* GeometricObject::GetSizePtr() {
    return &m_size;
}

void GeometricObject::SetType(int t) {
    m_type = t;
}
int GeometricObject::GetType() {
    return m_type;
}


//  .d88b 8    8 888b. 8888 
//  8P    8    8 8wwwP 8www 
//  8b    8b..d8 8   b 8    
//  `Y88P `Y88P' 888P' 8888 
//                          

Cube::Cube(sf::Vector3f _size){
    m_size = _size;
    m_position = sf::Vector3f(0, 0, 0);
    m_rotation = sf::Vector3f(0, 0, 0);
    material = Material();
}

Cube::Cube(sf::Vector3f _pos, sf::Vector3f _rot, sf::Vector3f _size, GLuint _texture){
    m_position = _pos;
    m_rotation = _rot;
    m_size = _size;
    material.bindTexture(_texture);
}

Cube::~Cube() {
    vertices.clear();
    material.~Material();
    deleteArrays();
}

Cube::Cube() {
    m_size = sf::Vector3f(1, 1, 1);
    m_position = sf::Vector3f(0, 0, 0);
    m_rotation = sf::Vector3f(0, 0, 0);

    material = Material();
}

void Cube::CreateVerticesLegacy() {
    std::string path = ".\\Meshes\\cube.obj";
    SetModel(path);
    material.createVAO_VBO(vertices);
}

void Cube::SetModel(std::string & path) {
    m_model_path = path;
    OBJLoader_v(path, this);
}

void Cube::Draw(){
    GLint previous_vao, previous_program;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vao);
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);

    UpdateModelMatrix();
    material.updateUniforms();
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());
    
    material.setModel(glm::mat4(1.0f));

    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eyepos");
    glUniform3f(uniEye, player_entity.GetPos().x, player_entity.GetPos().y, player_entity.GetPos().z);

    GLuint uniPos = glGetUniformLocation(material.getShaderProgram(), "pos");
    glUniform3f(uniPos, m_position.x, m_position.y, m_position.z);

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform3f(uniSize,m_size.x, m_size.y,  m_size.z);

    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, material.sun_rot->x, material.sun_rot->y, material.sun_rot->z);

    uniLight = glGetUniformLocation(material.getShaderProgram(), "lightDistance");
    glUniform1f(uniLight, material.sun_dist);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(previous_vao);
    glUseProgram(previous_program);
}

void Cube::Draw(sf::Vector3f cameraPos){
    //used for drawing with specific camera position

    GLint previous_vao, previous_program;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vao);
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);

    UpdateModelMatrix();
    material.updateUniforms();
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());
    
    material.setModel(glm::mat4(1.0f));

    GLuint uniPos = glGetUniformLocation(material.getShaderProgram(), "pos");
    glUniform3f(uniPos, m_position.x, m_position.y, m_position.z);

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform3f(uniSize,m_size.x, m_size.y,  m_size.z);

    GLuint uniLight = glGetUniformLocation(material.getShaderProgram(), "light");
    glUniform3f(uniLight, material.sun_rot->x, material.sun_rot->y, material.sun_rot->z);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(previous_vao);
    glUseProgram(previous_program);
}


//  .d88b. .d88b 888b. 8888 8888 8888 8b  8 
//  YPwww. 8P    8  .8 8www 8www 8www 8Ybm8 
//      d8 8b    8wwK' 8    8    8    8  "8 
//  `Y88P' `Y88P 8  Yb 8888 8888 8888 8   8 
//   

void Screen::CreateFrameBuffer(int width, int height){
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glGenTextures(1, &m_color_texture);
    glBindTexture(GL_TEXTURE_2D, m_color_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_color_texture, 0);
    
    
    glGenTextures(1, &m_albedo_texture);
    glBindTexture(GL_TEXTURE_2D, m_albedo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_albedo_texture, 0);
    
    glGenTextures(1, &m_norm_texture);
    glBindTexture(GL_TEXTURE_2D, m_norm_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_norm_texture, 0);

    glGenTextures(1, &m_depth_texture);
    glBindTexture(GL_TEXTURE_2D, m_depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);



    glGenTextures(1, &m_pos_texture);
    glBindTexture(GL_TEXTURE_2D, m_pos_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_pos_texture, 0);



    const GLenum buffers[]{ GL_COLOR_ATTACHMENT0, // albedo
                            GL_COLOR_ATTACHMENT1, // normal
                            GL_COLOR_ATTACHMENT2, // position
                            GL_COLOR_ATTACHMENT3  // color with shading (remove later)
    };
    glDrawBuffers(4, buffers);
    

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "framebuffer isn't complete!!!!!!!!\n";

}

Screen::Screen(){
    material = Material();
    CreateVertices();
    material.createVAO_VBO(vertices);
}

Screen::~Screen() {
    vertices.clear();
    material.~Material();
    deleteArrays();
    glDeleteBuffers(1, &frameBuffer);
    glDeleteTextures(1, &m_color_texture);
}

GLuint* Screen::GetColorBuffer(){
    return &m_color_texture;
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
    GLint previous_vao, previous_program;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vao);
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);
    GLboolean previous_depth_test;
    glGetBooleanv(GL_DEPTH_TEST, &previous_depth_test);


    
    //material.attachUniform("albedo", albedo_texture);
    //material.attachUniform("position", pos_texture);
    //material.attachUniform("normal", norm_texture);

    glDisable(GL_DEPTH_TEST);

    glUseProgram(material.getShaderProgram());
    glBindVertexArray(material.getVAO());

    if (material.lightSpaceMatrixPtr != nullptr) {
        //GLuint lightSpaceMatrix = glGetUniformLocation(material.getShaderProgram(), "lightSpaceMatrix");
        //glUniformMatrix4fv(lightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(*material.lightSpaceMatrixPtr));
    }
    
    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "albedo"), 0);
    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "normal"), 2);
    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "position"), 1);
    glUniform1i(glGetUniformLocation(material.getShaderProgram(), "depthTex"), 4);

   // glm::mat4 mat = glm::translate(glm::mat4(1.0), *material.sun_rot);
        
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_albedo_texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_pos_texture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_norm_texture);
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, *material.shadowmap);
    
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_depth_texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(previous_vao);
    glUseProgram(previous_program);
    if (previous_depth_test)
        glEnable(GL_DEPTH_TEST);
}


//    _____ _____ ____  ____      _    ___ _   _ 
//   |_   _| ____|  _ \|  _ \    / \  |_ _| \ | |
//     | | |  _| | |_) | |_) |  / _ \  | ||  \| |
//     | | | |___|  _ <|  _ <  / ___ \ | || |\  |
//     |_| |_____|_| \_\_| \_\/_/   \_\___|_| \_|
//                                  

Terrain::Terrain(float _size, float _res, float _height){
    material = Material();

    m_size = _size;
    m_resolution = _res;
    height = _height;
    m_position = sf::Vector3f(0, 0, 0);
    m_rotation = sf::Vector3f(0, 0, 0);

}

Terrain::Terrain() {
    m_ambient_occlusion_texture = 0;
    height = 0;
    m_height_texture = 0;
    m_normal_texture = 0;
    m_resolution = 0;
    m_size = 0;
}

Terrain::~Terrain(){
    material = Material();
    m_position = sf::Vector3f(0, 0, 0);
    m_rotation = sf::Vector3f(0, 0, 0);
    m_size = 1;

    CreateVertices();
    material.createVAO_VBO(vertices);
}

void Terrain::Draw(){
    GLint previous_vao, previous_program;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vao);
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);
    UpdateModelMatrix();
    material.updateUniforms();
    glUseProgram(material.getShaderProgram());
    glBindVertexArray(material.getVAO());
    material.setModel(glm::mat4(1.0f));

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform1f(uniSize, m_size);
    
    GLuint uniHeight = glGetUniformLocation(material.getShaderProgram(), "height");
    glUniform1f(uniHeight, height);
    
    GLuint uniRes = glGetUniformLocation(material.getShaderProgram(), "res");
    glUniform1f(uniRes, (float)m_resolution+1);

    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eye");
    glUniform3f(uniEye, player_entity.GetPos().x, player_entity.GetPos().y, player_entity.GetPos().z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.getTexture());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_height_texture);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_ambient_occlusion_texture);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_normal_texture);

    glTranslatef(m_position.x, m_position.y, m_position.y);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size()/4);
    glTranslatef(-m_position.x, -m_position.y, -m_position.y);
    glBindVertexArray(previous_vao);
    glUseProgram(previous_program);
}

void Terrain::CreateVertices(){
    float step = m_size / (float)m_resolution;
    for (int j = 0; j < m_resolution; j++) {
        for (int i = 0; i < m_resolution; i++) {
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
    m_position = sf::Vector3f(0, 0, 0);
    m_rotation = sf::Vector3f(0, 0, 0);
    OBJLoader(path, this);

    m_size = sf::Vector3f( 1, 1, 1);

    material = Material();
}

void Mesh::CreateVertices() {
    material.createVAO_VBO_mesh((*vert_vec3)[array_index], (*uv_vec2)[array_index],(* norm_vec3)[array_index]);
}

void Mesh::SetModel(std::string& path) {
    model_path = path;
    OBJLoader(path, this);
    CreateVertices();
}

Mesh::~Mesh(){
    deleteArrays();
    material.~Material();
    model_path.erase();
}

Mesh::Mesh(){
    m_position = sf::Vector3f(0, 0, 0);
    m_rotation = sf::Vector3f(0, 0, 0);
    material = *(new Material());
}

void Mesh::Draw() {
    GLint previous_vao, previous_program;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vao);
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);

    UpdateModelMatrix();
    material.updateUniforms();
    glBindVertexArray(material.getVAO());
    glUseProgram(material.getShaderProgram());

    //const GLenum buffers[]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    //glDrawBuffers(2, buffers);

    material.setModel(glm::mat4(1.0));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, material.getTexture());

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform3f(uniSize, m_size.x, m_size.y, m_size.z);

    GLuint uniRepeat = glGetUniformLocation(material.getShaderProgram(), "texture_repeat");
    glUniform1f(uniRepeat, m_texture_repeat);

    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eye");
    glUniform3f(uniEye, player_entity.GetPos().x, player_entity.GetPos().y, player_entity.GetPos().z);

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

    //const GLenum buffers[]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    //glDrawBuffers(2, buffers);

    

    glDrawArrays(GL_TRIANGLES, 0,  (*vert_vec3)[array_index].size());

    glUseProgram(previous_program);
    glBindVertexArray(previous_vao);
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
    m_position = sf::Vector3f(0, 0, 0);
    m_rotation = sf::Vector3f(0, 0, 0);

    material = Material();
}

Plane::~Plane(){
    material.~Material();
    deleteArrays();
}

void Plane::Draw(){
    GLint previous_vao, previous_program;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vao);
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);

    UpdateModelMatrix();
    material.updateUniforms();
    glUseProgram(material.getShaderProgram());
    glBindVertexArray(material.getVAO());
    material.setModel(glm::mat4(1.0f));

    GLuint uniSize = glGetUniformLocation(material.getShaderProgram(), "size");
    glUniform3f(uniSize, m_size.x, m_size.y, m_size.z);
    
    GLuint uniRepeat = glGetUniformLocation(material.getShaderProgram(), "texture_repeat");
    glUniform1f(uniRepeat, m_texture_repeat);

    GLuint uniEye = glGetUniformLocation(material.getShaderProgram(), "eye");
    glUniform3f(uniEye, player_entity.GetPos().x, player_entity.GetPos().y, player_entity.GetPos().z);
    
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
    glBindVertexArray(previous_vao);
    glUseProgram(previous_program);
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
    
    m_shadowmap_resolution = 2048;//default resolution

    GLint previous_buffer;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_buffer);

    //creating framebuffer
    glGenFramebuffers(1, &m_depthmap_buffer);
    glGenTextures(1, &m_depthmap);
    glBindTexture(GL_TEXTURE_2D, m_depthmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowmap_resolution, m_shadowmap_resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, m_depthmap_buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthmap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, previous_buffer);

    m_near_plane = 1.0f, m_far_plane = 50.0f;
    m_light_projection_matrix = glm::ortho(-m_field_of_view, m_field_of_view, -m_field_of_view, m_field_of_view, m_near_plane, m_far_plane);

}

void LightSource::SetShader(GLenum type, const GLchar* path){
    switch (type){
    case GL_VERTEX_SHADER:
        m_vertex_shader_source = std::string(path);
        break;
    case GL_FRAGMENT_SHADER:
        m_fragment_shader_source = std::string(path);
        break;
    default:
        throw std::exception("Unknown shader type");
        break;
    }
}

int LightSource::CreateShaders(){
    return CreateShaderProgram();
}

void LightSource::Draw(std::vector <GeometricObject*> obj_list) {
    GLint previous_vao, previous_program;
    GLint previous_buffer;
    GLboolean previous_depth_test;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vao);
    glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_buffer);
    glGetBooleanv(GL_DEPTH_TEST, &previous_depth_test);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(m_shader_program);
    
 
    glViewport(0, 0, m_shadowmap_resolution, m_shadowmap_resolution);
    glBindFramebuffer(GL_FRAMEBUFFER, m_depthmap_buffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    updatePos();

    m_light_view_matrix = glm::lookAt(ConvertSFML2GLM(m_position), ConvertSFML2GLM(sf::Vector3f(0, 0, 0)), glm::vec3(0.0f, 1.0f, 0.0f));

    m_light_space_matrix = m_light_projection_matrix * m_light_view_matrix;

    for (int i = 0; i < obj_list.size(); i++) {
       
        if (obj_list[i]->cast_shadow) {

            sf::Vector3f pos = obj_list[i]->GetPos();
            sf::Vector3f rot = obj_list[i]->GetRot();

            glm::mat4 model = glm::mat4(1.0);
            model = glm::translate(model, ConvertSFML2GLM(pos));
            
            model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0, 0.0, 0.0));
            model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0, 1.0, 0.0));
            model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0, 0.0, 1.0));

            GLuint lightSpaceMatrixLocation = glGetUniformLocation(m_shader_program, "mvpMatrix");
            glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_light_space_matrix));

            GLuint model_loc = glGetUniformLocation(m_shader_program, "model");
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

            GLuint size_loc = glGetUniformLocation(m_shader_program, "size");
            glUniform3f(size_loc, obj_list[i]->GetSize().x, obj_list[i]->GetSize().y, obj_list[i]->GetSize().z);

            glBindVertexArray(obj_list[i]->material.getVAO());

            int size = (*(*obj_list[i]).vert_vec3)[i].size();
            glDrawArrays(GL_TRIANGLES, 0, size);
            glBindVertexArray(0);
        }
    }
    glBindVertexArray(previous_vao);
    glUseProgram(previous_program);
    glBindBuffer(GL_FRAMEBUFFER, previous_buffer);
    if (previous_depth_test)
        glEnable(GL_DEPTH_TEST);

}

inline unsigned int *LightSource::GetShadowMap() {
    return &m_depthmap;
}

int LightSource::CreateShaderProgram() {

    const char* vs_str = m_vertex_shader_source.c_str();
    const char* fs_str = m_fragment_shader_source.c_str();

    int vertexShader = loadShader(GL_VERTEX_SHADER, vs_str);
    if (vertexShader == -1)
        throw std::exception("Vertex shader loading error");
    
    int fragmentShader = loadShader(GL_FRAGMENT_SHADER, fs_str);
    if (fragmentShader == -1)
        throw std::exception("Fragment shader loading error");

    m_shader_program = glCreateProgram();
    glAttachShader(m_shader_program, vertexShader);
    glAttachShader(m_shader_program, fragmentShader);

    glLinkProgram(m_shader_program);
}

 int LightSource::loadShader(GLenum type, const GLchar* path) {

    std::ifstream file;
    file.open(path);

    if (!file.is_open()) 
        throw std::exception("Light source shader file loading error");
    

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
        throw std::exception("Shader compilation error");
    }
    free(shaderSource);
    file.close();
    return shader;
}


 void LightSource::updatePos() {
     m_position = - Normalize(m_direction) * m_draw_distance;
 }

 void LightSource::SetDir() {
     
     glm::vec4 _dir = glm::vec4(1, 0, 0, 1);
     glm::mat4 rot_mat = glm::mat4(1);

     rot_mat = glm::rotate(rot_mat, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
     rot_mat = glm::rotate(rot_mat, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
     rot_mat = glm::rotate(rot_mat, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));

     _dir = rot_mat * _dir;

     m_direction = sf::Vector3f(_dir.x, _dir.y, _dir.z);
 }

 void LightSource::SetDir(sf::Vector3f vec) {
     m_direction = Normalize(vec);
 }


 LightSource::~LightSource() {
     m_name.clear();
     m_vertex_shader_source.clear();
     m_fragment_shader_source.clear();
     glDeleteBuffers(1, &m_depthmap_buffer);
     glDeleteTextures(1, &m_depthmap);
     glDeleteProgram(m_shader_program);
 }

 //     _____ _                 _ ____            
//    / ____| |               | |  _ \           
//   | |    | | ___  _   _  __| | |_) | _____  __
//   | |    | |/ _ \| | | |/ _` |  _ < / _ \ \/ /
//   | |____| | (_) | |_| | (_| | |_) | (_) >  < 
//    \_____|_|\___/ \__,_|\__,_|____/ \___/_/\_\
//                                               

 Cloudbox::Cloudbox(sf::Vector3f _pos, sf::Vector3f _res, sf::Vector3f _size) {
    m_position = _pos;
    m_rotation = sf::Vector3f(0, 0, 0);
    m_cloud_texure_resolution = _res;
    m_size = _size;

    CreateVerticesLegacy();
    initBuffer();
    initTexture();
    attachBuffer();
 }

 Cloudbox::~Cloudbox() {
     deleteArrays();
     material.~Material();
     glDeleteTextures(1, &m_cloud_texture);
     glDeleteBuffers(1, &m_cloudbuffer);
 }

 void Cloudbox::RenderCloud(float innerRadius, float outerRadius, sf::Vector3f center) {
     AttachUniforms();
     glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);
     glEnable(GL_TEXTURE_3D);

     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_3D, m_cloud_texture);

     material.attachUniform("r", innerRadius);
     material.attachUniform("R", outerRadius);
     material.attachUniform("center", ConvertSFML2GLM( center));
        

     Draw();

     glDisable(GL_CULL_FACE);
     glDisable(GL_TEXTURE_3D);
 }

 void Cloudbox::RenderTexture(int SCREEN_WIDTH, int SCREEN_HEIGHT) {
     GLint previous_vao, previous_program;
     GLint previous_buffer;
     glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vao);
     glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);
     glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_buffer);
     
     glViewport(0, 0, m_cloud_texure_resolution.x, m_cloud_texure_resolution.y);
     glUseProgram(material.getShaderProgram());
     glBindVertexArray(material.getVAO());
     
     int grid_res_loc = glGetUniformLocation(material.getShaderProgram(), "grid_resolution");
     glUniform1i(grid_res_loc, m_points_in_grid);
     
     int layer_loc = glGetUniformLocation(material.getShaderProgram(), "layers");
     glUniform1i(layer_loc, m_cloud_texure_resolution.z);
     
     for (int i = 0; i < m_cloud_texure_resolution.z; ++i) {
         int layer_loc = glGetUniformLocation(material.getShaderProgram(), "layer");
         glUniform1f(layer_loc, (float)i / (float)m_cloud_texure_resolution.z);
         glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, m_cloud_texture, 0, i);
         glDrawArrays(GL_TRIANGLES, 0, vertices.size());
     }
     
     
     glGenerateMipmap(GL_TEXTURE_3D);
     
     glBindVertexArray(previous_vao);
     glUseProgram(previous_program);
     glBindBuffer(GL_FRAMEBUFFER, previous_buffer);
     
     glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    
 }

 void Cloudbox::initBuffer() {
     glGenFramebuffers(1, &m_cloudbuffer);
     glBindFramebuffer(GL_FRAMEBUFFER, m_cloudbuffer);
 }

 void Cloudbox::initTexture() {
     glGenTextures(1, &m_cloud_texture);
     glBindTexture(GL_TEXTURE_3D, m_cloud_texture);

     glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, m_cloud_texure_resolution.x, m_cloud_texure_resolution.y, m_cloud_texure_resolution.z, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
     

     material.bindTexture(m_cloud_texture);
 }

 void Cloudbox::attachBuffer() {
     glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_cloud_texture, 0);


     if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
         throw std::exception("Error in cloud buffer ");
     }

     glBindFramebuffer(GL_FRAMEBUFFER, m_cloudbuffer);
     glDrawBuffer(GL_COLOR_ATTACHMENT0);
 }

 void Cloudbox::ChangeShaders() {
     glDeleteProgram(material.getShaderProgram());
     material.loadShader(GL_VERTEX_SHADER, ".\\Shaders\\cloudrenderer.vs");
     material.loadShader(GL_FRAGMENT_SHADER, ".\\Shaders\\cloudrender.fs");
     material.CreateShaders();
 }

 void Cloudbox::AttachUniforms() {
     material.attachUniform("DensityThreshold", cloudParams.m_density_threshold);
     material.attachUniform("lightAbsorptionTowardSun", cloudParams.m_light_absorption_toward_light);
     material.attachUniform("darknessThreshold", cloudParams.m_darkness_threshold);
     material.attachUniform("DensityMultiplier", cloudParams.m_density_multiplier);
     material.attachUniform("num_of_steps", cloudParams.m_num_of_steps_shape);
     material.attachUniform("num_of_steps_inside", cloudParams.m_num_of_steps_light);
     material.attachUniform("lightAbsorptionThroughCloud", cloudParams.m_light_absorption_through_cloud);
     material.attachUniform("LightColor", cloudParams.m_light_color);
     material.attachUniform("cloudScale", cloudParams.m_cloud_scale);
     material.attachUniform("cloudOffset", cloudParams.m_cloud_offset);
     material.attachUniform("secondLayerScale", cloudParams.m_second_layer_scale);
     material.attachUniform("secondLayerOffset", cloudParams.m_second_layer_offset);
     material.attachUniform("thirdLayerScale", cloudParams.m_third_layer_scale);
     material.attachUniform("thirdLayerOffset", cloudParams.m_third_layer_offset);
     material.attachUniform("phaseParams", cloudParams.m_phase_params);

     material.attachUniform("tex_res", glm::vec3(m_cloud_texure_resolution.x, m_cloud_texure_resolution.y, m_cloud_texure_resolution.z));
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

 }

 void Sky::Cloud::initTexture(int res){
     glGenTextures(1, &cloudCubeMap);
     glBindTexture(GL_TEXTURE_CUBE_MAP, cloudCubeMap);

     for (int i = 0; i < 6; i++) 
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, res, res, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
     
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
 }

 void Sky::RenderCloud() {
     GLint previous_vao, previous_program;
     glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &previous_vao);
     glGetIntegerv(GL_CURRENT_PROGRAM, &previous_program);


    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_3D);

    cloudsOnSky.cloudbox->AttachUniforms();

    //cloudsOnSky.cloudbox->material.updateUniforms();
    cloudsOnSky.cloudbox->material.attachUniform("r", c_inner_radius);
    cloudsOnSky.cloudbox->material.attachUniform("R", c_outer_radius);
    cloudsOnSky.cloudbox->material.attachUniform("center", ConvertSFML2GLM(c_center_position));

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
    glBindVertexArray(previous_vao);
    glUseProgram(previous_program);

    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_3D);
 }

 void Sky::initTexture(){
     glGenTextures(1, &skyBoxTexture);
     glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexture);

     if (int error = glGetError())
         throw std::exception("GlError ", error);

     for (int i = 0; i < 6; i++) 
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, cubemapRes, cubemapRes, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
 }

 Sky::Sky(Cloudbox* _cloudbox) {
     
     if (int error = glGetError())
         throw std::exception("GlError ", error);

     cloudsOnSky.cloudbox = _cloudbox;
     camera.cameraPos.x = cloudsOnSky.cloudbox->GetPos().x;
     camera.cameraPos.y = cloudsOnSky.cloudbox->GetPos().y - cloudsOnSky.cloudbox->GetSize().y / 2.f;
     camera.cameraPos.z = cloudsOnSky.cloudbox->GetPos().z;

     cloudsOnSky.initTexture(cubemapRes);
     skySphere.initTexture(cubemapRes);
     initTexture();
     skyBoxFrameBuffer = createFrameBuffer(cubemapRes, cubemapRes);
     camera.createMatrices();
 }



 void Sky::initFramebuffer() {
     glGenBuffers(1, &skyBoxFrameBuffer);
 }

 void Sky::Render(int i) {
     GLint previous_buffer;
     glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_buffer);

    glViewport(0, 0, cubemapRes, cubemapRes);
    glBindFramebuffer(GL_FRAMEBUFFER, skyBoxFrameBuffer);
     
    //for (int i = 0; i < 6; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, skyBoxTexture, 0);
        camera.switchToFace(i);
        updateMatrices();
        
        RenderSky();
        camera.cameraPos.y -= 0.1;
        updateMatrices();
        RenderCloud();
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        camera.cameraPos.y += 0.1;
    //}
    
    glBindFramebuffer(GL_FRAMEBUFFER, previous_buffer);
 }

 GLuint Sky::GetTex() {
     return skyBoxTexture;
 }

 void Sky::SkySphere::initTexture(int res) {
     glGenTextures(1, &m_texture);
     glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

     for (int i = 0; i < 6; i++) 
         glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, res, res, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

     
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
 }

 void Sky::attachMeshToSky(GeometricObject* sky_obj) {
     skySphere.m_sphere_mesh = (Mesh*)sky_obj;
 }

 void Sky::SkySphere::attachShader(const std::string& fragment_shader_path) {
     vertexShader = loadShader(GL_VERTEX_SHADER, m_sphere_mesh->material.GetVSPath().c_str());
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
     glBindVertexArray(skySphere.m_sphere_mesh->material.getVAO());

     skySphere.m_sphere_mesh->material.setModel(glm::mat4(1.0));

     GLuint uniView = glGetUniformLocation(skySphere.shaderProgram, "view");
     glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(camera.view));

     GLuint uniProj = glGetUniformLocation(skySphere.shaderProgram, "proj");
     glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(camera.proj));

     GLuint uniModel = glGetUniformLocation(skySphere.shaderProgram, "model");
     glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(skySphere.m_sphere_mesh->material.getModel()));

     GLuint uniEye = glGetUniformLocation(skySphere.shaderProgram, "eyepos");
     glUniform3f(uniEye, 0, 0, 0);

     GLuint uniPos = glGetUniformLocation(skySphere.shaderProgram, "pos");
     glUniform3f(uniPos, 0, 0, 0);

     GLuint uniSize = glGetUniformLocation(skySphere.shaderProgram, "size");
     glUniform3f(uniSize, .04f, .04f, .04f);

     GLuint uniLight = glGetUniformLocation(skySphere.shaderProgram, "light");
     glUniform3f(uniLight, -skySphere.m_sphere_mesh->material.sun_rot->x, -skySphere.m_sphere_mesh->material.sun_rot->y, -skySphere.m_sphere_mesh->material.sun_rot->z);


     glDrawArrays(GL_TRIANGLES, 0, (*skySphere.m_sphere_mesh).vert_vec3[0][skySphere.m_sphere_mesh->array_index].size());
     glBindVertexArray(0);
     glUseProgram(0);
 }

 Sky::~Sky() {
     glDeleteTextures(1, &skySphere.m_texture);
     glDeleteShader(skySphere.vertexShader);
     glDeleteShader(skySphere.fragmentShader);
     glDeleteProgram(skySphere.shaderProgram);

     glDeleteTextures(1, &cloudsOnSky.cloudCubeMap);
     
     glDeleteTextures(1, &skyBoxTexture);
     glDeleteBuffers(1, &skyBoxFrameBuffer);
 }