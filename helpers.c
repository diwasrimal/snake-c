#include "helpers.h"

Cell randomCell(void)
{
    Cell c;
    c.idx_y = rand() % (CELLS_VER - 1);
    c.idx_x = rand() % (CELLS_HOR - 1);
    updateCellPosition(&c);
    return c;
}

Cell newApple(const bool **used)
{
    Cell c;
    do {
        c = randomCell();
    } while (used[c.idx_y][c.idx_x]);
    return c;
}

// Finds and writes position to provided cell
void updateCellPosition(Cell *c)
{

    c->pos_y = FRAME_PADDING + c->idx_y * (CELL_SIZE + MARGIN);
    c->pos_x = FRAME_PADDING + c->idx_x * (CELL_SIZE + MARGIN);
}

Snake *initSnake(bool **used)
{
    // Initialize head randomly
    Snake *head = malloc(sizeof(Snake));
    head->cell = randomCell();

    // Initialize next cell
    Snake *next = malloc(sizeof(Snake));
    int offset = CELL_SIZE + MARGIN;
    bool y_touches_boundary = head->cell.idx_y + 1 >= CELLS_VER;
    next->cell.idx_x = head->cell.idx_x;
    if (y_touches_boundary) {
        next->cell.idx_y = head->cell.idx_y - 1;
        head->direction = LEFT;
    } else {
        next->cell.idx_y = head->cell.idx_y + 1;
        head->direction = UP;
    }
    updateCellPosition(&(next->cell));

    // Join to head
    next->next = NULL;
    head->next = next;

    // Mark cells as used
    for (int y = 0; y < CELLS_VER; y++)
        for (int x = 0; x < CELLS_HOR; x++)
            used[y][x] = false;
    used[head->cell.idx_y][head->cell.idx_x] = true;
    used[next->cell.idx_y][next->cell.idx_x] = true;

    return head;
}

bool moveSnake(Snake *head, bool **used, Cell *apple)
{
    Cell prev = head->cell;

    // Move head
    switch (head->direction) {
    case UP:
        head->cell.idx_y = (head->cell.idx_y - 1) < 0 ? CELLS_VER - 1 : head->cell.idx_y - 1;
        break;
    case DOWN:
        head->cell.idx_y = (head->cell.idx_y + 1) % CELLS_VER;
        break;
    case LEFT:
        head->cell.idx_x = (head->cell.idx_x - 1) < 0 ? CELLS_HOR - 1 : head->cell.idx_x - 1;
        break;
    case RIGHT:
        head->cell.idx_x = (head->cell.idx_x + 1) % CELLS_HOR;
        break;
    }

    bool eatsItself = used[head->cell.idx_y][head->cell.idx_x];
    if (eatsItself) {
        fprintf(stderr, "INFO: Snake eats itself\n");
        return false;
    }

    updateCellPosition(&(head->cell));
    used[head->cell.idx_y][head->cell.idx_x] = true;

    // If apple is consumed, remaining body of snake won't need to follow head
    // Since the head is at apple, previous head is now empty
    // It will be filled by new cell, elongating the snake
    if (head->cell.idx_x == apple->idx_x && head->cell.idx_y == apple->idx_y) {
        printf("Apple consumed!\n");
        Snake *s = malloc(sizeof(Snake));
        s->cell = prev;
        s->next = head->next;
        head->next = s;
        used[s->cell.idx_y][s->cell.idx_x] = true;
        *apple = newApple((const bool**) used);
        return true;
    }

    // Remaining pieces follow head
    for (Snake *s = head->next; s != NULL; s = s->next) {
        Cell tmp = s->cell;
        s->cell = prev;
        updateCellPosition(&(s->cell));
        prev = tmp;
    }

    // Last piece will be unused
    used[prev.idx_y][prev.idx_x] = false;
    // printUsed(used);
    return true;

}

void freeSnake(Snake *head)
{
    Snake *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

void printUsed(bool **used)
{
    printf("\n");
    for (int i = 0; i < CELLS_VER; i++) {
        for (int j = 0; j < CELLS_HOR; j++) {
            printf("%d ", used[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
