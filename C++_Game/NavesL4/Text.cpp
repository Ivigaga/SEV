#include "Text.h"

Text::Text(string content, float x, float y, Game* game) {
	this->content = content;
	this->x = x;
	this->y = y;
	this->game = game;
}

void Text::draw() {
    SDL_Color textColor = { 255, 255, 255, 255 }; // Color del texto (blanco)
    SDL_Color bgColor = { 0, 0, 0, 255 };        // Color del fondo (negro translúcido, alpha = 128)

    // Renderizar el texto en una superficie
    SDL_Surface* surface = TTF_RenderText_Blended(game->font, content.c_str(), textColor);

    // Crear una textura a partir de la superficie
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);

    // Calcular el rectángulo del texto
    SDL_Rect textRect;
    textRect.x = x - surface->w / 2;
    textRect.y = y - surface->h / 2;
    textRect.w = surface->w;
    textRect.h = surface->h;

    // Calcular el rectángulo del fondo, con un margen de 2 píxeles
    SDL_Rect bgRect = textRect;
    bgRect.x -= 5; // Mover hacia la izquierda
    bgRect.y -= 4; // Mover hacia arriba
    bgRect.w += 10; // Aumentar ancho
    bgRect.h += 4; // Aumentar altura

    // Habilitar el modo de mezcla para permitir la transparencia
    SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);

    // Dibujar el fondo translúcido
    SDL_SetRenderDrawColor(game->renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(game->renderer, &bgRect);

    // Dibujar el texto sobre el fondo
    SDL_RenderCopy(game->renderer, texture, NULL, &textRect);

    // Liberar recursos
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}



