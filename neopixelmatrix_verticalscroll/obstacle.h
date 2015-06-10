#include <Adafruit_NeoMatrix.h>
#include <stdint.h>

class Obstacle
{
  public:
  // variables
  int width;
  int height;
  int holes;
  int* holePositions;
  
  int ypos;
  
  int red = 0;
  int green = 0;
  int blue = 255;
  
  // functions
  Obstacle(int w = 0, int h = 0, int num_holes = 1);
  ~Obstacle();
  
  uint16_t getColor();
  
  void drawObstacle(Adafruit_NeoMatrix* matrix);
  void generateHoles(int range);
};
