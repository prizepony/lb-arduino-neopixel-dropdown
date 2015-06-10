#include "obstacle.h"

Obstacle::Obstacle(int w, int h, int num_holes)
{
  width = w;
  height = h;
  holes = num_holes;
  holePositions = new int[holes];

  ypos = -1;

  for (int i = 0; i < holes; i++)
    holePositions[i] = 0;
}

uint16_t Obstacle::getColor()
{
  return ((uint16_t)(red & 0xF8) << 8) |
         ((uint16_t)(green & 0xFC) << 3) |
         (blue         >> 3);
}

void Obstacle::drawObstacle(Adafruit_NeoMatrix* matrix)
{
  if (ypos >= 0)
  {
    for (int i = 0; i < width; i++)
    {
      matrix->drawPixel(i, ypos, getColor());
    }
    
    for (int i = 0; i < holes; i++)
    {
      matrix->drawPixel(holePositions[i], ypos, matrix->Color(0, 0, 0));
    }
  }
}

void Obstacle::generateHoles(int range)
{
  // TODO: improve generation with multiple holes
  for (int i = 0; i < holes; i++)
  {
    holePositions[i] = random(range);
  }
}

Obstacle::~Obstacle()
{
  delete [] holePositions;
  holePositions = NULL;
}
