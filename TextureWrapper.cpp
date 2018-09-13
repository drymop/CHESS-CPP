#include "TextureWrapper.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

TextureWrapper::TextureWrapper()
{
  texture = NULL;
  textureHeight = 0;
  textureWidth = 0;
}

TextureWrapper::~TextureWrapper()
{
  destroyTexture();
}

bool TextureWrapper::loadFromFile(SDL_Renderer* renderer, std::string path)
{
  //remove existing texture
  destroyTexture();

  SDL_Texture* newTexture = NULL;

  //load image to surface
  SDL_Surface* loadedImage = IMG_Load(path.c_str());
  if(loadedImage == NULL)
  {
    printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    return false;
  }

  //Color key image (make a color transparent. currently using cyan 0, 255, 255)
  //SDL_SetColorKey(loadedImage, SDL_TRUE, SDL_MapRGB(loadedImage->format, 0, 0xFF, 0xFF));

  //Create texture from the loaded surface
  newTexture = SDL_CreateTextureFromSurface(renderer, loadedImage);
  if(newTexture == NULL)
  {
    printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    return false;
  }
  // Get size of texture
  textureWidth = loadedImage->w;
  textureHeight = loadedImage->h;
  // Deallocate surface used to load image
  SDL_FreeSurface(loadedImage);

  texture = newTexture;
  return texture != NULL;
}

void TextureWrapper::destroyTexture()
{
  if (texture != NULL)
  {
    SDL_DestroyTexture(texture);
    texture = NULL;
  }
}

void TextureWrapper::setBlendMode(SDL_BlendMode blendMode)
{
  SDL_SetTextureBlendMode(texture, blendMode);
}

void TextureWrapper::setTransparency(Uint8 alpha)
{
  SDL_SetTextureAlphaMod(texture, alpha);
}

void TextureWrapper::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* crop, SDL_Rect* stretch)
{
  SDL_Rect renderQuad = {x, y, textureWidth, textureHeight};

  if(crop != NULL) {
    if (stretch == NULL)
    {
      renderQuad.w = crop->w;
      renderQuad.h = crop->h;
    }
    else
    {
      renderQuad.w = stretch->w;
      renderQuad.h = stretch->h;
    }

  }
  SDL_RenderCopy(renderer, texture, crop, &renderQuad);
}

void TextureWrapper::render(SDL_Renderer* renderer, SDL_Rect* crop, SDL_Rect* dst)
{
  SDL_RenderCopy(renderer, texture, crop, dst);
}
