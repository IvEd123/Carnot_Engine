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

    float time_passed = 0;
    float _time = clock.getElapsedTime().asSeconds() * 100;
    
    float size = 20.f;

    //cloud map
    
        int x, y, z;
        x = 100;
        y = 100;
        z = 100;
        const int n = 20;

        std::srand(_time);

        float points[n*3];
        for (int i = 0; i < n*3; i++) {
            points[i] = (float)std::rand() / RAND_MAX;
        }

        glEnable(GL_TEXTURE_3D);
        //PFNGLTEXIMAGE3DPROC glTexImage3D;
        //glTexImage3D = (PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");

        GLuint cloudbuffer;
        glGenFramebuffers(1, &cloudbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, cloudbuffer);
        
        GLuint cloudtex;
        glGenTextures(1, &cloudtex);
        glBindTexture(GL_TEXTURE_2D, cloudtex);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x*z, y, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cloudtex, 0);

        //GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
       // glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
       

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERRROR WITH CLOUD BUFFER\n";

        glBindFramebuffer(GL_FRAMEBUFFER, cloudbuffer); 
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glViewport(0, 0, x*z, y);


        Cube cloudbox = Cube(sf::Vector3f(0, 1.7, 0), sf::Vector3f(0, 0, 0), 1, &cloudtex);
        cloudbox.SetSize(1);
        cloudbox.CreateVerticesLegacy();
        cloudbox.material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\cloud.vs");
        cloudbox.material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\cloud.fs");
        cloudbox.material.CreateShaders();
        cloudbox.material.specifyVertexAttributes3D(cloudbox.material.getShaderProgram());
        cloudbox.SetRot(sf::Vector3f(0, 90, 0));
        glUseProgram(cloudbox.material.getShaderProgram());
        glBindVertexArray(cloudbox.material.getVAO());

        int sh_location = glGetUniformLocation(cloudbox.material.getShaderProgram(), "point_pos");
        glUniform3fv(sh_location, n, points);


        int layer_loc = glGetUniformLocation(cloudbox.material.getShaderProgram(), "layers");
        glUniform1i(layer_loc, z);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cloudtex, 0);
        cloudbox.Draw();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /*for (int i = 0; i < z; i++) {
            //glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, cloudtex, 0, i);
            glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, cloudtex, 0, i);
            glDrawArrays(GL_TRIANGLES, 0, cloudbox.vertices.size());
        }*/

       cloudbox.material.bindTexture("C:\\Users\\IvEda\\Desktop\\tex.jpg");

        glBindVertexArray(0);
        glUseProgram(0);
        glBindBuffer(GL_FRAMEBUFFER, 0);
        
        glViewport(0, 0, WIDTH, HEIGHT);
    

    //light
    LightSource sun = LightSource();
    sun.setShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\Shadow.vs");
    sun.setShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\Shadow.fs");
    error = sun.CreateShaders();
    if (error != 0)
        std::cout << error << std::endl;
    sun.SetPos(Vector3f(20, 20, 00));
    sun.SetRot(Vector3f(180, 0, 0));

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
        glDeleteProgram(cloudbox.material.getShaderProgram());
        cloudbox.material.loadShader(GL_VERTEX_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\cloudrenderer.vs");
        cloudbox.material.loadShader(GL_FRAGMENT_SHADER, "C:\\Users\\IvEda\\Desktop\\sfml\\rtx\\Shaders\\cloudrender.fs");
        
        float DensityThreshold = 0.5, DensityMultiplier = 10;

        ImGui::Begin("cloud");
        //ImGui::InputFloat("Threshold", &DensityThreshold);
        //ImGui::InputFloat("mult", &DensityMultiplier);
        ImGui::End(); // end window*/


        //cloudbox.material.attachUniform("DensityThreshold", DensityThreshold);
        //cloudbox.material.attachUniform("DensityMultiplier", DensityMultiplier);

        cloudbox.SetRot(cloudbox.GetRot() + sf::Vector3f(0, 0.2, 0));

        cloudbox.material.CreateShaders();
        cloudbox.material.setTexture(cloudtex);
        cloudbox.material.attachUniform("time", time_passed);
        cloudbox.Draw();


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
            //screen.setColorBuffer(*sun.getShadowMap());
            screen.setColorBuffer(cloudtex);
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