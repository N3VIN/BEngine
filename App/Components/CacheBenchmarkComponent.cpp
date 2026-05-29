#include "CacheBenchmarkComponent.h"

#include <chrono>
#include <numeric>

#include <imgui_plot.h>

app::CacheBenchmarkComponent::CacheBenchmarkComponent(bengine::GameObject *owner)
    : bengine::Component(owner) {}

template<typename T, typename ModifyFunc>
    requires std::default_initializable<T> && std::invocable<ModifyFunc, T &>
std::vector<app::CacheBenchmarkComponent::BenchmarkResult> app::CacheBenchmarkComponent::RunBenchmark(int samples, ModifyFunc modify) {
    constexpr int size = 10'000'000;
    const auto buffer = new T[size]{};

    std::vector<BenchmarkResult> results;
    std::vector<float> timings;

    for (int stepSize = 1; stepSize <= 1024; stepSize *= 2) {
        timings.clear();

        for (int run = 0; run < samples; run++) {
            const auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < size; i += stepSize) {
                modify(buffer[i]);
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const float elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f;
            timings.push_back(elapsed);
        }

        // remove outliers
        if (timings.size() > 2) {
            auto [min, max] = std::ranges::minmax(timings);
            std::erase(timings, min);
            std::erase(timings, max);
        }

        const float sum = std::accumulate(timings.begin(), timings.end(), 0.0f);
        const float avg = sum / static_cast<float>(timings.size());

        results.push_back({stepSize, avg});
    }

    delete[] buffer;
    return results;
}

void app::CacheBenchmarkComponent::Update(float /*deltaTime*/) {
    if (m_gameObjectBenchmark.requested) {
        m_gameObjectBenchmark.results = RunBenchmark<GameObject3D>(m_gameObjectBenchmark.samples, [](GameObject3D &o) {
                                                                       o.id *= 2;
                                                                   }
        );

        m_gameObjectBenchmark.resultsAlt = RunBenchmark<GameObject3DAlt>(m_gameObjectBenchmark.samples, [](GameObject3DAlt &o) {
                                                                             o.id *= 2;
                                                                         }
        );

        m_gameObjectBenchmark.timings.clear();
        m_gameObjectBenchmark.timingsAlt.clear();
        m_gameObjectBenchmark.stepLabels.clear();

        for (size_t i = 0; i < m_gameObjectBenchmark.results.size(); i++) {
            m_gameObjectBenchmark.timings.push_back(m_gameObjectBenchmark.results[i].avgTimeMs);
            m_gameObjectBenchmark.timingsAlt.push_back(m_gameObjectBenchmark.resultsAlt[i].avgTimeMs);
            m_gameObjectBenchmark.stepLabels.push_back(static_cast<float>(m_gameObjectBenchmark.results[i].stepSize));
        }

        m_gameObjectBenchmark.done = true;
        m_gameObjectBenchmark.requested = false;
    }

    if (m_intBenchmark.requested) {
        m_intBenchmark.results = RunBenchmark<int>(m_intBenchmark.samples, [](int &v) {
                                                       v *= 2;
                                                   }
        );

        m_intBenchmark.timings.clear();
        m_intBenchmark.stepLabels.clear();

        for (size_t i = 0; i < m_intBenchmark.results.size(); i++) {
            m_intBenchmark.timings.push_back(m_intBenchmark.results[i].avgTimeMs);
            m_intBenchmark.stepLabels.push_back(static_cast<float>(m_intBenchmark.results[i].stepSize));
        }

        m_intBenchmark.done = true;
        m_intBenchmark.requested = false;
    }
}

void app::CacheBenchmarkComponent::Render() const { {
        ImGui::Begin("GameObject3D Benchmark");

        ImGui::InputInt("# Samples", &m_gameObjectBenchmark.samples, 1, 10);
        m_gameObjectBenchmark.samples = std::clamp(m_gameObjectBenchmark.samples, 1, 100);

        if (!m_gameObjectBenchmark.done && !m_gameObjectBenchmark.requested) {
            if (ImGui::Button("Run Benchmark")) {
                m_gameObjectBenchmark.requested = true;
            }
        }
        else {
            if (ImGui::Button("Re-run Benchmark")) {
                m_gameObjectBenchmark.requested = true;
                m_gameObjectBenchmark.done = false;
            }

            ImGui::Text("Results (step size vs avg time in ms):");

            if (ImGui::BeginTable("BenchResults", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("Step Size");
                ImGui::TableSetupColumn("Object (ms)");
                ImGui::TableSetupColumn("Pointer (ms)");
                ImGui::TableHeadersRow();

                for (size_t i = 0; i < m_gameObjectBenchmark.results.size(); i++) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", m_gameObjectBenchmark.results[i].stepSize);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.4f", m_gameObjectBenchmark.results[i].avgTimeMs);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.4f", m_gameObjectBenchmark.resultsAlt[i].avgTimeMs);
                }
                ImGui::EndTable();
            }

            ImGui::Spacing();

            if (!m_gameObjectBenchmark.timings.empty()) {
                const float maxObj = *std::ranges::max_element(m_gameObjectBenchmark.timings);
                const float maxPtr = *std::ranges::max_element(m_gameObjectBenchmark.timingsAlt);
                const float maxVal = std::max(maxObj, maxPtr);

                const float *ysList[] = {m_gameObjectBenchmark.timings.data(), m_gameObjectBenchmark.timingsAlt.data()};
                const ImU32 colors[] = {ImColor(0, 255, 0), ImColor(255, 80, 80)};

                ImGui::PlotConfig conf{};
                conf.values.xs = m_gameObjectBenchmark.stepLabels.data();
                conf.values.ys_list = ysList;
                conf.values.ys_count = 2;
                conf.values.colors = colors;
                conf.values.count = static_cast<int>(m_gameObjectBenchmark.timings.size());
                conf.scale.min = 0;
                conf.scale.max = maxVal;
                conf.tooltip.show = false;
                conf.grid_y.show = true;
                conf.grid_y.size = maxVal / 5.f;
                conf.frame_size = ImVec2(400, 200);
                conf.line_thickness = 2.f;

                ImGui::Plot("gameobject3d_plot", conf);

                ImGui::TextColored(ImVec4(0, 1, 0, 1), "Green: Object Transform");
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1, 0.3f, 0.3f, 1), "Red: Pointer Transform");
            }
        }

        ImGui::End();
    } {
        ImGui::Begin("Int Buffer Benchmark");

        ImGui::InputInt("# Samples", &m_intBenchmark.samples, 1, 10);
        m_intBenchmark.samples = std::clamp(m_intBenchmark.samples, 1, 100);

        if (!m_intBenchmark.done && !m_intBenchmark.requested) {
            if (ImGui::Button("Run Benchmark")) {
                m_intBenchmark.requested = true;
            }
        }
        else {
            if (ImGui::Button("Re-run Benchmark")) {
                m_intBenchmark.requested = true;
                m_intBenchmark.done = false;
            }

            ImGui::Text("Results (step size vs avg time in ms):");

            if (ImGui::BeginTable("BenchResultsInt", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("Step Size");
                ImGui::TableSetupColumn("Int (ms)");
                ImGui::TableHeadersRow();

                for (auto &[stepSize, avgTimeMs]: m_intBenchmark.results) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", stepSize);
                    ImGui::TableNextColumn();
                    ImGui::Text("%.4f", avgTimeMs);
                }
                ImGui::EndTable();
            }

            ImGui::Spacing();

            if (!m_intBenchmark.timings.empty()) {
                const float maxVal = *std::ranges::max_element(m_intBenchmark.timings);

                const float *ysList[] = {m_intBenchmark.timings.data()};
                const ImU32 colors[] = {ImColor(0, 200, 255)};

                ImGui::PlotConfig conf{};
                conf.values.xs = m_intBenchmark.stepLabels.data();
                conf.values.ys_list = ysList;
                conf.values.ys_count = 1;
                conf.values.colors = colors;
                conf.values.count = static_cast<int>(m_intBenchmark.timings.size());
                conf.scale.min = 0;
                conf.scale.max = maxVal;
                conf.tooltip.show = false;
                conf.grid_y.show = true;
                conf.grid_y.size = maxVal / 5.f;
                conf.frame_size = ImVec2(400, 200);
                conf.line_thickness = 2.f;

                ImGui::Plot("int_plot", conf);

                ImGui::TextColored(ImVec4(0, 0.78f, 1, 1), "Blue: Int Buffer");
            }
        }

        ImGui::End();
    }
}
