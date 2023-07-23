class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    void RenderString(std::string text, int x, int y, SDL_Color col = {255,255,255,255});
private:
    TTF_Font* font = NULL;
    TTF_Font* fontJP = NULL;
    std::map<uint32_t, GlyphData> renderedGlyphs;

    void RenderGlyph(uint32_t a);
};