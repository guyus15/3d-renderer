#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "gui.h"
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
#include <memory>

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
    Model model_obj = ResourceManager::load_model("backpack_model", "resources/models/backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);

    glm::mat4 model{ 1.0f };
    glm::mat4 view{ 1.0f };
    glm::mat4 projection{ 1.0f };

    glm::vec3 camera_target{ 0.0f, 0.0f, 0.0f };

    OrbitCamera camera{ camera_target, 5.0f };

    while (!glfwWindowShouldClose(window))
    {
        static float pitch = 90.0f, yaw = 0.0f, yaw_step = 10.0f, radius = 3.0f;
        static WorldPosition light_pos { 4.0f, 0.0f, 0.0f };
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
            light_pos.x = sin(glm::radians((float)glfwGetTime() * 5.0f)) * 5.0f;
            light_pos.z = cos(glm::radians((float)glfwGetTime() * 5.0f)) * 5.0f;
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

        // -- UI --
        std::unique_ptr<CameraControlsUI> camera_ui = std::make_unique<CameraControlsUI>(yaw, pitch, radius, orthographic);
        std::unique_ptr<LightingControlsUI> lighting_ui = std::make_unique<LightingControlsUI>(light_pos, auto_light_movement);
        std::unique_ptr<RenderOptionsUI> render_ui =  std::make_unique<RenderOptionsUI>(wireframe);

        std::vector<IGui*> gui_items{ camera_ui.get(), lighting_ui.get(), render_ui.get() };

        // Dear ImGui - initialising new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
         
        // Render each IGui
        for (const auto& ui : gui_items)
        {
            ui->render();
        }

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