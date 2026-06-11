#include "BuildLevelScene.h"
#include "Tileset.h"
#include "UIHelpers.h"
#include "Components/LevelGridComponent.h"
#include "Components/GridRenderComponent.h"
#include "Components/BombManagerComponent.h"
#include "Components/EnemyManagerComponent.h"
#include "Components/HazardComponent.h"
#include "Components/ExitComponent.h"
#include "Components/PickupComponent.h"
#include "Components/GameAudioComponent.h"
#include "Components/SpriteRendererComponent.h"
#include "Components/BrickComponent.h"
#include "Components/RenderComponent.h"
#include "Components/TextComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/FPSComponent.h"
#include "Components/CameraComponent.h"
#include "GameModes/IGameMode.h"
#include "PlayerFactory.h"
#include "SceneGraph/Scene.h"
#include "SceneGraph/SceneManager.h"
#include "SceneGraph/GameObject.h"
#include "Renderer/ResourceManager.h"
#include "Renderer/Renderer.h"
#include "Input/InputManager.h"

bomberman::LevelGridComponent *bomberman::CreateLevelBackground(bengine::Scene &scene, const Tileset &tileset, LevelGridComponent *gridComponent) {
    const auto levelOrigin = gridComponent->GetOrigin();
    const int tileSize = tileset.tileSize;

    auto *levelGameObject = scene.Add(std::make_unique<bengine::GameObject>());
    levelGameObject->SetLocalPosition(levelOrigin);

    const SDL_Rect backgroundSourceRect{
        tileset.backgroundCoord.x, tileset.backgroundCoord.y,
        gridComponent->GetColumns() * tileSize,
        gridComponent->GetRows() * tileSize
    };

    levelGameObject->AddComponent<GridRenderComponent>(tileset.texturePath, backgroundSourceRect, tileset.spriteScale);

    return gridComponent;
}

bengine::CameraComponent *bomberman::CreateCamera(bengine::Scene &scene, const LevelGridComponent *gridComponent) {
    auto *cameraGo = scene.Add(std::make_unique<bengine::GameObject>());
    auto *cameraComp = cameraGo->AddComponent<bengine::CameraComponent>();

    const float levelHeight = static_cast<float>(gridComponent->GetRows()) * gridComponent->GetCellSize();
    const float windowHeight = bengine::Renderer::GetInstance().GetWindowSize().y;
    cameraComp->SetZoom(windowHeight / levelHeight);

    cameraComp->SetTargetOffset(glm::vec2{gridComponent->GetCellSize() * 0.5f}); // we do 0.5 to center the camera on the player

    cameraComp->SetLevelBounds(
        gridComponent->GetOrigin(),
        glm::vec2(gridComponent->GetColumns(), gridComponent->GetRows()) * gridComponent->GetCellSize()
    );

    bengine::Renderer::GetInstance().SetActiveCamera(cameraComp);
    return cameraComp;
}

bomberman::LevelSpawns bomberman::SpawnLevelTiles(bengine::Scene &scene, LevelGridComponent *gridComponent) {
    LevelSpawns spawns{};

    const auto spawnTile = [&scene, gridComponent](glm::ivec2 cell, SpriteType spriteType) {
        auto *tile = scene.Add(std::make_unique<bengine::GameObject>());
        tile->SetLocalPosition(gridComponent->CellToWorld(cell));
        tile->AddComponent<SpriteRendererComponent>(spriteType);

        return tile;
    };

    const auto dims = gridComponent->GetDimensions();
    for (int column = 0; column < dims.x; ++column) {
        for (int row = 0; row < dims.y; ++row) {
            const glm::ivec2 cell{column, row};
            switch (gridComponent->GetTile(column, row)) {
                case TileType::Wall:
                    gridComponent->SetWall(cell, true);
                    break;
                case TileType::Brick: {
                    auto *tile = spawnTile(cell, SpriteType::Brick);
                    tile->AddComponent<BrickComponent>(0.875f);
                    gridComponent->SetWall(cell, true);
                    gridComponent->SetTile(tile, cell);
                    break;
                }
                case TileType::Exit: {
                    auto *tile = spawnTile(cell, SpriteType::Exit);
                    gridComponent->SetTile(tile, cell);
                    break;
                }
                case TileType::Player1Spawn:
                    spawns.players[0] = cell;
                    break;
                case TileType::Player2Spawn:
                    spawns.players[1] = cell;
                    break;
                case TileType::BalloomSpawn:
                    spawns.enemies.emplace_back(EnemyType::Balloom, cell);
                    break;
                case TileType::OnilSpawn:
                    spawns.enemies.emplace_back(EnemyType::Onil, cell);
                    break;
                case TileType::DallSpawn:
                    spawns.enemies.emplace_back(EnemyType::Dall, cell);
                    break;
                case TileType::MinvoSpawn:
                    spawns.enemies.emplace_back(EnemyType::Minvo, cell);
                    break;
                case TileType::Empty:
                    break;
            }
        }
    }

    return spawns;
}

void bomberman::CreateFPSDisplay(bengine::Scene &scene) {
    auto *fpsGo = scene.Add(std::make_unique<bengine::GameObject>());
    auto *fpsText = fpsGo->AddComponent<bengine::TextComponent>();
    fpsGo->GetComponent<bengine::RenderComponent>()->SetIgnoreCamera(true);
    fpsText->SetFont(bengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
    fpsText->SetColor({0, 255, 0, 255});
    fpsGo->AddComponent<bengine::FPSComponent>();
    fpsGo->SetLocalPosition(bengine::ScreenFraction(0.01f, 0.01f));
}

bomberman::LevelScene bomberman::BuildLevelScene(std::string_view jsonRelativePath, const IGameMode &mode) {
    const auto &tileset = GetTileset();

    auto &scene = bengine::SceneManager::GetInstance().CreateScene();

    auto *audioManagerGO = scene.Add(std::make_unique<bengine::GameObject>());
    audioManagerGO->AddComponent<GameAudioComponent>();

    auto *levelGameObject = scene.Add(std::make_unique<bengine::GameObject>());
    auto *levelGridComponent = levelGameObject->AddComponent<LevelGridComponent>(bengine::ResourceManager::GetInstance().GetDataPath() / jsonRelativePath);

    CreateLevelBackground(scene, tileset, levelGridComponent);

    auto *cameraComponent = CreateCamera(scene, levelGridComponent);

    const auto spawns = SpawnLevelTiles(scene, levelGridComponent);

    auto *bombManagerGO = scene.Add(std::make_unique<bengine::GameObject>());
    auto *bombManager = bombManagerGO->AddComponent<BombManagerComponent>(levelGridComponent);

    auto hazardManagerGO = std::make_unique<bengine::GameObject>();
    auto *hazardManager = hazardManagerGO->AddComponent<HazardComponent>(levelGridComponent);

    auto *enemyManagerGO = scene.Add(std::make_unique<bengine::GameObject>());
    auto *enemyManager = enemyManagerGO->AddComponent<EnemyManagerComponent>(levelGridComponent, hazardManager);

    auto *exitManagerGO = scene.Add(std::make_unique<bengine::GameObject>());
    auto *exitManager = exitManagerGO->AddComponent<ExitComponent>(levelGridComponent);

    auto *pickupManagerGO = scene.Add(std::make_unique<bengine::GameObject>());
    auto *pickupManager = pickupManagerGO->AddComponent<PickupComponent>(levelGridComponent);

    const bool player2IsBalloom = mode.Player2IsBalloom();

    std::vector<bengine::GameObject *> players;
    players.push_back(CreatePlayer(scene, {levelGridComponent, spawns.players[0], 4.0f}));

    if (mode.PlayerCount() > 1) {
        const PlayerConfig player2Config{levelGridComponent, spawns.players[1], 4.0f};
        players.push_back(player2IsBalloom ? CreateBallomPlayer(scene, player2Config) : CreatePlayer(scene, player2Config));
    }

    for (size_t playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
        auto *player = players[playerIndex];
        if (player2IsBalloom && playerIndex == 1) {
            hazardManager->RegisterEnemy(player); // dies to bombs, kills player 1 on touch
            continue;
        }

        bombManager->RegisterPlayer(player);
        enemyManager->RegisterPlayer(player);
        hazardManager->RegisterPlayer(player);
        exitManager->RegisterPlayer(player);
        pickupManager->RegisterPlayer(player);
    }

    mode.ConfigureCamera(*cameraComponent, players);

    if (mode.SpawnsEnemies()) {
        for (const auto &[type, cell]: spawns.enemies) {
            enemyManager->SpawnEnemy(type, cell);
        }
    }

    scene.Add(std::move(hazardManagerGO));

    CreateFPSDisplay(scene);

    if (mode.ShowsScore()) {
        for (size_t playerIndex = 0; playerIndex < players.size(); ++playerIndex) {
            auto *scoreGameObject = scene.Add(std::make_unique<bengine::GameObject>());
            auto *scoreText = scoreGameObject->AddComponent<bengine::TextComponent>();
            scoreGameObject->GetComponent<bengine::RenderComponent>()->SetIgnoreCamera(true);
            scoreText->SetFont(bengine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
            scoreText->SetColor({255, 255, 255, 255});
            scoreGameObject->AddComponent<ScoreComponent>(players[playerIndex], static_cast<int>(playerIndex));

            const float fractionX = (playerIndex == 0) ? 0.02f : 0.80f; // TODO: this is temp
            scoreGameObject->SetLocalPosition(bengine::ScreenFraction(fractionX, 0.06f));
        }
    }

    mode.ConfigureInput(bengine::InputManager::GetInstance(), players, *bombManager);

    return LevelScene{scene, std::move(players)};
}
