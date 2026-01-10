#include "cube.h"


class Cloudbox : public Cube {
public:
    Cloudbox(glm::vec3 _pos, glm::vec3 _res, glm::vec3 size);
    void                                    Delete();
    void                                    RenderCloud(float innerRadius, float outerRadius, glm::vec3 center);
    void                                    renderTexture(int SCREEN_WIDTH, int SCREEN_HEIGHT);
    struct                                  CloudParams {
        glm::vec4                           phaseParams = glm::vec4(0.72, 0.33, 1, 0.74);

        glm::vec3
                                            LightColor = glm::vec3(1, 1, 1),
                                            cloudScale = glm::vec3(1),
                                            cloudOffset = glm::vec3(0),
                                            secondLayerScale = glm::vec3(2),
                                            secondLayerOffset = glm::vec3(0),
                                            thirdLayerScale = glm::vec3(3),
                                            thirdLayerOffset = glm::vec3(0);

        float
                                            DensityThreshold = 0.93,
                                            DensityMultiplier = 72,
                                            lightAbsorptionThroughCloud = 0.85,
                                            lightAbsorptionTowardSun = 2.0,
                                            darknessThreshold = 0.2;

        int
                                            num_of_steps = 80,
                                            num_of_steps_inside = 50;
    };
    CloudParams                             cloudParams;
    void                                    recreateShaders();
    void                                    uniforms();
    GLuint                                  GetTexture() { return cloudtex; }
private:
    glm::vec3                            cloudTexRes;
    const int                               pointsGrid = 7;
    GLuint                                  cloudbuffer;
    GLuint                                  cloudtex;

    void                                    initBuffer();
    void                                    initTexture();
    void                                    attachBuffer();
    
    
};
