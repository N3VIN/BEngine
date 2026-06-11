#include <cstring>
#include "Renderer/Renderer.h"
#include "SceneGraph/SceneManager.h"
#include "Components/CameraComponent.h"
#include "Texture2D.h"

#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

void bengine::Renderer::Init(SDL_Window *window) {
    m_window = window;

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

#if defined(__EMSCRIPTEN__)
    m_renderer = SDL_CreateRenderer(window, nullptr);
#else
    m_renderer = SDL_CreateRenderer(window, nullptr);
#endif

    if (m_renderer == nullptr) {
        std::cout << "Failed to create the renderer: " << SDL_GetError() << "\n";
        throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

#if defined(__EMSCRIPTEN__)
    io.IniFilename = NULL;
#endif
    ImGui_ImplSDL3_InitForSDLRenderer(window, m_renderer);
    ImGui_ImplSDLRenderer3_Init(m_renderer);
}

void bengine::Renderer::Render() const {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    const auto &color = GetBackgroundColor();
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);

    SceneManager::GetInstance().Render();

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_renderer);
    SDL_RenderPresent(m_renderer);
}

void bengine::Renderer::Destroy() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    if (m_renderer != nullptr) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
}

void bengine::Renderer::RenderTexture(const Texture2D &texture, const SDL_FRect &dst, const SDL_Rect *src, const float angleDegrees) const {
    SDL_FRect srcF{};
    const SDL_FRect *srcPtr = nullptr;
    if (src != nullptr) {
        srcF.x = static_cast<float>(src->x);
        srcF.y = static_cast<float>(src->y);
        srcF.w = static_cast<float>(src->w);
        srcF.h = static_cast<float>(src->h);
        srcPtr = &srcF;
    }

    SDL_RenderTextureRotated(GetSDLRenderer(), texture.GetSDLTexture(), srcPtr, &dst, angleDegrees, nullptr, SDL_FLIP_NONE);
}

SDL_Renderer *bengine::Renderer::GetSDLRenderer() const {
    return m_renderer;
}

void bengine::Renderer::SetActiveCamera(const CameraComponent *camera) {
    m_activeCamera = camera;
}

const bengine::CameraComponent *bengine::Renderer::GetActiveCamera() const {
    return m_activeCamera;
}

glm::vec2 bengine::Renderer::GetWindowSize() const {
    int w{}, h{};
    SDL_GetWindowSize(m_window, &w, &h);
    return {static_cast<float>(w), static_cast<float>(h)};
}

glm::vec2 bengine::Renderer::WorldToScreen(const glm::vec2 &worldPos) const {
    if (!m_activeCamera) {
        return worldPos;
    }

    const auto center = m_activeCamera->GetViewCenter();
    const auto zoom = m_activeCamera->GetZoom();
    const auto screenCenter = GetWindowSize() * 0.5f + m_viewOffset;

    return (worldPos - center) * zoom + screenCenter;
}

const SDL_Color &bengine::Renderer::GetBackgroundColor() const {
    return m_clearColor;
}

void bengine::Renderer::SetBackgroundColor(const SDL_Color &color) {
    m_clearColor = color;
}

void bengine::Renderer::SetViewOffset(const glm::vec2 &offset) {
    m_viewOffset = offset;
}
