// TextRenderer.h
// Author: Bairuo

#ifndef TEXTRENDERER_H_
#define TEXTRENDERER_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include <freetype/ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>
#include <freetype/ftbitmap.h>
#include <iostream>

class Color;

class CharTexture
{
public:
    GLuint texID = 0;
    float width = 0, height = 0;
    float adv_x, adv_y;
    float start_x, start_y;
};

class TextRenderer
{
public:
    bool Load(const char* file);

    float GetCharWidth(char ch, int size, bool bold);
    float GetCharAdvx(char ch, int size, bool bold);
    float GetCharAdvy(char ch, int size, bool bold);
    float GetTexLength(const std::string &str, int size, bool bold);
    float GetTexHeight(const std::string &str, int size, bool bold);

    void DrawText(const std::string &str, int x0, int y0, const Color &color, int size, bool bold = false);
    void DrawText(const std::string &str, float x0, float y0, const Color &color, int size, bool bold = false);

    static TextRenderer* GetInstance();

private:
    static TextRenderer* instance;
    FT_Library library;
    FT_Face face;
    CharTexture textures[256][72][2];

    CharTexture* GetTextChar(char ch, int size, bool bold);
    bool LoadChar(char ch, int size, bool bold);

    const int maxSize = 72;
};


#endif // TEXTRENDERER_H_
