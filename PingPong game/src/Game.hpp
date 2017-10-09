
#ifndef ASSIGNMENT1_GAME_HPP
#define ASSIGNMENT1_GAME_HPP



#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <memory>


class GLFWwindow;

class Game {

public:
    Game(int width,int height,const std::string & title = "MainWindow");



    void run();
    ~Game();


    int height;
    int width;
private:

    GLFWwindow * mainWindow = nullptr;

    void update(double delta);
    void draw();
    void drawRectangle(float vx1,float vy1,float vz1,float vx2,float vy2,float vz2,float vx3,float vy3,float vz3,float vx4,float vy4,float vz4);
    void drawCircle();
private:

    GLuint spriteProgram;

    glm::mat4 projectionMatrix;
    void showScore();


};


#endif //ASSIGNMENT1_GAME_HPP
