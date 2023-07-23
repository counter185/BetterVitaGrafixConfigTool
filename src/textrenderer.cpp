#include "all.h"

TextRenderer::TextRenderer() {
    font = TTF_OpenFont(FONT_PATH, 18);
    fontJP = TTF_OpenFont(FONT_PATH_JP, 18);
}

TextRenderer::~TextRenderer() {
    if (font != NULL) {
        TTF_CloseFont(font);
    }
    if (fontJP != NULL) {
        TTF_CloseFont(fontJP);
    }
}

void TextRenderer::RenderString(std::string text, int x, int y, SDL_Color col) {
    int drawX = x;
    int drawY = y;
    uint32_t currentUTF8Sym = 0;
    int nextUTFBytes = 0;
    for (int chx = 0; chx != text.size(); chx++){
        char target = text.at(chx);
        if (target == '\n') {
            drawX = x;
            drawY += 18;
        }
        else {
            bool shouldDraw = ParseUTF8(target, &nextUTFBytes, currentUTF8Sym);
            if (shouldDraw) {
                /*if (!renderedGlyphs.contains(target)) {
                    RenderGlyph(target);
                }*/
                if (!renderedGlyphs.contains(currentUTF8Sym)) {
                    RenderGlyph(currentUTF8Sym);
                }
                GlyphData glyphData = renderedGlyphs[currentUTF8Sym];
                SDL_Rect drawRect = { drawX, drawY, glyphData.w, glyphData.h };
                SDL_SetTextureColorMod(glyphData.texture, col.r, col.g, col.b);
                SDL_SetTextureAlphaMod(glyphData.texture, col.a);
                SDL_RenderCopy(rdr, glyphData.texture, NULL, &drawRect);
                drawX += glyphData.advance;
            }
        }
    }
}

void TextRenderer::RenderGlyph(uint32_t a) {
    TTF_Font* usedFont =
        (a >= 0x3000 && a <= 0x30ff) || (a >= 0xff00 && a <= 0xffef) || (a >= 0x4e00 && a <= 0x9faf) ? fontJP
        : font;
    SDL_Surface* gl = TTF_RenderGlyph32_Blended(usedFont, (Uint32)a, SDL_Color{255,255,255,255});
    if (gl != NULL){
        SDL_Texture* newTexture = SDL_CreateTextureFromSurface(rdr, gl);
        
        GlyphData newGlyphData;
        TTF_GlyphMetrics32(usedFont, a, &newGlyphData.minx, &newGlyphData.maxx, &newGlyphData.miny, &newGlyphData.maxy, &newGlyphData.advance);
        newGlyphData.texture = newTexture;
        newGlyphData.w = gl->w;
        newGlyphData.h = gl->h;
        SDL_FreeSurface(gl);
        renderedGlyphs[a] = newGlyphData;

    } else {
        renderedGlyphs[a] = GlyphData {0,0,0,0,0,0,0,NULL};
    }
}