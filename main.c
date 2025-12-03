#define SDL_MAIN_USE_CALLBACKS 1
#define SDL_MAIN_HANDLED

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>
#include <stdlib.h>

int W = 40, H = 24;
int generation = 0;
int population = 0;
int **grid;
int **next_grid;


static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static int running = 1;          
static int needs_redraw = 1;     

void init_grids() {
    grid = malloc(H * sizeof(int*));
    next_grid = malloc(H * sizeof(int*));
    for (int i = 0; i < H; i++) {
        grid[i] = calloc(W, sizeof(int));
        next_grid[i] = calloc(W, sizeof(int));
    }
}

void free_grids() {
    for (int i = 0; i < H; i++) {
        free(grid[i]);
        free(next_grid[i]);
    }
    free(grid);
    free(next_grid);
}

void clear_next_grid() {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            next_grid[y][x] = 0;
}

void copy_grids() {
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            grid[y][x] = next_grid[y][x];
}

void step() {
    clear_next_grid();
    generation++;
    population = 0;

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            int alive = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < W && ny >= 0 && ny < H && grid[ny][nx])
                        alive++;
                }
            }
            int was_alive = grid[y][x];
            if (was_alive)
                next_grid[y][x] = (alive == 2 || alive == 3);
            else
                next_grid[y][x] = (alive == 3);

            if (next_grid[y][x]) population++;
        }
    }
    copy_grids();
    needs_redraw = 1;
}

void DrawGrid(int width, int height) {
    float cellW = (float)width / W;
    float cellH = (float)height / H;

    for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
            if (grid[r][c]) {
                SDL_FRect rect = { c * cellW, r * cellH, cellW, cellH };
                SDL_SetRenderDrawColorFloat(renderer, 0.0f, 1.0f, 0.0f, 1.0f);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_SetRenderDrawColorFloat(renderer, 1.0f, 1.0f, 1.0f, 1.0f);
    for (int c = 0; c <= W; c++) {
        float x = c * cellW;
        SDL_RenderLine(renderer, x, 0, x, height);
    }
    for (int r = 0; r <= H; r++) {
        float y = r * cellH;
        SDL_RenderLine(renderer, 0, y, width, y);
    }
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("Conway's Game of Life", "1.0", "com.example.gol");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Conway's Game of Life", 800, 600,
                                    SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Window/renderer error: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, 800, 600,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    printf("Enter width and height (default 40 24): ");
    if (scanf("%d %d", &W, &H) != 2 || W <= 0 || H <= 0) {
        W = 40; H = 24;
    }
    printf("Grid size: %dx%d\n", W, H);

    init_grids();

    printf("\nStarting simulation.\nPress space to make a step.\nClose window or press Ctrl+C to quit.\n");

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        running = 0;
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int mouseX = event->button.x;
        int mouseY = event->button.y;
        int w, h;
        SDL_GetRenderOutputSize(renderer, &w, &h);

        float cellW = (float)w / W;
        float cellH = (float)h / H;
        int col = mouseX / cellW;
        int row = mouseY / cellH;

        if (row >= 0 && row < H && col >= 0 && col < W) {
            grid[row][col] = !grid[row][col];
            if (grid[row][col]) population++;
            else population--;
            needs_redraw = 1;
        }
    }

    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_SPACE) {
        step(); 
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    if (!running) return SDL_APP_SUCCESS;

    int w, h;
    SDL_GetRenderOutputSize(renderer, &w, &h);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    DrawGrid(w, h);

    char title[64];
    snprintf(title, sizeof(title), "Game of Life — Generation: %d  Population: %d", generation, population);
    SDL_SetWindowTitle(window, title);

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    free_grids();
    SDL_Quit();
}