#include "direction.h"
#include "racket.h"

class Ball {
  public:
    Ball(int maxBoardX, int maxBoardY) {
      pos.x = 1;
      pos.y = maxBoardY / 2;

      direct = RIGHT;
      maxX = maxBoardX;
      maxY = maxBoardY;
    }

    Point getPosition() {
      return pos;
    }
    
    int getMaxX() {
      return maxX;
    }
    
    int getMaxY() {
      return maxY;
    }

    Direction getDirection() {
      return direct;
    }

    void update() {
      if (pos.y == 0) {
        if (direct == UP_RIGHT)
          direct = DOWN_RIGHT;
        else
          direct = DOWN_LEFT;
      }
      
      if (pos.y == maxY - 1){
        if (direct == DOWN_RIGHT)
          direct = UP_RIGHT;
        else
          direct = UP_LEFT;
      }
      switch(direct) {
        case RIGHT:
          pos.x += 1;
          break;
        case LEFT:
          pos.x -= 1;
          break;
        case UP_RIGHT:
          pos.y -= 1;
          pos.x += 1;
          break;
        case UP_LEFT:
          pos.y -= 1;
          pos.x -= 1;
          break;  
        case DOWN_RIGHT:
          pos.y += 1;
          pos.x += 1;
          break;
        case DOWN_LEFT:
          pos.y += 1;
          pos.x -= 1;
          break;
      }
    }

      bool out() {
        return pos.x <= 0 || pos.x >= maxX - 1;
      }

      void contact(Racket &racket) {
        int racketPart = -1;
        if (pos.y == racket.getPosition(0).y)
          racketPart = 0;
        if (pos.y == racket.getPosition(1).y)
          racketPart = 1;
        if (pos.y == racket.getPosition(2).y)
          racketPart = 2;
        if (pos.x + 1 == racket.getPosition(0).x && (direct == RIGHT || direct == UP_RIGHT || direct == DOWN_RIGHT))
          setDirect(racketPart, UP_LEFT, DOWN_LEFT, LEFT, UP_RIGHT, DOWN_RIGHT, RIGHT);
        if (pos.x - 1 == racket.getPosition(0).x && (direct == LEFT || direct == UP_LEFT || direct == DOWN_LEFT))
          setDirect(racketPart, UP_RIGHT, DOWN_RIGHT, RIGHT, UP_LEFT, DOWN_LEFT, LEFT);
        }

        void setDirect(int part, Direction dir0, Direction dir2, Direction dir1, Direction back_dir0, Direction back_dir2, Direction back_dir1)
        {
          if (part == 0) 
            direct = dir0;
          if (part == 1){
            if (direct == back_dir0)
              direct = dir0;
            if (direct == back_dir1)
              direct = dir1;
            if (direct == back_dir2)
              direct = dir2;
          }
          if (part == 2)
            direct = dir2;
        }
      
   private:
    Point pos;
    Direction direct;
    int maxX;
    int maxY;
};
