// generated from snake.yaml -- editing this file by hand is not recommended
inline std::ostream& operator<<(std::ostream& ss, const duels::snake::Position &position)
{
  ss << "{";
  ss << "x: " << position.x << ',';
  ss << "y: " << position.y << "}";
  return ss;
}

inline std::ostream& operator<<(std::ostream& ss, const duels::snake::Pose &pose)
{
  ss << "{";
  ss << "x: " << pose.x << ',';
  ss << "y: " << pose.y << ',';
  ss << "t: " << pose.t << "}";
  return ss;
}

inline std::ostream& operator<<(std::ostream& ss, const duels::snake::Snake &snake)
{
  ss << "{";
  ss << "head: " << snake.head << ',';
  ss << "body: " << snake.body << "}";
  return ss;
}

namespace YAML
{
template<>
struct convert<duels::snake::Position> 
{
  static bool decode(Node const& node, duels::snake::Position & rhs)
  {
    rhs.x = node["x"].as<int>();
    rhs.y = node["y"].as<int>();
    return true;
  }
};

template<>
struct convert<duels::snake::Pose> 
{
  static bool decode(Node const& node, duels::snake::Pose & rhs)
  {
    rhs.x = node["x"].as<int>();
    rhs.y = node["y"].as<int>();
    rhs.t = node["t"].as<duels::snake::Orientation>();
    return true;
  }
};

template<>
struct convert<duels::snake::Snake> 
{
  static bool decode(Node const& node, duels::snake::Snake & rhs)
  {
    rhs.head = node["head"].as<duels::snake::Pose>();
    rhs.body = node["body"].as<std::vector<duels::snake::Position>>();
    return true;
  }
};
}