#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "stb_image.h"
#include "camera.h"
using namespace std;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// time
float deltaTime = 0.0f;
float lastTime = 0.0f;
Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX, lastY;
bool firstMouse = true;

void processInput(GLFWwindow *window)
{
    // 当旋转的时候需要 glm::cross(cameraFront, cameraUp) 判断方向
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    cam.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    cam.ProcessMouseScroll(yoffset);
}

int main()
{
    // 调用glfwInit函数来初始化GLFW
    glfwInit();
    // 我们将主版本号(Major)和次版本号(Minor)都设为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 明确告诉GLFW我们使用的是核心模式(Core-profile)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 不允许用户调整窗口的大小
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // glfwCreateWindow函数需要窗口的宽和高作为它的前两个参数；
    // 第三个参数表示这个窗口的名称（标题），这里我们使用"LearnOpenGL"，当然你也可以使用你喜欢的名称；
    // 最后两个参数我们暂时忽略，先设置为空指针就行
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了
    glfwMakeContextCurrent(window);

    // must have
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Faile to initialize GLAD" << endl;
        return -1;
    }

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
        // 顶点坐标，       法线向量
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // cubeVAO
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0); // 在渲染前指定OpenGL该如何解释顶点数据
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float))); // 在渲染前指定OpenGL该如何解释顶点数据
    glEnableVertexAttribArray(1);

    // lightVAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightColor(1, 1, 1);
    Shader cubeShader("Shaders/2-3-Material-1.vs", "Shaders/2-3-Material-1.fs");
    cubeShader.use();
    cubeShader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
    cubeShader.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
    cubeShader.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
    cubeShader.setFloat("material.shininess", 32.0f);
    cubeShader.setVec3("light.position", lightPos);
    cubeShader.setVec3("light.ambient", lightColor);
    cubeShader.setVec3("light.diffuse", lightColor);
    cubeShader.setVec3("light.specular", lightColor);

    Shader lightShader("Shaders/2-1-Colors-1.vs", "Shaders/2-1-Colors-light.fs");

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) // 让GLFW退出前一直保持运行
    {
        float time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;

        processInput(window);    // 输入
        glfwPollEvents();        // glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等），然后调用对应的回调函数
        glfwSwapBuffers(window); // glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色的大缓冲）

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = cam.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);

        // cubeShader
        cubeShader.use();
        cubeShader.setMat4("model", model);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);
        cubeShader.setVec3("viewPos", cam.Position);
        glBindVertexArray(cubeVAO); // 刚创建的程序对象作为它的参数，以激活这个程序对象
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // lightShader
        lightShader.use();
        glm::mat4 lightmodel = glm::mat4(1.0f);
        lightmodel = glm::translate(lightmodel, lightPos);
        lightmodel = glm::scale(lightmodel, glm::vec3(0.2f));
        lightShader.setMat4("model", lightmodel);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);
        glBindVertexArray(lightVAO); // 刚创建的程序对象作为它的参数，以激活这个程序对象
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwTerminate(); // glfwTerminate函数来释放GLFW分配的内存
    return 0;
}