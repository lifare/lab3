
#include <Servo.h>
#include "game.h"
#include "LedControl.h"

#define INVALID_DIRECTION -1

const char keyUp = 'U';
const char keyDown = 'D';
const char keyNone = 'N';

const int boardSizeX = 24;
const int boardSizeY = 8;
const int boardSizeDisplay = 8;

Game game(boardSizeX, boardSizeY);

LedControl ledDisplay = LedControl(26, 22, 24, 3);

uint64_t lastGameUpdate;
uint64_t lastDisplayUpdate;
uint64_t lastFoodBlink;
const uint64_t gameUpdateDelayMs = 100;
const uint64_t displayUpdateDelayMs = 100;
const uint64_t foodBlinkDelayMs = 100;
uint64_t lastInputUpdate = 0;
bool showingFood = true;

char currentKey1 = keyNone;
char currentKey2 = keyNone;

const byte rowAmount = 4;
const byte colAmount = 4;

char keyMatrix[rowAmount][colAmount] = {
  {keyNone, keyUp,    keyNone,  keyNone},
  {keyNone, keyNone,  keyNone, keyNone},
  {keyNone, keyDown,  keyNone,  keyNone},
  {keyNone, keyNone,  keyNone,  keyNone}
};

static bool keyDownMatrix[rowAmount][colAmount];

byte rowPins1[rowAmount] = { 43, 45, 47, 49 }; 
byte colPins1[colAmount] = { 41, 39, 37, 35 };

byte rowPins2[rowAmount] = { 42, 44, 46, 48 };
byte colPins2[colAmount] = { 40, 38, 36, 34 };

Servo servo1;
Servo servo2;

void setup()
{
  int devices = ledDisplay.getDeviceCount();
  for (int address = 0; address < devices; address++){
    ledDisplay.shutdown(address, false);
    ledDisplay.setIntensity(address, 10);
    ledDisplay.clearDisplay(address);
  }
  setDataKey(true, OUTPUT, HIGH, 1);
  setDataKey(false, INPUT, HIGH, 1);
  setDataKey(true, OUTPUT, HIGH, 2);
  setDataKey(false, INPUT, HIGH, 2);

  servo1.attach(10);
  servo2.attach(9);
  servo1.write(0);
  servo2.write(0);
  
  Serial.begin(115200);
}

void setDataKey(bool isRow, int set_pin, int set_data, int number){
  int limit = isRow ? rowAmount : colAmount;
  byte pins[limit];
  if (isRow)
    pins[limit] = (number == 1) ? rowPins1 : rowPins2;
  else
    pins[limit] = (number == 1) ? colPins1 : colPins2;
  
  for (int i = 0; i < limit; i++) {
    pinMode(pins[i], set_pin);
    digitalWrite(pins[i], set_data);
  }
  
}

void loop()
{
  if (game.isRunning()) {
    updateInput();
    updateGame();
    updateDisplay();
  }
  else {
    if (game.getWinner() == 0) {
      servo1.write(90);
    }
    else {
      servo2.write(90);
    }
  }
}

RacketDirection keyToDirection(char key)
{
  switch(key) {
    case keyUp: return UP;
    case keyDown: return DOWN;
  }
  return INVALID_DIRECTION;
}

bool isKeyDown(int i, int j, byte* rowPins, byte* colPins)
{
  bool result = false;
  digitalWrite(rowPins[i], LOW);
  if (digitalRead(colPins[j]) == LOW)
    result = true;
  digitalWrite(rowPins[i], HIGH);
  return result;
}


char getKey(int racketIndex)
{  
  for (int i = 0; i < rowAmount; i++) {
    for (int j = 0; j < colAmount; j++) {
      bool keyDown;
      if (racketIndex == 0)
        keyDown = isKeyDown(i, j, rowPins1, colPins1);
      else
        keyDown = isKeyDown(i, j, rowPins2, colPins2);
      if (keyDown)
        return keyMatrix[i][j];
    } 
  }
  return keyNone;
}

void drawPoint(Point point)
{
  int displayAddress = int(point.x / boardSizeDisplay);
  int column = point.x - displayAddress * boardSizeDisplay;
  int row = boardSizeY - 1 - point.y;
  ledDisplay.setLed(displayAddress, row, column, true);
}

void drawRacket(Racket &racket)
{
  for (int i = 0; i < SIZE; i++) {
    drawPoint(racket.getPosition(i));
  }
}

void updateDisplay()
{
  if (millis() - lastDisplayUpdate > displayUpdateDelayMs) {
    Ball ball = game.getBall();
    Racket racket1 = game.getRacket(0);
    Racket racket2 = game.getRacket(1);
    
    for (int address = 0; address < ledDisplay.getDeviceCount(); address++) {
      ledDisplay.clearDisplay(address);
    }

    drawPoint(ball.getPosition());
    drawRacket(racket1);
    drawRacket(racket2);
    lastDisplayUpdate = millis();
  }
}

void updateInput() {
  lastInputUpdate = millis();
  char key1 = getKey(0);
  char key2 = getKey(1);
  if (key1 != keyNone)
    currentKey1 = key1;
  if (key2 != keyNone)
    currentKey2 = key2;
}

void updateGame()
{
  if (millis() - lastGameUpdate > gameUpdateDelayMs) {
    
    if (currentKey1 != keyNone) {
      game.racketMove(keyToDirection(currentKey1), 0);
      
    }
    if (currentKey2 != keyNone) {
      game.racketMove(keyToDirection(currentKey2), 1);
    }
    game.update();
    currentKey1 = keyNone;
    currentKey2 = keyNone;
    lastGameUpdate = millis();
  }
}
