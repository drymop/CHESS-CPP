#ifndef TEXTUREWRAPPER_H
#define TEXTUREWRAPPER_H

#include <SDL2/SDL.h>
#include <string>

/**
 * Contain image data
 * and functions to load and draw image on screen
 */
class TextureWrapper
{
  public:
    /**
     * Init TextureWrapper
     */
    TextureWrapper();

    /**
     * Deallocate TextureWrapper
     */
    ~TextureWrapper();

    /**
     * Load image from file
     * @param path path to file
     * @return true if load successfully
     */
    bool loadFromFile(SDL_Renderer* renderer, std::string path);

    /**
     * Enable image blending (opaque <-> transparent)
     * @param one of the SDL_BlendMode const
     */
    void setBlendMode(SDL_BlendMode blendMode);

    /**
     * Set the transparency of texture.
     * Must call setBlendMode at first.
     * @param alpha between 0 and 255, 0 is transparent, 255 is opaque.
     */
    void setTransparency (Uint8 alpha);

    /**
     * Render a rectangle region of texture into a rectangle region of screen
     *
     * @param renderer the renderer to draw the texture on scree
     * @param x,y coordinate of left upper point of the screen rectangle
     * @param crop the texture is cropped to this rectangle
     * @param stretch the cropped texture is stretched to fit this rectangle
     */
    void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* crop = NULL, SDL_Rect* stretch = NULL);

    /**
     * Render a rectangle region of texture into a rectangle region of screen
     *
     * @param renderer the renderer to draw the texture on scree
     * @param crop only the part of the texture in side this rectangle is drawn
     * @param dist the texture part will fill in this rectangle on screen
     */
    void render(SDL_Renderer* renderer, SDL_Rect* crop = NULL, SDL_Rect* dst = NULL);

  private:
    /**
     * The texture being wrapped
     */
    SDL_Texture* texture;

    /**
     * Size of the texture
     */
    int textureHeight, textureWidth;

    /**
     * Deallocate Texture
     */
    void destroyTexture();
};

#endif // TEXTUREWRAPPER_H
