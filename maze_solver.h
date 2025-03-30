#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <algorithm>
#include <queue>
#include <vector>
#include <map>
#include <unordered_map>

#include "maze.h"

struct Vector2Comparator
{
    bool operator()(const sf::Vector2f& lhs, const sf::Vector2f& rhs) const
    {
        if (lhs.x != rhs.x)
            return lhs.x < rhs.x;
        return lhs.y < rhs.y;
    }
};

struct Vector2iHash
{
    size_t operator()(const sf::Vector2i& v) const
    {
        return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
    }
};

inline bool canMove(Cell labyrinth[width][height], const sf::Vector2i from, const sf::Vector2i to)
{
    if (to.x < 0 || to.x >= width || to.y < 0 || to.y >= height) return false;

    if (from.x < to.x && labyrinth[from.x][from.y].Right == Open) return true;
    if (from.x > to.x && labyrinth[from.x][from.y].Left == Open) return true;
    if (from.y < to.y && labyrinth[from.x][from.y].Bottom == Open) return true;
    if (from.y > to.y && labyrinth[from.x][from.y].Top == Open) return true;

    return false;
}

inline std::vector<sf::Vector2i> findPath(Cell labyrinth[width][height], const sf::Vector2f bot, const sf::Vector2f player)
{
    std::cout << "Bot: " <<  bot.x << " " << bot.y << std::endl;
    std::cout << "Player: " << player.x << " " << player.y << std::endl;

    std::queue<sf::Vector2i> queue;
    std::unordered_map<sf::Vector2i, sf::Vector2i, Vector2iHash> cameFrom;
    std::vector<sf::Vector2i> path;

    if (!canMove(labyrinth, sf::Vector2i(bot), sf::Vector2i(bot)))
    {
        std::cout << "ERROR: Bot starts in an unwalkable cell (" << bot.x << ", " << bot.y << ")!" << std::endl;
        return {};
    }

    queue.push(sf::Vector2i(bot));
    cameFrom[sf::Vector2i(bot)] = {-1, -1};

    int exploredCells = 0;
    while (!queue.empty())
    {
        sf::Vector2i current = queue.front();
        std::cout << "Processing: (" << current.x << ", " << current.y << ")" << std::endl;
        queue.pop();

        if (current.x == player.x && current.y == player.y)
        {
            auto step = static_cast<sf::Vector2i>(player);
            while (step.x != -1 && step.y != -1)
            {
                path.push_back(step);
                step = cameFrom[step];
                exploredCells++;
            }
            std::ranges::reverse(path);
            std::cout << "Final Path: ";
            for (const auto& p : path)
            {
                std::cout << "(" << p.x << ", " << p.y << ") -> ";
            }
            std::cout << "END\n";
            std::cout << "Total explored cells: " << exploredCells << std::endl;
            return path;
        }

        std::vector<sf::Vector2i> neighbors =
        {
            {current.x + 1, current.y}, {current.x - 1, current.y},
            {current.x, current.y + 1}, {current.x, current.y - 1}
        };

        for (sf::Vector2i next : neighbors)
        {
            if (canMove(labyrinth, sf::Vector2i(current), sf::Vector2i(next)) && !cameFrom.contains(next))
            {
                std::cout << "Checking neighbor: (" << next.x << ", " << next.y << ") ";

                if (!canMove(labyrinth, sf::Vector2i(current), sf::Vector2i(next)))
                {
                    std::cout << "- Blocked by canMove()!\n";
                }
                else if (cameFrom.contains(next))
                {
                    std::cout << "- Already visited\n";
                }
                else
                {
                    std::cout << "- Added to queue\n";
                }
                queue.push(next);
                cameFrom[next] = current;
            }
        }
    }
    std::cout << "ERROR: No path found!\n";
    return {};
}

#endif //MAZE_SOLVER_H
