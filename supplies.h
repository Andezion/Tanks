#ifndef SUPPLIES_H
#define SUPPLIES_H

class supplies
{
public:
    sf::RectangleShape rect;

    float x;
    float y;

    int type_of_supply;
    int show_or_not;

    sf::Texture supply_texture;
    sf::Sprite supply_sprite;

    supplies(const float dx, const float dy, const int type)
    {
        x = dx;
        y = dy;

        type_of_supply = type;
        show_or_not = 1;

        supply_texture.loadFromFile(get_filename_by_type(type));
        supply_sprite.setTexture(supply_texture);
        supply_sprite.setPosition(sf::Vector2f(x, y));
        supply_sprite.setScale(sf::Vector2f(2, 2));
    }

    void set_show()
    {
        show_or_not = 0;
    }

    int get_show() const
    {
        return show_or_not;
    }

    static std::string get_filename_by_type(const int type)
    {
        switch (type)
        {
            case 0: return "Photo/health.png";
            case 1: return "Photo/collision.png";
            case 2: return "Photo/speed.png";
            case 3: return "Photo/damage.png";
            default: return "Photo/ammo.png";
        }
    }

    int get_type() const
    {
        return type_of_supply;
    }

    sf::Vector2f get_position() const
    {
        return supply_sprite.getPosition();
    }
    sf::FloatRect get_global_bounds() const
    {
        return supply_sprite.getGlobalBounds();
    }

    void draw(sf::RenderWindow& window) const
    {
        if(get_show())
        {
            window.draw(supply_sprite);
        }
    }

    void set_position(const float dx, const float dy)
    {
        supply_sprite.setPosition(sf::Vector2f(dx, dy));
    }
};

#endif //SUPPLIES_H
