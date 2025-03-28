#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <algorithm>
#include <queue>
#include <vector>
#include <map>
#include <SFML/System/Vector2.hpp>

#include "maze.h"

inline bool canMove(Cell labyrinth[width][height], const sf::Vector2i from, const sf::Vector2i to)
{
    if (to.x < 0 || to.x >= width || to.y < 0 || to.y >= height) return false;

    if (from.x < to.x && labyrinth[from.x][from.y].Right == Open) return true;
    if (from.x > to.x && labyrinth[from.x][from.y].Left == Open) return true;
    if (from.y < to.y && labyrinth[from.x][from.y].Bottom == Open) return true;
    if (from.y > to.y && labyrinth[from.x][from.y].Top == Open) return true;

    return false;
}

inline std::vector<sf::Vector2f> findPath(Cell labyrinth[width][height], const sf::Vector2f bot, const sf::Vector2f player)
{
    std::queue<sf::Vector2f> queue;
    std::map<sf::Vector2f, sf::Vector2f> cameFrom;
    std::vector<sf::Vector2f> path;

    queue.push(bot);
    cameFrom[bot] = {-1, -1};

    while (!queue.empty())
    {
        sf::Vector2f current = queue.front();
        queue.pop();

        if (current.x == player.x && current.y == player.y)
        {
            sf::Vector2f step = player;
            while (step.x != -1 && step.y != -1)
            {
                path.push_back(step);
                step = cameFrom[step];
            }
            std::ranges::reverse(path);
            return path;
        }

        std::vector<sf::Vector2f> neighbors =
        {
            {current.x + 1, current.y}, {current.x - 1, current.y},
            {current.x, current.y + 1}, {current.x, current.y - 1}
        };

        for (sf::Vector2f next : neighbors)
        {
            if (canMove(labyrinth, sf::Vector2i(current), sf::Vector2i(next)) && !cameFrom.contains(next))
            {
                queue.push(next);
                cameFrom[next] = current;
            }
        }
    }

    return {};
}

#endif //MAZE_SOLVER_H
