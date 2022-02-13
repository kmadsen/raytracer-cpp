#include <GL/glut.h>

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc,char **argv) {
  glutInit(&argc,argv);
  glutCreateWindow("Hello,world!");
  glutDisplayFunc(display);
  glutMainLoop();
}
