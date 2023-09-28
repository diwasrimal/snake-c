#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MARGIN 2
// #define CELL_SIZE 20
#define CELL_SIZE 50
#define FRAME_PADDING 10
// #define CELLS_HOR 40
// #define CELLS_VER 30
#define CELLS_HOR 10
#define CELLS_VER 10
#define FRAME_WIDTH CELL_SIZE * CELLS_HOR + MARGIN * (CELLS_HOR - 1)
#define FRAME_HEIGHT CELL_SIZE * CELLS_VER + MARGIN * (CELLS_VER - 1)
#define WINDOW_WIDTH FRAME_WIDTH + FRAME_PADDING * 2
#define WINDOW_HEIGHT FRAME_HEIGHT + FRAME_PADDING * 2

enum SnakeDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

typedef struct {
    int x;
    int y;
} Cell;

typedef struct Snake {
    Cell cell;
    struct Snake *next;
} Snake;

Snake *initSnake(void);
Cell randomCell(void);

int main(void)
{
    int count = 0;
    srand(time(0));

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake");
    Color colors[] = {GOLD, MAGENTA, LIME, YELLOW};

    Snake *head = initSnake();
    Cell apple = randomCell();

    while (!WindowShouldClose()) {
        BeginDrawing();

        for (int y = FRAME_PADDING; y < FRAME_HEIGHT; y += CELL_SIZE + MARGIN) {
            for (int x = FRAME_PADDING; x < FRAME_WIDTH; x += CELL_SIZE + MARGIN) {
                DrawRectangle(apple.x, apple.y, CELL_SIZE, CELL_SIZE, colors[count % 4]);
                for (Snake *s = head; s != NULL; s = s->next) {
                    DrawRectangle(s->cell.x, s->cell.y, CELL_SIZE, CELL_SIZE, RAYWHITE);
                }
            }
        }

        count++;
        printf("Count: %d\n", count);
        WaitTime(0.1);
        EndDrawing();
    }

    // free snake
}

Cell randomCell(void)
{
    Cell c;
    int offset = FRAME_PADDING;
    int rand_hor_cell = rand() % CELLS_HOR - 1;
    int rand_ver_cell = rand() % CELLS_VER - 1;

    c.y = offset + rand_ver_cell * (CELL_SIZE + MARGIN);
    c.x = offset + rand_hor_cell * (CELL_SIZE + MARGIN);

    return c;
}

Snake *initSnake(void)
{
    Snake *head = malloc(sizeof(Snake));
    head->cell = randomCell();

    Snake *next = malloc(sizeof(Snake));
    next->cell.y = head->cell.y + (CELL_SIZE + MARGIN);
    next->cell.x = head->cell.x;
    next->next = NULL;

    head->next = next;
    return head;
}
