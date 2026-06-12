#pragma once
#include <string>
#include <vector>
#include <glm/vec2.hpp>

#include "SceneGraph/ISceneState.h"

namespace bengine {
    class Scene;
}

namespace bomberman {
    class NameEntryComponent;

    class HighScoreState final : public bengine::ISceneState {
    public:
        struct NewScore {
            std::string label{};
            int score{0};
        };

        HighScoreState() = default;
        explicit HighScoreState(std::vector<NewScore> newScores);
        ~HighScoreState() override;

        void OnEnter() override;
        void OnExit() override;
        void Update(float deltaTime) override;

    private:
        [[nodiscard]] static glm::vec2 Centered(float fractionY);

        void Rebuild();
        void BuildTable() const;
        void BuildEntry(const NewScore &entry);
        void BindTable() const;
        void BindEntry() const;

        std::vector<NewScore> m_newScores{};
        bengine::Scene *m_scene{};
        NameEntryComponent *m_nameEntry{};
        bool m_dirty{false};
    };

    [[nodiscard]] std::vector<HighScoreState::NewScore> GetNewScores();
}
