#pragma once

#include <string>
#include <SDL_ttf.h>
#include "Players.h"

class Output
{
private:
    SDL_Window *window;
    SDL_Renderer *rend;
    TTF_Font *font;

    static bool IsCursorIn(SDL_Rect rect)
    {
        int x = 0, y = 0;
        SDL_GetMouseState(&x, &y);
        return rect.x <= x && x < rect.x + rect.w && rect.y <= y && y < rect.y + rect.h;
    }

    void DrawString(std::string s, SDL_Rect dest, SDL_Color color)
    {
        auto surface = TTF_RenderText_Blended(font, s.c_str(), SDL_Color{255, 255, 255});
        auto texture = SDL_CreateTextureFromSurface(rend, surface);
        dest.x += (dest.w - surface->w) / 2;
        dest.y += (dest.h - surface->h) / 2;
        dest.w = surface->w;
        dest.h = surface->h;
        SDL_RenderCopy(rend, texture, 0, &dest);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }

    SDL_Rect BoundingRectangle(const Players &players, int i, int window_width, int window_height)
    {
        int height = players[i].Score() * window_height / players[0].Score();
        return SDL_Rect{i * window_width / players.size(), window_height - height, window_width / players.size() * 10 / 11, height};
    }
public:
    Output()
    {
        font = TTF_OpenFont("font.ttf", 12);
        window = SDL_CreateWindow("Game theory", 50, 50, 800, 600, SDL_WINDOW_RESIZABLE);
        rend = SDL_CreateRenderer(window, -1, 0);
    }

    ~Output()
    {
        TTF_CloseFont(font);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
    }

    void Draw(const Players &players, int rounds)
    {
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        int window_width = 0, window_height = 0;
        SDL_GetWindowSize(window, &window_width, &window_height);

        DrawString("Round count: " + std::to_string(rounds + 1), SDL_Rect{window_width / 2, 0, window_width / 2, 24}, SDL_Color{255, 255, 255, 255});

        for (int i = 0; i < players.size(); ++i)
        {
            auto area = BoundingRectangle(players, i, window_width, window_height);
            SDL_SetRenderDrawColor(rend, i * 255 / players.size(), 255 - i * 255 / players.size(), 0, 0);
            SDL_RenderFillRect(rend, &area);
        }

        for (int i = 0; i < players.size(); ++i)
        {
            auto area = BoundingRectangle(players, i, window_width, window_height);
            if (IsCursorIn(area))
            {
                SDL_SetRenderDrawColor(rend, 64, 64, 255, 255);
                SDL_RenderDrawRect(rend, &area);
                DrawString(std::to_string(players[i].Score() * 1000 / players[0].Score()), SDL_Rect{area.x, area.y + 5, area.w, 24}, SDL_Color{0, 0, 0, 255});

                DrawString(players[i].StrategyName(), SDL_Rect{area.x, area.y + 50, area.w, 24}, SDL_Color{0, 0, 0, 255});
            }
        }

        SDL_RenderPresent(rend);
    }
};