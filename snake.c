#include "helpers.c"

int main(void)
{
    srand(time(0));

    Color background = {.r = 31, .g = 31, .b = 40, .a = 255};
    Color colors[] = {GREEN, RAYWHITE};
    int colors_len = 2;

    int score = 0;
    int count = 0;
    int limit = 4;
    int fps = 40;
    bool dir_should_change = true;

    // Array to keep track of pieces used by snake
    bool **used_cells = malloc(sizeof(bool*) * CELLS_VER);
    for (int i = 0; i < CELLS_VER; i++)
        used_cells[i] = malloc(sizeof(bool) * CELLS_HOR);

    // Snake and apple
    Snake *head = initSnake(used_cells);
    Cell apple = newApple((const bool**) used_cells);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake");
    SetTargetFPS(fps);

    while (!WindowShouldClose()) {
        ClearBackground(background);
        BeginDrawing();

        if (IsKeyPressed(KEY_R)) {
            ClearBackground(background);
            head = initSnake(used_cells);
            apple = newApple((const bool**) used_cells);
        }

        // Register just single direction for upcoming snake movement
        if (dir_should_change) {
            if (IsKeyPressed(KEY_RIGHT) && head->direction != LEFT) {
                dir_should_change = false;
                head->direction = RIGHT;
            }
            if (IsKeyPressed(KEY_LEFT) && head->direction != RIGHT) {
                dir_should_change = false;
                head->direction = LEFT;
            }
            if (IsKeyPressed(KEY_UP) && head->direction != DOWN) {
                dir_should_change = false;
                head->direction = UP;
            }
            if (IsKeyPressed(KEY_DOWN) && head->direction != UP) {
                dir_should_change = false;
                head->direction = DOWN;
            }
        }

        // Limit snake speed by moving it once every `limit` frames
        // If fps is reduced directly, key presses become less responsive
        bool snake_should_move = count % limit == 0;
        bool snake_moves;
        if (snake_should_move) {
            snake_moves = moveSnake(head, used_cells, &apple, &score);
            dir_should_change = true;
        }

        // Draw score
        char text[15];
        int fontSize = SCORE_HEIGHT;
        sprintf(text, "Score: %d", score);
        int width = MeasureText(text, fontSize);
        DrawText(text, WINDOW_WIDTH / 2 - width / 2, FRAME_PADDING, fontSize, RAYWHITE);

        // Draw apple and snake
        DrawRectangle(apple.pos_x, apple.pos_y, CELL_SIZE, CELL_SIZE, colors[count % colors_len]);
        for (Snake *s = head; s != NULL; s = s->next)
            DrawRectangle(s->cell.pos_x, s->cell.pos_y, CELL_SIZE, CELL_SIZE, RAYWHITE);

        if (!snake_moves) {
            const char *text = "Game over";
            int fontSize = 50;
            int width = MeasureText(text, fontSize);
            DrawText(text, WINDOW_WIDTH / 2 - width / 2, WINDOW_HEIGHT / 2 - fontSize / 2, fontSize, RED);
            EndDrawing();
            WaitTime(2);
            break;
        }

        // Mark used cells
        for (int i = 0; i < CELLS_VER; i++) {
            for (int j = 0; j < CELLS_HOR; j++) {
                if (!used_cells[i][j])
                    continue;
                Cell c = {.idx_y = i, .idx_x = j};
                updateCellPosition(&c);
                DrawText("U", c.pos_x + CELL_SIZE / 2, c.pos_y + CELL_SIZE / 2, 10, RED);
            }
        }

        count = (count + 1) % limit;
        EndDrawing();
    }

    freeSnake(head);

    for (int i = 0; i < CELLS_VER; i++)
        free(used_cells[i]);
    free(used_cells);

    return 0;
}