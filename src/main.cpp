#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>  // For GetOpenFileName
#include <string>
#include <iostream>
using namespace std;

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

struct Song {
    char title[100];
    char artist[100];
};

ma_engine engine;

// Select FILE and RETURN File Path (GPT Generated)
std::string OpenFileDialog() {
    OPENFILENAME ofn;       // common dialog box structure
    char szFile[260] = { 0 }; // buffer for file name

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL; // or your window handle
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "MP3 Files\0*.mp3\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return std::string(szFile);
    }
    return ""; // user canceled
}


int main() {
    float volume = 0.5f;
    bool muted = false; //Volume Slider stuff (dummy)
    std::string filepath = "Nothing"; //to be used with ADD SONG button
    int winWidth, winHeight;
    if (!glfwInit()) return 1;

    
    

    
    GLFWwindow* window = glfwCreateWindow(600, 400, "Playter - MP3 Player", NULL, NULL);
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
            glfwGetWindowSize(window, &winWidth, &winHeight);
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2((float)winWidth, (float)winHeight)); 
            //get window size and the following line sets the subwindow size accordingly
            ImGui::Begin("Playter Control Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            
            ImGui::Text("Now Playing:");
            ImGui::SameLine();
            ImGui::Text(filepath.c_str());

            ImGui::NewLine();
            
            ImGui::Separator();

            ImGui::NewLine();
            
            if (ImGui::Button("Play")) {
                ma_engine_play_sound(&engine, filepath.c_str(), NULL); 
                //Multiple times play hota if you repeatedly press play
            }
            ImGui::SameLine();
            if (ImGui::Button("Pause")) {
            }
            ImGui::SameLine();
            if (ImGui::Button("Next")) {
            }
            ImGui::SameLine();
            if (ImGui::Button("Add Song")) {
                filepath = OpenFileDialog(); //Brings Filepath
                if (!filepath.empty()) {
                    std::cout << "User selected: " << filepath << std::endl; //terminal
                }
            }

            ImGui::NewLine();

            ImGui::Separator();
            
            ImGui::NewLine();

            ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
            ImGui::Checkbox("Mute", &muted);

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