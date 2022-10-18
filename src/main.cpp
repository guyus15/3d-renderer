#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "input_manager.h"
#include "model.h"
#include "orbit_camera.h"
#include "resource_manager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>

#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT 576

InputManager input_manager;

static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Renderer", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cerr << "Error: Failed to create GLFW window." << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    input_manager.set_target_window(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

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

    Shader our_shader = ResourceManager::load_shader("model_loading_shader", "resources/shaders/model_loading.vs", "resources/shaders/model_loading.fs");
    Model model_obj{ "resources/models/backpack/backpack.obj" };

    glEnable(GL_DEPTH_TEST);

    glm::mat4 model{ 1.0f };
    glm::mat4 view{ 1.0f };
    glm::mat4 projection{ 1.0f };

    glm::vec3 camera_target{ 0.0f, 0.0f, 0.0f };

    OrbitCamera camera{ camera_target, 5.0f };

    while (!glfwWindowShouldClose(window))
    {
        static float pitch = 90.0f, yaw = 0.0f, yaw_step = 10.0f, radius = 3.0f;
        static float light_pos[3] = { 4.0f, 0.0f, 0.0f };
        static bool wireframe = false, orthographic = false, auto_light_movement = false;

        // Input
        input_manager.process_input();

        radius += input_manager.get_scroll_y();
        radius = std::clamp(radius, 1.0f, 10.0f);

        yaw += input_manager.get_scroll_x() * yaw_step;
        yaw = std::clamp(yaw, 0.0f, 360.0f);

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
        }
        else
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
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    input_manager.process_scroll_callback(xoffset, yoffset);
}