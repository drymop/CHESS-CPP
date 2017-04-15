#ifndef TEXTUREWRAPPER_H
#define TEXTUREWRAPPER_H

#include <SDL.h>
#include <string>


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
     * Deallocate Texture
     */
    void destroyTexture();

    /**
     * Render texture at given point
     * @param x,y coordinate of the point
     */
    void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* crop = NULL);

  protected:

  private:
    /**
     * The texture being wrapped
     */
    SDL_Texture* texture;

    /**
     * Size of the texture
     */
    int textureHeight, textureWidth;
};

#endif // TEXTUREWRAPPER_H
