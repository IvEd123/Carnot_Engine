#define GLEW_STATIC

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <map>
#include <fstream>
#include <stdint.h>
#include <chrono>

#include "../thirdparty/glew-2.1.0/include/GL/glew.h"
#include <gl/GLU.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "../thirdparty/glm/glm.hpp"
#include "../thirdparty/glm/gtc/matrix_transform.hpp"
#include "../thirdparty/glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "imgui-SFML.h"


#include "../Headers/GeometricObject.h"
#include "../Headers/DLLScriptHandler.h"
#include "../Headers/gui.h"

std::vector <GeometricObject*> obj_list;
std::vector <LightSource*> light_list;

std::vector<std::vector<glm::vec3>> array_of_vertecies;
std::vector<std::vector<glm::vec3>> array_of_normals;
std::vector<std::vector<glm::vec2>> array_of_uvs;
std::vector<DLLScriptHandler> scripts;

#include "../Headers/AddObject.h"
#include "../Headers/PLayer.h"
#include "../Headers/Render.h"
#include "../Headers/Material.h"
#include "../Headers/SaveLoad.h"


using namespace sf;

#define ever (;;)

#define M_PI 3.1415926535897932384626433832795
#define DRAW_DISTANCE 1000

#define HEIGHT 800
#define WIDTH 800

int main(int argc, char* argv[]) {
    ImGuiContext* ctx2 = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx2);
    Player& pl = Player::Get();
    pl.SetPos(Vector3f(0, 2, 0));

   

    std::cout << argv[0] << std::endl;
    std::string path = argv[0];

    std::cout << "WASD - move,\nalt - close,\nesc - pause(doesn't work good)" << std::endl;
    Clock clock;

    ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 2;

    err().rdbuf(NULL);
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "win", 7u, settings);
   // window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    assert(window.getSettings().depthBits == 24);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    pl.proj = glm::perspective(glm::radians(90.0f), static_cast<float>(WIDTH) / static_cast<float>(WIDTH), 0.1f, 1000.0f);
    glEnable(GL_TEXTURE_2D);

    int error;

    float time_passed = 0;
    float _time = clock.getElapsedTime().asSeconds() * 100;
    
    float size = 20.f;
    
   
        

    //light
    LightSource sun = LightSource();
    sun.setShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\Shadow.vs");
    sun.setShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\Shadow.fs");
    error = sun.CreateShaders();
    if (error != 0)
        std::cout << error << std::endl;
    sun.SetPos(Vector3f(20, 20, 00));
    sun.SetRot(Vector3f(180, 0, 0));

    //cloud map

    sf::Vector3i cloudTexRes;
    cloudTexRes.x = 100;
    cloudTexRes.y = 100;
    cloudTexRes.z = 100;

    const int pointsGrid = 7; //must be multiple of 7

    std::srand(_time);


    GLuint cloudbuffer;
    glGenFramebuffers(1, &cloudbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, cloudbuffer);

    GLuint cloudtex;
    glGenTextures(1, &cloudtex);
    glBindTexture(GL_TEXTURE_2D, cloudtex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloudTexRes.x * cloudTexRes.z, cloudTexRes.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cloudtex, 0);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERRROR WITH CLOUD BUFFER\n";

    glBindFramebuffer(GL_FRAMEBUFFER, cloudbuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glViewport(0, 0, cloudTexRes.x * cloudTexRes.z, cloudTexRes.y);


    Cube cloudbox = Cube(sf::Vector3f(0, 5, 0), sf::Vector3f(0, 0, 0), 1, &cloudtex);
    cloudbox.SetSize(1);
    cloudbox.size_v = sf::Vector3f(10, 2, 10);
    cloudbox.CreateVerticesLegacy();
    cloudbox.material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\cloud.vs");
    cloudbox.material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\cloud.fs");
    cloudbox.material.CreateShaders();
    cloudbox.material.specifyVertexAttributes3D(cloudbox.material.getShaderProgram());
    glUseProgram(cloudbox.material.getShaderProgram());
    glBindVertexArray(cloudbox.material.getVAO());

    int grid_res_loc = glGetUniformLocation(cloudbox.material.getShaderProgram(), "grid_resolution");
    glUniform1i(grid_res_loc, pointsGrid);

    int layer_loc = glGetUniformLocation(cloudbox.material.getShaderProgram(), "layers");
    glUniform1i(layer_loc, cloudTexRes.z);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cloudtex, 0);
    cloudbox.addLightSource(&sun);
    cloudbox.Draw();

    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, WIDTH, HEIGHT);

    //framebuffer
    Screen screen = Screen();
    screen.frameBuffer = createFrameBuffer(WIDTH, HEIGHT, screen.getDepthSteencilBuffer(), screen.getColorBuffer());
    screen.material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\screen.vs");
    screen.material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\screen.fs");
    error = screen.material.CreateShaders();
    screen.material.specifyVertexAttributes_screen(screen.material.getShaderProgram());
    if (error != 0)
        std::cout << error << std::endl;
    screen.addLightSource(&sun);




    sf::Vector3f sun_spawn_pos = sun.GetPos();
    sf::Vector3f sun_spawn_pov = sun.GetPov();

    const unsigned int t = *screen.getColorBuffer();
    
    bool pause = 0;
    bool pause_prev = 0;

    SaveLoad sf = SaveLoad();
    sf.SetPath("C:\\Users\\IvEda\\Desktop\\sfml");
    sf.SetName("test");

    sf.Load();

    
    for (int i = 0; i < obj_list.size(); i++) 
        obj_list[i]->addLightSource(&sun);
    
    for (int i = 0; i < scripts.size(); i++)
        scripts[i].Start();

    GUI_Object obj_win = GUI_Object();
    glDeleteProgram(cloudbox.material.getShaderProgram());
    cloudbox.material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\cloudrenderer.vs");
    cloudbox.material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\cloudrender.fs");
    cloudbox.material.CreateShaders();

    for ever{
        

        float Dtime = clock.getElapsedTime().asSeconds() * 100;

        time_passed += Dtime;
        Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            //ShowCursor(true);
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::LAlt)) {
                pause = 1;
                window.close();
            }
        }


          
        pause = pause_prev ^ Keyboard::isKeyPressed(Keyboard::Escape);
        //window.setMouseCursorVisible(pause);

        pause_prev = pause;

        

        if (!pause) {
            POINT mousexy;
            GetCursorPos(&mousexy);
            Vector2<int> t = Vector2<int>(window.getPosition().x + 400, window.getPosition().y + 400);
            pl.SetAng(Vector2f((int)(pl.GetAng().x + (t.x - mousexy.x) / 4)%360 , pl.GetAng().y + (t.y - mousexy.y) / 3));
            if (pl.GetAng().y < -89)
                pl.SetAng(Vector2f(pl.GetAng().x, -89));
            if (pl.GetAng().y > 89)
                pl.SetAng(Vector2f( pl.GetAng().x, 89));
            SetCursorPos(t.x, t.y); 
            //ShowCursor(false);
        }
        
        
        
        ImGui::SFML::Update(window, clock.restart());
        ImGui::ShowDemoWindow();

        int selected = -1;

        ImGui::Begin("object"); 
        if (ImGui::BeginMenu("objects")) {
            for (int i = 0; i < obj_list.size(); i++) {
                char name[100];
                strcpy_s(name, obj_list[i]->GetName().c_str());
                if(ImGui::MenuItem(name, NULL))
                    selected = i;
            }
            ImGui::EndMenu();
        }
        ImGui::End(); // end window*/

        //helpful gui tutorial
        //https://docs.unity3d.com/ru/current/Manual/gui-Controls.html

        //clear depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //shadows
        glCullFace(GL_FRONT);
        sun.Draw(obj_list);
        glCullFace(GL_BACK);
        glViewport(0, 0, WIDTH, HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, screen.frameBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        pl.view = glm::lookAt(
            ConvertSFML2GLM(pl.GetPos()),
            glm::vec3(pl.GetPos().x - sin(pl.GetAng().x / 180 * M_PI), pl.GetPos().y + tan(pl.GetAng().y / 180 * M_PI), pl.GetPos().z - cos(pl.GetAng().x / 180 * M_PI)),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        pl.Move(pl.GetAng(), Dtime);
        sun.SetPov(pl.GetPos() + sun_spawn_pov);
        sun.SetPos(pl.GetPos() + sun_spawn_pos);

        for (int i = 0; i < obj_list.size(); i++)
            obj_list[i]->Draw();    

        

        //draw clouds
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        
        static glm::vec4 phaseParams = glm::vec4(0.72, 0.33, 1, 0.74);

        static glm::vec3
            LightColor = glm::vec3(1, 1, 1),
            cloudScale = glm::vec3(1),
            cloudOffset = glm::vec3(0),
            secondLayerScale = glm::vec3(1),
            secondLayerOffset = glm::vec3(0),
            thirdLayerScale = glm::vec3(1),
            thirdLayerOffset = glm::vec3(0);

        static float
            DensityThreshold = 0.3,
            DensityMultiplier = 50,
            lightAbsorptionThroughCloud = 0.85,
            lightAbsorptionTowardSun = 2.0,
            darknessThreshold = 0.2;
        static int
            num_of_steps = 40,
            num_of_steps_inside = 50;
        ImGui::Begin("cloud");
            ImGui::Text("March settings");
                ImGui::InputInt("Steps for shape", &num_of_steps);
                ImGui::InputInt("Steps for shading", &num_of_steps_inside);
            ImGui::Text("Base shape");
                ImGui::InputFloat3("Cloud scale", &cloudScale.x);
                ImGui::InputFloat3("Cloud offset", &cloudOffset.x);
                ImGui::SliderFloat("Threshold", &DensityThreshold, 0.0, 1.0);
                ImGui::SliderFloat("Density Multiplier", &DensityMultiplier, 0.0, 100.0);
            ImGui::Text("Detail parametrs");
                ImGui::InputFloat3("Noise 2nd layer scale", &secondLayerScale.x);
                ImGui::InputFloat3("Noise 2nd layer offset", &secondLayerOffset.x);
                ImGui::InputFloat3("Noise 3rd layer scale", &thirdLayerScale.x);
                ImGui::InputFloat3("Noise 3rd layer offset", &thirdLayerOffset.x);
            ImGui::Text("Lighting");
                ImGui::ColorPicker3("Light Color", &LightColor.x);
                ImGui::SliderFloat("light Absorption Through Cloud", &lightAbsorptionThroughCloud, 0.0, 1.0);
                ImGui::SliderFloat("light Absorption Toward Sun", &lightAbsorptionTowardSun, 0.0, 2.0);
                ImGui::SliderFloat("darknessThreshold", &darknessThreshold, 0.0, 1.0);

                ImGui::SliderFloat("Froward scattering", &phaseParams.x, 0.0, 1.0);
                ImGui::SliderFloat("Back scattering", &phaseParams.y, 0.0, 1.0);
                ImGui::SliderFloat("Base brightness", &phaseParams.z, 0.0, 1.0);
                ImGui::SliderFloat("Phase factor", &phaseParams.w, 0.0, 1.0);

        ImGui::End(); // end window

        cloudbox.material.attachUniform("DensityThreshold", DensityThreshold);
        cloudbox.material.attachUniform("lightAbsorptionTowardSun", lightAbsorptionTowardSun);
        cloudbox.material.attachUniform("darknessThreshold", darknessThreshold);
        cloudbox.material.attachUniform("DensityMultiplier", DensityMultiplier);
        cloudbox.material.attachUniform("num_of_steps", num_of_steps);
        cloudbox.material.attachUniform("num_of_steps_inside", num_of_steps_inside);
        cloudbox.material.attachUniform("lightAbsorptionThroughCloud", lightAbsorptionThroughCloud);
        cloudbox.material.attachUniform("LightColor", LightColor);
        cloudbox.material.attachUniform("cloudScale", cloudScale);
        cloudbox.material.attachUniform("cloudOffset", cloudOffset);
        cloudbox.material.attachUniform("secondLayerScale", secondLayerScale);
        cloudbox.material.attachUniform("secondLayerOffset", secondLayerOffset);
        cloudbox.material.attachUniform("thirdLayerScale", thirdLayerScale);
        cloudbox.material.attachUniform("thirdLayerOffset", thirdLayerOffset);
        cloudbox.material.attachUniform("phaseParams", phaseParams);




        cloudbox.material.setTexture(cloudtex);
        cloudbox.material.attachUniform("time", time_passed);
        cloudbox.Draw();

        glDisable(GL_CULL_FACE);
        

        if (selected != -1)
            obj_win.SetObject(obj_list[selected]);
        obj_win.Update();


        for (int i = 0; i < scripts.size(); i++)
            scripts[i].Update();


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        screen.material.attachUniform("time", Dtime);
        
        if (event.type == Event::KeyReleased && event.key.code == Keyboard::F5){
            int t = sf.Save(&obj_list, &light_list);
            if (t == 0)
                std::cout << "saved" << std::endl;
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
            screen.setColorBuffer(*sun.getShadowMap());

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            screen.setColorBuffer(t);
        }
                

        screen.view = &pl.view;
        screen.proj = &pl.proj;
        screen.addLightSource(&sun);
        screen.Draw();


        
        


        ImGui::SFML::Render(window);

        
        window.display();

    }

    ImGui::SFML::Shutdown();
    window.close();
    
    return 0;
}