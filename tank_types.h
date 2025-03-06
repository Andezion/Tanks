#ifndef TANK_TYPES_H
#define TANK_TYPES_H

class tank
{
    float speed_of_bullet;
    float speed_of_tank;
    float damage_of_tank;
    float health_of_tank;
    float cooldown;
    float rotate_speed;

    float x_cord;
    float y_cord;
    float for_bullet;

    int tank_type;
    int color;
    int ammo;
    int collision_number;

    sf::RectangleShape health;
    sf::RectangleShape back;
public:
    sf::RectangleShape player;
    sf::RectangleShape trunk;
    sf::RectangleShape left;
    sf::RectangleShape right;

    sf::Texture dead_texture;
    sf::Sprite dead_sprite;

    tank(const float x, const float y, const int type, const int what_tank,
         const float player_width, const float player_height,
         const float both_width, const float both_height,
         const float trunk_width, const float trunk_height,
         const float speed_tank, const float speed_bullet,
         const float damage_tank, const float health_tank,
         const float cooldown_tank, const float rotate_value,
         const int ammo_number)
    {
        ammo = ammo_number;
        color = type;
        tank_type = what_tank;
        x_cord = x;
        y_cord = y;

        speed_of_bullet = speed_bullet;
        speed_of_tank = speed_tank;
        damage_of_tank = damage_tank;
        health_of_tank = health_tank;
        rotate_speed = rotate_value;
        cooldown = cooldown_tank;

        dead_texture.loadFromFile("Photo/grave.png");
        dead_sprite.setTexture(dead_texture);
        dead_sprite.setScale(0.2f, 0.2f);
        dead_sprite.setPosition(x_cord - 60.0f, y_cord - 60.0f);

        if(tank_type == 0)
        {
            health = create_bar(health_of_tank, sf::Color::Red, {x - 25.0f, y - 38.0f}, 2);
            back = create_bar(60, sf::Color::White, {x - 30.0f, y - 40.0f}, 1);

            for_bullet = 5.0f;
            collision_number = 10;
        }
        else if(tank_type == 1)
        {
            health = create_bar(health_of_tank, sf::Color::Red, {x - 40.0f, y - 44.0f}, 2);
            back = create_bar(85, sf::Color::White, {x - 45.0f, y - 46.0f}, 1);

            for_bullet = 7.0f;
            collision_number = 7;
        }
        else if(tank_type == 2)
        {
            health = create_bar(health_of_tank, sf::Color::Red, {x - 48.0f, y - 54.0f}, 2);
            back = create_bar(115, sf::Color::White, {x - 55.0f, y - 56.0f}, 1);

            for_bullet = 9.0f;
            collision_number = 4;
        }

        player.setSize(sf::Vector2f(player_width, player_height));
        player.setOrigin(player_width / 2, player_height / 2);
        player.setFillColor(get_color_by_type(color));
        player.setPosition(x, y);

        trunk.setSize(sf::Vector2f(trunk_width, trunk_height));
        trunk.setOrigin(0, trunk_height / 2);
        trunk.setFillColor(get_color_by_type(color));

        left.setSize(sf::Vector2f(both_width, both_height));
        left.setOrigin(both_width / 2, both_height / 2);
        left.setFillColor(get_color_by_type(color));
        left.setOutlineColor(sf::Color::Black);
        left.setOutlineThickness(1.0f);

        right.setSize(sf::Vector2f(both_width, both_height));
        right.setOrigin(both_width / 2, both_height / 2);
        right.setFillColor(get_color_by_type(color));
        right.setOutlineColor(sf::Color::Black);
        right.setOutlineThickness(1.0f);

        tank::update_positions();
    }

    virtual ~tank() = default;

    virtual float get_width() const = 0;
    virtual float get_height() const = 0;

    void move(const float dx, const float angle)
    {
        const float radian_angle = angle * (3.14159265f / 180.0f);
        const float delta_x = dx * std::cos(radian_angle);
        const float delta_y = dx * std::sin(radian_angle);

        player.move(delta_x, delta_y);

        const sf::Vector2f new_position = player.getPosition();

        if(tank_type == 0)
        {
            health.setPosition(new_position.x - 25.0f, new_position.y - 38.0f);
            back.setPosition(new_position.x - 30.0f, new_position.y - 40.0f);
        }
        else if(tank_type == 1)
        {
            health.setPosition(new_position.x - 40.0f, new_position.y - 44.0f);
            back.setPosition(new_position.x - 45.0f, new_position.y - 46.0f);
        }
        else if(tank_type == 2)
        {
            health.setPosition(new_position.x - 48.0f, new_position.y - 54.0f);
            back.setPosition(new_position.x - 55.0f, new_position.y - 56.0f);
        }

        dead_sprite.setPosition(new_position.x - 60.0f, new_position.y - 60.0f);

        update_positions();
    }
    void rotate(const float delta_angle)
    {
        player.rotate(delta_angle);
        update_positions();
    }

    float get_speed_of_tank() const { return speed_of_tank; }
    float get_speed_of_bullet() const { return speed_of_bullet; }
    float get_damage_of_tank() const { return damage_of_tank; }
    float get_health_of_tank() const { return health_of_tank; }
    float get_cooldown() const { return cooldown; }
    float get_radius() const { return for_bullet; }
    int get_ammo() const { return ammo; }
    int get_collision() const { return collision_number; }
    float get_rotation_speed() const { return rotate_speed; }

    void set_radius(const float rad) { for_bullet = rad; }
    void set_collision(const int value) { collision_number = value; }
    void set_speed_of_tank(const float speed) { speed_of_tank = speed; }
    void set_speed_of_bullet(const float speed) { speed_of_bullet = speed; }
    void set_damage_of_tank(const float damage) { damage_of_tank = damage; }
    void set_ammo_of_tank(const int value) { ammo = value; }
    void set_cooldown(const float new_cooldown) { cooldown = new_cooldown; }
    void set_rotation_speed(const float new_rotation_speed) { rotate_speed = new_rotation_speed; }

    void update_health(const float amount) // to change
    {
        if(amount > health_of_tank)
        {
            health_of_tank = 0.0f;
        }
        if (health_of_tank - amount > 0 && health_of_tank - amount <= 100)
        {
            health_of_tank -= amount;
        }
        health.setSize(sf::Vector2f(health_of_tank, health.getSize().y));
    }
    void update_ammo()
    {
        ammo--;
    }

    void draw(sf::RenderWindow& window) const
    {
        window.draw(back);
        window.draw(health);
        window.draw(player);
        window.draw(trunk);
        window.draw(left);
        window.draw(right);
    }
    void draw_dead(sf::RenderWindow& window) const
    {
        window.draw(dead_sprite);
    }

    virtual void update_positions()
    {
        update_trunk_position();
        update_left();
        update_right();
    }
    sf::FloatRect getGlobalBounds() const
    {
        return player.getGlobalBounds();
    }
    sf::Vector2f get_position() const
    {
        return trunk.getPosition();
    }
    float get_rotation_player() const
    {
        return player.getRotation();
    }

    static sf::Color get_color_by_type(const int type)
    {
        switch (type)
        {
            case 0: return sf::Color::Green;
            case 1: return sf::Color::Red;
            default: return sf::Color::Blue;
        }
    }
    static sf::RectangleShape create_bar(const float size, const sf::Color color, const sf::Vector2f position, const int type)
    {
        sf::RectangleShape bar;
        if (type == 1)
        {
            bar.setSize(sf::Vector2f(size, 9.0f));
        }
        else
        {
            bar.setSize(sf::Vector2f(size, 5.0f));
        }

        bar.setPosition(position);
        bar.setFillColor(color);
        bar.setOutlineColor(sf::Color::Black);
        bar.setOutlineThickness(1);

        return bar;
    }

    void update_trunk_position()
    {
        const float angle_radians = player.getRotation() * (3.14159265f / 180.0f);

        trunk.setPosition(
            player.getPosition().x + std::cos(angle_radians) * (player.getSize().x / 2),
            player.getPosition().y + std::sin(angle_radians) * (player.getSize().x / 2)
        );
        trunk.setRotation(player.getRotation());
    }
    void update_left()
    {
        const float angle_radians = player.getRotation() * (3.14159265f / 180.0f);
        const float perpendicular_angle_radians = angle_radians + (3.14159265f / 2);

        left.setPosition(
            player.getPosition().x + std::cos(perpendicular_angle_radians) * (player.getSize().y / 2 * 1.3f),
            player.getPosition().y + std::sin(perpendicular_angle_radians) * (player.getSize().y / 2 * 1.3f)
        );
        left.setRotation(player.getRotation());
    }
    void update_right()
    {
        const float angle_radians = player.getRotation() * (3.14159265f / 180.0f);
        const float perpendicular_angle_radians = angle_radians - (3.14159265f / 2);

        right.setPosition(
            player.getPosition().x + std::cos(perpendicular_angle_radians) * (player.getSize().y / 2 * 1.3f),
            player.getPosition().y + std::sin(perpendicular_angle_radians) * (player.getSize().y / 2 * 1.3f)
        );
        right.setRotation(player.getRotation());
    }

    float get_health() const
    {
        return health_of_tank;
    }
    int get_what_type() const
    {
        return tank_type;
    }
    int get_color() const
    {
        return color;
    }

    void applyPowerUp(const int type, const float value)
    {
        if (type == 0)
        {
            update_health(value);
        }
        else if(type == 1)
        {
            set_collision(get_collision() * 2);
        }
        else if (type == 2)
        {
            set_speed_of_tank(get_speed_of_tank() * 2);
            set_speed_of_bullet(get_speed_of_bullet() * 2);
            set_cooldown(get_cooldown() / 2);
            set_rotation_speed(get_rotation_speed() * 2);
        }
        else if (type == 3)
        {
            set_damage_of_tank(2 * get_damage_of_tank());
            set_radius(get_radius() * 2);
        }
        else if (type > 3)
        {
            set_ammo_of_tank(get_ammo() + 10);
        }
    }
};

class tank_small final : public tank
{
public:
    static constexpr float PLAYER_WIDTH = 30.0f;
    static constexpr float PLAYER_HEIGHT = 20.0f;

    tank_small(const float x, const float y, const int type) :
    tank(x, y, type, 0,
        PLAYER_WIDTH, PLAYER_HEIGHT, 40.0f, 7.0f, 15.0f, 5.0f,
        2.0f, 1.0f, 0.1f, 50.0f, 1.0f, 2.0, 20) {}

    float get_width() const override
    {
        return PLAYER_WIDTH;
    }
    float get_height() const override
    {
        return PLAYER_HEIGHT;
    }
};

class tank_medium final : public tank
{
public:
    static constexpr float PLAYER_WIDTH = 45.0f;
    static constexpr float PLAYER_HEIGHT = 25.0f;

    tank_medium(const float x, const float y, const int type) :
    tank(x, y, type, 1,
        PLAYER_WIDTH, PLAYER_HEIGHT, 60.0f, 10.0f, 30.0f, 8.0f,
        1.5f, 0.9f, 0.3f, 75.0f, 0.7f, 1.5, 15) {}

    float get_width() const override
    {
        return PLAYER_WIDTH;
    }
    float get_height() const override
    {
        return PLAYER_HEIGHT;
    }
};

class tank_big final : public tank
{
public:
    static constexpr float PLAYER_WIDTH = 60.0f;
    static constexpr float PLAYER_HEIGHT = 35.0f;

    tank_big(const float x, const float y, const int type) :
    tank(x, y, type ,2,
        60.0f, 35.0f, 75.0f, 13.0f, 45.0f, 10.0f,
        1.0f, 0.8f, 0.5f, 100.0f, 0.4f, 1.0, 10) {}

    float get_width() const override
    {
        return PLAYER_WIDTH;
    }
    float get_height() const override
    {
        return PLAYER_HEIGHT;
    }
};

#endif //TANK_TYPES_H
