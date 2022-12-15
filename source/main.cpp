#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw.h"
//#include "imgui/imgui_impl_opengl3.h"

#include "FluidSim2D.h"
#include "shader.h"

#include <chrono>

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

GLenum glCheckError()
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << std::endl;
    }
    return errorCode;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Construct the window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Template", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create the GLFW window\n";
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    // Setup Dear ImGui context
    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    //ImGui_ImplGlfw_InitForOpenGL(window, true);
    //ImGui_ImplOpenGL3_Init("#version 330");

    float vertices[] = {
         -1.f, -1.f, 0.f, 0.f, 0.f,
         -1.f,  1.f, 0.f, 0.f, 1.f,
          1.f, -1.f, 0.f, 1.f, 0.f,
          1.f,  1.f, 0.f, 1.f, 1.f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3,
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // uv attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader("../../source/shaders/quad.vert", "../../source/shaders/quad.frag");
    shader.use();

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    FluidSim2D sim(100, 100);
    Texture fluidTexture(sim.GetWidth(), sim.GetHeight());

    std::chrono::time_point<std::chrono::system_clock> last_time = std::chrono::system_clock::now();

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        // imgui
        {
            //ImGui_ImplOpenGL3_NewFrame();
            //ImGui_ImplGlfw_NewFrame();
            //ImGui::NewFrame();

            //bool my_tool_active = true;
            //// Create a window called "My First Tool", with a menu bar.
            //ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
            //if (ImGui::BeginMenuBar())
            //{
            //    if (ImGui::BeginMenu("File"))
            //    {
            //        if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
            //        if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
            //        if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
            //        ImGui::EndMenu();
            //    }
            //    ImGui::EndMenuBar();
            //}
            //
            //// Edit a color stored as 4 floats
            ////ImGui::ColorEdit4("Color", my_color);
            //
            //// Generate samples and plot them
            //float samples[100];
            //for (int n = 0; n < 100; n++)
            //    samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
            //ImGui::PlotLines("Samples", samples, 100);
            //
            //// Display contents in a scrolling region
            //ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
            //ImGui::BeginChild("Scrolling");
            //for (int n = 0; n < 50; n++)
            //    ImGui::Text("%04d: Some text", n);
            //ImGui::EndChild();

            ////ImGui::ShowDemoWindow();
            //ImGui::End();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            //if (show_demo_window)
            //    ImGui::ShowDemoWindow(&show_demo_window);

            //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            //{
            //    static float f = 0.0f;
            //    static int counter = 0;

            //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            //    ImGui::Checkbox("Another Window", &show_another_window);

            //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            //    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            //        counter++;
            //    ImGui::SameLine();
            //    ImGui::Text("counter = %d", counter);

            //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            //    ImGui::End();
            //}
        }

        //ImGui::Render();

        glClearColor(1.00f, 0.49f, 0.04f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        std::chrono::time_point<std::chrono::system_clock> new_time = std::chrono::system_clock::now();
        float dt = (new_time - last_time).count();
        sim.Step(dt);

        // Rendering
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fluidTexture.width, fluidTexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sim.GetTextureData());
            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, texture);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplGlfw_Shutdown();
    //ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
