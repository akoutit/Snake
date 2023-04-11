#ifndef SNAKE_MECHANICS_H
#define SNAKE_MECHANICS_H
#include"iostream"
#include <duels/snake/msg.h>
#include"vector"
#include <iostream>
#include<algorithm>

#include <stdlib.h>
#include <time.h>

// modern way
#include <random>
using namespace std;
using namespace duels::snake;
using duels::Result;


// base mechanics class, should be heavily adapted to reflect the game rules
class SnakeMechanics
{
    inline auto& snake1() {return display_msg.snake1;}
    inline auto snake1() const {return display_msg.snake1;}
    inline auto& snake2() {return display_msg.snake2;}
    inline auto snake2() const {return display_msg.snake2;}
    inline auto& apples() {return display_msg.apples;}
    inline auto apples() const {return display_msg.apples;}
    inline auto score1() const {return display_msg.score1;}
    inline auto& score1() {return display_msg.score1;}
    inline auto score2() const {return display_msg.score2;}
    inline auto& score2() {return display_msg.score2;}
public:

    SnakeMechanics() {}
    InitDisplay initGame()
    {

        // forward to init msg
        InitDisplay msg;




        score1()=0;
        score2()=0;

        for(int i=0;i<nbr_apples;i++){move_apple(i);}

        snake1().head.x=60;
        snake1().head.y=160;
        snake1().head.t=Orientation::RIGHT;
        snake1().body.push_back({40,160});
        snake1().body.push_back({20,160});
        snake1().body.push_back({0,160});



        snake2().head.x= 420;
        snake2().head.y= 440;
        snake2().head.t=Orientation::LEFT;
        snake2().body.push_back({440,440});
        snake2().body.push_back({460,440});
        snake2().body.push_back({480,440});

        return msg;}
    void move_apple(int i)
    {

        srand(time(0));
        Position P={20*(rand()%25),100+20*(rand()%20)};
        bool apple_obs1=(find(snake1().body.begin(),snake1().body.end(),P) !=snake1().body.end())or P==Position {snake1().head.x,snake1().head.y};

        bool apple_obs2=(find(snake2().body.begin(),snake2().body.end(),P) !=snake2().body.end()) or P==Position {snake2().head.x,snake2().head.y};
        bool apples_obs=(find(apples().begin(),apples().end(),P) !=apples().end());

        while(apple_obs1 or apple_obs2 or apples_obs)
        {
            P={20*(rand()%25),100+20*(rand()%20)};

            apple_obs1=(find(snake1().body.begin(),snake1().body.end(),P) !=snake1().body.end()) or P==Position {snake1().head.x,snake1().head.y};

            apple_obs2=(find(snake2().body.begin(),snake2().body.end(),P) !=snake2().body.end()) or P==Position {snake2().head.x,snake2().head.y};

            apples_obs=(find(apples().begin(),apples().end(),P) !=apples().end());

        }

        if (i<apples().size()){apples()[i]=P;}
        else{apples().push_back(P);}


    }

    void delete_apple(int i)

    {
        apples().erase(apples().begin()+i);

    }

    void add_apple()
    {

        srand(time(0));
        if (apples().size()<max_apples)
        {
            Position P={20*(rand()%25),100+20*(rand()%20)};
            bool apple_obs1=(find(snake1().body.begin(),snake1().body.end(),P) !=snake1().body.end())or P==Position {snake1().head.x,snake1().head.y};

            bool apple_obs2=(find(snake2().body.begin(),snake2().body.end(),P) !=snake2().body.end()) or P==Position {snake2().head.x,snake2().head.y};
            bool apples_obs=(find(apples().begin(),apples().end(),P) !=apples().end());

            while(apple_obs1 or apple_obs2 or apples_obs)
            {
                P={20*(rand()%25),100+20*(rand()%20)};

                apple_obs1=(find(snake1().body.begin(),snake1().body.end(),P) !=snake1().body.end()) or P==Position {snake1().head.x,snake1().head.y};

                apple_obs2=(find(snake2().body.begin(),snake2().body.end(),P) !=snake2().body.end()) or P==Position {snake2().head.x,snake2().head.y};

                apples_obs=(find(apples().begin(),apples().end(),P) !=apples().end());

            }
            apples().push_back(P);
        }


    }




    bool miam(int i){

            return (snake1().head.x==apples()[i].x and snake1().head.y==apples()[i].y);



    }
    bool miam(){
        bool s=miam(0);
        for(int i=1;i<apples().size();i++)
        {
            s= s or miam(i);
        }
        return s;
    }
    void move(){
        {snake1().body.push_back({snake1().head.x,snake1().head.y});

            if(!miam()){snake1().body.erase(snake1().body.begin());}

            else
            {for(int i=0;i<apples().size();i++)
                {

                    if (miam(i))
                    {
                        score1()++;
                        delete_apple(i);
                        if (apples().size()==0)
                        {
                            add_apple();
                        }
                    }
                }

            }
            if (rand()%25==1)
            {
                add_apple();
            }
                if (snake1().head.t==Orientation::UP)
                    {snake1().head.y-=20;}
                if (snake1().head.t==Orientation::LEFT)
                    {snake1().head.x-=20;}
                if (snake1().head.t==Orientation::RIGHT)
                    {snake1().head.x+=20;}
                if (snake1().head.t==Orientation::DOWN)
                    {snake1().head.y+=20;}



        }

    }

    bool miam2(int i){
        return (snake2().head.x==apples()[i].x and snake2().head.y==apples()[i].y);

    }
    bool miam2(){
        bool s=miam2(0);
        for(int i=1;i<apples().size();i++)
        {
            s= s or miam2(i);
        }
        return s;
    }

    void move2(){
        {snake2().body.push_back({snake2().head.x,snake2().head.y});

                if(!miam2()){snake2().body.erase(snake2().body.begin());}

                else
                {for(int i=0;i<apples().size();i++)
                    {

                        if (miam2(i))
                        {
                            score2()++;
                            delete_apple(i);
                            if (apples().size()==0)
                            {
                                add_apple();
                            }
                        }
                    }

                }
                if (rand()%15==1)
                {
                    add_apple();
                }
                if (snake2().head.t==Orientation::UP)
                    {snake2().head.y-=20;}
                if (snake2().head.t==Orientation::LEFT)
                    {snake2().head.x-=20;}
                if (snake2().head.t==Orientation::RIGHT)
                    {snake2().head.x+=20;}
                if (snake2().head.t==Orientation::DOWN)
                    {snake2().head.y+=20;}


        }

    }

    inline const Display& display() const {return display_msg;}

    // game evolution can be put here, or just save the inputs for later when building the feedbacks
    void update(const Input &input1, const Input &input2) {

        if (input1.action==Input::Action::MOVE)
                {move();}

        if (input1.action==Input::Action::TURN_LEFT)
        {switch(snake1().head.t){
            case Orientation::UP:
                snake1().head.t=Orientation::LEFT;
                break;
            case Orientation::LEFT:
                snake1().head.t=Orientation::DOWN;
                break;
            case Orientation::RIGHT:
                snake1().head.t=Orientation::UP;
                break;
            case Orientation::DOWN:
                snake1().head.t=Orientation::RIGHT;
                break;

            }

        move();
        }
        if (input1.action==Input::Action::TURN_RIGHT)
        {switch(snake1().head.t){
            case Orientation::UP:
                snake1().head.t=Orientation::RIGHT;
                break;
            case Orientation::LEFT:
                snake1().head.t=Orientation::UP;
                break;
            case Orientation::RIGHT:
                snake1().head.t=Orientation::DOWN;
                break;
            case Orientation::DOWN:
                snake1().head.t=Orientation::LEFT;
                break;

            }

        move();
        }
        if (input2.action==Input::Action::MOVE)
                {move2();}

        if (input2.action==Input::Action::TURN_LEFT)
        {switch(snake2().head.t){
            case Orientation::UP:
                snake2().head.t=Orientation::LEFT;
                break;
            case Orientation::LEFT:
                snake2().head.t=Orientation::DOWN;
                break;
            case Orientation::RIGHT:
                snake2().head.t=Orientation::UP;
                break;
            case Orientation::DOWN:
                snake2().head.t=Orientation::RIGHT;
                break;

            }

        move2();
        }
        if (input2.action==Input::Action::TURN_RIGHT)
        {switch(snake2().head.t){
            case Orientation::UP:
                snake2().head.t=Orientation::RIGHT;
                break;
            case Orientation::LEFT:
                snake2().head.t=Orientation::UP;
                break;
            case Orientation::RIGHT:
                snake2().head.t=Orientation::DOWN;
                break;
            case Orientation::DOWN:
                snake2().head.t=Orientation::LEFT;
                break;

            }

        move2();
        }
    }




    // should return who has just won, if any. May also compute display
    Result buildPlayerFeedbacks(Feedback &feedback1, Feedback &feedback2)
 {

        bool obs1_other=(find(snake2().body.begin(),snake2().body.end(),Position {snake1().head.x,snake1().head.y}) !=snake2().body.end());
        bool obs1=(find(snake1().body.begin(),snake1().body.end(),Position {snake1().head.x,snake1().head.y}) !=snake1().body.end());
        bool obs2_other=(find(snake1().body.begin(),snake1().body.end(),Position {snake2().head.x,snake2().head.y}) !=snake1().body.end());
        bool obs2=(find(snake2().body.begin(),snake2().body.end(),Position {snake2().head.x,snake2().head.y}) !=snake2().body.end());
       feedback1.pose=snake1();
       feedback2.pose=snake2();
       feedback1.pose_other=snake2();
       feedback2.pose_other=snake1();
       feedback1.apple=display_msg.apples;
       feedback2.apple=display_msg.apples;

       bool obs_up,obs_down,obs_left,obs_right;
       obs_up= feedback1.pose.head.y<100;
       obs_left= feedback1.pose.head.x<0;
       obs_down= feedback1.pose.head.y>480;
       obs_right= feedback1.pose.head.x>480;
       bool obst=obs_up or obs_down or obs_left or obs_right or obs1 or obs1_other;

        bool equal= feedback1.pose.head==feedback2.pose.head;
        bool obs2_up,obs2_down,obs2_left,obs2_right  ;

        obs2_up= feedback2.pose.head.y<100;
        obs2_left= feedback2.pose.head.x<0;
        obs2_down= feedback2.pose.head.y>480;
        obs2_right= feedback2.pose.head.x>480;
        bool obs2t=obs2_up or obs2_down or obs2_left or obs2_right or obs2 or obs2_other;

         if( obst and !obs2t){return Result::P2_WINS;}
         if( obs2t and !obst ){return Result::P1_WINS;}
         if ((obst and obs2t) or equal )
         {
             if(display_msg.score1<display_msg.score2)
             {
                 return Result::P2_WINS;
             }
             if(display_msg.score1>display_msg.score2)
             {
                 return Result::P1_WINS;
             }
             if(display_msg.score1==display_msg.score2)
             {
                 return Result::DRAW;
             }
         }


        return Result::NONE;    // game goes on


    }

private:


    int max_apples = 4;
    int nbr_apples = 1;
  Display display_msg;
};

#endif 
