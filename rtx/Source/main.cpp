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
//привет

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
    //window.setFramerateLimit(60);
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
    //sun.SetPos(Vector3f(20, 20, 00));
    sun.SetDistance(30);
    sun.SetDir(Vector3f(1, -1, 1));

    //cloud map
    glEnable(GL_TEXTURE_3D);
    Cloudbox cloudbox = Cloudbox(sf::Vector3f(0, 0, 0), sf::Vector3f(256, 256, 256), sf::Vector3f(10, 1, 10));
    cloudbox.material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\cloud.vs");
    cloudbox.material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\cloud.fs");
    cloudbox.material.CreateShaders();
    cloudbox.material.specifyVertexAttributes3D(cloudbox.material.getShaderProgram());
    cloudbox.renderTexture(WIDTH, HEIGHT);
    cloudbox.recreateShaders();
    cloudbox.addLightSource(&sun);



    std::cout << "glerror " << glGetError() << std::endl;
    Sky sky = Sky(&cloudbox);
    


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

    sky.initSky("C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\proceduralSky.fs", obj_list[0]);
    GUI_Object obj_win = GUI_Object();
    GUI_cloud gui_cloud = GUI_cloud();
    gui_cloud.SetObject(&cloudbox);

    
    //cloudbox.RenderCloud();
   
    unsigned int counter = 0;
    bool cloudRender = true;
        
    for ever{
        counter++;

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
            pl.SetAng(Vector2f((int)(pl.GetAng().x + (t.x - mousexy.x) / 4) % 360 , pl.GetAng().y + (t.y - mousexy.y) / 3));
            if (pl.GetAng().y < -89)
                pl.SetAng(Vector2f(pl.GetAng().x, -89));
            if (pl.GetAng().y > 89)
                pl.SetAng(Vector2f(pl.GetAng().x, 89));
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
                if (ImGui::MenuItem(name, NULL))
                    selected = i;
            }
            ImGui::EndMenu();
        }
        ImGui::End(); // end window*/


        //clear depth buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        pl.view = glm::lookAt(
            ConvertSFML2GLM(pl.GetPos()),
            glm::vec3(pl.GetPos().x - sin(pl.GetAng().x / 180 * M_PI), pl.GetPos().y + tan(pl.GetAng().y / 180 * M_PI), pl.GetPos().z - cos(pl.GetAng().x / 180 * M_PI)),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        pl.Move(pl.GetAng(), Dtime);
        //sun.SetPov(pl.GetPos() + sun_spawn_pov);
        //sun.SetPos(pl.GetPos() + sun_spawn_pos);


        //shadows
        glCullFace(GL_FRONT);
        sun.Draw(obj_list);
        glCullFace(GL_BACK);
        glViewport(0, 0, WIDTH, HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, screen.frameBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        
        gui_cloud.Update();
        //std::cout << "clear glerror " << glGetError() << std::endl;
        if (cloudRender) {
            for (int i = 0; i < 6; i++) {
                //std::cout << counter / 10 % 6 << std::endl;
                sky.Render(i);
                glViewport(0, 0, WIDTH, HEIGHT);
                glBindFramebuffer(GL_FRAMEBUFFER, screen.frameBuffer);
            }
        }
        cloudRender = false;
        obj_list[0]->SetPos(pl.GetPos());

        for (int i = 0; i < obj_list.size(); i++) {
            obj_list[i]->material.setEnvironmentMap(sky.GetTex());
            obj_list[i]->Draw();
        }

        sf::Vector3f p = cloudbox.GetPos();
        p.y = 0;
        //cloudbox.RenderCloud(0.5, 1, p);
        
        ImGui::Begin("sky");
        ImGui::InputFloat3("sun dir", &(sun.GetDirPtr()->x));
        ImGui::End();
       

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
            cloudRender = !cloudRender;

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