#include <Adafruit_NeoMatrix.h>
#include <stdint.h>

class Player {
  public:
    // variables
    int xpos;
    int ypos;

    int red;
    int green;
    int blue;

    // functions
    Player(int x = 0, int y = 0, int red = 0, int green = 255, int blue = 0);
    void setPos(int x, int y);

    void setColor(int r, int g, int b);
    uint16_t getColor();

    void draw(Adafruit_NeoMatrix* matrix);
};
