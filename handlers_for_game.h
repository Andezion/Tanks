#ifndef HANDLERS_FOR_GAME_H
#define HANDLERS_FOR_GAME_H

inline void sounds_handler(sf::Sound &move_sound, sf::Sound &rotate_sound, sf::Sound &shoot_sound,
                           sf::Sound &rico_sound, sf::Sound &supply1_sound, sf::Sound &supply2_sound,
                           sf::Sound &death_sound, sf::Sound &main_sound, sf::Sound &game_sound,
                           sf::SoundBuffer &move_buffer, sf::SoundBuffer &rotate_buffer, sf::SoundBuffer &shoot_buffer, sf::SoundBuffer &rico_buffer,
                           sf::SoundBuffer &supply1_buffer, sf::SoundBuffer &supply2_buffer, sf::SoundBuffer &dead_buffer,
                           sf::SoundBuffer &main_menu, sf::SoundBuffer &game_buffer)
{
    move_buffer.loadFromFile("Music/move.mp3");
    move_sound.setBuffer(move_buffer);
    move_sound.setVolume(40.0f);
    move_sound.setLoop(true);

    rotate_buffer.loadFromFile("Music/rotation.mp3");
    rotate_sound.setBuffer(rotate_buffer);
    rotate_sound.setVolume(10.0f);

    shoot_buffer.loadFromFile("Music/shoot_sound.mp3");
    shoot_sound.setBuffer(shoot_buffer);
    shoot_sound.setVolume(100.0f);

    rico_buffer.loadFromFile("Music/ricochet_sound.mp3");
    rico_sound.setBuffer(rico_buffer);
    rico_sound.setVolume(5.f);

    supply1_buffer.loadFromFile("Music/take_supply1.mp3");
    supply1_sound.setBuffer(supply1_buffer);
    supply1_sound.setVolume(200.0f);

    supply2_buffer.loadFromFile("Music/take_supply2.mp3");
    supply2_sound.setBuffer(supply2_buffer);
    supply2_sound.setVolume(200.0f);

    dead_buffer.loadFromFile("Music/death.wav");
    death_sound.setBuffer(dead_buffer);
    death_sound.setVolume(60.f);

    main_menu.loadFromFile("Music/start_menu.mp3");
    main_sound.setBuffer(main_menu);
    main_sound.setLoop(true);
    main_sound.setVolume(20.f);

    game_buffer.loadFromFile("Music/game_music.mp3");
    game_sound.setBuffer(game_buffer);
    main_sound.setLoop(true);
    game_sound.setVolume(10.f);
}

inline void handle_sounds_of_tank(const bool moving_now, const bool rotating_now,
                                  sf::Sound &rotate_sound, sf::Sound &move_sound)
{
    if(moving_now == true)
    {
        rotate_sound.stop();
        move_sound.play();
    }
    if(moving_now == false)
    {
        move_sound.stop();
    }
    if(rotating_now == true)
    {
        move_sound.stop();
        rotate_sound.play();
    }
    if(rotating_now == false)
    {
        rotate_sound.stop();
    }
}

inline int did_i_clicked_on_sound(const sf::Vector2f pos)
{
    if(pos.x >= 10 && pos.y >= 10 && pos.x <= 60 && pos.y <= 60)
    {
        return 1;
    }
    return 0;
}

inline int play_sound = 1;
inline void start_screen_handler(sf::RenderWindow& window, std::unique_ptr<tank>& player,
                                 bool& selected, const int x_player, const int y_player)
{
    sf::Event event{};
    while (window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            player = std::make_unique<tank_small>(static_cast<float>(x_player) * 100, static_cast<float>(y_player) * 100, 0);
            selected = true;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            player = std::make_unique<tank_medium>(static_cast<float>(x_player) * 100, static_cast<float>(y_player) * 100, 0);
            selected = true;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            player = std::make_unique<tank_big>(static_cast<float>(x_player) * 100, static_cast<float>(y_player) * 100, 0);
            selected = true;
        }

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f pos = window.mapPixelToCoords(pixelPos);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && did_i_clicked_on_sound(pos))
        {
            if(play_sound == 0)
            {
                play_sound = 1;
            }
            else
            {
                play_sound = 0;
            }
        }
    }
}

inline void handle_shooting(const std::unique_ptr<tank>& player, std::vector<bullet>& bullets,
                            sf::Sound& shoot_sound, sf::Clock& shoot_cooldown)
{
    if(play_sound == 1)
    {
        shoot_sound.play();
    }
    player->update_ammo();
    const float angle_radians = player->get_rotation_player() * (3.14159265f / 180.0f);

    const float bullet_speed = player->get_speed_of_bullet();

    float dx = bullet_speed * std::cos(angle_radians);
    float dy = bullet_speed * std::sin(angle_radians);

    bullets.emplace_back(player->get_position().x, player->get_position().y,
        dx, dy, player->get_color(), player->get_what_type(),
        player->get_collision(), player->get_radius(), 1);

    shoot_cooldown.restart();
}

inline int check(const float x, const float y)
{
    if(x < 0 || x + 5 > 1600 || y < 0 || y + 5 > 900)
    {
        return 1;
    }
    return 0;
}

inline bool can_move(const tank& object, const Cell maze[width][height], const sf::Vector2f& direction)
{
    const float tankWidth = object.get_width();
    const float tankHeight = object.get_height();

    const float x = object.get_position().x;
    const float y = object.get_position().y;

    const float futureX = x + direction.x;
    const float futureY = y + direction.y;

    const int i = static_cast<int>(futureX / cellSize);
    const int j = static_cast<int>(futureY / cellSize);

    constexpr float safeDistance = 5.0f;

    const float cellLeft = static_cast<float>(i) * cellSize;
    const float cellRight = cellLeft + cellSize;
    const float cellTop = static_cast<float>(j) * cellSize;
    const float cellBottom = cellTop + cellSize;

    if (maze[i][j].Left == Close && futureX - tankWidth / 2 < cellLeft + lineThickness + safeDistance)
    {
        return false;
    }
    if (maze[i][j].Right == Close && futureX + tankWidth / 2 > cellRight - lineThickness - safeDistance)
    {
        return false;
    }
    if (maze[i][j].Top == Close && futureY - tankHeight / 2 < cellTop + lineThickness + safeDistance)
    {
        return false;
    }
    if (maze[i][j].Bottom == Close && futureY + tankHeight / 2 > cellBottom - lineThickness - safeDistance)
    {
        return false;
    }

    const float tankHalfWidth = tankWidth / 2;
    const float tankHalfHeight = tankHeight / 2;

    if (direction.x != 0 || direction.y != 0)
    {
        if (direction.x > 0 || direction.y > 0)
        {
            if (maze[i][j].Top == Close && futureY - tankHalfHeight < cellTop + lineThickness + safeDistance)
            {
                return false;
            }
            if (maze[i][j].Left == Close && futureX - tankHalfWidth < cellLeft + lineThickness + safeDistance)
            {
                return false;
            }
        }

        else if (direction.x < 0 || direction.y < 0)
        {
            if (maze[i][j].Bottom == Close && futureY + tankHalfHeight > cellBottom - lineThickness - safeDistance)
            {
                return false;
            }
            if (maze[i][j].Right == Close && futureX + tankHalfWidth > cellRight - lineThickness - safeDistance)
            {
                return false;
            }
        }
    }

    return true;
}

inline void handle_moving(const std::unique_ptr<tank>& player, Cell labyrinth[width][height], bool& moving_now, bool& rotating_now)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        rotating_now = true;
        moving_now = false;

        player->rotate(player->get_rotation_speed());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        rotating_now = true;
        moving_now = false;

        player->rotate(-player->get_rotation_speed());
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        moving_now = true;
        rotating_now = false;

        if(player->get_position().x > 0 && player->get_position().y > 0 &&
            player->get_position().x < windowWidth && player->get_position().y < windowHeight)
        {
            sf::Vector2f direction = {static_cast<float>(std::cos(player->get_rotation_player() * M_PI / 180.0f) * 2),
                                      static_cast<float>(std::sin(player->get_rotation_player() * M_PI / 180.0f) * 2)};
            if (can_move(*player, labyrinth, direction))
            {
                player->move(player->get_speed_of_tank(), player->get_rotation_player());
            }
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        moving_now = true;
        rotating_now = false;

        if(player->get_position().x > 0 && player->get_position().y > 0 &&
            player->get_position().x < windowWidth && player->get_position().y < windowHeight)
        {
            sf::Vector2f direction = {static_cast<float>(std::cos(player->get_rotation_player() * M_PI / 180.0f) * 2),
                                      static_cast<float>(std::sin(player->get_rotation_player() * M_PI / 180.0f) * 2)};
            if (can_move(*player, labyrinth, direction))
            {
                player->move(-player->get_speed_of_tank(), player->get_rotation_player());
            }
        }
    }
}

inline bool hit(const sf::FloatRect& rect1, const sf::FloatRect& rect2)
{
    return rect1.intersects(rect2);
}

inline std::pair<int, int> get_random_position(const std::vector<std::pair<int, int>>& available_positions, std::mt19937& gen)
{
    std::uniform_int_distribution<size_t> dist(0, available_positions.size() - 1);
    return available_positions[dist(gen)];
}

inline void supply_handler(std::vector<supplies> &active_supplies,
                    const std::vector<std::pair<int, int>>& available_positions,
                    std::mt19937 &gen, sf::Clock &spawn_timer, sf::Clock &lifetime_timer)
{
    if (spawn_timer.getElapsedTime().asSeconds() >= 20.0f)
    {
        if (!available_positions.empty())
        {
            auto [fst, snd] = get_random_position(available_positions, gen);

            std::uniform_int_distribution type_dist(0, 4);
            int type = type_dist(gen);

            active_supplies.emplace_back(fst * 100, snd * 100, type);

            spawn_timer.restart();
            lifetime_timer.restart();
        }
    }
    if (lifetime_timer.getElapsedTime().asSeconds() >= 10.0f && !active_supplies.empty())
    {
        active_supplies.clear();
        lifetime_timer.restart();
    }
}

inline void start_screen(sf::RenderWindow &window, const sf::Font &font, const sf::Sprite &start_photo)
{
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);
    text.setString("Hello tankers!");
    text.setPosition(650, 100);

    sf::Text pick, info, more_info, shoot_info;

    pick.setFont(font);
    pick.setCharacterSize(30);
    pick.setFillColor(sf::Color::White);
    pick.setString("To start game you should pick a tank!");
    pick.setPosition(550, 180);

    info.setFont(font);
    info.setCharacterSize(20);
    info.setFillColor(sf::Color::White);
    info.setString("W - up | S - down | A - left | D - right");
    info.setPosition(50, 550);

    more_info.setFont(font);
    more_info.setCharacterSize(20);
    more_info.setFillColor(sf::Color::White);
    more_info.setString("Press Esc to close game");
    more_info.setPosition(80, 680);

    shoot_info.setFont(font);
    shoot_info.setCharacterSize(20);
    shoot_info.setFillColor(sf::Color::White);
    shoot_info.setString("Press Space to shoot");
    shoot_info.setPosition(95, 360);

    sf::Texture esc_texture;
    sf::Sprite esc_sprite;
    esc_texture.loadFromFile("Photo/esc.png");
    esc_sprite.setTexture(esc_texture);
    esc_sprite.setScale(0.15f, 0.15f);
    esc_sprite.setPosition(145, 600);

    sf::Texture space_texture;
    sf::Sprite space_sprite;
    space_texture.loadFromFile("Photo/space.png");
    space_sprite.setTexture(space_texture);
    space_sprite.setScale(0.5f, 0.5f);
    space_sprite.setPosition(125, 280);

    sf::Text tank_pick1, tank_pick2, tank_pick3;

    tank_pick1.setFont(font);
    tank_pick1.setCharacterSize(30);
    tank_pick1.setFillColor(sf::Color::White);
    tank_pick1.setString("Press 1 - to pick fast but weak tank");
    tank_pick1.setPosition(560, 280);

    tank_pick2.setFont(font);
    tank_pick2.setCharacterSize(30);
    tank_pick2.setFillColor(sf::Color::White);
    tank_pick2.setString("Press 2 - to pick medium tank");
    tank_pick2.setPosition(590, 340);

    tank_pick3.setFont(font);
    tank_pick3.setCharacterSize(30);
    tank_pick3.setFillColor(sf::Color::White);
    tank_pick3.setString("Press 3 - to pick slow but strong tank");
    tank_pick3.setPosition(550, 400);

    sf::Texture key_texture;
    sf::Sprite key_sprite;
    key_texture.loadFromFile("Photo/keys.png");
    key_sprite.setTexture(key_texture);
    key_sprite.setScale(0.75f, 0.75f);
    key_sprite.setPosition(100, 400);

    sf::Texture off_sound_texture;
    sf::Sprite off_sound_sprite;
    off_sound_texture.loadFromFile("Photo/off_sound.png");
    off_sound_sprite.setTexture(off_sound_texture);
    off_sound_sprite.setScale(0.1f, 0.1f);
    off_sound_sprite.setPosition(10.f, 10.f);

    sf::Texture on_sound_texture;
    sf::Sprite on_sound_sprite;
    on_sound_texture.loadFromFile("Photo/on_sound.png");
    on_sound_sprite.setTexture(on_sound_texture);
    on_sound_sprite.setScale(0.05f, 0.05f);
    on_sound_sprite.setPosition(10.f, 10.f);

    window.clear();

    window.draw(start_photo);
    window.draw(text);
    window.draw(pick);
    window.draw(key_sprite);
    window.draw(info);
    window.draw(esc_sprite);
    window.draw(more_info);
    window.draw(space_sprite);
    window.draw(shoot_info);
    window.draw(tank_pick1);
    window.draw(tank_pick2);
    window.draw(tank_pick3);

    if(play_sound == 1)
    {
        window.draw(on_sound_sprite);
    }
    else
    {
        window.draw(off_sound_sprite);
    }

    window.display();
}

#endif //HANDLERS_FOR_GAME_H
