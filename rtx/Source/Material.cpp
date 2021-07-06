
#include "../Headers/Material.h"
#include "../Headers/PLayer.h"
#include "../Headers/GeometricObject.h"




Player& _Pl = Player::Get();

Material::Material() {
    vertexShader_source = nullptr;
    fragmentShader_source = nullptr;
    geometryShader_source = nullptr;
}

Material::~Material() {
    glDeleteTextures(1, &texture);
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    //free(vertexShader_source);
    //free(geometryShader_source);
    //free(fragmentShader_source);
}

int Material::createShaderProgram(const GLchar* vertSrc, const GLchar* geomSrc, const GLchar* fragSrc, GLuint& vertexShader, GLuint& geometryShader, GLuint& fragmentShader, GLuint& shaderProgram) {
    //vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertSrc, NULL);
    glCompileShader(vertexShader);

    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        getError(log_vs, vertexShader);
        return -1;
    }

    //geometry shader
    if (geomSrc != nullptr) {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geomSrc, NULL);
        glCompileShader(geometryShader);

        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE) {
            getError(log_gs, geometryShader);
            return -2;
        }
    }

    //fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        
        getError(log_fs, fragmentShader);
        return -3;
    }

    //link
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");

    glLinkProgram(shaderProgram);


    return 0;
}

int Material::CreateShaders() {
    glBindVertexArray(vao);
    int t = createShaderProgram(vertexShader_source, geometryShader_source, fragmentShader_source, vertexShader, geometryShader, fragmentShader, shaderProgram);

    glBindVertexArray(0);
    return 0;
}

void Material::loadShader(GLenum type, const GLchar* path) {

    std::ifstream file;
    file.open(path);

    if (!file.is_open()) {
        std::cout << "error!!" << std::endl;
    }

    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* shaderSource = (char*)calloc(file_size, 1);

    file.read(shaderSource, file_size);

    switch (type) {
    case GL_VERTEX_SHADER:
        vertexShader_source = shaderSource;
        vertexShader_path = std::string(path);
        break;
    case GL_GEOMETRY_SHADER:
        geometryShader_source = shaderSource;
        geometryShader_path = std::string(path);
        break;
    case GL_FRAGMENT_SHADER:
        fragmentShader_source = shaderSource;
        fragmentShader_path = std::string(path);
        break;

    default:
        break;
    }
}

void Material::bindTexture(const GLchar* path) {
    tex_path = path;
    texture = loadTexture(path);
    glBindVertexArray(vao);
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
    glUseProgram(0);
    glBindVertexArray(0);
}

void Material::bindTexture(GLuint texure) {
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
}

void Material::bindTexture(GLuint texure, const GLchar* name) {
    glBindVertexArray(vao);
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, name), 1);
    glUseProgram(0);
    glBindVertexArray(0);
}

void Material::specifyVertexAttributes(GLuint shaderProgram) {
    glBindVertexArray(vao);
    std::cout << "SCENE" << std::endl;
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);
    std::cout << "posattrib " << posAttrib << " texattrib " << texAttrib << std::endl;
}

void Material::specifyVertexAttributes_screen(GLuint shaderProgram) {
    glBindVertexArray(vao);
    std::cout << "SCREEN" << std::endl;
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position_screen");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    std::cout << "posattrib " << posAttrib << " texattrib " << texAttrib << std::endl;
    glBindVertexArray(0);
}

void Material::specifyVertexAttributes_terrain(GLuint shaderProgram) {
    glBindVertexArray(vao);
    std::cout << "Terrain" << std::endl;
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    std::cout << "posattrib " << posAttrib << " texattrib " << texAttrib << std::endl;
    glBindVertexArray(0);
}

void Material::specifyVertexAttributes_mesh() {

    glBindVertexArray(vao);
    std::cout << "MESH" << std::endl;
    
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buff);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE,  sizeof(glm::vec3), 0);
    
    GLint normAttrib = glGetAttribLocation(shaderProgram, "normal");
    glEnableVertexAttribArray(normAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, norm_buff);
    glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE,  sizeof(glm::vec3), 0);
    
    GLint uvAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(uvAttrib);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buff);
    glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE,  sizeof(glm::vec2), 0);

     std::cout << "posattrib " << posAttrib << " normattrib " << normAttrib << " texattrib " << uvAttrib << std::endl;
}



void Material::createVAO_VBO(std::vector<GLfloat>vertices) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(0);
}

void Material::createVAO_VBO_mesh(std::vector<glm::vec3>&vert_vec3, std::vector<glm::vec2>&uv_vec2, std::vector<glm::vec3>&norm_vec3) {
    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vert_buff);
    glBindBuffer(GL_ARRAY_BUFFER, vert_buff);
    glBufferData(GL_ARRAY_BUFFER, vert_vec3.size() * sizeof(glm::vec3), &vert_vec3[0], GL_STATIC_DRAW);

    if (norm_vec3.size() != 0) {
        glGenBuffers(1, &norm_buff);
        glBindBuffer(GL_ARRAY_BUFFER, norm_buff);
        glBufferData(GL_ARRAY_BUFFER, norm_vec3.size() * sizeof(glm::vec3), &norm_vec3[0], GL_STATIC_DRAW);
    }

    glGenBuffers(1, &uv_buff);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buff);
    glBufferData(GL_ARRAY_BUFFER, uv_vec2.size() * sizeof(glm::vec2), &uv_vec2[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Material::attachUniform(const char* name, float value) {
    glUseProgram(shaderProgram);
    GLuint Uni = glGetUniformLocation(shaderProgram, name);
    glUniform1f(Uni, value);
    glUseProgram(0);
}

void Material::attachUniform(const char* name, GLuint tex) {
    glUseProgram(shaderProgram);
    GLuint Uni = glGetUniformLocation(shaderProgram, name);
    glUniform1i(Uni, tex);
    glUseProgram(0);
}

void Material::updateUniforms() {
    glUseProgram(shaderProgram);

    GLuint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(_Pl.view));

    GLuint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(_Pl.proj));

    if (lightSpaceMatrixPtr != nullptr) {
        GLuint lightSpaceMatrix = glGetUniformLocation(shaderProgram, "lightSpaceMatrix");
        glUniformMatrix4fv(lightSpaceMatrix, 1, GL_FALSE, glm::value_ptr(*lightSpaceMatrixPtr));
    }

    uniModel = glGetUniformLocation(shaderProgram, "model");

    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(_Pl.proj));

    glUniformMatrix4fv(getUniModel(), 1, GL_FALSE, glm::value_ptr(getModel()));

    glUseProgram(0);
}