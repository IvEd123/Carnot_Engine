#include "../Headers/Render.h"


#pragma warning(disable : 4996)

#define ever (;;)
#define GL_CLAMP_TO_EDGE 0x812f



Player Player::s_Instance;

Player& Pl = Player::Get();

sf::Vector2f Player::ang = sf::Vector2f(0, 0);
sf::Vector3f Player::pos = sf::Vector3f(0, 0, 0);
float Player::a = .01;
sf::Vector3f Player::velocity = sf::Vector3f(0, 0, 0);



inline float deg_to_rad(float deg) {
    return (M_PI * deg) / 180;
}

void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}


glm::vec3 ConvertSFML2GLM(sf::Vector3f vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}


GLuint loadTexture(const GLchar* path) {
    sf::Image image;
    if (!image.loadFromFile(path))
        return EXIT_FAILURE;

    image.flipVertically();

    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        image.getSize().x, image.getSize().y,
        0,
        GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr()
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    return texture;
}

GLuint createFrameBuffer(int width, int height, GLuint* depghstencil, GLuint *colorBuff){
    GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    GLuint texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    GLuint rboDepthStencil;
    glGenRenderbuffers(1, &rboDepthStencil);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);

    *depghstencil = rboDepthStencil;
    *colorBuff = texColorBuffer;

    return frameBuffer;
}

GLuint createFrameBuffer(int width, int height){
    GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);


    return frameBuffer;
}

void bindTexture(const char* path, const char* name, GLuint* handler, Material* material, int i) {
    *handler = loadTexture(path);
    glBindVertexArray(material->getVAO());
    glUseProgram(material->getShaderProgram());
    glUniform1i(glGetUniformLocation(material->getShaderProgram(), name), i);
    glUseProgram(0);
    glBindVertexArray(0);
}

// my own .obj loader. doesn't work very well 
void OBJLoaderLegacy(const char* path, GeometricObject* object) {
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    
    FILE * file = fopen(path, "r");
    if( file == NULL ){    
        printf("Impossible to open the file !\n");    
        return;
    }

    for ever{
        char lineHeader[1000];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        else {
            if (strcmp(lineHeader, "v") == 0) {
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                temp_vertices.push_back(vertex);
            }else if (strcmp(lineHeader, "vt") == 0) {
                glm::vec2 uv;
                fscanf(file, "%f %f\n", &uv.x, &uv.y);
                temp_uvs.push_back(uv);
            }else if (strcmp(lineHeader, "vn") == 0) {
                glm::vec3 normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                temp_normals.push_back(normal);
            }else if (strcmp(lineHeader, "f") == 0) {
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches != 9) {
                    printf("File can't be read :( Try exporting with other options\n");
                    return;
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
        }
    }

    for (unsigned int i = 0; i < vertexIndices.size(); i++){
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        //(*object->vert_vec3)[object->array_index]->push_back(vertex);
        (*object->vert_vec3)[object->array_index].push_back(vertex);
    }
    for (unsigned int i = 0; i < uvIndices.size(); i++) {
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = temp_uvs[uvIndex - 1];
        //object->uv_vec2[object->array_index]->push_back(uv);
        (*object->uv_vec2)[object->array_index].push_back(uv);
    }
    for (unsigned int i = 0; i < normalIndices.size(); i++){
        unsigned int normIdex = normalIndices[i];
        glm::vec3 normal = temp_normals[normIdex - 1];
        //object->norm_vec3[object->array_index]->push_back(normal);
        (*object->norm_vec3)[object->array_index].push_back(normal);
    }
}


// convert objl::vec to glm::vec
glm::vec3 toglmv3(objl::Vector3 vec) {
    return glm::vec3(vec.X, vec.Y, vec.Z);
}

glm::vec2 toglmv2(objl::Vector2 vec) {
    return glm::vec2(vec.X, vec.Y);
}


void OBJLoader(const char* path, GeometricObject* object) {
    objl::Loader loader;
    loader.LoadFile(path);
    
    for (int i = 0; i < loader.LoadedVertices.size(); i++) {
        (*object->vert_vec3)[object->array_index].push_back(toglmv3(loader.LoadedVertices[i].Position));
        (*object->norm_vec3)[object->array_index].push_back(toglmv3(loader.LoadedVertices[i].Normal));
        (*object->uv_vec2)[object->array_index].push_back(toglmv2(loader.LoadedVertices[i].TextureCoordinate));
    }

    return;
}

void OBJLoader_v(const char* path, GeometricObject* object) {
    objl::Loader loader;
    loader.LoadFile(path);

    for (int i = 0; i < loader.LoadedVertices.size(); i++) {
        object->vertices.push_back(toglmv3(loader.LoadedVertices[i].Position).x);
        object->vertices.push_back(toglmv3(loader.LoadedVertices[i].Position).y);
        object->vertices.push_back(toglmv3(loader.LoadedVertices[i].Position).z);
        //(*object->vert_vec3)[object->array_index].push_back(toglmv3(loader.LoadedVertices[i].Position));
        //(*object->norm_vec3)[object->array_index].push_back(toglmv3(loader.LoadedVertices[i].Normal));
        //(*object->uv_vec2)[object->array_index].push_back(toglmv2(loader.LoadedVertices[i].TextureCoordinate));
    }

    return;
}

int loadShader(GLenum type, const GLchar* path) {

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
        else if (type == GL_FRAGMENT_SHADER)
            std::cout << "frag ";
        getError(log, shader);
    }


    return shader;
}