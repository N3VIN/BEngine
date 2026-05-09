#pragma once
#include <SDL3/SDL.h>
#include <array>

#include "Tileset.h"
#include "Components/LevelGridComponent.h"
#include "Components/GridRenderComponent.h"
#include "Commands/GridMoveCommand.h"
#include "Commands/BombPlaceCommand.h"
#include "Components/BombManagerComponent.h"
#include "PlayerFactory.h"

#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "Components/RenderComponent.h"
#include "Components/TextComponent.h"
#include "Components/FPSComponent.h"
#include "Components/CameraComponent.h"
#include "Renderer/ResourceManager.h"
#include "Input/InputManager.h"
#include "Input/Gamepad.h"
#include "Renderer/Renderer.h"

namespace dae {
    inline Scene &BuildLevelScene(std::string_view jsonRelativePath) {
        const auto &tileset = GetTileset();
        const float spriteScale = tileset.spriteScale;
        const int tileSize = tileset.tileSize;

        auto &scene = SceneManager::GetInstance().CreateScene();
        auto levelGameObject = std::make_unique<GameObject>();
        auto *levelGridComponent = levelGameObject->AddComponent<LevelGridComponent>(ResourceManager::GetInstance().GetDataPath() / jsonRelativePath);

        const auto levelOrigin = levelGridComponent->GetOrigin();
        const auto levelSize = glm::vec2(levelGridComponent->GetColumns(), levelGridComponent->GetRows()) * levelGridComponent->GetCellSize();

        levelGameObject->SetLocalPosition(levelOrigin);

        const SDL_Rect backgroundSourceRect{
            tileset.backgroundCoord.x, tileset.backgroundCoord.y,
            levelGridComponent->GetColumns() * tileSize,
            levelGridComponent->GetRows() * tileSize
        };

        levelGameObject->AddComponent<GridRenderComponent>(tileset.texturePath, backgroundSourceRect, spriteScale);

        scene.Add(std::move(levelGameObject));

        auto cameraGo = std::make_unique<GameObject>();
        auto *cameraComp = cameraGo->AddComponent<CameraComponent>();
        cameraComp->SetZoom(2.0f);
        cameraComp->SetLevelBounds(levelOrigin, levelSize);
        scene.Add(std::move(cameraGo));

        Renderer::GetInstance().SetActiveCamera(cameraComp);

        std::array<glm::ivec2, 2> playerSpawnPositions{};

        const auto spawnTile = [&scene, levelGridComponent, &tileset, tileSize, spriteScale](glm::ivec2 cell, glm::ivec2 coord) -> GameObject * {
            const SDL_Rect srcRect{coord.x, coord.y, tileSize, tileSize};
            auto tile = std::make_unique<GameObject>();
            tile->SetLocalPosition(levelGridComponent->CellToWorld(cell));

            auto *renderComponent = tile->AddComponent<RenderComponent>();
            renderComponent->SetTexture(tileset.texturePath);
            renderComponent->SetSourceRect(srcRect);
            renderComponent->SetScale(spriteScale);
            auto *ptr = tile.get();
            scene.Add(std::move(tile));

            return ptr;
        };

        const glm::ivec2 dims = levelGridComponent->GetDimensions();
        for (int column = 0; column < dims.x; ++column) {
            for (int row = 0; row < dims.y; ++row) {
                const glm::ivec2 cell{column, row};
                switch (levelGridComponent->GetTile(column, row)) {
                    case TileType::Wall:
                        levelGridComponent->SetWall(cell, true);
                        break;
                    case TileType::Brick: {
                        auto *tile = spawnTile(cell, tileset.brickCoord);
                        levelGridComponent->SetWall(cell, true);
                        levelGridComponent->SetTile(tile, cell);
                        break;
                    }
                    case TileType::Exit: {
                        auto *tile = spawnTile(cell, tileset.exitCoord);
                        levelGridComponent->SetTile(tile, cell);
                        break;
                    }
                    case TileType::Player1Spawn:
                        playerSpawnPositions[0] = cell;
                        break;
                    case TileType::Player2Spawn:
                        playerSpawnPositions[1] = cell;
                        break;
                    case TileType::Empty:
                        break;
                }
            }
        }

        auto bombManagerGO = std::make_unique<GameObject>();
        auto *bombManager = bombManagerGO->AddComponent<BombManagerComponent>(scene, levelGridComponent);
        scene.Add(std::move(bombManagerGO));

        auto *p1 = CreatePlayer(scene, {levelGridComponent, playerSpawnPositions[0], "bomberman.png", 4.0f, spriteScale});
        cameraComp->SetTarget(p1);

        GameObject *p2 = CreatePlayer(scene, {levelGridComponent, playerSpawnPositions[1], "bomberman.png", 4.0f, spriteScale});

        bombManager->RegisterPlayer(p1);
        bombManager->RegisterPlayer(p2);

        auto fpsGo = std::make_unique<GameObject>();
        auto *fpsText = fpsGo->AddComponent<TextComponent>();
        fpsGo->GetComponent<RenderComponent>()->SetIgnoreCamera(true);
        fpsText->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
        fpsText->SetColor({0, 255, 0, 255});
        fpsGo->AddComponent<FPSComponent>();
        fpsGo->SetLocalPosition(glm::vec2(10, 10));
        scene.Add(std::move(fpsGo));

        auto &input = InputManager::GetInstance();
        input.BindCommand(SDL_SCANCODE_W, KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{0, -1}));
        input.BindCommand(SDL_SCANCODE_S, KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{0, 1}));
        input.BindCommand(SDL_SCANCODE_A, KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{-1, 0}));
        input.BindCommand(SDL_SCANCODE_D, KeyState::Pressed, std::make_unique<GridMoveCommand>(p1, glm::ivec2{1, 0}));

        input.BindCommand(0, Gamepad::Button::DpadUp, KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{0, -1}));
        input.BindCommand(0, Gamepad::Button::DpadDown, KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{0, 1}));
        input.BindCommand(0, Gamepad::Button::DpadLeft, KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{-1, 0}));
        input.BindCommand(0, Gamepad::Button::DpadRight, KeyState::Pressed, std::make_unique<GridMoveCommand>(p2, glm::ivec2{1, 0}));

        input.BindCommand(SDL_SCANCODE_SPACE, KeyState::Down, std::make_unique<BombPlaceCommand>(p1, bombManager));
        input.BindCommand(0, Gamepad::Button::A, KeyState::Down, std::make_unique<BombPlaceCommand>(p2, bombManager));

        return scene;
    }
}
