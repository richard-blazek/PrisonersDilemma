#include "io.h"
#include <SDL2/SDL_ttf.h>

static SDL_Window *window;
static SDL_Renderer *rend;
static TTF_Font *font;

static bool is_cursor_in(SDL_Rect rect)
{
    int x = 0, y = 0;
    SDL_GetMouseState(&x, &y);
    return rect.x <= x && x < rect.x + rect.w && rect.y <= y && y < rect.y + rect.h;
}


static SDL_Color white = {255, 255, 255, 255}, black = {0, 0, 0, 255};

static void draw_string(const char *s, SDL_Rect dest, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Blended(font, s, white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surface);
    dest.x += (dest.w - surface->w) / 2;
    dest.y += (dest.h - surface->h) / 2;
    dest.w = surface->w;
    dest.h = surface->h;
    SDL_RenderCopy(rend, texture, 0, &dest);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

static SDL_Rect bounding_rect(player *players, int count, int i, int window_width, int window_height)
{
    int height = players[i].score * window_height / players[0].score;
    SDL_Rect rect = {i * window_width / count, window_height - height, window_width / count * 10 / 11, height};
    return rect;
}

void io_init(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
    font = TTF_OpenFont("font.ttf", 12);
    window = SDL_CreateWindow("Prisonner's Dilemma", 50, 50, 800, 600, SDL_WINDOW_RESIZABLE);
    rend = SDL_CreateRenderer(window, -1, 0);
}

void io_quit(void)
{
    TTF_CloseFont(font);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
}

void io_draw(player *players, int count, int rounds)
{
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    int window_width = 0, window_height = 0;
    SDL_GetWindowSize(window, &window_width, &window_height);

    char text_buffer[256];
    SDL_Rect round_rect = {window_width / 2, 0, window_width / 2, 24};
    snprintf(text_buffer, 256, "Round count: %d", rounds + 1);
    draw_string(text_buffer, round_rect, white);

    for (int i = 0; i < count; ++i)
    {
        SDL_Rect area = bounding_rect(players, count, i, window_width, window_height);
        SDL_SetRenderDrawColor(rend, i * 255 / count, 255 - i * 255 / count, 0, 255);
        SDL_RenderFillRect(rend, &area);
    }

    for (int i = 0; i < count; ++i)
    {
        SDL_Rect area = bounding_rect(players, count, i, window_width, window_height);
        if (is_cursor_in(area))
        {
            SDL_SetRenderDrawColor(rend, 64, 64, 255, 255);
            SDL_RenderDrawRect(rend, &area);

            SDL_Rect score_rect = {area.x, area.y + 5, area.w, 24};
            snprintf(text_buffer, 256, "%d", players[i].score * 1000 / players[0].score);
            draw_string(text_buffer, score_rect, black);

            SDL_Rect name_rect = {area.x, area.y + 50, area.w, 24};
            draw_string(players[i].name, name_rect, black);
        }
    }

    SDL_RenderPresent(rend);
}

bool io_loop(void)
{
    SDL_Delay(50);
    return !SDL_QuitRequested();
}

int io_mouse_scroll(void)
{
    static SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_MOUSEWHEEL)
        {
            return event.wheel.y;
        }
    }
    return 0;
}
