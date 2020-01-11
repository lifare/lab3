#include "point.h"
#include "racketDirection.h"
const int SIZE = 3;

class Racket {
  public:
    Racket(int maxBoardX, int maxBoardY, int positionX) {
      int positionY = maxBoardY / 2;
      
      position[0].x = positionX;
      position[0].y = positionY - 1;

      position[1].x = positionX;
      position[1].y = positionY;

      position[2].x = positionX;
      position[2].y = positionY + 1;

      maxX = maxBoardX;
      maxY = maxBoardY;
    }

    Point getPosition(int index) {
      return position[index];
    }
    
    int getMaxX() {
      return maxX;
    }
    
    int getMaxY() {
      return maxY;
    }

    void move(RacketDirection direction) {
      int shift = 0;
      if (direction == UP){
        if (position[0].y != 0)
          shift = -1;
      } 
      if (direction == DOWN){
        if (position[SIZE - 1].y != maxY - 1)
          shift = 1;
      }

      for (int i = 0; i < SIZE; i++) {
        position[i].y += shift;
      }
    }

    bool containsPoint(Point &point) {
      for (int i = 0; i < SIZE; i++) {
        if (point.x == position[i].x && point.y == position[i].y) {
          return true;
        }
      }
      return false;
    }
  
  private:
    Point position[SIZE];
    int maxX;
    int maxY;
  
};
