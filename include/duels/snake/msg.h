// generated from snake.yaml -- editing this file by hand is not recommended
#ifndef SNAKE_MSG_H
#define SNAKE_MSG_H
#include <sstream>
#include <duels/game_state.h>
namespace duels {
namespace snake {

// utility structures
enum class Orientation{RIGHT,UP,LEFT,DOWN};
struct Position
{
  int x;int y;
  inline bool operator==(const Position &other) const
  {
    return x == other.x && y == other.y;
  }
};
struct Pose
{
  int x;int y;Orientation t;
  inline bool operator==(const Pose &other) const
  {
    return x == other.x && y == other.y && t == other.t;
  }
};
struct Snake
{
  Pose head;std::vector<Position> body;
  inline bool operator==(const Snake &other) const
  {
    return head == other.head && body == other.body;
  }
};
}}

//detail on how to stream these structures
#include "msg_detail.h"

// core game messages
namespace duels {
namespace snake {

struct InitDisplay
{
  Pose snake1; Pose snake2; Position apples;
  std::string serialize(std::string name1, std::string name2) const 
  {
    std::stringstream ss;
    ss << "name1: " << name1;
    ss << "\nname2: " << name2;
    ss << "\nsnake1: " << snake1;
    ss << "\nsnake2: " << snake2;
    ss << "\napples: " << apples;
    return ss.str();
  }
};

struct Input
{
  enum class Action{TURN_LEFT,TURN_RIGHT,MOVE};  Action action;
  std::string serialize() const 
  {
    std::stringstream ss;
    ss << "action: " << action;
    return ss.str();
  }
  void deserialize(const std::string &yaml)
  {
    const auto node{YAML::Load(yaml)};
    action = node["action"].as<Action>();
  }
};

struct Feedback
{
  Snake pose; Snake pose_other; Position apple; State __state;
  std::string serialize() const 
  {
    std::stringstream ss;
    ss << "pose: " << pose;
    ss << "\npose_other: " << pose_other;
    ss << "\napple: " << apple;
    ss << "\n__state: " << __state;
    return ss.str();
  }
  void deserialize(const std::string &yaml)
  {
    const auto node{YAML::Load(yaml)};
    pose = node["pose"].as<Snake>();
    pose_other = node["pose_other"].as<Snake>();
    apple = node["apple"].as<Position>();
    __state = node["__state"].as<State>();
  }
};

struct Display
{
  Snake snake1; Snake snake2; Position apples; float score1; float score2;
  std::string serialize(Result result) const 
  {
    std::stringstream ss;
    ss << "result: " << result;
    ss << "\nsnake1: " << snake1;
    ss << "\nsnake2: " << snake2;
    ss << "\napples: " << apples;
    ss << "\nscore1: " << score1;
    ss << "\nscore2: " << score2;
    return ss.str();
  }
};

}}
#endif