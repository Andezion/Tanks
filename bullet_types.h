#ifndef BULLET_TYPES_H
#define BULLET_TYPES_H
#include "maze.h"

class bullet final
{
public:
    sf::CircleShape shot;

    float dx;
    float dy;

    float radius;
    int counter;
    int hit;

    bullet(const float x, const float y, const float ddx, const float ddy,
        const int type_of_player, const int type_of_tank, const int coll_number,
        const float rad, const int should_hit)
    {
        dx = ddx;
        dy = ddy;

        radius = rad;
        counter = coll_number;
        hit = should_hit;

        shot.setRadius(radius);
        shot.setPosition(x, y);
        shot.setOrigin(shot.getRadius(), shot.getRadius());
        shot.setFillColor(get_color_by_type(type_of_player));
    }

    ~bullet() = default;

    static sf::Color get_color_by_type(const int type)
    {
        switch (type)
        {
            case 0: return sf::Color::Green;
            case 1: return sf::Color::Red;
            default: return sf::Color::Blue;
        }
    }
    static float get_radius_by_type(const int type)
    {
        switch (type)
        {
            case 0: return 5.0f;
            case 1: return 7.0f;
            default: return 9.0f;
        }
    }
    static int get_collision_by_type(const int type)
    {
        switch (type)
        {
            case 0: return 10;
            case 1: return 7;
            default: return 4;
        }
    }

    int get_should_hit() const
    {
        return hit;
    }

    sf::FloatRect getGlobalBounds() const
    {
        return shot.getGlobalBounds();
    }

    sf::Vector2f get_pos() const
    {
        return shot.getPosition();
    }

    int update(Cell labyrinth[width][height])
    {
        const int res = handle_labyrinth_collision(labyrinth);

        shot.move(dx, dy);

        return res;
    }

    void draw(sf::RenderWindow& window) const
    {
        window.draw(shot);
    }

    int handle_labyrinth_collision(Cell labyrinth[width][height])
    {
        const sf::Vector2f pos = shot.getPosition();

        const int cellX = static_cast<int>(pos.x) / cellSize;
        const int cellY = static_cast<int>(pos.y) / cellSize;

        if (cellX < 0 || cellX >= width || cellY < 0 || cellY >= height)
        {
            return 0;
        }

        const Cell& cell = labyrinth[cellX][cellY];

        const float cellLeft = cellX * cellSize;
        const float cellRight = (cellX + 1) * cellSize;
        const float cellTop = cellY * cellSize;
        const float cellBottom = (cellY + 1) * cellSize;

        if (cell.Top == Close && pos.y - shot.getRadius() <= cellTop)
        {
            radius = radius - 0.5f;
            counter--;
            dy = -dy;
            shot.setPosition(pos.x, cellTop + shot.getRadius());
            shot.setRadius(radius);

            return 1;
        }
        if (cell.Bottom == Close && pos.y + shot.getRadius() >= cellBottom)
        {
            radius = radius - 0.5f;
            counter--;
            dy = -dy;
            shot.setPosition(pos.x, cellBottom - shot.getRadius());
            shot.setRadius(radius);

            return 1;
        }
        if (cell.Left == Close && pos.x - shot.getRadius() <= cellLeft)
        {
            radius = radius - 0.5f;
            counter--;
            dx = -dx;
            shot.setPosition(cellLeft + shot.getRadius(), pos.y);
            shot.setRadius(radius);

            return 1;
        }
        if (cell.Right == Close && pos.x + shot.getRadius() >= cellRight)
        {
            radius = radius - 0.5f;
            counter--;
            dx = -dx;
            shot.setPosition(cellRight - shot.getRadius(), pos.y);
            shot.setRadius(radius);

            return 1;
        }
        return 0;
    }
};

#endif //BULLET_TYPES_H
