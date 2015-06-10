#include "player.h"
#include <Adafruit_NeoMatrix.h>

Player::Player(int x, int y, int red, int green, int blue)
{
  setPos(x, y);
  setColor(red, green, blue);
}

void Player::setPos(int x, int y)
{
  xpos = x;
  ypos = y;
}

void Player::setColor(int r, int g, int b)
{
  red = r;
  green = g;
  blue = b;
}

uint16_t Player::getColor()
{
  return ((uint16_t)(red & 0xF8) << 8) |
         ((uint16_t)(green & 0xFC) << 3) |
                    (blue         >> 3);
}

void Player::draw(Adafruit_NeoMatrix* matrix)
{
  matrix->drawPixel(xpos, ypos, getColor());
}
