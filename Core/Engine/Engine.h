#pragma once
#include <string>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

namespace bengine {
    class Engine final {
        bool m_quit{};

    public:
        explicit Engine(const fs::path &dataPath);
        ~Engine();
        void Run(const std::function<void()> &load);
        void RunOneFrame();

        Engine(const Engine &other) = delete;
        Engine(Engine &&other) = delete;
        Engine &operator=(const Engine &other) = delete;
        Engine &operator=(Engine &&other) = delete;

    private:
        std::chrono::high_resolution_clock::time_point m_lastTime{};
        float m_lag{};
    };
}
