#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"
#include "geometry_msgs/msg/twist.hpp"

using std::placeholders::_1;

class joy2vel : public rclcpp::Node
{
  public:
  joy2vel() : Node("joy2vel")
  {
    sub_joy = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy" , 10 , std::bind(&joy2vel::joy_topic_callback, this , _1)
    );

    pub_cmd_vel_rover = this->create_publisher<geometry_msgs::msg::Twist>("robo2022/cmd_vel/rover" , 10);
    pub_cmd_vel_updown = this->create_publisher<geometry_msgs::msg::Twist>("robo2022/cmd_vel/updown" , 10);

  }

  private:

  void 
  joy_topic_callback(const sensor_msgs::msg::Joy::SharedPtr get_msg)
  {
    auto twist_msg = geometry_msgs::msg::Twist();
    twist_msg.linear.z = (double)(get_msg->axes[0]) * -1;
    twist_msg.linear.x = (double)(get_msg->axes[1]);
    twist_msg.angular.y = (double)(get_msg->axes[3]);
    pub_cmd_vel_rover -> publish(twist_msg);

    auto pub_msg = geometry_msgs::msg::Twist();
    if(get_msg->axes[7] > 0.5){
      //up
      pub_msg.linear.y = 1;
    }else if(get_msg->axes[7] < -0.5){
      //down
      pub_msg.linear.y = -1;
    }
    pub_cmd_vel_updown->publish(pub_msg);
  }

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_joy;

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel_rover;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel_updown;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<joy2vel>());
  rclcpp::shutdown();
  return 0;
}