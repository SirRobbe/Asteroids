#define KS_ENABLE_AUDIO 1
#define KS_DRAW_DEBUG_COLLIDERS 0
#define STB_TRUETYPE_IMPLEMENTATION

// TODO: Maybe make for some of them one large header
#include "platform/window.h"
#include "platform/renderer.h"
#include "platform/vertexbuffer.h"
#include "platform/vertexarray.h"
#include "platform/shader.h"
#include "platform/audio.h"
#include "platform/file.h"
#include "platform/types.h"
#include "platform/clock.h"
#include "math/matrix4x4.h"
#include "math/matrix3x3.h"
#include "math/vec2.h"
#include "vertexbufferlayout.h"
#include "input.h"
#include "color.h"
#include "platform/texture.h"
#include "data_structures/fixed_array.h"
#include "data_structures/dynamic_array.h"
#include "sprite.h"
#include "math/random.h"
#include "font.h"
#include "string.h"

// TODO: Do everything so that these are no longer needed
#include <cassert>
#include <iostream>
#include <glad/glad.h>
#include <string>

constexpr ks::uint32 WINDOW_WIDTH = 480U;
constexpr ks::uint32 WINDOW_HEIGHT = 800U;

constexpr ks::float32 PI = 3.14159265359;

// TODO: make this local and kill the external reference
extern bool g_IsRunning;

ks::Matrix4x4 projection;
ks::ShaderHandle spriteShader;
ks::ShaderHandle backgroundShader;
ks::ShaderHandle fontShader;

struct Entity
{
    ks::Vec2 Position;
    float Speed;
    float ColliderRadius;
};

struct Meteor : public Entity
{
    void Update(float ellapsedTime, ks::uint32 windowHeight, ks::uint32 halfHeight)
    {
        Position.Y += Speed * ellapsedTime;
        if(Position.Y > windowHeight + halfHeight)
        {
            IsDead = true;
        }
        Rotation += RotationSpeed * ellapsedTime;
        if(Rotation > 360.f)
        {
            Rotation -= 360.f;
        }
    }
    
    float Rotation;
    float RotationSpeed;
    bool IsDead;
};

struct MeteorManager
{
    ks::DynamicArray<Meteor> Meteors;
    float SpawnCooldown;
    float SpawnTimer;
    ks::uint32 MaxSpawnsPerWave;
};

INTERNAL void DrawMeteors(const ks::Sprite& sprite, const ks::DynamicArray<Meteor>& meteors)
{
    ks::Shader::Use(spriteShader);
    ks::Shader::SetUniform(spriteShader, (ks::string)"uProjection", projection);
    ks::Texture::Bind(sprite.Texture);
    ks::VertexArray::Bind(sprite.VertexArray);
    for(int i = 0; i < meteors.Count(); i++)
    {
        const Meteor& meteor = meteors[i];
        ks::Matrix3x3 model = ks::Matrix3x3::Translation(meteor.Position) * 
            ks::Matrix3x3::Rotation(meteor.Rotation * PI / 180);
        
        ks::Shader::SetUniform(spriteShader, (ks::string)"uPosition", model);
        ks::DrawElements(6);
    }
}

INTERNAL void DrawBackground(const ks::Sprite& sprite, ks::int32 startOffset)
{
    ks::Shader::Use(backgroundShader);
    ks::Shader::SetUniform(backgroundShader, (ks::string)"uProjection", projection);
    ks::Texture::Bind(sprite.Texture);
    ks::VertexArray::Bind(sprite.VertexArray);
    ks::Vec2 offset = { sprite.Width / 2.f, startOffset + sprite.Height / 2.f };
    
    for(ks::int32 height = startOffset; height < (ks::int32)WINDOW_HEIGHT; height += sprite.Height)
    {
        for(ks::uint32 width = 0; width < WINDOW_WIDTH; width += sprite.Width)
        {
            ks::Shader::SetUniform(backgroundShader, (ks::string)"uOffset", offset);
            ks::DrawElements(6);
            offset.X += sprite.Width;
        }
        
        offset.X = sprite.Width / 2.f;
        offset.Y += sprite.Height;
    }
}

INTERNAL void DrawSprites(const ks::FixedArray<ks::Sprite>& sprites, const ks::FixedArray<ks::Matrix3x3>& transforms)
{
    ks::Shader::Use(spriteShader);
    ks::Shader::SetUniform(spriteShader, (ks::string)"uProjection", projection);
    for (size_t i = 0; i < transforms.Count; i++)
    {
        ks::Shader::SetUniform(spriteShader, (ks::string)"uPosition", transforms[i]);
        ks::Texture::Bind(sprites[i].Texture);
        ks::VertexArray::Bind(sprites[i].VertexArray);
        ks::DrawElements(6);
    }
}

INTERNAL bool DoIntersect(const Entity& entity, const Entity& other)
{
    return (entity.Position - other.Position).Length() <=
        (entity.ColliderRadius + other.ColliderRadius);
}

enum class GameState
{
	MainMenue,
	Game,
	GameOver
};

enum class MenuButton
{
	Start,
	Quit,
	Retry
};

int main()
{
    ks::Window* pWindow = ks::PlatformCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 
                                                   (ks::stringUTF8)u8"Asteroids");
    
    ks::RenderState* renderState 
        = ks::InitRenderer(*pWindow, ks::RendererVersion::OPEN_GL_3_3);

	projection = ks::Matrix4x4::Orthographic(0.f, 0.f, WINDOW_WIDTH, WINDOW_HEIGHT,
		-0.1f, 100.f);

    ks::string vertexSource = ks::File::ReadAllText((ks::stringUTF8)u8"res/shaders/sprite.vert");
    ks::string fragSource = ks::File::ReadAllText((ks::stringUTF8)u8"res/shaders/sprite.frag");
    spriteShader = ks::Renderer::CreateShader(*renderState, vertexSource, fragSource);
    delete[] vertexSource;
    
    vertexSource = ks::File::ReadAllText((ks::stringUTF8)u8"res/shaders/background.vert");
    backgroundShader = ks::Renderer::CreateShader(*renderState, vertexSource, fragSource);
    delete[] vertexSource;
    delete[] fragSource;
    
    ks::TextRenderer textRenderer = {};
    textRenderer.VertexArray = ks::Renderer::CreateVertexArray(*renderState);
    ks::VertexArray::Bind(textRenderer.VertexArray);
    textRenderer.VertexBuffer = ks::Renderer::CreateVertexBuffer(*renderState, nullptr,
                                                                 96,
                                                                 ks::VertexBufferUsage::Dynamic);
	textRenderer.Projection = &projection;

    ks::VertexBufferLayoutElement layoutElements[] =
    {
        {0, false, 2},
        {1, false, 2}
    };
    
    ks::VertexBufferLayout layout = ks::VertexBufferLayout(layoutElements, 2);
    ks::VertexArray::ApplyVertexBufferLayout(textRenderer.VertexArray, layout); 
    
    vertexSource = ks::File::ReadAllText((ks::stringUTF8)u8"res/shaders/font.vert");
    fragSource = ks::File::ReadAllText((ks::stringUTF8)u8"res/shaders/font.frag");
    textRenderer.Shader = ks::Renderer::CreateShader(*renderState, vertexSource,
                                                    fragSource);
    delete[] vertexSource;
    delete[] fragSource;
    
    ks::FixedArray<ks::Sprite> sprites(2);
    sprites[0] = ks::Sprite::LoadFromFile((ks::stringUTF8)u8"res/sprites/playerShip1_blue.png", renderState);
    sprites[1] = ks::Sprite::LoadFromFile((ks::stringUTF8)u8"res/sprites/meteorBrown_med1.png", renderState);
    ks::Sprite background = ks::Sprite::LoadFromFile((ks::stringUTF8)u8"res/sprites/darkPurple.png", renderState);
    
#if KS_ENABLE_AUDIO == 1
    ks::AudioState* pAudio = ks::InitAudio();
    ks::AudioClip* pClip = ks::CreateAudioClip(L"res/sounds/background_music.wav", *pAudio, true);
    ks::PlayAudioClip(*pClip);
#endif
    
    Entity player =
    {
        { WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - sprites[0].Height },
        300,
        27.5
    };
    
    MeteorManager meteorManager =
    {
        ks::DynamicArray<Meteor>(10U),
        1.5,
        0,
        3
    };
    
    ks::int32 msLast = ks::Clock::Now();
    ks::int32 backgroundY = 0.f;
    ks::float32 backgroundScrollSpeed = 100.f;
    
#if KS_DRAW_DEBUG_COLLIDERS
    float circleData[] = { 0, 0 };
    
    vertexSource = ks::File::ReadAllText((ks::stringUTF8)u8"res/shaders/circle.vert");
    fragSource = ks::File::ReadAllText((ks::stringUTF8)u8"res/shaders/basic.frag");
    ks::string geometrySource = ks::File::ReadAllText((ks::stringUTF8)u8"res/shaders/circle.geom");
    ks::ShaderHandle circleShader = ks::Renderer::CreateShader(*renderState, vertexSource, fragSource, geometrySource);
    delete[] geometrySource;
    
    ks::VertexBufferHandle circleVB = ks::Renderer::CreateVertexBuffer(*renderState, circleData, sizeof(circleData), ks::VertexBufferUsage::Static);
    ks::VertexArrayHandle circleVA = ks::Renderer::CreateVertexArray(*renderState);
    ks::VertexArray::Bind(circleVA);
    
    ks::VertexBufferLayoutElement circleElement = { 0, false, 2 };
    ks::VertexBufferLayout circleLayout(&circleElement, 1);
    
    ks::VertexArray::ApplyVertexBufferLayout(circleVA, circleLayout);
#endif
    
    ks::Font* fontDefault = ks::Font::Load((ks::stringUTF8)u8"res/fonts/militech.ttf", 32);
	ks::Font* fontLarge = ks::Font::Load((ks::stringUTF8)u8"res/fonts/militech.ttf", 64);

    int   score = 0;
    std::string scoreStr = std::to_string(score);
    bool  scoreChanged = false;
	GameState currentState = GameState::MainMenue;
	MenuButton selectedButton = MenuButton::Start;
	ks::Color highlightColor = ks::Color{ 1, 1, 0, 1 };
	ks::Color selectedButtonColor = ks::Color{};

    while(g_IsRunning)
    {
        ks::int32 msNow = ks::Clock::Now();
        ks::float32 sEllapsedTime = (msNow - msLast) / 1000.f;
        msLast = msNow;
        
		ks::Input::Update();
		ks::PlatformPollEvents();

        if(ks::Input::IsKeyDown(ks::KeyCodes::Escape))
        {
            g_IsRunning = false;
        }

		backgroundY += backgroundScrollSpeed * sEllapsedTime;
		if (backgroundY >= background.Height)
		{
			backgroundY = 0.f;
		}

		if(currentState == GameState::Game)
		{
			ks::Vec2 movement = {};
			movement.X = ks::Input::IsKeyDown(ks::KeyCodes::D) - ks::Input::IsKeyDown(ks::KeyCodes::A);
			movement.Y = ks::Input::IsKeyDown(ks::KeyCodes::S) - ks::Input::IsKeyDown(ks::KeyCodes::W);
			movement.Normalize();

			player.Position += movement * sEllapsedTime * player.Speed;
			if (player.Position.X < sprites[0].Width / 2.f)
			{
				player.Position.X = sprites[0].Width / 2.f;
			}
			else if (player.Position.X > WINDOW_WIDTH - sprites[0].Width / 2.f)
			{
				player.Position.X = WINDOW_WIDTH - sprites[0].Width / 2.f;
			}

			if (player.Position.Y > WINDOW_HEIGHT - sprites[0].Height / 2.f)
			{
				player.Position.Y = WINDOW_HEIGHT - sprites[0].Height / 2.f;
			}
			else if (player.Position.Y < 475.f)
			{
				player.Position.Y = 475.f;
			}

			for (int i = 0; i < meteorManager.Meteors.Count(); i++)
			{
				meteorManager.Meteors[i].Update(sEllapsedTime, WINDOW_HEIGHT, sprites[1].Height / 2);
			}

			for (int i = 0; i < meteorManager.Meteors.Count(); i++)
			{
				if (meteorManager.Meteors[i].IsDead)
				{
					meteorManager.Meteors.UnorderedRemove(i);
					score++;
					scoreChanged = true;
				}
			}

			for (int i = 0; i < meteorManager.Meteors.Count(); i++)
			{
				const Meteor& meteor = meteorManager.Meteors[i];
				if (DoIntersect(player, meteor))
				{
					scoreStr = "Score: " + std::to_string(score);
					selectedButton = MenuButton::Retry;
					currentState = GameState::GameOver;
				}
			}

			meteorManager.SpawnTimer += sEllapsedTime;
			if (meteorManager.SpawnTimer >= meteorManager.SpawnCooldown)
			{
				int xMin = sprites[1].Width / 2;
				int xMax = WINDOW_WIDTH - sprites[1].Width / 2;
				int meteorsToSpawn = ks::Random::Range(1, meteorManager.MaxSpawnsPerWave);
				meteorManager.SpawnTimer = 0;

				for (int i = 0; i < meteorsToSpawn; i++)
				{
					Meteor meteor =
					{
						{
							{ 0, -150.f },
							175,
							20
						},
						0,
						100
					};


					bool intersectsOthers = true;
					while (intersectsOthers)
					{
						meteor.Position.X = ks::Random::Range(xMin, xMax);

						intersectsOthers = false;
						for (int j = 0; j < i; j++)
						{
							if (DoIntersect(meteor, meteorManager.Meteors[
								meteorManager.Meteors.Count() - (j + 1)]))
							{
								intersectsOthers = true;
								break;
							}
						}

					}

					meteorManager.Meteors.Add(meteor);
				}
			}

			ks::FixedArray<ks::Matrix3x3> transforms(1);
			transforms[0] = ks::Matrix3x3::Translation(player.Position) * ks::Matrix3x3::Scale(0.75f, 0.75f);

			ks::ClearBuffer();

			DrawBackground(background, -backgroundY);
			DrawMeteors(sprites[1], meteorManager.Meteors);
			DrawSprites(sprites, transforms);

#if KS_DRAW_DEBUG_COLLIDERS
			ks::Shader::Use(circleShader);
			ks::Shader::SetUniform(circleShader, (ks::string)"uVertices", 32U);
			ks::Shader::SetUniform(circleShader, (ks::string)"uProjection", projection);
			ks::Shader::SetUniform(circleShader, (ks::string)"uModel", ks::Matrix3x3::Translation(player.Position) * ks::Matrix3x3::Scale(player.ColliderRadius));
			ks::VertexArray::Bind(circleVA);
			glDrawArrays(GL_POINTS, 0, 1);

			for (int i = 0; i < meteorManager.Meteors.Count(); i++)
			{
				const Meteor& meteor = meteorManager.Meteors[i];
				ks::Shader::SetUniform(circleShader, (ks::string)"uModel", ks::Matrix3x3::Translation(meteor.Position) * ks::Matrix3x3::Scale(meteor.ColliderRadius));
				glDrawArrays(GL_POINTS, 0, 1);
			}

#endif
			if (scoreChanged)
			{
				scoreStr = std::to_string(score);
			}
			scoreChanged = false;

			ks::TextRenderer::RenderText(&textRenderer, WINDOW_WIDTH - 150, 50, "Score:", fontDefault, ks::Color::WHITE);
			ks::TextRenderer::RenderText(&textRenderer, WINDOW_WIDTH - 50, 50, scoreStr.c_str(), fontDefault, ks::Color::WHITE);
		}
		else if (currentState == GameState::MainMenue)
		{
			if(ks::Input::IsKeyPressed(ks::KeyCodes::Down) || ks::Input::IsKeyPressed(ks::KeyCodes::Up))
			{
				selectedButton = selectedButton == MenuButton::Start ? MenuButton::Quit : MenuButton::Start;
			}
			if(ks::Input::IsKeyPressed(ks::KeyCodes::Return))
			{
				if(selectedButton == MenuButton::Start)
				{
					currentState = GameState::Game;
				}
				else if(selectedButton == MenuButton::Quit)
				{
					g_IsRunning = false;
				}
			}

			float t = abs(sin(msNow * 0.0025));
			selectedButtonColor = ks::Color::WHITE * (1 - t) + t * highlightColor;

			ks::ClearBuffer();

			DrawBackground(background, -backgroundY);
			ks::TextRenderer::RenderTextCentered(&textRenderer, WINDOW_WIDTH / 2, 100, "Asteroids", fontLarge, ks::Color::WHITE);
			ks::Color textColor = selectedButton == MenuButton::Start ? selectedButtonColor : ks::Color::WHITE;
			ks::TextRenderer::RenderTextCentered(&textRenderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 32, "Start", fontLarge, textColor);
			textColor = selectedButton == MenuButton::Quit ? selectedButtonColor : ks::Color::WHITE;
			ks::TextRenderer::RenderTextCentered(&textRenderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 32, "Quit", fontLarge, textColor);
		}
		else if (currentState == GameState::GameOver)
		{
			if (ks::Input::IsKeyPressed(ks::KeyCodes::Down) || ks::Input::IsKeyPressed(ks::KeyCodes::Up))
			{
				selectedButton = selectedButton == MenuButton::Retry ? MenuButton::Quit : MenuButton::Retry;
			}
			if (ks::Input::IsKeyPressed(ks::KeyCodes::Return))
			{
				if (selectedButton == MenuButton::Retry)
				{
					meteorManager.Meteors.Clear();
					meteorManager.SpawnTimer = 0;
					player.Position = { (float)WINDOW_WIDTH / 2.f, (float)WINDOW_HEIGHT - sprites[0].Height };
					score = 0;
					scoreStr = std::to_string(score);
					currentState = GameState::Game;
				}
				else if (selectedButton == MenuButton::Quit)
				{
					g_IsRunning = false;
				}
			}

			float t = abs(sin(msNow * 0.0025));
			selectedButtonColor = ks::Color::WHITE * (1 - t) + t * highlightColor;

			ks::ClearBuffer();

			DrawBackground(background, -backgroundY);
			ks::TextRenderer::RenderTextCentered(&textRenderer, WINDOW_WIDTH / 2, 200, "Game Over!", fontLarge, ks::Color::WHITE);
			ks::TextRenderer::RenderTextCentered(&textRenderer, WINDOW_WIDTH / 2, 300, scoreStr.c_str(), fontLarge, ks::Color::WHITE);
			ks::Color textColor = selectedButton == MenuButton::Retry ? selectedButtonColor : ks::Color::WHITE;
			ks::TextRenderer::RenderTextCentered(&textRenderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 32, "Retry", fontLarge, textColor);
			textColor = selectedButton == MenuButton::Quit ? selectedButtonColor : ks::Color::WHITE;
			ks::TextRenderer::RenderTextCentered(&textRenderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 96, "Quit", fontLarge, textColor);
		}
        
        ks::SwapBuffers(*pWindow);
        
    }
    
    return 0;
}
