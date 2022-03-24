#ifndef FRAMEBUFFERTEXTURE_H_
#define FRAMEBUFFERTEXTURE_H_

#include <GL/glew.h>

class FrameBufferTexture {
 public:
  FrameBufferTexture(int width, int height);
  virtual ~FrameBufferTexture();
  void bind();
  void resize(int width, int height);

  GLuint frameBuffer;
  GLuint textureColorBuffer;
  GLuint renderBuffer;
};

#endif  // FRAMEBUFFERTEXTURE_H_
