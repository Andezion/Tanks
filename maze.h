#ifndef MAZE_H
#define MAZE_H
#include <stack>

constexpr int windowWidth = 1600;
constexpr int windowHeight = 800;

constexpr int cellSize = 200;
constexpr int lineThickness = 2;

constexpr int width = 8;
constexpr int height = 4;

enum CellState { Close, Open };
class Cell
{
public:
    int x{}, y{};
    CellState Left = Close;
    CellState Right = Close;
    CellState Top = Close;
    CellState Bottom = Close;
    bool Visited = false;
};

inline void generate_labyrinth(Cell labyrinth[width][height])
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            labyrinth[x][y].x = x;
            labyrinth[x][y].y = y;
            labyrinth[x][y].Visited = false;
        }
    }

    srand(static_cast<unsigned>(time(nullptr)));
    const int startX = rand() % width;
    const int startY = rand() % height;

    labyrinth[startX][startY].Visited = true;

    std::stack<Cell> path;
    path.push(labyrinth[startX][startY]);

    while (!path.empty())
    {
        Cell _cell = path.top();

        std::vector<Cell*> nextStep;
        if (_cell.x > 0 && !labyrinth[_cell.x - 1][_cell.y].Visited)
        {
            nextStep.push_back(&labyrinth[_cell.x - 1][_cell.y]);
        }
        if (_cell.x < width - 1 && !labyrinth[_cell.x + 1][_cell.y].Visited)
        {
            nextStep.push_back(&labyrinth[_cell.x + 1][_cell.y]);
        }
        if (_cell.y > 0 && !labyrinth[_cell.x][_cell.y - 1].Visited)
        {
            nextStep.push_back(&labyrinth[_cell.x][_cell.y - 1]);
        }
        if (_cell.y < height - 1 && !labyrinth[_cell.x][_cell.y + 1].Visited)
        {
            nextStep.push_back(&labyrinth[_cell.x][_cell.y + 1]);
        }

        if (!nextStep.empty())
        {
            Cell* next = nextStep[rand() % nextStep.size()];

            if (next->x != _cell.x)
            {
                if (_cell.x - next->x > 0)
                {
                    labyrinth[_cell.x][_cell.y].Left = Open;
                    labyrinth[next->x][next->y].Right = Open;
                }
                else
                {
                    labyrinth[_cell.x][_cell.y].Right = Open;
                    labyrinth[next->x][next->y].Left = Open;
                }
            }
            if (next->y != _cell.y)
            {
                if (_cell.y - next->y > 0)
                {
                    labyrinth[_cell.x][_cell.y].Top = Open;
                    labyrinth[next->x][next->y].Bottom = Open;
                }
                else
                {
                    labyrinth[_cell.x][_cell.y].Bottom = Open;
                    labyrinth[next->x][next->y].Top = Open;
                }
            }

            next->Visited = true;
            path.push(*next);
        }
        else
        {
            path.pop();
        }
    }
}

inline void draw_labyrinth(sf::RenderWindow& window, Cell labyrinth[width][height])
{
    sf::RectangleShape line;
    line.setFillColor(sf::Color::White);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            const auto px = static_cast<float>(x * cellSize);
            const auto py = static_cast<float>(y * cellSize);

            if (labyrinth[x][y].Top == Close)
            {
                line.setSize(sf::Vector2f(cellSize, lineThickness));
                line.setPosition(px, py);
                window.draw(line);
            }

            if (labyrinth[x][y].Left == Close)
            {
                line.setSize(sf::Vector2f(lineThickness, cellSize));
                line.setPosition(px, py);
                window.draw(line);
            }

            if (x == width - 1 && labyrinth[x][y].Right == Close)
            {
                line.setSize(sf::Vector2f(lineThickness, cellSize));
                line.setPosition(px + cellSize - lineThickness, py);
                window.draw(line);
            }

            if (y == height - 1 && labyrinth[x][y].Bottom == Close)
                {
                line.setSize(sf::Vector2f(cellSize, lineThickness));
                line.setPosition(px, py + cellSize - lineThickness);
                window.draw(line);
            }
        }
    }
}

#endif //MAZE_H
