#include "resource_manager.h"
#include "orbit_camera.h"
#include "model.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cstdlib>
#include <iostream>

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 576

static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void processInput(GLFWwindow *window);

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Error: Failed to initialise GLFW." << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Renderer", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cerr << "Error: Failed to create GLFW window." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Error: Failed to initialise GLAD." << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Setting up Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int cube_VAO, light_cube_VAO, VBO;

    glGenVertexArrays(1, &light_cube_VAO);
    glGenVertexArrays(1, &cube_VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cube_VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(light_cube_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Shader lighting_shader = ResourceManager::load_shader("lighting_shader",
                                "resources/shaders/lighting.vs",
                                "resources/shaders/lighting.fs");

    Shader light_cube_shader = ResourceManager::load_shader("light_cube_shader",
                                                            "resources/shaders/cube.vs",
                                                            "resources/shaders/cube.fs");

    Shader our_shader = ResourceManager::load_shader("model_loading_shader", "resources/shaders/model_loading.vs", "resources/shaders/model_loading.fs");

    Model model_obj{ "resources/models/backpack/backpack.obj" };

    glEnable(GL_DEPTH_TEST);

    glm::mat4 model{1.0f};
    glm::mat4 view{1.0f};
    glm::mat4 projection{1.0f};

    glm::vec3 camera_target{0.0f, 0.0f, 0.0f};

    OrbitCamera camera{camera_target, 5.0f};

    while (!glfwWindowShouldClose(window))
    {
        static float pitch = 90.0f, yaw = 0.0f, radius = 3.0f;
        static float light_pos[3] = {4.0f, 0.0f, 0.0f};
        static bool wireframe = false, orthographic = false, auto_light_movement = false;

        model = glm::mat4{1.0f};

        // Input
        processInput(window);

        // Rendering
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.set_pitch(pitch);
        camera.set_yaw(yaw);
        camera.set_radius(radius);

        view = camera.get_view_matrix();

        if (auto_light_movement)
        {
            light_pos[0] = sin(glm::radians((float)glfwGetTime() * 5.0f)) * 5.0f;
            light_pos[2] = cos(glm::radians((float)glfwGetTime() * 5.0f)) * 5.0f;
        }

        // Cube
        /*
        lighting_shader.use();
        lighting_shader.set_vec3("objectColor", 1.0f, 0.5f, 0.31f);
        lighting_shader.set_vec3("lightColor", 1.0f, 1.0f, 1.0f);
        lighting_shader.set_vec3("lightPos", light_pos[0], light_pos[1], light_pos[2]);

        glm::vec3 camera_pos = camera.get_camera_pos();
        lighting_shader.set_vec3("viewPos", camera_pos);

        lighting_shader.set_mat4("model", model);
        lighting_shader.set_mat4("view", view);
        lighting_shader.set_mat4("projection", projection);

        glBindVertexArray(cube_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Light cube

        model = glm::mat4{1.0f};
        model = glm::translate(model, glm::vec3{light_pos[0], light_pos[1], light_pos[2]});
        model = glm::scale(model, glm::vec3{0.4f, 0.4f, 0.4f});

        light_cube_shader.use();
        light_cube_shader.set_mat4("model", model);
        light_cube_shader.set_mat4("view", view);
        light_cube_shader.set_mat4("projection", projection);

        glBindVertexArray(light_cube_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        */

        our_shader.use();
        our_shader.set_mat4("projection", projection);
        our_shader.set_mat4("view", view);

        model = glm::mat4{ 1.0f };
        our_shader.set_mat4("model", model);

        model_obj.draw(our_shader);

        // Transformations

        view = camera.get_view_matrix();

        float aspect_ratio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

        if (orthographic)
        {
            projection = glm::ortho(-aspect_ratio, aspect_ratio, -1.0f, 1.0f, 0.1f, 100.0f);
        } else
        {
            projection = glm::perspective(45.0f, aspect_ratio, 0.1f, 100.0f);
        }

        // Dear ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Camera controls
        ImGui::Begin("Camera Controls");

        ImGui::SliderFloat("Yaw", &yaw, 0.0f, 360.0f);
        ImGui::SliderFloat("Pitch", &pitch, 0.1f, 179.9f);
        ImGui::SliderFloat("Radius", &radius, 1.0f, 10.0f);

        ImGui::Checkbox("Orthographic", &orthographic);

        ImGui::End();

        // Lighting controls
        ImGui::Begin("Lighting Controls");

        ImGui::InputFloat3("Light Position", light_pos);
        ImGui::Checkbox("Auto-Movement", &auto_light_movement);

        ImGui::End();

        // Rendering options
        ImGui::Begin("Rendering Options");

        ImGui::Checkbox("Wireframe Mode", &wireframe);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        if (wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        }

        // Clean-up
    glDeleteVertexArrays(1, &cube_VAO);
    glDeleteVertexArrays(1, &light_cube_VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();

    return 0;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}