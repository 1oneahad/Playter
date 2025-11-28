#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>


#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

struct Song {
    char title[100];
    char artist[100];
};

ma_engine engine;

int main() {
    
    if (!glfwInit()) return 1;
    
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Playter - MP3 Player", NULL, NULL);
    if (window == NULL) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.\n");
        return -1;
    }
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Playter Control Panel");
            
            ImGui::Text("Now Playing: Nothing");

            ImGui::NewLine();
            
            ImGui::Separator();

            ImGui::NewLine();
            
            if (ImGui::Button("Play")) {
            }
            ImGui::SameLine();
            if (ImGui::Button("Pause")) {
            }
            ImGui::SameLine();
            if (ImGui::Button("Next")) {
            }

            ImGui::NewLine();

            ImGui::Separator();
            
            ImGui::NewLine();

            ImGui::Text("Playlist Queue");
            
            ImGui::NewLine();

            ImGui::Separator();

            ImGui::End();
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ma_engine_uninit(&engine);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}