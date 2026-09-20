#include "../include/UI.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl3.h"
#include "imgui/imgui_impl_sdlrenderer3.h"

UI::UI(SDL_Window* sdlWindow, SDL_Renderer* sdlRenderer)
    : window(sdlWindow), renderer(sdlRenderer) {}

UI::~UI() {
    // Cleanup ImGui
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

bool UI::init() {
    if (!window || !renderer) {
        SDL_Log("UI initialization failed: Window or Renderer is null.\n");
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    if (!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer)) {
        SDL_Log("ImGui_ImplSDL3_InitForSDLRenderer failed.\n");
        return false;
    }
    if (!ImGui_ImplSDLRenderer3_Init(renderer)) {
        SDL_Log("ImGui_ImplSDLRenderer3_Init failed.\n");
        return false;
    }

    return true;
}

void UI::processEvent(const SDL_Event* event) {
    ImGui_ImplSDL3_ProcessEvent(event);
}

void UI::beginFrame() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void UI::render() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open ROM...")) {
                const SDL_DialogFileFilter filters[] = {{ "CHIP-8 ROMs", "*.ch8" }};
                SDL_ShowOpenFileDialog(
                       [](void* userdata, const char* const* filelist, int filter) {
                           UI* ui = static_cast<UI*>(userdata);
                           // If user selected a file and callback is registered
                           if (filelist && filelist[0] && ui->onROMSelected) {
                                ui->onROMSelected(filelist[0]);
                           }
                       }, 
                       this, // pass UI context to the lambda callback
                       window, 
                       filters, 
                       0, 
                       nullptr, 
                       false
                );
            }
            if(ImGui::MenuItem("Reset Rom")){
                onResetSelected();
            }
            if (ImGui::MenuItem("Exit")) {
                onExitSelected();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::MenuItem("Emulation Speed...")) {
                showSettingsWindow = !showSettingsWindow;
            }


            if(ImGui::MenuItem(muteMenuLabel)){
                muteSound();
                muteMenuLabel = "Unmute";
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    // Render the Settings Window if toggled on
       if (showSettingsWindow) {
           // ImGui::Begin with a bool* parameter automatically renders a close [X] button
           if (ImGui::Begin("Settings", &showSettingsWindow, ImGuiWindowFlags_AlwaysAutoResize)) {
               if (getIPF && setIPF) {
                   int speed = static_cast<int>(getIPF());
                   if (ImGui::SliderInt("Instructions/Frame", &speed, 1, 100)) {
                       setIPF(static_cast<uint64_t>(speed));
                   }
               }
           }
           ImGui::End();
       }
}

void UI::endFrame() {
    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
}
