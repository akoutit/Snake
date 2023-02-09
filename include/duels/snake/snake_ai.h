#ifndef SNAKE_AI_H
#define SNAKE_AI_H
#include"iostream"
#include <duels/player.h>
#include <duels/snake/msg.h>
#include"mechanics.h"
#include<algorithm>
using namespace std;
namespace duels {
namespace snake {

// built-in AI class, should be heavily adapted to your needs
class SnakeAI : public duels::Player<Input, Feedback>
{
public:
    SnakeAI(int difficulty = 1) : difficulty(difficulty) {}

    double dist_to_apple(int x,int y)
    {

        return abs(x-feedback.apple.x)+abs(y-feedback.apple.y);
    }

    vector<Position> voisinage()
       {
           vector<Position> next_Pose = {};
           int head_x = feedback.pose.head.x;
           int head_y = feedback.pose.head.y;
           switch(feedback.pose.head.t){
               case Orientation::RIGHT :
                   next_Pose.push_back({head_x+size_case,head_y});
                   next_Pose.push_back({head_x,head_y+size_case});
                   next_Pose.push_back({head_x,head_y-size_case});
               break;
               case Orientation::LEFT :
                   next_Pose.push_back({head_x-size_case,head_y});
                   next_Pose.push_back({head_x,head_y+size_case});
                   next_Pose.push_back({head_x,head_y-size_case});
               break;
               case Orientation::UP :
                   next_Pose.push_back({head_x+size_case,head_y});
                   next_Pose.push_back({head_x-size_case,head_y});
                   next_Pose.push_back({head_x,head_y-size_case});
               break;
               case Orientation::DOWN :
                   next_Pose.push_back({head_x+size_case,head_y});
                   next_Pose.push_back({head_x-size_case,head_y});
                   next_Pose.push_back({head_x,head_y+size_case});
               break;
           }
           return next_Pose;
       }

    void move_to(int x,int y)
        {
        int head_x=feedback.pose.head.x;
        int head_y=feedback.pose.head.y;

        int d_x=head_x-x;
        int d_y=head_y-y;
        switch(feedback.pose.head.t)
        {
        case Orientation::RIGHT:
            if (d_y==0){input.action=Input::Action::MOVE;}
            if (d_y>0){input.action=Input::Action::TURN_LEFT;}
            if (d_y<0){input.action=Input::Action::TURN_RIGHT;}
            break;
        case Orientation::LEFT:
            if (d_y==0){input.action=Input::Action::MOVE;}
            if (d_y<0){input.action=Input::Action::TURN_LEFT;}
            if (d_y>0){input.action=Input::Action::TURN_RIGHT;}
            break;
        case Orientation::UP:
            if (d_x==0){input.action=Input::Action::MOVE;}
            if (d_x>0){input.action=Input::Action::TURN_LEFT;}
            if (d_x<0){input.action=Input::Action::TURN_RIGHT;}
            break;
        case Orientation::DOWN:
            if (d_x==0){input.action=Input::Action::MOVE;}
            if (d_x<0){input.action=Input::Action::TURN_LEFT;}
            if (d_x>0){input.action=Input::Action::TURN_RIGHT;}
            break;




            }
    }

    Position other_next_Position()
       {
           Position other_next_Pose;
           switch(feedback.pose_other.head.t){
               case Orientation::RIGHT :
                   other_next_Pose = {feedback.pose_other.head.x+size_case,feedback.pose_other.head.y};
               break;
               case Orientation::LEFT :
                   other_next_Pose = {feedback.pose_other.head.x-size_case,feedback.pose_other.head.y};
               break;
               case Orientation::UP :
                   other_next_Pose = {feedback.pose_other.head.x,feedback.pose_other.head.y-size_case};
               break;
               case Orientation::DOWN :
                   other_next_Pose = {feedback.pose_other.head.x,feedback.pose_other.head.y+size_case};
               break;
       }
           return other_next_Pose;
    }

    void try_kill()
           {
               vector<Position> next_Pose=voisinage();
               Position other_next_Pose = other_next_Position();
               if ((find(next_Pose.begin(),next_Pose.end(),other_next_Pose) !=next_Pose.end()) and !obstacle(other_next_Pose.x,other_next_Pose.y,true))
               {
                   move_to(other_next_Pose.x,other_next_Pose.y);
               };
           }


    bool wall(int x ,int y)
    {

        bool wall_right = ( x > 480 );
        bool wall_left = ( x < 0 );
        bool wall_up = ( y < 100 );
        bool wall_down = ( y > 480 );
        return wall_right or wall_left or wall_up or wall_down;
    }

    bool obstacle(int x, int y,bool next)
        {
            bool other_snake = (find(feedback.pose_other.body.begin(),feedback.pose_other.body.end(),Position {x,y}) !=feedback.pose_other.body.end());
            bool my_snake = (find(feedback.pose.body.begin(),feedback.pose.body.end(),Position {x,y}) !=feedback.pose.body.end());

            vector<Position> head_neighbor = {};
            head_neighbor.push_back({feedback.pose_other.head.x+size_case,feedback.pose_other.head.y});
            head_neighbor.push_back({feedback.pose_other.head.x,feedback.pose_other.head.y+size_case});
            head_neighbor.push_back({feedback.pose_other.head.x-size_case,feedback.pose_other.head.y});
            head_neighbor.push_back({feedback.pose_other.head.x,feedback.pose_other.head.y-size_case});
            bool  next_step_obstacle = (find(head_neighbor.begin(),head_neighbor.end(),Position {x,y}) !=head_neighbor.end());


            if (score_self<=score_other and next)
            {
                return  wall(x,y) or other_snake or my_snake or next_step_obstacle;
            }
            else
            {
                return  wall(x,y) or other_snake or my_snake;
            }

        };


    bool wall(){
        int x,y;
        x=feedback.pose.head.x;
        y=feedback.pose.head.y;
        Orientation t =feedback.pose.head.t;
        bool wall_right = ( x == 480 ) and t == Orientation::RIGHT;
        bool wall_left = ( x == 0 )and t == Orientation::LEFT;
        bool wall_up = ( y == 100 )and t == Orientation::UP;
        bool wall_down = ( y == 480 )and t == Orientation::DOWN;
        return wall_right or wall_left or wall_up or wall_down;

    }

    void calcul_distance_voisins(int x, int y, array<array<int,22>,27> &grid)
    {
        int distance_obstacle = 10000000;
        int distance_parent = grid[x][y] ;

        bool must_calcul_right = false;
        bool must_calcul_left = false;
        bool must_calcul_up = false;
        bool must_calcul_down = false;

        // fils droite
        if ( ((x+1)<25) && (distance_parent+1 < grid[x+1][y]) && (grid[x+1][y]<distance_obstacle) )
        {
            grid[x+1][y] = distance_parent + 1;
            must_calcul_right = true;
        }
        // fils gauche
        if ( ((x-1)>=0) && (distance_parent+1 < grid[x-1][y]) && (grid[x-1][y]<distance_obstacle) )
        {
            grid[x-1][y] = distance_parent + 1;
            must_calcul_left = true;
        }
        // fils haut
        if ( ((y-1)>=0) && (distance_parent+1 < grid[x][y-1]) && (grid[x][y-1]<distance_obstacle) )
        {
            grid[x][y-1] = distance_parent + 1;
            must_calcul_up = true;
        }
        // fils bas
        if ( ((y+1)<20) && (distance_parent+1 < grid[x][y+1]) && (grid[x][y+1]<distance_obstacle) )
        {
            grid[x][y+1] = distance_parent + 1;
            must_calcul_down = true;
        }

        if (must_calcul_right)
            calcul_distance_voisins(x+1,y,grid);
        if (must_calcul_left)
            calcul_distance_voisins(x-1,y,grid);
        if (must_calcul_up)
            calcul_distance_voisins(x,y-1,grid);
        if (must_calcul_down)
            calcul_distance_voisins(x,y+1,grid);
    }

    array<array<int,22>,27> distance_grid(Position objective)
    {
        int distance_obstacle = 10000000;
        int default_value = 100000;
        int offset_y = 80;
        int offset_x = -20;

        array<array<int,22>,27> grid;

        for (int x=0; x<27; x=x+1)
        {
            for (int y=0; y<22; y=y+1)
            {
                if (obstacle(offset_x+size_case*x,offset_y+size_case*y,true))
                    grid[x][y] = distance_obstacle;
                else
                    grid[x][y] = default_value;
            }
        }

        int x_objective = (objective.x-offset_x)/size_case;
        int y_objective = (objective.y-offset_y)/size_case;

        grid[x_objective][y_objective] = 0;
        calcul_distance_voisins(x_objective,y_objective,grid);
        if(obstacle(objective.x,objective.y,true))
        {
            grid[x_objective][y_objective] = distance_obstacle;
        }

        return grid;
    }







    void updateInput()
    {
        // in this function the `feedback` member variable was updated from the game
        // TODO update the `input` member variable
        // the `difficulty` member variable may be used to tune your AI (0 = most stupidest)
        // do not hesitate to create a .cpp file if this function is long
        score_self=feedback.pose.body.size();
        score_other=feedback.pose_other.body.size();


        // Se déplace aléatoirement sur la map sans éviter les obstacles. Continue tout droit avec une probabilité de 3/5.
        if(difficulty==1)
        {
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


        //Tourne autour de la map en évitant les murs. Ne cherche pas à obtenir la pomme.
        if(difficulty == 2)
        {
            if(wall())
            {
                input.action=Input::Action::TURN_RIGHT;

            }
            else{input.action=Input::Action::MOVE;}



        }


        // Tourne autour de la map et va dans la direction de la pomme lorsqu'il est aligné sur la même ligne ou la même colonne. N'évite pas l'autre Snake.
        if(difficulty == 3)
        {
            bool apple_up = feedback.pose.head.y>feedback.apple.y;
            bool apple_left = feedback.pose.head.x>feedback.apple.x;

            if(wall()){input.action=Input::Action::TURN_RIGHT;}

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


        // Se déplace dans la direction de la pomme et évitent tous les obstacles possibles.
        if(difficulty == 4)
        {
                    int head_x=feedback.pose.head.x;
                    int head_y=feedback.pose.head.y;

                    bool apple_equ_x = feedback.pose.head.x==feedback.apple.x;
                    bool apple_equ_y = feedback.pose.head.y==feedback.apple.y;

                    bool apple_up = feedback.pose.head.y>feedback.apple.y;
                    bool apple_left = feedback.pose.head.x>feedback.apple.x;
                    bool apple_down = feedback.pose.head.y<feedback.apple.y;
                    bool apple_right = feedback.pose.head.x<feedback.apple.x;

                    bool can_move_r=!obstacle(head_x+size_case,head_y,false);
                    bool can_move_l=!obstacle(head_x-size_case,head_y,false);
                    bool can_move_u=!obstacle(head_x,head_y-size_case,false);
                    bool can_move_d=!obstacle(head_x,head_y+size_case,false);


                    switch(feedback.pose.head.t){
                    case Orientation::RIGHT :
                        if (apple_right)
                        {
                            if (can_move_r)
                            {
                                input.action = Input::Action::MOVE;
                            }
                            else
                            {
                                if(apple_down or apple_equ_y )
                                {
                                    if(can_move_d)
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                }
                                else{
                                    if(can_move_u)
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
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
                                }
                                else
                                {
                                    if(can_move_d)
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                }
                            }
                            else
                            {
                                if(can_move_d)
                                {
                                    input.action = Input::Action::TURN_RIGHT;
                                }
                                else
                                {
                                    if(can_move_u)
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::MOVE;
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
                                }
                                else
                                {
                                    if(can_move_r)
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                }
                            }
                            else
                            {
                                if(can_move_u)
                                {
                                    input.action = Input::Action::TURN_LEFT;
                                }
                                else
                                {
                                    if(can_move_r)
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
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
                            }
                            else
                            {
                                if(apple_down or apple_equ_y )
                                {
                                    if(can_move_d)
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                }
                                else{
                                    if(can_move_u)
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_LEFT;
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
                                }
                                else
                                {
                                    if(can_move_d)
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                }
                            }
                            else
                            {
                                if(can_move_d)
                                {
                                    input.action = Input::Action::TURN_LEFT;
                                }
                                else
                                {
                                    if(can_move_u)
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::MOVE;
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
                                }
                                else
                                {
                                    if(can_move_l)
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                }
                            }
                            else
                            {
                                if(can_move_u)
                                {
                                    input.action = Input::Action::TURN_RIGHT;
                                }
                                else
                                {
                                    if(can_move_l)
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_LEFT;
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
                            }
                            else
                            {
                                if(apple_right or apple_equ_x )
                                {
                                    if(can_move_r)
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                }
                                else{
                                    if(can_move_l)
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
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
                                }
                                else
                                {
                                    if(can_move_l)
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                }
                            }
                            else
                            {
                                if(can_move_l)
                                {
                                    input.action = Input::Action::TURN_LEFT;
                                }
                                else
                                {
                                    if(can_move_r)
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::MOVE;
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
                                }
                                else
                                {
                                    if(can_move_u)
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                }
                            }
                            else
                            {
                                if(can_move_l)
                                {
                                    input.action = Input::Action::TURN_LEFT;
                                }
                                else
                                {
                                    if(can_move_u)
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
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
                            }
                            else
                            {
                                if(apple_right or apple_equ_x )
                                {
                                    if(can_move_r)
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                }
                                else{
                                    if(can_move_l)
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_LEFT;
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
                                }
                                else
                                {
                                    if(can_move_l)
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                }
                            }
                            else
                            {
                                if(can_move_l)
                                {
                                    input.action = Input::Action::TURN_RIGHT;
                                }
                                else
                                {
                                    if(can_move_r)
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::MOVE;
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
                                }
                                else
                                {
                                    if(can_move_d)
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_RIGHT;
                                    }
                                }
                            }
                            else
                            {
                                if(can_move_l)
                                {
                                    input.action = Input::Action::TURN_RIGHT;
                                }
                                else
                                {
                                    if(can_move_d)
                                    {
                                        input.action = Input::Action::MOVE;
                                    }
                                    else
                                    {
                                        input.action = Input::Action::TURN_LEFT;
                                    }
                                }
                            }
                        }
                        break;


                    }
                }


        // Calcule le chemin réduisant la distance à la pomme pour les cases voisines
        // Evite tous les obstacles possibles et anticipe les positions de l'autre snake au prochain coup.
        if(difficulty == 5)
        {
            vector<Position> V=voisinage();
            vector<Position> vois_not_obs;
            int x=-1,y=-1;
            double d=dist_to_apple(feedback.pose.head.x,feedback.pose.head.y);
            for(int i=0; i<3; i++){
                Position P = V[i];
                if (!obstacle(P.x,P.y,true)){

                    vois_not_obs.push_back(P);
                    if ((dist_to_apple(P.x,P.y)<d)){
                        d=dist_to_apple(P.x,P.y);
                        x=P.x;
                        y=P.y;
                }
                }
            }


            if(x==-1)
            {
                if(vois_not_obs.size()==0){input.action = Input::Action::MOVE;}
                else{x=vois_not_obs[0].x;y=vois_not_obs[0].y;}}
            move_to(x,y);

        }




        // Calcul le plus court chemin vers la pomme à l'aide d'un algorithme récursif.
        // Evite tous les obstacles possibles et anticipe les positions de l'autre snake au prochain coup.
        // Essaye de rejoindre le centre de la map lorsqu'il est trop en retard par rapport à l'autre snake.
        // Essaye de tuer l'autre snake lorsque c'est possible et qu'il a un score plus élevé.
        if (difficulty == 6)
               {
                   int offset_y = 80;
                   int offset_x = -20;

                   Position objective = feedback.apple;

                   int distance_obstacle = 10000000;

                   Position middle = {240,300};
                   int head_x = feedback.pose.head.x;
                   int head_y = feedback.pose.head.y;

                   int my_distance = abs(head_x-feedback.apple.x)+abs(head_y-feedback.apple.y);
                   int other_distance = abs(feedback.pose_other.head.x-feedback.apple.x)+abs(feedback.pose_other.head.y-feedback.apple.y);
                   int retard = my_distance-other_distance;

                   if (retard>=5*size_case)
                       objective = middle;





                   auto grid_of_distances = distance_grid(objective);



                   Position head_in_grid = { (head_x-offset_x)/size_case , (head_y-offset_y)/size_case };
                   Position other_head_in_grid = { (feedback.pose_other.head.x-offset_x)/size_case , (feedback.pose_other.head.y-offset_y)/size_case };




                   int distance_left = grid_of_distances[head_in_grid.x-1][head_in_grid.y];
                   int distance_right = grid_of_distances[head_in_grid.x+1][head_in_grid.y];
                   int distance_up = grid_of_distances[head_in_grid.x][head_in_grid.y-1];
                   int distance_down = grid_of_distances[head_in_grid.x][head_in_grid.y+1];

                   auto distance_voisins = {distance_left,distance_right,distance_up,distance_down};

                   if (*min_element(distance_voisins.begin(),distance_voisins.end())==distance_obstacle)
                       input.action = Input::Action::MOVE;
                   else
                   {
                       if (*min_element(distance_voisins.begin(),distance_voisins.end())==distance_left)
                           move_to(head_x-size_case,head_y);
                       if (*min_element(distance_voisins.begin(),distance_voisins.end())==distance_right)
                           move_to(head_x+size_case,head_y);
                       if (*min_element(distance_voisins.begin(),distance_voisins.end())==distance_up)
                           move_to(head_x,head_y-size_case);
                       if (*min_element(distance_voisins.begin(),distance_voisins.end())==distance_down)
                           move_to(head_x,head_y+size_case);

                       if (score_self > score_other)
                       {
                           try_kill();
                       }
                   }
               }


    }




private:
    int difficulty = 1;
    int size_case=20;
    int grid_height = 500;
    int grid_width = 500;
    int score_self=feedback.pose.body.size();
    int score_other=feedback.pose_other.body.size();
};
}
}
#endif
