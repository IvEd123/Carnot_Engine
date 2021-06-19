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

std::vector <GeometricObject*> obj_list;
std::vector <LightSource*> light_list;

std::vector<std::vector<glm::vec3>> array_of_vertecies;
std::vector<std::vector<glm::vec3>> array_of_normals;
std::vector<std::vector<glm::vec2>> array_of_uvs;

#include "../Headers/AddObject.h"
#include "../Headers/PLayer.h"
#include "../Headers/Render.h"
#include "../Headers/Material.h"


#include "../Headers/SaveLoad.h"
#include "../Headers/DLLScriptHandler.h"



#define ever (;;)

using namespace sf;

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
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    assert(window.getSettings().depthBits == 24);   
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    pl.proj = glm::perspective(glm::radians(90.0f), static_cast<float>(WIDTH) / static_cast<float>(WIDTH), 0.1f, 1000.0f);
    glEnable(GL_TEXTURE_2D);

    int error;
    

    
    //light
    LightSource sun = LightSource();
    sun.setShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\Shadow.vs");
    sun.setShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\Shadow.fs");
    error = sun.CreateShaders();
    if (error != 0)
        std::cout << error << std::endl;
    sun.SetPos(Vector3f(20, 20, 00));
    sun.SetRot(Vector3f(180, 0,  0));

     //framebuffer
    Screen screen = Screen();
    screen.frameBuffer  = createFrameBuffer(WIDTH, HEIGHT, screen.getDepthSteencilBuffer(), screen.getColorBuffer());
    screen.material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\screen.vs");
    screen.material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\screen.fs");
    error = screen.material.CreateShaders();
    screen.material.specifyVertexAttributes_screen(screen.material.getShaderProgram());
    if (error != 0)
        std::cout<<error<<std::endl;
    screen.addLightSource(&sun);


   
    /*
    //Skybox
    AddObject(CUBE, "skybox");
    LAST_OBJ_PTR->SetSize(500);
    LAST_OBJ_PTR->material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\sky.vs");
    LAST_OBJ_PTR->material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\sky.fs");
    error = LAST_OBJ_PTR->material.CreateShaders();
    LAST_OBJ_PTR->material.specifyVertexAttributes_mesh();
    if (error != 0)
        std::cout << error << std::endl;
    LAST_OBJ_PTR->material.bindTexture("C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Textures\\sky.png"); 
    LAST_OBJ_PTR->cast_shadow = false;
    LAST_OBJ_PTR->addLightSource(&sun);
    
   

    //ground
    AddObject(PLANE, "ground");
    LAST_OBJ_PTR->SetSize(100);
    LAST_OBJ_PTR->SetPos(Vector3f(0.0, 1.0, 0.0));
    LAST_OBJ_PTR->SetRot(Vector3f(5.0, 0.0, 0.0));
    LAST_OBJ_PTR->material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\plane.vs");
    LAST_OBJ_PTR->material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\dirt.fs");
    error = LAST_OBJ_PTR->material.CreateShaders();
    LAST_OBJ_PTR->material.specifyVertexAttributes_mesh();
    if (error != 0)
        std::cout << error << std::endl;
    LAST_OBJ_PTR->material.bindTexture("C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Textures\\grass.jpg");
    ((Plane*)LAST_OBJ_PTR)->texture_repeat = 10;
    LAST_OBJ_PTR->addLightSource(&sun);

    //sea
    AddObject(PLANE, "sea");
    LAST_OBJ_PTR->SetSize(300);
    LAST_OBJ_PTR->SetRot(Vector3f(0.0, 180.0, 0.0));
    LAST_OBJ_PTR->SetPos(Vector3f(100, 0.0, 100));
    LAST_OBJ_PTR->material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\plane.vs");
    LAST_OBJ_PTR->material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\sea.fs");
    error = LAST_OBJ_PTR->material.CreateShaders();
    LAST_OBJ_PTR->material.specifyVertexAttributes_mesh();
    if (error != 0)
        std::cout << error << std::endl;
    LAST_OBJ_PTR->material.bindTexture("C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Textures\\Water\\Water_COLOR.jpg");
    LAST_OBJ_PTR->addLightSource(&sun);

    //house
     AddObject(MESH, "house");
    (dynamic_cast<Mesh*>(LAST_OBJ_PTR))->setModel("C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Meshes\\house.obj");
    LAST_OBJ_PTR->SetPos(Vector3f(15.0, 0.7, 2.0));
    LAST_OBJ_PTR->SetRot(Vector3f(5.0, 0.0, 0.0));
    LAST_OBJ_PTR->SetSize(0.03);
    LAST_OBJ_PTR->material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\mesh.vs");
    LAST_OBJ_PTR->material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\mesh.fs");
    error = LAST_OBJ_PTR->material.CreateShaders();
    LAST_OBJ_PTR->material.specifyVertexAttributes_mesh();
    if (error != 0)
        std::cout << error << std::endl;
    LAST_OBJ_PTR->material.bindTexture("C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Textures\\wood\\albedo.png");
    LAST_OBJ_PTR->addLightSource(&sun);
    
    //crowbar
    AddObject(MESH, "crowbar");
    (dynamic_cast<Mesh*>(LAST_OBJ_PTR))->setModel("C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Meshes\\model.obj");
    LAST_OBJ_PTR->SetPos(Vector3f(2.0, 1.5, 5.0));
    LAST_OBJ_PTR->material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\mesh.vs");
    LAST_OBJ_PTR->material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\mesh.fs");
    error = LAST_OBJ_PTR->material.CreateShaders();
    LAST_OBJ_PTR->material.specifyVertexAttributes_mesh();
    if (error != 0)
        std::cout << error << std::endl;
    LAST_OBJ_PTR->material.bindTexture("C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Textures\\Crowbar_albedo.jpg");
    LAST_OBJ_PTR->addLightSource(&sun);
    LAST_OBJ_PTR->SetSize(1);
    LAST_OBJ_PTR->SetRot(Vector3f(90, 0, 0));
    
   
   //*/


    sf::Vector3f sun_spawn_pos = sun.GetPos();
    sf::Vector3f sun_spawn_pov = sun.GetPov();
   
    const unsigned int t = *screen.getColorBuffer();
    float time_passed = 0;
    bool pause = 0;
    bool pause_prev = 0;

    SaveLoad sf = SaveLoad();
    sf.SetPath("C:\\Users\\IvEda\\Desktop\\sfml");
    sf.SetName("test");

    sf.Load();
    for (int i = 0; i < obj_list.size(); i++)
        obj_list[i]->addLightSource(&sun);


    /*DLLScriptHandler d = DLLScriptHandler();
    d.SetDLL("C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\scrtipts\\crowbar.sus");
    d.setObj(obj_list[4]);*/

    for ever{
        //d.Update();
        
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
            pl.SetAng(Vector2f(pl.GetAng().x + (t.x - mousexy.x) / 4, pl.GetAng().y + (t.y - mousexy.y) / 3));
            if (pl.GetAng().y < -89)
                pl.SetAng(Vector2f(pl.GetAng().x, -89));
            if (pl.GetAng().y > 89)
                pl.SetAng(Vector2f(pl.GetAng().x, 89));
            SetCursorPos(t.x, t.y); 
            //ShowCursor(false);
        }
        
        float time = clock.getElapsedTime().asSeconds() * 100;
        time_passed += time;
        float size = 20.f;
        
        ImGui::SFML::Update(window, clock.restart());
        ImGui::ShowDemoWindow();

        ImGui::Begin("object"); 
        if (ImGui::BeginMenu("objects")) {
            for (int i = 0; i < obj_list.size(); i++) {
                char name[100];
                strcpy_s(name, obj_list[i]->GetName().c_str());
                ImGui::MenuItem(name, NULL);
            }
            ImGui::EndMenu();
        }
        ImGui::End(); // end window

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

        pl.Move(pl.GetAng());
        sun.SetPov(pl.GetPos() + sun_spawn_pov);
        sun.SetPos(pl.GetPos() + sun_spawn_pos);

        for (int i = 0; i < obj_list.size(); i++) 
            obj_list[i]->Draw();
        


        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        screen.material.attachUniform("time", time);
        
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