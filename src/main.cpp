#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
#include <string>
#include <iostream>

#include "LinkedList.h"

using namespace std;

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"


ma_engine engine;
Playlist myPlaylist; // list object from linkedlist.h
Node* currentlyPlaying = nullptr;

std::string GetFilenameFromPath(std::string path) {
    // Finds the last "\" and returns the text after it
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        return path.substr(lastSlash + 1);
    }
    return path;
}

std::string OpenFileDialog() {
    OPENFILENAME ofn;
    char szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Audio Files\0*.mp3;*.wav;*.flac;*.mp4\0All Files\0*.*\0"; // Added mp4
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return std::string(szFile);
    }
    return "";
}


int main() {
    float volume = 0.5f;
    bool muted = false;

    int winWidth, winHeight;
    if (!glfwInit()) return 1;

    
    

    
    GLFWwindow* window = glfwCreateWindow(800, 600, "Playter - MP3 Player", NULL, NULL); // changed size to be bigger
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
            ImGui::Begin("Playter Control Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
            ImGui::TextColored(ImVec4(0,1,0,1), "Now Playing:"); 
            ImGui::SameLine();
            if (currentlyPlaying != nullptr) {
                ImGui::Text(currentlyPlaying->data.title); // show name from node
            } else {
                ImGui::Text("Nothing");
            }

            ImGui::NewLine();
            ImGui::Separator();
            ImGui::NewLine();

            // CONTROLS
            if (ImGui::Button("Previous")) {
                // stack for history will go here (pop on finished track)
            }

            ImGui::SameLine();

            // play only when there is a song selected
            if (ImGui::Button("Play")) {
                if (currentlyPlaying != nullptr) {
                     ma_engine_play_sound(&engine, currentlyPlaying->data.filePath, NULL);
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Next")) {
                 // linked list travesal to go next
                 if(currentlyPlaying != nullptr && currentlyPlaying->next != nullptr){
                     currentlyPlaying = currentlyPlaying->next;
                     ma_engine_play_sound(&engine, currentlyPlaying->data.filePath, NULL);
                 }
            }

            ImGui::SameLine();

            if (ImGui::Button(" + Add Song ")) {
                std::string path = OpenFileDialog();
                if (!path.empty()) {
                    // I extracted the filename to use as a "Title" 
                    std::string title = GetFilenameFromPath(path); 

                    // I passed 3 arguments to satisfy the LinkedList:
                    // 1. Title, 2. A Dummy Artist, 3. The Path
                    myPlaylist.addSong(title.c_str(), "Unknown Artist", path.c_str());
                    cout << "Added node for: " << path << endl;
                }
            }

            ImGui::NewLine();
            ImGui::Separator();
            ImGui::NewLine();

            ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f);
            ma_engine_set_volume(&engine, volume); // set volume in audio engine
            
            ImGui::Checkbox("Mute", &muted);

            ImGui::Separator();
            ImGui::NewLine();

            // LIST VISUALIZATON
            ImGui::Text("Playlist Queue (Linked List Content):");
            
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 200), true, ImGuiWindowFlags_HorizontalScrollbar); // scrolling region for playlist
            
            // POINTER TRAVERSAL
            Node* temp = myPlaylist.head;
            int i = 1;
            while (temp != nullptr) {
                std::string label = std::to_string(i) + ". " + temp->data.title;
                
                // change currentplaying item to be the selected (clicked) one
                if (ImGui::Selectable(label.c_str(), currentlyPlaying == temp)) {
                    currentlyPlaying = temp;
                }
                
                temp = temp->next; // moving pointer to next node
                i++;
            }
            ImGui::EndChild();

            ImGui::End();
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.2f, 0.2f, 0.2f, 1.00f);
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