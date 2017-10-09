#include <imgui_impl_glfw_gl3.h>
#include "Game.hpp"
#include "utitliy.hpp"
#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>

#define TOP 1
#define BOTTOM -1
#define LEFT -1
#define RIGHT 1
using std::cout;
using std::endl;
float x=0.0f;
float y=0.0f;
float vx=0.0f;
float vy=0.0f;
float r = 0.05f;
float cx=0.0f;
float cy=0.0f;
int start=0;
int directionY = -1;
int directionX = -1;
int startFlag=-1;
int score=0;
float speed = 0.025f;
float rectLeftX;
float rectRightX;
int highScore;
std::vector<float> vertices;
std::ifstream fin;
std::ofstream fout;
Game::Game(int width,int height,const std::string & title):width(width),height(height) {

    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 0);
        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }
    // Create a GLFWwindow object
    mainWindow = glfwCreateWindow(width,height,title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(mainWindow,this);
    if (mainWindow == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(mainWindow);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        cout << "Failed to initialize OpenGL context" << endl;
        exit(-1);
    }

    ImGui_ImplGlfwGL3_Init(mainWindow,false);

    //setup matrix
    {
        projectionMatrix = glm::ortho(0.0f,width*1.0f,0.0f,height*1.0f,-1.0f,1.0f);

    }

    glViewport(0,0, width, height);

    //compile shader
    {
        auto vs = fileToCharArr ("resources/sprite.vert");
        auto fs = fileToCharArr ("resources/sprite.frag");
        spriteProgram = makeProgram(compileShader(shDf(GL_VERTEX_SHADER,&vs[0],vs.size())),compileShader(shDf(GL_FRAGMENT_SHADER,&fs[0],fs.size())));

    }

}


void Game::run() {



    double last = 0;
    glfwSetTime(last);
    double delta = 0.0f;
    glfwSwapInterval(1);

    fin.open("HighScore.txt",std::ios::in);
    fin>>highScore;


    while (!glfwWindowShouldClose(mainWindow)) {

        double curr = glfwGetTime();
        delta = curr-last;
        last = curr;

        //input polling
        glfwPollEvents();
        //update objects
        update(delta);
        //draw them
        draw();

            speed=speed+0.00001;
        if(score==0)
        {
            speed=0.025;
        }

        if(startFlag==0) {
            if (directionY == BOTTOM) {
                if (vy + r > -0.7f) {
                    vy = vy - speed;
                } else {

                    directionY = TOP;
                }
            }
            if (directionY == TOP) {
                if (vy + r < 1.0f) {
                    vy = vy + speed;
                } else {
                    directionY = BOTTOM;
                }
            }
        }
        else if(startFlag==1)
        {
            if (directionY == BOTTOM) {
                if (vy + r > -0.7f) {
                    vy = vy - speed;
                } else {
                    if(vx>=rectLeftX && vx<=rectRightX)
                    {
                        score++;
                    } else
                    {
                        if(score>highScore)
                        {
                            fout.open("HighScore.txt",std::ios::out | std::ios::trunc);
                            highScore=score;
                            fout<<score;

                        }
                        score=0;
                    }
                    directionY = TOP;
                }
            }
            if (directionY == TOP) {
                if (vy + r < 1.0f) {
                    vy = vy + speed;
                } else {
                    directionY = BOTTOM;
                }
            }
            if (directionX == LEFT) {
                if (vx + r > -0.9f) {
                    vx = vx - speed;
                } else {
                    directionX = RIGHT;
                }
            }
            if (directionX == RIGHT) {
                if (vx + r < 1.0f) {
                    vx = vx + speed;
                } else {
                    directionX = LEFT;
                }
            }
        }

        glfwSwapBuffers(mainWindow);

    }


}

Game::~Game() {
    glfwTerminate();
}

void Game::update(double delta) {
    if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if(highScore==0)
        {
            fout<<highScore;
        }
        fin.close();
        fout.close();
        glfwSetWindowShouldClose(mainWindow, GLFW_TRUE);
    }
    else if (glfwGetKey(mainWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        printf("Left Key Pressed\n");

        if((x-0.1) >=-0.58 && startFlag!=-1)
            x=x-0.05;



    }
    else if (glfwGetKey(mainWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        printf("Right Key pressed\n");
        if((x-0.1)<0.4 && startFlag!=-1)
        {
            x=x+0.05;
        }

    }

    else if (glfwGetKey(mainWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
    {

        startFlag=1;

    }
    else if (glfwGetKey(mainWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
    {

        startFlag=0;

    }
    else if (glfwGetKey(mainWindow, GLFW_KEY_P) == GLFW_PRESS)
    {

        startFlag=-1;

    }
}

void Game::draw() {

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    rectRightX=0.5f+x;
    rectLeftX=-0.5f+x;
    glClear(GL_COLOR_BUFFER_BIT);
    drawRectangle(0.5f+x,-0.8f+y,0.0f,
                  0.5f+x,-0.9f+y,0.0f,
                  -0.5f+x,-0.8f+y,0.0f,
                  -0.5f+x,-0.9f+y,0.0f);
    drawCircle();
    drawRectangle(-1.0f,1.0f,0.0f,-1.0f,-1.0f,0.0f,-0.99f,1.0f,0.0f,-0.99f,-1.0f,0.0f);
    drawRectangle(1.0f,1.0f,0.0f,1.0f,-1.0f,0.0f,0.99f,1.0f,0.0f,0.99f,-1.0f,0.0f);
    drawRectangle(1.0f,1.0f,0.0f,-1.0f,1.0f,0.0f,1.0f,0.99f,0.0f,-1.0f,0.99f,0.0f);
    showScore();

}


void Game::drawCircle(){

    float twoPi = 2.0f * M_PI;
    int nos = 360;
    vertices.push_back(cx+vx);
    vertices.push_back(cy+vy);
    for (int i = 1; i < nos+2; ++i) {
        vertices.push_back(r*cos(i*twoPi/nos)+vx);
        vertices.push_back(r*sin(i*twoPi/nos)+vy);

    }

    unsigned int VAO,VBO;
    glGenVertexArrays(1,&VAO);

    glBindVertexArray(VAO);
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, 4*vertices.size(),&vertices[0],GL_STATIC_DRAW);

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2* sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glUseProgram(spriteProgram);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN,0,nos+2);
    vertices.clear();
}

void Game::drawRectangle(float vx1,float vy1,float vz1,float vx2,float vy2,float vz2,float vx3,float vy3,float vz3,float vx4,float vy4,float vz4)
{
    float vertices[]={
          vx1,vy1,vz1,
          vx2,vy2,vz2,
          vx3,vy3,vz3,
          vx4,vy4,vz4
    };
    unsigned int indices[]={
            0,1,3,
            0,2,3
    };
    unsigned int VAO,VBO,EBO;

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3* sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glUseProgram(spriteProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    glDisableVertexAttribArray(0);
}

void Game::showScore() {


    ImGui_ImplGlfwGL3_NewFrame();
    ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiSetCond_FirstUseEver);

    ImGui::SetNextWindowPos(ImVec2(690,0));
    bool show_another_window = true;
    ImGui::Begin("Score", &show_another_window,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize);

    ImGui::Text("Score %d    ",score);

    ImGui::Text("HighScore %d",highScore);
    ImGui::End();
    ImGui::Render();


}

