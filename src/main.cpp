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
Playlist myPlaylist; 
Node* currentlyPlaying = nullptr;

static ma_sound sound;
static bool soundLoaded = false;

float volume = 50.0f;
bool muted = false;

std::string GetFilenameFromPath(std::string path) {
    // Finds the last "\" and returns the text after it
    size_t lastSlash = path.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        return path.substr(lastSlash + 1);
    }
    return path;
}

void LoadSound(char* SoundFile) {
    if (soundLoaded) {
        ma_sound_uninit(&sound); // sound hold the current audio file
        // if sound is already loaded unload it to add the new sound file
        soundLoaded = false;
    }
    if (ma_sound_init_from_file(&engine, SoundFile, 0, NULL, NULL, &sound) == MA_SUCCESS) {
        //sound loaded
        soundLoaded = true;
    }
    else {
        std::cout << "Failed to load sound!\n";
    }
}

void Play() {
    if (soundLoaded) {
        ma_sound_start(&sound);
    }
}

void Pause() {
    if (soundLoaded) {
        ma_sound_stop(&sound);
    }
}

std::string OpenFileDialog() {
    OPENFILENAME ofn;
    char szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Audio Files\0*.mp3;*.wav;*.flac;*.mp4\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return std::string(szFile);
    }
    return "";
}

void AddSong() {
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
}

void UI_PreReq() {
    ImGui::Begin("Playter Control Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Now Playing:");
}

void Diplay_CurrentlyPlaying() {
    if (currentlyPlaying != nullptr) {
        ImGui::Text(currentlyPlaying->data.title); // show name from node
    }
    else {
        ImGui::Text("Nothing");
    }
}

void Previous_Button() {
    if (ImGui::Button("Previous")) {
        // stack for history will go here (pop on finished track)
    }
}

void Play_Button() {
    if (ImGui::Button("Play")) {
        Play();
    }
}

void Pause_Button() {
    if (ImGui::Button("Pause")) {
        Pause();
    }
}


void Next_Button() {
    if (ImGui::Button("Next")) {
        // linked list travesal to go next
        if (currentlyPlaying != nullptr && currentlyPlaying->next != nullptr) {
            //TODO:
            // Copy To Stack
            // Delete From currentlyPlaying
            
            currentlyPlaying = currentlyPlaying->next;
            LoadSound(currentlyPlaying->data.filePath);
            Play(); // Play Next Track
        }
    }
}

void Set_Volume() {
    ImGui::Checkbox("##Mute", &muted);
    ImGui::SameLine();

    // Use int for the slider
    static int intVolume = 100; // 0-100
    if (ImGui::SliderInt("Volume", &intVolume, 0, 100)) {
        // do nothing here, handled below
    }

    // Apply to miniaudio engine
    float scaledVolume = muted ? 0.0f : (float)intVolume / 100.0f;
    ma_engine_set_volume(&engine, scaledVolume);
}


void Queue() {
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
            LoadSound(currentlyPlaying->data.filePath);
        }

        temp = temp->next; // moving pointer to next node
        i++;
    }
}

// Converts seconds → mm:ss format
std::string FormatTime(int seconds) {
    int minutes = seconds / 60;
    int sec = seconds % 60;

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, sec);
    return std::string(buffer);
}

void Timeline_Slider() {
    ma_uint64 cursorFrames = 0;
    ma_uint64 totalFrames = 0;

    if (ma_sound_get_cursor_in_pcm_frames(&sound, &cursorFrames) != MA_SUCCESS) return;
    if (ma_sound_get_length_in_pcm_frames(&sound, &totalFrames) != MA_SUCCESS) return;
    if (totalFrames == 0) return;

    int sampleRate = (int)sound.engineNode.pEngine->sampleRate;

    int currentSec = (int)(cursorFrames / sampleRate);
    int totalSec = (int)(totalFrames / sampleRate);

    // Slider using integers
    float sliderValue = (float)currentSec;
    if (ImGui::SliderFloat("##Timeline", &sliderValue, 0.0f, totalSec, "")) {
        int newFrame = (int)(sliderValue * sampleRate);
        ma_sound_seek_to_pcm_frame(&sound, newFrame);
    }

    ImGui::SameLine();
    // Show mm:ss display
    ImGui::Text("%s / %s",
        FormatTime(currentSec).c_str(),
        FormatTime(totalSec).c_str()
    );
}



int run() {
    

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
            UI_PreReq();
            ImGui::SameLine();

            Diplay_CurrentlyPlaying();
            Timeline_Slider();

            ImGui::NewLine();

            ImGui::Separator();

            ImGui::NewLine();

            // CONTROLS
            Previous_Button();

            ImGui::SameLine();

            // play only when there is a song selected (FIXED)
            Play_Button();

            ImGui::SameLine();

            Pause_Button();

            ImGui::SameLine();

            Next_Button();

            ImGui::SameLine();
           
            AddSong();

            ImGui::NewLine();

            ImGui::Separator();

            ImGui::NewLine();

            Set_Volume();

            ImGui::Separator();

            ImGui::NewLine();

            // LIST VISUALIZATON
            Queue();

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
}

int main() {
    run();

    return 0;
}