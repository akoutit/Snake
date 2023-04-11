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


    double dist_to_apple(int x,int y, Position objective)
    {
        return abs(x-objective.x)+abs(y-objective.y);
    }

    // Renvoie les positions des trois potentielles prochaines positions
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

    // Renvoie la prochaine position de l'adversaire dans le cas où il continue tout droit
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

    // Essaye d'anticiper la prochaine position de l'adversaire.
    void try_kill()
           {
               vector<Position> next_Pose=voisinage();
               Position other_next_Pose = other_next_Position();
               if ((find(next_Pose.begin(),next_Pose.end(),other_next_Pose) !=next_Pose.end()) and !obstacle(other_next_Pose.x,other_next_Pose.y,true))
               {
                   move_to(other_next_Pose.x,other_next_Pose.y);
               };
           }


    // Retourne vrai si la position (x,y) correspond à un mur.
    bool wall(int x ,int y)
    {

        bool wall_right = ( x > grid_width-size_case );
        bool wall_left = ( x < 0 );
        bool wall_up = ( y < height_offset );
        bool wall_down = ( y > grid_height-size_case );
        return wall_right or wall_left or wall_up or wall_down;
    }

    // Renvoie vrai si la case (x,y) correspond à un mur, l'autre snake, où son propre corps.
    // Si (next == true), considère également les trois potentielles prochaines positions de l'adversaire comme des obstacles.
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
        bool wall_right = ( x == grid_width-size_case ) and t == Orientation::RIGHT;
        bool wall_left = ( x == 0 )and t == Orientation::LEFT;
        bool wall_up = ( y == height_offset )and t == Orientation::UP;
        bool wall_down = ( y == grid_height-size_case )and t == Orientation::DOWN;
        return wall_right or wall_left or wall_up or wall_down;

    }


    void calcul_distance_voisins(vector<Position> to_check, array<array<int,22>,27> &grid)
        {


            if (to_check.size()>0)
            {
                int distance_obstacle = 10000000;

                int x = to_check[0].x;
                int y = to_check[0].y;
                int distance_parent = grid[x][y] ;


                // fils droite
                if ( ((x+1)<25) && (distance_parent+1 < grid[x+1][y]) && (grid[x+1][y]<distance_obstacle) )
                {
                    grid[x+1][y] = distance_parent + 1;
                    to_check.push_back(Position{x+1,y});
                }
                // fils gauche
                if ( ((x-1)>=0) && (distance_parent+1 < grid[x-1][y]) && (grid[x-1][y]<distance_obstacle) )
                {
                    grid[x-1][y] = distance_parent + 1;
                    to_check.push_back(Position{x-1,y});
                }
                // fils haut
                if ( ((y-1)>=0) && (distance_parent+1 < grid[x][y-1]) && (grid[x][y-1]<distance_obstacle) )
                {
                    grid[x][y-1] = distance_parent + 1;
                    to_check.push_back(Position{x,y-1});
                }
                // fils bas
                if ( ((y+1)<20) && (distance_parent+1 < grid[x][y+1]) && (grid[x][y+1]<distance_obstacle) )
                {
                    grid[x][y+1] = distance_parent + 1;
                    to_check.push_back(Position{x,y+1});
                }
                to_check.erase(to_check.begin());
                calcul_distance_voisins(to_check,grid);
            }
        }

    // Fonction récursive permettant de construire un grille contenant les distances entre chaque case et la position objective.
    // La distance d'une case obstacle est égale à distance_obstacle, et celles correspondant aux potentielles prochaines positions de l'adversaire à distance_obstacle-1.
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
                if (obstacle(offset_x+size_case*x,offset_y+size_case*y,false))
                    grid[x][y] = distance_obstacle;
                else
                    grid[x][y] = default_value;
            }
        }

        int x_objective = (objective.x-offset_x)/size_case;
        int y_objective = (objective.y-offset_y)/size_case;

        grid[x_objective][y_objective] = 0;
        calcul_distance_voisins({Position{x_objective,y_objective}},grid);

        if(obstacle(objective.x,objective.y,true))
        {
            grid[x_objective][y_objective] = distance_obstacle;
        }

        for (int x=0; x<27; x=x+1)
                {
                    for (int y=0; y<22; y=y+1)
                    {
                        if ( (grid[x][y]<distance_obstacle) and (obstacle(offset_x+size_case*x,offset_y+size_case*y,true)) )
                            grid[x][y] = distance_obstacle-1;
                    }
                }


        return grid;
    }







    void updateInput()
    {

        score_self=feedback.pose.body.size();
        score_other=feedback.pose_other.body.size();


        // Niveau de difficulté très simple : Déplacements aléatoires
        if(difficulty==1)
                {
                    srand(time(0));

                    int head_x = feedback.pose.head.x;
                    int head_y = feedback.pose.head.y;

                    Position next_position = {-20,80}; // Correspond à un obstacle

                    while(obstacle(next_position.x,next_position.y,false))
                    {

                        switch(rand()%5)
                        {
                            case 0:
                                input.action = Input::Action::TURN_RIGHT;
                                switch(feedback.pose.head.t)
                                {
                                    case Orientation::RIGHT:
                                        next_position = {head_x,head_y+size_case};
                                    break;
                                    case Orientation::LEFT:
                                        next_position = {head_x,head_y-size_case};
                                    break;
                                    case Orientation::UP:
                                        next_position = {head_x+size_case,head_y};
                                    break;
                                    case Orientation::DOWN:
                                        next_position = {head_x-size_case,head_y};
                                    break;
                                }
                            break;

                            case 1:
                                input.action = Input::Action::TURN_LEFT;
                                switch(feedback.pose.head.t)
                                {
                                    case Orientation::RIGHT:
                                        next_position = {head_x,head_y-size_case};
                                    break;
                                    case Orientation::LEFT:
                                        next_position = {head_x,head_y+size_case};
                                    break;
                                    case Orientation::UP:
                                        next_position = {head_x-size_case,head_y};
                                    break;
                                    case Orientation::DOWN:
                                        next_position = {head_x+size_case,head_y};
                                    break;

                                }
                            break;

                            default:
                                input.action = Input::Action::MOVE;
                                switch(feedback.pose.head.t)
                                {
                                    case Orientation::RIGHT:
                                        next_position = {head_x+size_case,head_y};
                                    break;
                                    case Orientation::LEFT:
                                        next_position = {head_x-size_case,head_y};
                                    break;
                                    case Orientation::UP:
                                        next_position = {head_x,head_y-size_case};
                                    break;
                                    case Orientation::DOWN:
                                        next_position = {head_x,head_y+size_case};
                                    break;
                                }
                        }
                    }
                }






        // Niveau de diffulté simple
        if(difficulty == 2)
        {

            Position objective = feedback.apple[0]; //Plus ancienne pomme encore sur la map.

            if(wall())
            {
                input.action=Input::Action::TURN_RIGHT;
            }
            else
            {
                input.action=Input::Action::MOVE;
                {
                    if(feedback.pose.head.y==objective.y)
                    {
                        if(feedback.pose.head.x>objective.x)
                        {if(feedback.pose.head.t==Orientation::DOWN)
                            {input.action=Input::Action::TURN_RIGHT;}
                            if(feedback.pose.head.t==Orientation::UP)
                            {input.action=Input::Action::TURN_LEFT;}
                            if(feedback.pose.head.t==Orientation::LEFT)
                            {input.action=Input::Action::MOVE;}
                            if(feedback.pose.head.t==Orientation::RIGHT)
                            {input.action=Input::Action::MOVE;}
                        }


                        if(feedback.pose.head.x<objective.x)
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
                    if(feedback.pose.head.x==objective.x)
                    {
                        if(feedback.pose.head.y>objective.y)
                        {if(feedback.pose.head.t==Orientation::DOWN)
                            {input.action=Input::Action::MOVE;}
                            if(feedback.pose.head.t==Orientation::UP)
                            {input.action=Input::Action::MOVE;}
                            if(feedback.pose.head.t==Orientation::LEFT)
                            {input.action=Input::Action::TURN_RIGHT;}
                            if(feedback.pose.head.t==Orientation::RIGHT)
                            {input.action=Input::Action::TURN_LEFT;}
                        }




                        if(feedback.pose.head.y<objective.y)
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


        // Niveau de difficulté moyen.
        if(difficulty == 3)
        {
                    int head_x=feedback.pose.head.x;
                    int head_y=feedback.pose.head.y;

                    int n_apples = feedback.apple.size();
                    int min_distance_apple = 1000000;
                    Position objective;

                    for (int i=0;i<n_apples;i++)
                    {
                        int distance_apple = abs((head_x)-feedback.apple[i].x)+abs((head_y)-feedback.apple[i].y);
                        if (distance_apple<min_distance_apple)
                        {
                            min_distance_apple = distance_apple;
                            objective = feedback.apple[i];
                        }
                    }

                    bool apple_equ_x = feedback.pose.head.x==objective.x;
                    bool apple_equ_y = feedback.pose.head.y==objective.y;

                    bool apple_up = feedback.pose.head.y>objective.y;
                    bool apple_left = feedback.pose.head.x>objective.x;
                    bool apple_down = feedback.pose.head.y<objective.y;
                    bool apple_right = feedback.pose.head.x<objective.x;

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


        // Niveau de difficulté difficile.
        if(difficulty == 4)
        {

            int head_x=feedback.pose.head.x;
            int head_y=feedback.pose.head.y;

            int n_apples = feedback.apple.size();

            int min_distance_apple = 1000000;
            int other_max_distance_apple = 0;

            Position objective;
            Position other_last_objective;


            for (int i=0;i<n_apples;i++)
            {
                int my_distance_apple = abs((head_x)-feedback.apple[i].x)+abs((head_y)-feedback.apple[i].y);
                int other_distance_apple = abs((feedback.pose_other.head.x)-feedback.apple[i].x)+abs((feedback.pose_other.head.y)-feedback.apple[i].y);
                if ( (my_distance_apple<other_distance_apple) and (my_distance_apple>0) and (my_distance_apple<min_distance_apple))
                {
                    min_distance_apple = my_distance_apple;
                    objective = feedback.apple[i];
                }
                if (other_distance_apple>other_max_distance_apple)
                {
                    other_max_distance_apple = other_distance_apple;
                    other_last_objective = feedback.apple[i];
                }

            }

            if (min_distance_apple == 1000000)
            {
                objective = other_last_objective;
            }

            vector<Position> V=voisinage();
            vector<Position> vois_not_obs;

            int x=-1,y=-1;
            double d=dist_to_apple(feedback.pose.head.x,feedback.pose.head.y,objective);
            for(int i=0; i<3; i++)
            {
                Position P = V[i];
                if (!obstacle(P.x,P.y,true))
                {

                    vois_not_obs.push_back(P);
                    if ((dist_to_apple(P.x,P.y,objective)<d))
                    {
                        d=dist_to_apple(P.x,P.y,objective);
                        x=P.x;
                        y=P.y;
                    }
                }
            }

            if(x==-1) // Si aucune position n'est accessible en considérant les prochaines potentielles positions de l'adversaire comme des obstacles, on ne les considère plus comme des obstacles
            {
                if(vois_not_obs.size()==0)
                {
                    for(int i=0; i<3; i++)
                    {
                        Position P = V[i];
                        if (!obstacle(P.x,P.y,false))
                        {

                            vois_not_obs.push_back(P);
                            if ((dist_to_apple(P.x,P.y,objective)<d))
                            {
                                d=dist_to_apple(P.x,P.y,objective);
                                x=P.x;
                                y=P.y;
                            }
                        }
                    }
                    if (vois_not_obs.size()==0)
                    {
                        input.action = Input::Action::MOVE;
                    }
                    else
                    {
                        x=vois_not_obs[0].x;
                        y=vois_not_obs[0].y;
                    }
                }

                else
                {
                    x=vois_not_obs[0].x;
                    y=vois_not_obs[0].y;
                }
            }
            move_to(x,y);

        }




        // Niveau de difficulté très difficile.
        if (difficulty == 5)
               {
                   input.action = Input::Action::MOVE;
                   int n_apples = feedback.apple.size();
                   int offset_y = 80;
                   int offset_x = -20;
                   int distance_obstacle = 10000000;

                   int head_x = feedback.pose.head.x;
                   int head_y = feedback.pose.head.y;
                   Position head_in_grid = { (head_x-offset_x)/size_case , (head_y-offset_y)/size_case };

                   int distance_objective = 1000000;
                   int other_max_distance_apple = 0;
                   Position objective;
                   Position other_last_objective;

                   //Choix de l'objectif
                   if ( (n_apples==1) and (dist_to_apple(head_x,head_y,feedback.apple[0])-dist_to_apple(feedback.pose_other.head.x,feedback.pose_other.head.y,feedback.apple[0])>=5*size_case))
                   {
                       objective = Position{(grid_width-size_case)/2,(grid_height+height_offset)/2};
                   }
                   else
                   {
                       for (int i=0;i<n_apples;i++)
                       {
                           int my_distance_apple = dist_to_apple(head_x,head_y,feedback.apple[i]);
                           int other_distance_apple = dist_to_apple(feedback.pose_other.head.x,feedback.pose_other.head.y,feedback.apple[i]);
                           if ( (my_distance_apple<other_distance_apple) and (my_distance_apple>0) and (my_distance_apple<distance_objective))
                           {
                               distance_objective = my_distance_apple;
                               objective = feedback.apple[i];
                           }
                           if (other_distance_apple>other_max_distance_apple)
                           {
                               other_max_distance_apple = other_distance_apple;
                               other_last_objective = feedback.apple[i];
                           }
                       }
                       if (distance_objective == 1000000)
                       {
                           objective = other_last_objective;
                           distance_objective = other_max_distance_apple;
                       }
                   }

                   // Détermination de la prochaine position
                   array<array<int,22>,27> grid_of_distances = distance_grid(objective);

                   int distance_left = grid_of_distances[head_in_grid.x-1][head_in_grid.y];
                   int distance_right = grid_of_distances[head_in_grid.x+1][head_in_grid.y];
                   int distance_up = grid_of_distances[head_in_grid.x][head_in_grid.y-1];
                   int distance_down = grid_of_distances[head_in_grid.x][head_in_grid.y+1];

                   vector<int> distance_voisins = {distance_left,distance_right,distance_up,distance_down};
                   int my_distance_to_objective = *min_element(distance_voisins.begin(),distance_voisins.end());

                   vector<Position> next_directions = {};

                   if (my_distance_to_objective==distance_left)
                       next_directions.push_back(Position{head_x-size_case,head_y});
                   if (my_distance_to_objective==distance_right)
                       next_directions.push_back(Position{head_x+size_case,head_y});
                   if (my_distance_to_objective==distance_up)
                       next_directions.push_back(Position{head_x,head_y-size_case});
                   if (my_distance_to_objective==distance_down)
                       next_directions.push_back(Position{head_x,head_y+size_case});


                   if (my_distance_to_objective == distance_obstacle)
                   {
                       input.action = Input::Action::MOVE;
                   }

                   else
                   {
                       //On choisi la prochaine direction aléatoirement parmi les cases voisines qui correspondent à l'un des plus courts chemins.
                       int n = next_directions.size();
                       int random = rand()%n;

                       move_to(next_directions[random].x,next_directions[random].y);

                       if (score_self > score_other)
                       {
                           try_kill();
                       }
                   }
               }

        if (difficulty == 6)
        {
            int n_apples = feedback.apple.size();

            int offset_y = 80;
            int offset_x = -20;

            int distance_obstacle = 10000000;

            int head_x = feedback.pose.head.x;
            int head_y = feedback.pose.head.y;
            Position head = {head_x,head_y};

            Position head_in_grid = { (head_x-offset_x)/size_case , (head_y-offset_y)/size_case };
            Position other_head_in_grid = { (feedback.pose_other.head.x-offset_x)/size_case , (feedback.pose_other.head.y-offset_y)/size_case };

            array<array<int,22>,27> zeros;
            vector<array<array<int,22>,27>> grids_of_distances;
            for (int i=0;i<n_apples;i++)
            {
                grids_of_distances.push_back(zeros);
            }

            int my_distance_to_objective = distance_obstacle;
            int int_objective = -1;
            Position objective;
            int other_max_apple_distance = 0;
            int other_farthest_apple;

            array<int,4> distances_left;
            array<int,4> distances_right;
            array<int,4> distances_up;
            array<int,4> distances_down;

            array<int,4> my_distances_to_apples;

            for (int i=0;i<n_apples;i++)
            {

                grids_of_distances[i] = distance_grid(feedback.apple[i]);

                int other_distance_left = grids_of_distances[i][other_head_in_grid.x-1][other_head_in_grid.y];
                int other_distance_right = grids_of_distances[i][other_head_in_grid.x+1][other_head_in_grid.y];
                int other_distance_up = grids_of_distances[i][other_head_in_grid.x][other_head_in_grid.y-1];
                int other_distance_down = grids_of_distances[i][other_head_in_grid.x][other_head_in_grid.y+1];

                distances_left[i] = grids_of_distances[i][head_in_grid.x-1][head_in_grid.y];
                distances_right[i] = grids_of_distances[i][head_in_grid.x+1][head_in_grid.y];
                distances_up[i] = grids_of_distances[i][head_in_grid.x][head_in_grid.y-1];
                distances_down[i] = grids_of_distances[i][head_in_grid.x][head_in_grid.y+1];

                vector<int> distance_voisins = {distances_left[i],distances_right[i],distances_up[i],distances_down[i]};
                vector<int> other_distance_voisins = {other_distance_left,other_distance_right,other_distance_up,other_distance_down};

                my_distances_to_apples[i] = *min_element(distance_voisins.begin(),distance_voisins.end());
                int other_distance_to_apple = *min_element(other_distance_voisins.begin(),other_distance_voisins.end());
                if (other_distance_to_apple > other_max_apple_distance)
                {
                    other_max_apple_distance = other_distance_to_apple;
                    other_farthest_apple = i;
                }
                if ( (my_distances_to_apples[i]<other_distance_to_apple)  and !(head==feedback.apple[i])and (my_distances_to_apples[i]<my_distance_to_objective))
                {
                    int_objective = i;
                    objective = feedback.apple[i];
                    my_distance_to_objective = my_distances_to_apples[i];
                }
            }

            // Si l'adversaire est plus proche de chacune des pommes, on choisi comme objectif celle dont il est le plus éloigné
            if (int_objective==-1)
            {
                int_objective = other_farthest_apple;
                objective = feedback.apple[other_farthest_apple];
                my_distance_to_objective = my_distances_to_apples[other_farthest_apple];
            }

            vector<Position> next_directions = {};

            if (my_distance_to_objective==distances_left[int_objective])
                next_directions.push_back(Position{head_x-size_case,head_y});
            if (my_distance_to_objective==distances_right[int_objective])
                next_directions.push_back(Position{head_x+size_case,head_y});
            if (my_distance_to_objective==distances_up[int_objective])
                next_directions.push_back(Position{head_x,head_y-size_case});
            if (my_distance_to_objective==distances_down[int_objective])
                next_directions.push_back(Position{head_x,head_y+size_case});

            //On choisi la prochaine direction aléatoirement parmi les cases voisines qui correspondent à l'un des plus courts chemins
            int n = next_directions.size();
            int random = rand()%n;

            move_to(next_directions[random].x,next_directions[random].y);


            if (score_self > score_other)
            {
                try_kill();
            }
        }


    }




private:
    int difficulty = 1;
    int size_case=20;
    int grid_height = 500;
    int grid_width = 500;
    int height_offset = 100;
    int score_self=feedback.pose.body.size();
    int score_other=feedback.pose_other.body.size();
};
}
}
#endif
