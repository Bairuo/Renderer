#include <cstdio>
#include <string>
#include <iostream>
#include <algorithm>
#include "Rectangle.h"
#include "TextRenderer.h"
#include "BasicConfig.h"

TextRenderer* TextRenderer::instance = nullptr;

TextRenderer* TextRenderer::GetInstance()
{
    if(instance == nullptr)
    {
        instance = new TextRenderer();
    }
    return instance;
}

bool TextRenderer::LoadChar(char ch, int size, bool bold)
{
    //if(FT_Load_Char(face, ch, FT_LOAD_RENDER))
    if(FT_Load_Char(face, ch, FT_LOAD_DEFAULT))
        return false;

    CharTexture &charTex = textures[static_cast<int>(ch)][size][bold];

    FT_Glyph glyph;
    if(FT_Get_Glyph(face->glyph, &glyph))
        return false;

    if(bold)
    {
        // have some trouble
        if(face->glyph->format == FT_GLYPH_FORMAT_OUTLINE)
        {
            FT_Outline_Embolden(&(face->glyph->outline), 32 << 6);
        }
//        else if(face->glyph->format == FT_GLYPH_FORMAT_BITMAP)
//        {
//            FT_Bitmap_Embolden(library, &face->glyph->bitmap, 32, 32);
//        }
    }

    //FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LCD);
    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

    FT_Bitmap& bitmap = bitmap_glyph->bitmap;

    int width = bitmap.width;
    int height = bitmap.rows;

    charTex.width = width * 1.0f / WindowWidth * 2;
    charTex.height = height * 1.0f / WindowWidth * 2;
    charTex.adv_x = face->glyph->advance.x * 1.0f / 64 / WindowWidth * 2;
    charTex.adv_y = face->size->metrics.y_ppem * 1.0f / WindowWidth * 2;
    //harTex.adv_y = face->glyph->advance.y * 1.0f / 64 / WindowWidth * 2;
    charTex.start_x = (float)bitmap_glyph->left *1.0f / WindowWidth * 2;
    charTex.start_y = ((float)bitmap_glyph->top - height) *1.0f / WindowWidth * 2 ;

    glGenTextures(1, &charTex.texID);
    glBindTexture(GL_TEXTURE_2D, charTex.texID);
    char* pbuf = new char[width * height * 4];
    for(int j = 0; j < height; j++)
    {
        for(int i = 0; i < width; i++)
        {
            unsigned char vl = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
            pbuf[(4 * i + (height - j - 1) * width * 4)] = 0xFF;
            pbuf[(4 * i + (height - j - 1) * width * 4) + 1] = 0xFF;
            pbuf[(4 * i + (height - j - 1) * width * 4) + 2] = 0xFF;
            pbuf[(4 * i + (height - j - 1) * width * 4) + 3] = vl;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pbuf);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);    // 设置纹理包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // 设置纹理过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    delete[] pbuf;
    return true;
}

CharTexture* TextRenderer::GetTextChar(char ch, int size, bool bold)
{
    if(!textures[static_cast<int>(ch)][size][bold].texID)
    {
        FT_Set_Pixel_Sizes(face, size, size);
        LoadChar(ch, size, bold);
    }
    return &textures[static_cast<int>(ch)][size][bold];
}

float TextRenderer::GetCharWidth(char ch, int size, bool bold)
{
    if(!textures[static_cast<int>(ch)][size][bold].texID)
    {
        FT_Set_Pixel_Sizes(face, size, size);
        LoadChar(ch, size, bold);
    }
    return textures[static_cast<int>(ch)][size][bold].width;
}


float TextRenderer::GetCharAdvx(char ch, int size, bool bold)
{
    if(!textures[static_cast<int>(ch)][size][bold].texID)
    {
        FT_Set_Pixel_Sizes(face, size, size);
        LoadChar(ch, size, bold);
    }
    return textures[static_cast<int>(ch)][size][bold].adv_x;
}

float TextRenderer::GetCharAdvy(char ch, int size, bool bold)
{
    if(!textures[static_cast<int>(ch)][size][bold].texID)
    {
        FT_Set_Pixel_Sizes(face, size, size);
        LoadChar(ch, size, bold);
    }
    return textures[static_cast<int>(ch)][size][bold].adv_y;
}

float TextRenderer::GetTexLength(const std::string &str, int size, bool bold)
{
    float sum = 0;

    for(unsigned int i = 0; i < str.length(); i++)
    {
        sum += GetCharAdvx(str[i], size, bold);
    }

    return sum;
}

float TextRenderer::GetTexHeight(const std::string &str, int size, bool bold)
{
    float maxHeight = 0;

    for(unsigned int i = 0; i < str.length(); i++)
    {
        maxHeight = std::max(maxHeight, GetCharAdvy(str[i], size, bold));
    }
    return maxHeight;
}

void TextRenderer::DrawText(const std::string &str, int x0, int y0, const Color &color, int size, bool bold)
{
    float x = x0 * 1.0f / WindowWidth * 2, y = y0 * 1.0f / WindowHeight * 2;
    DrawText(str, x, y, color, size, bold);
}

void TextRenderer::DrawText(const std::string &str, float x0, float y0, const Color &color, int size, bool bold)
{
    float x = x0, y = y0;
    float w, h, ch_x, ch_y;

    if(size > maxSize)
        size = maxSize;

    glEnable(GL_TEXTURE_2D);
    for(unsigned int i = 0; i < str.length(); i++)
    {
        CharTexture* charTex = GetTextChar(str[i], size, bold);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, charTex->texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        w = charTex->width; h = charTex->height;

        ch_x = x + charTex->start_x; ch_y = y + charTex->start_y;

        glColor4f(color.r, color.g, color.b, color.a);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);glVertex3f(ch_x, ch_y, 1.0f);
        glTexCoord2f(1.0f, 0.0f);glVertex3f(ch_x + w, ch_y, 1.0f);
        glTexCoord2f(1.0f, 1.0f);glVertex3f(ch_x + w, ch_y + h, 1.0f);
        glTexCoord2f(0.0f, 1.0f);glVertex3f(ch_x, ch_y + h, 1.0f);
        glEnd();

        x += charTex->adv_x;
    }
}

bool TextRenderer::Load(const char* file)
{
    if(FT_Init_FreeType(&library))
    {
        //printf("library init failed\n");
        return false;
    }

    if(FT_New_Face(library, file, 0, &face))
    {
        //printf("load ttf failed\n");
        return false;
    }

    FT_Select_Charmap(face, FT_ENCODING_UNICODE);

    return true;
}
