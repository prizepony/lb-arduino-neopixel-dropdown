/***
 * Dropdown-style Game
 * by Rory Nugent (rory@littlebits.cc)
 * Last updated: June 10, 2015
 ***/

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#include "player.h"
#include "obstacle.h"
#include "finishline.h"

#define ANALOG_IN_PIN    0
#define ANALOG_SCALAR  146

#define MATRIX_PIN       1
#define MATRIX_WIDTH     8
#define MATRIX_HEIGHT   16

#define AUDIO_OUT_PIN    9
#define OSC_OUT_PIN      5

#define DEFAULT_SPEED   250
#define NUM_OBSTACLES    10

#define OBSTACLE_COLLISION 1
#define FINISH_LINE        2

#define GAME_RUNNING       0
#define GAME_OVER          1
#define GAME_WON           2

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_PIN,
                            NEO_TILE_TOP     + NEO_TILE_LEFT +
                            NEO_TILE_COLUMNS + NEO_MATRIX_PROGRESSIVE,
                            NEO_GRB          + NEO_KHZ800);

Player player(0, MATRIX_HEIGHT - 2);
Obstacle obstacles[NUM_OBSTACLES];
FinishLine finishline;

uint16_t analogInput;
unsigned long lastMove = 0;
uint8_t game_status = 0;
uint16_t current_speed = DEFAULT_SPEED;

void setup() {
  matrix.begin();
  matrix.setBrightness(20);

  pinMode(AUDIO_OUT_PIN, OUTPUT);
  pinMode(OSC_OUT_PIN, OUTPUT);

  // initialize obstacles
  initializeObstacles();
}

void loop() {
  if (game_status != GAME_OVER)
    updateElements();

  // GAME RUNNING
  if (game_status == GAME_RUNNING) {

    int collisionResults = detectCollision();
    if (collisionResults == OBSTACLE_COLLISION) {
      game_status = GAME_OVER;
      digitalWrite(AUDIO_OUT_PIN, HIGH);
    }
    else if (collisionResults == FINISH_LINE) {
      game_status = GAME_WON;
    }
  }

  updateMatrix();

  digitalWrite(OSC_OUT_PIN, LOW);
  if (game_status == GAME_OVER) {
    resetGame();
  } else if (game_status == GAME_WON && finishline.ypos > (MATRIX_HEIGHT + 7)) {
    resetGame();
  }

  delay(10);
}

void updateElements()
{
  // update obstacles and finish line
  if (millis() - lastMove > current_speed)
  {
    digitalWrite(OSC_OUT_PIN, HIGH);
    
    for (int i = 0; i < NUM_OBSTACLES; i++) {
      obstacles[i].ypos++;
    }
    finishline.ypos++;

    lastMove = millis();
  }

  // update player position
  player.xpos = analogRead(ANALOG_IN_PIN) / ANALOG_SCALAR;
}

int detectCollision()
{
  uint8_t thru = 0;

  if (player.ypos == finishline.ypos) {
    return GAME_WON;
  }

  // cycle through obstacles
  for (int i = 0; i < NUM_OBSTACLES; i++) {
    // if obstacle is on the same level as the player...
    if (obstacles[i].ypos == player.ypos) {
      // check to see if the player is inside of a hole
      for (int j = 0; j < obstacles[i].holes; j++) {
        if (player.xpos == obstacles[i].holePositions[j]) {
          thru = 1;
        }
      }
      if (thru)
        return GAME_RUNNING;
      else
        return GAME_OVER;
    }
  }
  return 0;
}

void updateMatrix()
{
    matrix.fillScreen(0);

    // draw obstacles
    for (int i = 0; i < NUM_OBSTACLES; i++) {
      obstacles[i].drawObstacle(&matrix);
    }

    finishline.drawFinishLine(&matrix);

    if(game_status == GAME_OVER) {
      player.setColor(255, 0, 0);
      digitalWrite(AUDIO_OUT_PIN, LOW);
    }
    
    player.draw(&matrix);

  matrix.show();
}

void initializeObstacles()
{
  // seed the random number generator
  randomSeed(analogRead(2));

  // initialize obstacles and generate the holes
  for (int i = 0; i < NUM_OBSTACLES; i++)
  {
    obstacles[i].width = MATRIX_WIDTH;
    obstacles[i].height = 1;
    obstacles[i].generateHoles(MATRIX_WIDTH);
    obstacles[i].ypos = -((i * 5) + 2);
  }

  finishline.width = MATRIX_WIDTH;
  finishline.ypos = obstacles[NUM_OBSTACLES - 1].ypos - 5;
}

void resetGame()
{
  if (game_status == GAME_WON) {
    current_speed -= 50;
    if (current_speed < 50)
      current_speed = DEFAULT_SPEED;
  } else if (game_status == GAME_OVER) {
    delay(2000);
    current_speed = DEFAULT_SPEED;
  }

  initializeObstacles();
  player.setColor(0, 255, 0);
  game_status = GAME_RUNNING;
}

