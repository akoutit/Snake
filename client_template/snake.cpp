#include <duels/snake/game.h>
#include <duels/snake/snake_ai.h>
#include <duels/snake/msg.h>
#include <duels/snake/mechanics.h>
#include <iostream>
using namespace std;
using namespace duels::snake;

int main(int argc, char** argv)
{
  const std::string my_name = "Me";

  Game game(argc, argv, my_name, 1);    // to play as player 1 against level 1 AI
  //Game game(argc, argv, my_name, -2);    // to play as player 2 against level 2 AI

  Input input;
  Feedback feedback;
  [[maybe_unused]] const auto timeout = game.timeout_ms();
  SnakeMechanics S;
  S.initGame();
 


  while(game.get(feedback))
  {
    // write input in less than timeout

    char c;
    cin>>c;
    if (c=='a'){
        input.action=Input::Action::TURN_RIGHT;
    }
    input.action=Input::Action::MOVE;
    game.send(input);
  }
}
