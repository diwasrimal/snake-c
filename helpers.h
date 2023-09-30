#include <stdbool.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MARGIN 2
#define CELL_SIZE 30
#define SCORE_HEIGHT 25
#define FRAME_PADDING 5
#define CELLS_HOR 20
#define CELLS_VER 20
#define FRAME_WIDTH (CELL_SIZE * CELLS_HOR + MARGIN * (CELLS_HOR - 1))
#define FRAME_HEIGHT (CELL_SIZE * CELLS_VER + MARGIN * (CELLS_VER - 1))
#define WINDOW_WIDTH (FRAME_WIDTH + FRAME_PADDING * 2)
#define WINDOW_HEIGHT (FRAME_HEIGHT + FRAME_PADDING * 2 + SCORE_HEIGHT)

enum SnakeDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

typedef struct {
    int pos_x;
    int pos_y;
    int idx_x;
    int idx_y;
} Cell;

typedef struct Snake {
    Cell cell;
    struct Snake *next;
    enum SnakeDirection direction;
} Snake;

Snake *initSnake(bool **has_snake);
Cell randomCell(void);
Cell newApple(const bool **used);
void updateCellPosition(Cell *c);
bool moveSnake(Snake *head, bool **used, Cell *apple, int *score);
void freeSnake(Snake *head);
void printUsed(bool **used);
