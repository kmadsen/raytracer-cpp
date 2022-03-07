#include "FrameBufferTexture.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

FrameBufferTexture::FrameBufferTexture(int width, int height) {
  glGenFramebuffers(1, &frameBuffer);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void FrameBufferTexture::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                          GL_TEXTURE_2D, texture, 0);
}

void FrameBufferTexture::resize(int width, int height) {
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
}

FrameBufferTexture::~FrameBufferTexture() {
  glDeleteFramebuffers(1, &frameBuffer);
  glDeleteTextures(1, &texture);
}
