#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "supplies.h"
#include "tank_types.h"
#include "bullet_types.h"
#include "maze.h"
#include "handlers_for_game.h"
#include "maze_solver.h"
#include "clever_collision.h"

int get_number(const int min, const int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution distrib(min / 2, max / 2);
    return distrib(gen) * 2 + 1;
}
std::pair<int, int> get_pair(std::vector<std::pair<int, int>>& vec, std::mt19937& gen)
{
    std::uniform_int_distribution<> dist(0, static_cast<int>(vec.size()) - 1);
    const int index = dist(gen);
    return vec[index];
}
bool take_supply(const tank& player, const supplies& supply)
{
    return player.getGlobalBounds().intersects(supply.get_global_bounds());
}

int clever_intersection(const sf::CircleShape& player, const sf::CircleShape& enemy)
{
    return player.getGlobalBounds().intersects(enemy.getGlobalBounds());
}

int main()
{
    Cell labyrinth[width][height];
    generate_labyrinth(labyrinth);

    std::set<std::pair<int, int>> unique_pairs;
    int num_pairs = 32;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    while (unique_pairs.size() < num_pairs)
    {
        int x = get_number(1, 15);
        int y = get_number(1, 7);
        unique_pairs.insert({x, y});
    }

    std::vector available_positions(unique_pairs.begin(), unique_pairs.end());

    std::vector<supplies> active_supplies;
    sf::Clock spawn_timer;
    sf::Clock lifetime_timer;

    std::unique_ptr<tank> player;
    auto [x_player, y_player] = get_pair(available_positions, gen);

    std::vector<bullet> bullets;

    auto [x_enemy, y_enemy] = get_pair(available_positions, gen);
    tank_small enemy(static_cast<float>(x_enemy) * 100, static_cast<float>(y_enemy) * 100, 2);

    std::vector<bullet> enemy_bullet;

    sf::Clock shoot_cooldown;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Tanchiki");

    sf::SoundBuffer move_buffer, rotate_buffer, shoot_buffer, rico_buffer, supply1_buffer,
                    supply2_buffer, dead_buffer, main_menu, game_buffer;
    sf::Sound move_sound, rotate_sound, shoot_sound, rico_sound, supply1_sound,
              supply2_sound, death_sound, main_sound, game_sound;
    sounds_handler(move_sound, rotate_sound, shoot_sound, rico_sound, supply1_sound, supply2_sound,
        death_sound, main_sound, game_sound,
        move_buffer, rotate_buffer, shoot_buffer, rico_buffer, supply1_buffer,
                    supply2_buffer, dead_buffer, main_menu, game_buffer);

    int is_dead_player = 0; 
    int is_dead_enemy = 0;
    int play_music_for_player = 0;
    int play_music_for_enemy = 0;

    sf::Font font;
    font.loadFromFile("Fonts/Roboto-Medium.ttf");

    sf::Text text, cooldown_text;
    text.setFont(font);
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::White);
    text.setPosition(10.0f, 0.0f);

    cooldown_text.setFont(font);
    cooldown_text.setCharacterSize(20);
    cooldown_text.setFillColor(sf::Color::White);
    cooldown_text.setPosition(500.0f, 0.0f);

    sf::Texture start_texture;
    sf::Sprite start_sprite;
    start_texture.loadFromFile("Photo/tank.png");
    start_sprite.setTexture(start_texture);

    main_sound.play();
    bool selected = false;
    bool is_playing = true;
    while (window.isOpen() && !selected)
    {
        start_screen_handler(window, player, selected, x_player, y_player);
        start_screen(window, font, start_sprite);

        if(play_sound == 0 && is_playing)
        {
            main_sound.pause();
            is_playing = false;
        }
        else if(play_sound == 1 && !is_playing)
        {
            main_sound.play();
            is_playing = true;
        }
    }

    main_sound.stop();
    if(play_sound == 1)
    {
        game_sound.play();
    }

    circle_for_collision for_player(player->get_position().x, player->get_position().y);
    circle_for_collision for_enemy(enemy.get_position().x, enemy.get_position().y);

    while(window.isOpen())
    {
        bool moving_now = false;
        bool rotating_now = false;

        sf::Event event{};
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }

            if(player->get_health() > 0)
            {
                handle_moving(player, labyrinth, moving_now, rotating_now);
                for_player.set_position(player->get_position().x, player->get_position().y);

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
                    shoot_cooldown.getElapsedTime().asSeconds() >= player->get_cooldown() && player->get_ammo() > 0)
                {
                    moving_now = false;
                    rotating_now = false;

                    handle_shooting(player, bullets, shoot_sound, shoot_cooldown);
                }

                if(!sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
                    !sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
                    !sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
                    !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                {
                    moving_now = false;
                    rotating_now = false;
                }

                if(play_sound == 1)
                {
                    handle_sounds_of_tank(moving_now, rotating_now, rotate_sound, move_sound);
                }
            }
            else
            {
                is_dead_player = 1;
                if(play_music_for_player == 0)
                {
                    if(play_sound == 1)
                    {
                        death_sound.play();
                    }
                    play_music_for_player = 1;
                }
            }

            if(clever_intersection(for_player.get_circle(), for_enemy.get_circle()))
            {
                enemy.move(enemy.get_speed_of_tank(), enemy.get_rotation_player());
                for_enemy.set_position(enemy.get_position().x, enemy.get_position().y);
            }

            if(enemy.get_health() == 0.0f)
            {
                is_dead_enemy = 1;
                if(play_music_for_enemy == 0)
                {
                    if(play_sound == 1)
                    {
                        death_sound.play();
                    }
                    play_music_for_enemy = 1;
                }
            }
        }

        supply_handler(active_supplies, available_positions, gen, spawn_timer, lifetime_timer);
        std::stringstream ss;
        ss << std::fixed << std::setprecision(3) << spawn_timer.getElapsedTime().asSeconds();
        cooldown_text.setString(ss.str());

        std::erase_if(bullets, [](const bullet& b)
        {
            return b.counter == 0;
        });

        for (auto& bullet : bullets)
        {
            if(bullet.update(labyrinth))
            {
                if(play_sound == 1)
                {
                    rico_sound.play();
                }
            }

            if (hit(bullet.getGlobalBounds(), enemy.getGlobalBounds()) && bullet.get_should_hit() == 1)
            {
                enemy.update_health(player->get_damage_of_tank());
            }
        }
        for (auto& bullet : enemy_bullet)
        {
            if(bullet.update(labyrinth))
            {
                if(play_sound == 1)
                {
                    rico_sound.play();
                }
            }

            if (hit(bullet.getGlobalBounds(), player->getGlobalBounds()) && bullet.get_should_hit() == 1)
            {
                player->update_health(enemy.get_damage_of_tank());
            }
        }

        text.setString(std::to_string(player->get_ammo()));

        window.clear();
        draw_labyrinth(window, labyrinth);

        for (auto& supply : active_supplies)
        {
            if(take_supply(*player, supply))
            {
                supply.set_show();
                player->applyPowerUp(supply.get_type(), 10);

                if(play_sound == 1)
                {
                    supply1_sound.play();
                }
            }
            else
            {
                supply.draw(window);
            }
        }

        if(is_dead_player)
        {
            player->draw_dead(window);
        }
        else
        {
            player->draw(window);
        }

        if(is_dead_enemy)
        {
            enemy.draw_dead(window);
        }
        else
        {
            enemy.draw(window);
        }

        for (const auto& bullet : bullets)
        {
            window.draw(bullet.shot);
        }
        for(const auto& bullet : enemy_bullet)
        {
            bullet.draw(window);
        }

        window.draw(text);
        window.draw(cooldown_text);
        for_player.draw(window);
        for_enemy.draw(window);

        window.display();
    }
    game_sound.stop();

    return 0;
}
