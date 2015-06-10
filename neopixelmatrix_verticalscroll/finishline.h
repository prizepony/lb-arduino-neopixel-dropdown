#include <Adafruit_NeoMatrix.h>
#include <stdint.h>

class FinishLine {
  public:
  int width;
  int ypos;
  
  FinishLine(int w = 0);
  void drawFinishLine(Adafruit_NeoMatrix* matrix);
};
