#include "ball.h"

enum GameState {
    RUNNING,
    FINISHED
};

class Game {
  public:
    Game(int maxBoardX, int maxBoardY):
    ball(maxBoardX, maxBoardY),
    racket1(maxBoardX, maxBoardY, 0),
    racket2(maxBoardX, maxBoardY, maxBoardX - 1) {
      maxX = maxBoardX;
      maxY = maxBoardY;
      state = RUNNING;
      winner = -1;
    }

  void update() {
    if (isRunning()) {
      ball.update();
      ball.contact(racket1);
      ball.contact(racket2);     
      if (ball.out()) {
        Point ballPos = ball.getPosition();
        if (ballPos.x <= 0)
          winner = 1;
        else
          winner = 0;
        state = FINISHED;
        return;
      }
    }
  }

  bool isRunning(){
    return state == RUNNING;
  }

  int getWinner(){
    return winner;
  }

  Ball &getBall(){
    return ball;
  }

  Racket &getRacket(int racketIndex){
    if (racketIndex == 0)
      return racket1;
    return racket2;
  }

  void racketMove(RacketDirection direction, int racketIndex){
    if (racketIndex == 0)
      racket1.move(direction);
    else
      racket2.move(direction);
  }
  
  private:  
    Ball ball;
    Racket racket1;
    Racket racket2;
    GameState state;
    int winner;
    int maxX;
    int maxY;
  
};
