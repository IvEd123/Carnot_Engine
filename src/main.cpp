#define GLEW_STATIC

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <map>
#include <fstream>
#include <stdint.h>
#include <chrono>

#include <GLFW/glfw3.h>

#include <GL/glew.h>
#include <gl/GLU.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
//привет

#include "object/geometricObject.h"
#include "script/dllScriptHandler.h"
#include "gui/gui.h"

std::vector <GeometricObject*> obj_list;
std::vector <LightSource*> light_list;

std::vector<std::vector<glm::vec3>> array_of_vertecies;
std::vector<std::vector<glm::vec3>> array_of_normals;
std::vector<std::vector<glm::vec2>> array_of_uvs;
std::vector<DLLScriptHandler> scripts;

#include "object/addObject.h"
#include "object/player.h"
#include "render/render.h"
#include "material/material.h"
#include "object/saveLoad.h"

#define ever (;;)

#define M_PI 3.1415926535897932384626433832795
#define DRAW_DISTANCE 1000

#define HEIGHT 800
#define WIDTH 800

int main(int argc, char* argv[]) {
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(WIDTH, HEIGHT, "Win", nullptr, nullptr);

    if (!window)
        return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    IMGUI_CHECKVERSION();
    ImGuiContext* ctx2 = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx2);
    Player& pl = Player::Get();
    pl.SetPos(glm::vec3(0, 2, 0));

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    std::cout << argv[0] << std::endl;
    std::string path = argv[0];

    std::cout << "WASD - move,\nalt - close,\nesc - pause(doesn't work good)" << std::endl;

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    pl.proj = glm::perspective(glm::radians(90.0f), static_cast<float>(WIDTH) / static_cast<float>(WIDTH), 0.1f, 1000.0f);
    glEnable(GL_TEXTURE_2D);

    int error;

    float size = 20.f;
    
    
   
    //light
    LightSource sun = LightSource();
    sun.setShader(GL_VERTEX_SHADER, ".\\Shaders\\Shadow.vs");
    sun.setShader(GL_FRAGMENT_SHADER, ".\\Shaders\\Shadow.fs");
    error = sun.CreateShaders();
    if (error != 0)
        std::cout << error << std::endl;
    //sun.SetPos(Vector3f(20, 20, 00));
    sun.SetDistance(30);
    sun.SetDir(glm::vec3(1, -1, 1));

    //cloud map
    glEnable(GL_TEXTURE_3D);
    Cloudbox cloudbox = Cloudbox(glm::vec3(0, 0, 0), glm::vec3(256, 256, 256), glm::vec3(10, 1, 10));
    cloudbox.material.loadShader(GL_VERTEX_SHADER, ".\\Shaders\\cloud.vs");
    cloudbox.material.loadShader(GL_FRAGMENT_SHADER, ".\\Shaders\\cloud.fs");
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
    screen.material.loadShader(GL_VERTEX_SHADER, ".\\Shaders\\screen.vs");
    screen.material.loadShader(GL_FRAGMENT_SHADER, ".\\Shaders\\screen.fs");
    error = screen.material.CreateShaders();
    screen.material.specifyVertexAttributes_screen(screen.material.getShaderProgram());
    if (error != 0)
        std::cout << error << std::endl;
    screen.addLightSource(&sun);


    glm::vec3 sun_spawn_pos = sun.GetPos();
    glm::vec3 sun_spawn_pov = sun.GetPov();

    const unsigned int t = *screen.getColorBuffer();
    
    bool pause = 0;
    bool pause_prev = 0;

    SaveLoad sf = SaveLoad();
    std::cout << "Enter path to save file:\n";
    std::string path_to_safe_file;
    std::cin >> path_to_safe_file;
    sf.SetPath(path_to_safe_file);
    std::string name;
    std::cout << "Enter name of save file (without extension):\n";
    std::cin >> name;
    sf.SetName(name);

    sf.Load();

    
    for (int i = 0; i < obj_list.size(); i++) 
        obj_list[i]->addLightSource(&sun);
    
    for (int i = 0; i < scripts.size(); i++)
        scripts[i].Start();

    sky.initSky(".\\Shaders\\proceduralSky.fs", obj_list[0]);
    GUI_Object obj_win = GUI_Object();
    GUI_cloud gui_cloud = GUI_cloud();
    gui_cloud.SetObject(&cloudbox);

    
    //cloudbox.RenderCloud();
   
    unsigned int counter = 0;
    bool cloudRender = true;
        
    while(!glfwWindowShouldClose(window)){
        counter++;
        glfwPollEvents();


        if (!pause) {
            POINT mousexy;
            GetCursorPos(&mousexy);
            glm::ivec2 t = { 
                400, 
                400 
            };

            pl.SetAng({
                (int)(pl.GetAng().x + (t.x - mousexy.x) / 4) % 360, 
                pl.GetAng().y + (t.y - mousexy.y) / 3 
            });

            if (pl.GetAng().y < -89)
                pl.SetAng({ pl.GetAng().x, -89 });
            if (pl.GetAng().y > 89)
                pl.SetAng({ pl.GetAng().x, 89 });
            SetCursorPos(t.x, t.y);
            //ShowCursor(false);
        }

        //pl.SetAng(sf::Vector2f(-100, 2));
        //std::cout << pl.GetAng().x << " ." << pl.GetAng().y << std::endl;

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
            pl.GetPos(),
            glm::vec3(pl.GetPos().x - sin(pl.GetAng().x / 180 * M_PI), pl.GetPos().y + tan(pl.GetAng().y / 180 * M_PI), pl.GetPos().z - cos(pl.GetAng().x / 180 * M_PI)),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        pl.Move(pl.GetAng(), 0.01);
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

        //obj_list[0]->SetPos(pl.GetPos());

        for (int i = 0; i < obj_list.size(); i++) {
            obj_list[i]->material.setEnvironmentMap(sky.GetTex());
            obj_list[i]->Draw();
        }

        glm::vec3 p = cloudbox.GetPos();
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
        screen.material.attachUniform("time", 0.01f);
        
      /*
        if (event.type == Event::KeyReleased && event.key.code == Keyboard::F5) {
            int t = sf.Save(&obj_list, &light_list);
            if (t == 0)
                std::cout << "saved" << std::endl;
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            int size = obj_list.size();
            if (size > 1)
                for (int i = 1; i < size; i++)
                    DeleteObject(1);


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
      */  

        screen.view = &pl.view;
        screen.proj = &pl.proj;
        screen.addLightSource(&sun);
        screen.Draw();


        
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}