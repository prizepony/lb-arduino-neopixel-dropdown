#include "finishline.h"

FinishLine::FinishLine(int w) {
  width = w;
}

void FinishLine::drawFinishLine(Adafruit_NeoMatrix* matrix) {
  for (int i = 0; i < width; i++) {
    for(int j = 0; j < 8; j++) {
      matrix->drawPixel(i, ypos-j, matrix->Color(255-(j*31), 255-(j*31), 0));
    }
  }
}
