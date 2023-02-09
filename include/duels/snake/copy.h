#ifndef SNAKE_AI_H
#define SNAKE_AI_H
#include"iostream"
#include <duels/player.h>
#include <duels/snake/msg.h>
#include"mechanics.h"
using namespace std;
namespace duels {
namespace snake {

// built-in AI class, should be heavily adapted to your needs
class SnakeAI : public duels::Player<Input, Feedback>
{
public:
    SnakeAI(int difficulty = 1) : difficulty(difficulty) {}
    int score_self=feedback.pose.body.size();
    int score_other=feedback.pose_other.body.size();

    bool obstacle(int x,int y)
    {

        bool obs_r, obs_l, obs_u, obs_d;
        obs_r=(x > 480 );
        obs_l=( y< 0 );
        obs_u=(y < 100 );
        obs_d=(y > 480 );
        bool OBS= obs_r or obs_l or obs_u or obs_d;
        bool obs_other=(find(feedback.pose_other.body.begin(),feedback.pose_other.body.end(),Position {x,y}) !=feedback.pose_other.body.end());
        bool obs_self=(find(feedback.pose.body.begin(),feedback.pose.body.end(),Position {x,y}) !=feedback.pose.body.end());
        vector<Position> head_neighbor={};
        head_neighbor.push_back({feedback.pose_other.head.x+20,feedback.pose_other.head.y});
        head_neighbor.push_back({feedback.pose_other.head.x,feedback.pose_other.head.y+20});
        head_neighbor.push_back({feedback.pose_other.head.x-20,feedback.pose_other.head.y});
        head_neighbor.push_back({feedback.pose_other.head.x,feedback.pose_other.head.y-20});
        bool obs_head_other_neighbor=(find(head_neighbor.begin(),head_neighbor.end(),Position {x,y}) !=head_neighbor.end());


        if (score_self>score_other)
        {return OBS or obs_self or obs_other   ;}
        else{return OBS or obs_self or obs_other or obs_head_other_neighbor;}
        // return OBS or obs_self or obs_other or obs_head_other_neighbor   ;
    };
    void updateInput()
    {
        // in this function the `feedback` member variable was updated from the game
        // TODO update the `input` member variable
        // the `difficulty` member variable may be used to tune your AI (0 = most stupidest)
        // do not hesitate to create a .cpp file if this function is long
        if(difficulty == 1)
        {
            bool obs_r, obs_l, obs_u, obs_d;
            obs_r=(feedback.pose.head.x == 480 and feedback.pose.head.t == Orientation::RIGHT);
            obs_l=(feedback.pose.head.x == 0 and feedback.pose.head.t == Orientation::LEFT);
            obs_u=(feedback.pose.head.y == 100 and feedback.pose.head.t == Orientation::UP);
            obs_d=(feedback.pose.head.y == 480 and feedback.pose.head.t == Orientation::DOWN);


            if(obs_r or obs_d or obs_l or obs_u)
            {
                input.action=Input::Action::TURN_RIGHT;

            }
            else{input.action=Input::Action::MOVE;}



        }
        if(difficulty == 4)
        {
            bool obs_r, obs_l, obs_u, obs_d;

            obs_r=(feedback.pose.head.x == 480 and feedback.pose.head.t == Orientation::RIGHT);
            obs_l=(feedback.pose.head.x == 0 and feedback.pose.head.t == Orientation::LEFT);
            obs_u=(feedback.pose.head.y == 100 and feedback.pose.head.t == Orientation::UP);
            obs_d=(feedback.pose.head.y == 480 and feedback.pose.head.t == Orientation::DOWN);


            if(obs_r or obs_d or obs_l or obs_u)
            {
                input.action=Input::Action::TURN_LEFT;

            }
            else{input.action=Input::Action::MOVE;}



        }
        if(difficulty == 2)
        {
            bool obs_r, obs_l, obs_u, obs_d;
            obs_r=(feedback.pose.head.x == 480 and feedback.pose.head.t == Orientation::RIGHT);
            obs_l=(feedback.pose.head.x == 0 and feedback.pose.head.t == Orientation::LEFT);
            obs_u=(feedback.pose.head.y == 100 and feedback.pose.head.t == Orientation::UP);
            obs_d=(feedback.pose.head.y == 480 and feedback.pose.head.t == Orientation::DOWN);
            bool OBS= obs_r or obs_l or obs_u or obs_d;
            //bool OBS=obstacle(feedback.pose.head.x,feedback.pose.head.y);
            bool apple_up = feedback.pose.head.y>feedback.apple.y;
            bool apple_left = feedback.pose.head.x>feedback.apple.x;

            if(OBS){input.action=Input::Action::TURN_RIGHT;}

            else
            {
                input.action=Input::Action::MOVE;
                {if(feedback.pose.head.y==feedback.apple.y)
                    {
                        if(feedback.pose.head.x>feedback.apple.x)
                        {if(feedback.pose.head.t==Orientation::DOWN)
                            {input.action=Input::Action::TURN_RIGHT;}
                            if(feedback.pose.head.t==Orientation::UP)
                            {input.action=Input::Action::TURN_LEFT;}
                            if(feedback.pose.head.t==Orientation::LEFT)
                            {input.action=Input::Action::MOVE;}
                            if(feedback.pose.head.t==Orientation::RIGHT)
                            {input.action=Input::Action::MOVE;}
                        }


                        if(feedback.pose.head.x<feedback.apple.x)
                        {if(feedback.pose.head.t==Orientation::DOWN)
                            {input.action=Input::Action::TURN_LEFT;}
                            if(feedback.pose.head.t==Orientation::UP)
                            {input.action=Input::Action::TURN_RIGHT;}
                            if(feedback.pose.head.t==Orientation::LEFT)
                            {input.action=Input::Action::MOVE;}
                            if(feedback.pose.head.t==Orientation::RIGHT)
                            {input.action=Input::Action::MOVE;}
                        }

                    }
                    if(feedback.pose.head.x==feedback.apple.x)
                    {
                        if(feedback.pose.head.y>feedback.apple.y)
                        {if(feedback.pose.head.t==Orientation::DOWN)
                            {input.action=Input::Action::MOVE;}
                            if(feedback.pose.head.t==Orientation::UP)
                            {input.action=Input::Action::MOVE;}
                            if(feedback.pose.head.t==Orientation::LEFT)
                            {input.action=Input::Action::TURN_RIGHT;}
                            if(feedback.pose.head.t==Orientation::RIGHT)
                            {input.action=Input::Action::TURN_LEFT;}
                        }




                        if(feedback.pose.head.y<feedback.apple.y)
                        {if(feedback.pose.head.t==Orientation::DOWN)
                            {input.action=Input::Action::MOVE;}
                            if(feedback.pose.head.t==Orientation::UP)
                            {input.action=Input::Action::MOVE;}
                            if(feedback.pose.head.t==Orientation::LEFT)
                            {input.action=Input::Action::TURN_LEFT;}
                            if(feedback.pose.head.t==Orientation::RIGHT)
                            {input.action=Input::Action::TURN_RIGHT;}
                        }
                    }

                }}







        }
        if(difficulty == 3){
            input.action=Input::Action::TURN_LEFT;
        }
        if(difficulty==5){
            srand(time(0));
            switch(rand()%5){
            case 0:
                input.action = Input::Action::MOVE;
                break;
            case 1:
                input.action = Input::Action::TURN_LEFT;
                break;
            case 2:
                input.action = Input::Action::TURN_RIGHT;
                break;
            case 3:
                input.action = Input::Action::MOVE;
                break;
            case 4:
                input.action = Input::Action::MOVE;
                break;
            }



        }

        if(difficulty == 6)
        {

            int head_x=feedback.pose.head.x;
            int head_y=feedback.pose.head.y;

            bool apple_equ_x = feedback.pose.head.x==feedback.apple.x;
            bool apple_equ_y = feedback.pose.head.y==feedback.apple.y;
            bool apple_up = feedback.pose.head.y>feedback.apple.y;
            bool apple_left = feedback.pose.head.x>feedback.apple.x;
            bool apple_down = feedback.pose.head.y<feedback.apple.y;
            bool apple_right = feedback.pose.head.x<feedback.apple.x;
            bool can_move_r=!obstacle(head_x+20,head_y);
            bool can_move_l=!obstacle(head_x-20,head_y);
            bool can_move_u=!obstacle(head_x,head_y-20);
            bool can_move_d=!obstacle(head_x,head_y+20);


            switch(feedback.pose.head.t){
            case Orientation::RIGHT :
                if (apple_right)
                {
                    if (can_move_r)
                    {

                        input.action = Input::Action::MOVE;
                        cout<<0<<endl;

                    }
                    else
                    {
                        if(apple_down or apple_equ_y )
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<1<<endl;


                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<2<<endl;

                            }
                        }
                        else{
                            if(can_move_u)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<3<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<4<<endl;
                            }
                        }
                    }

                }
                if(apple_left)
                {
                    if(apple_up or apple_equ_y)
                    {
                        if(can_move_u)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<5<<endl;

                        }
                        else
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<6<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<7<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_d)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<8<<endl;
                        }
                        else
                        {
                            if(can_move_u)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<9<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<10<<endl;
                            }

                        }
                    }
                }
                if(apple_equ_x)
                {
                    if(apple_down or apple_equ_y)
                    {
                        if(can_move_d)
                        {

                            input.action = Input::Action::TURN_RIGHT;
                            cout<<11<<endl;

                        }
                        else
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<12<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<13<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_u)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<14<<endl;
                        }
                        else
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<15<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<16<<endl;
                            }
                        }
                    }
                }
                break;


            case Orientation::LEFT :
                if (apple_left)
                {
                    if (can_move_l)
                    {

                        input.action = Input::Action::MOVE;
                        cout<<17<<endl;

                    }
                    else
                    {
                        if(apple_down or apple_equ_y )
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<18<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<19<<endl;

                            }
                        }
                        else{
                            if(can_move_u)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<20<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<21<<endl;
                            }
                        }
                    }

                }
                if(apple_right)
                {
                    if(apple_up or apple_equ_y)
                    {
                        if(can_move_u)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<22<<endl;

                        }
                        else
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<23<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<24<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_d)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<25<<endl;
                        }
                        else
                        {
                            if(can_move_u)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<26<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<27<<endl;
                            }

                        }
                    }
                }
                if(apple_equ_x)
                {
                    if(apple_down or apple_equ_y)
                    {
                        if(can_move_d)
                        {

                            input.action = Input::Action::TURN_LEFT;
                            cout<<28<<endl;

                        }
                        else
                        {
                            if(can_move_l)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<29<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<30<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_u)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<31<<endl;
                        }
                        else
                        {
                            if(can_move_l)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<32<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<33<<endl;
                            }
                        }
                    }
                }
                break;
            case Orientation::UP :
                if (apple_up)
                {
                    if (can_move_u)
                    {

                        input.action = Input::Action::MOVE;
                        cout<<34<<endl;

                    }
                    else
                    {
                        if(apple_right or apple_equ_x )
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<35<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<36<<endl;

                            }
                        }
                        else{
                            if(can_move_l)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<37<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<38<<endl;
                            }
                        }
                    }

                }
                if(apple_down)
                {
                    if(apple_right or apple_equ_x)
                    {
                        if(can_move_r)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<39<<endl;

                        }
                        else
                        {
                            if(can_move_l)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<40<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<41<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_l)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<42<<endl;
                        }
                        else
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<43<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<44<<endl;
                            }

                        }
                    }
                }
                if(apple_equ_y)
                {
                    if(apple_right or apple_equ_x)
                    {
                        if(can_move_r)
                        {

                            input.action = Input::Action::TURN_RIGHT;
                            cout<<45<<endl;

                        }
                        else
                        {
                            if(can_move_u)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<46<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<46<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_l)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<47<<endl;
                        }
                        else
                        {
                            if(can_move_u)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<48<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<49<<endl;
                            }
                        }
                    }
                }
                break;
            case Orientation::DOWN :
                if (apple_down)
                {
                    if (can_move_d)
                    {

                        input.action = Input::Action::MOVE;
                        cout<<50<<endl;

                    }
                    else
                    {
                        if(apple_right or apple_equ_x )
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<51<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<52<<endl;

                            }
                        }
                        else{
                            if(can_move_l)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<53<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<54<<endl;
                            }
                        }
                    }

                }
                if(apple_up)
                {
                    if(apple_right or apple_equ_x)
                    {
                        if(can_move_r)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<55<<endl;

                        }
                        else
                        {
                            if(can_move_l)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<56<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<57<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_l)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<58<<endl;
                        }
                        else
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<59<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<60<<endl;
                            }

                        }
                    }
                }
                if(apple_equ_y)
                {
                    if(apple_right or apple_equ_x)
                    {
                        if(can_move_r)
                        {

                            input.action = Input::Action::TURN_LEFT;
                            cout<<61<<endl;

                        }
                        else
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<62<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<63<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_l)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<64<<endl;
                        }
                        else
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<65<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<66<<endl;
                            }
                        }
                    }
                }
                break;


            }
        }

        if(difficulty == 7)
        {
            Position objective=feedback.apple;
            Position middle = {240,300};

            int head_x=feedback.pose.head.x;
            int head_y=feedback.pose.head.y;
            int dist_self=abs(head_x-feedback.apple.x)+abs(head_y-feedback.apple.y);
            int dist_other=abs(feedback.pose_other.head.x-feedback.apple.x)+abs(feedback.pose_other.head.y-feedback.apple.y);
            int retard = -(dist_other-dist_self);
            if (retard>100){objective=middle;};
            bool apple_equ_x = feedback.pose.head.x==objective.x;
            bool apple_equ_y = feedback.pose.head.y==objective.y;
            bool apple_up = feedback.pose.head.y>objective.y;
            bool apple_left = feedback.pose.head.x>objective.x;
            bool apple_down = feedback.pose.head.y<objective.y;
            bool apple_right = feedback.pose.head.x<objective.x;
            bool can_move_r=!obstacle(head_x+20,head_y);
            bool can_move_l=!obstacle(head_x-20,head_y);
            bool can_move_u=!obstacle(head_x,head_y-20);
            bool can_move_d=!obstacle(head_x,head_y+20);


            switch(feedback.pose.head.t){
            case Orientation::RIGHT :
                if (apple_right)
                {
                    if (can_move_r)
                    {

                        input.action = Input::Action::MOVE;
                        cout<<0<<endl;

                    }
                    else
                    {
                        if(apple_down or apple_equ_y )
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<1<<endl;


                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<2<<endl;

                            }
                        }
                        else{
                            if(can_move_u)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<3<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<4<<endl;
                            }
                        }
                    }

                }
                if(apple_left)
                {
                    if(apple_up or apple_equ_y)
                    {
                        if(can_move_u)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<5<<endl;

                        }
                        else
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<6<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<7<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_d)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<8<<endl;
                        }
                        else
                        {
                            if(can_move_u)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<9<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<10<<endl;
                            }

                        }
                    }
                }
                if(apple_equ_x)
                {
                    if(apple_down or apple_equ_y)
                    {
                        if(can_move_d)
                        {

                            input.action = Input::Action::TURN_RIGHT;
                            cout<<11<<endl;

                        }
                        else
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<12<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<13<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_u)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<14<<endl;
                        }
                        else
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<15<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<16<<endl;
                            }
                        }
                    }
                }
                break;


            case Orientation::LEFT :
                if (apple_left)
                {
                    if (can_move_l)
                    {

                        input.action = Input::Action::MOVE;
                        cout<<17<<endl;

                    }
                    else
                    {
                        if(apple_down or apple_equ_y )
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<18<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<19<<endl;

                            }
                        }
                        else{
                            if(can_move_u)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<20<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<21<<endl;
                            }
                        }
                    }

                }
                if(apple_right)
                {
                    if(apple_up or apple_equ_y)
                    {
                        if(can_move_u)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<22<<endl;

                        }
                        else
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<23<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<24<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_d)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<25<<endl;
                        }
                        else
                        {
                            if(can_move_u)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<26<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<27<<endl;
                            }

                        }
                    }
                }
                if(apple_equ_x)
                {
                    if(apple_down or apple_equ_y)
                    {
                        if(can_move_d)
                        {

                            input.action = Input::Action::TURN_LEFT;
                            cout<<28<<endl;

                        }
                        else
                        {
                            if(can_move_l)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<29<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<30<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_u)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<31<<endl;
                        }
                        else
                        {
                            if(can_move_l)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<32<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<33<<endl;
                            }
                        }
                    }
                }
                break;
            case Orientation::UP :
                if (apple_up)
                {
                    if (can_move_u)
                    {

                        input.action = Input::Action::MOVE;
                        cout<<34<<endl;

                    }
                    else
                    {
                        if(apple_right or apple_equ_x )
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<35<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<36<<endl;

                            }
                        }
                        else{
                            if(can_move_l)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<37<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<38<<endl;
                            }
                        }
                    }

                }
                if(apple_down)
                {
                    if(apple_right or apple_equ_x)
                    {
                        if(can_move_r)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<39<<endl;

                        }
                        else
                        {
                            if(can_move_l)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<40<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<41<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_l)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<42<<endl;
                        }
                        else
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<43<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<44<<endl;
                            }

                        }
                    }
                }
                if(apple_equ_y)
                {
                    if(apple_right or apple_equ_x)
                    {
                        if(can_move_r)
                        {

                            input.action = Input::Action::TURN_RIGHT;
                            cout<<45<<endl;

                        }
                        else
                        {
                            if(can_move_u)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<46<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<46<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_l)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<47<<endl;
                        }
                        else
                        {
                            if(can_move_u)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<48<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<49<<endl;
                            }
                        }
                    }
                }
                break;
            case Orientation::DOWN :
                if (apple_down)
                {
                    if (can_move_d)
                    {

                        input.action = Input::Action::MOVE;
                        cout<<50<<endl;

                    }
                    else
                    {
                        if(apple_right or apple_equ_x )
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<51<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<52<<endl;

                            }
                        }
                        else{
                            if(can_move_l)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<53<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<54<<endl;
                            }
                        }
                    }

                }
                if(apple_up)
                {
                    if(apple_right or apple_equ_x)
                    {
                        if(can_move_r)
                        {
                            input.action = Input::Action::TURN_LEFT;
                            cout<<55<<endl;

                        }
                        else
                        {
                            if(can_move_l)
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<56<<endl;

                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<57<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_l)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<58<<endl;
                        }
                        else
                        {
                            if(can_move_r)
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<59<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::MOVE;
                                cout<<60<<endl;
                            }

                        }
                    }
                }
                if(apple_equ_y)
                {
                    if(apple_right or apple_equ_x)
                    {
                        if(can_move_r)
                        {

                            input.action = Input::Action::TURN_LEFT;
                            cout<<61<<endl;

                        }
                        else
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<62<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_RIGHT;
                                cout<<63<<endl;
                            }
                        }
                    }
                    else
                    {
                        if(can_move_l)
                        {
                            input.action = Input::Action::TURN_RIGHT;
                            cout<<64<<endl;
                        }
                        else
                        {
                            if(can_move_d)
                            {
                                input.action = Input::Action::MOVE;
                                cout<<65<<endl;
                            }
                            else
                            {
                                input.action = Input::Action::TURN_LEFT;
                                cout<<66<<endl;
                            }
                        }
                    }
                }
                break;


            }
        }
    }




private:
    int difficulty = 1;
};
}
}
#endif
