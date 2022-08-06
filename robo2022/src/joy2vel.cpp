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
    sub_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy" , 10 , std::bind(&joy2vel::topic_callback, this , _1)
    );

    pub_ = this->create_publisher<geometry_msgs::msg::Twist>("robo2022/cmd_vel" , 10);
  }

  private:
  void topic_callback(const sensor_msgs::msg::Joy::SharedPtr get_msg) const
  {
    auto twist_msg = geometry_msgs::msg::Twist();
    twist_msg.linear.x = (double)(get_msg->axes[0]);
    twist_msg.linear.y = (double)(get_msg->axes[1]);
    twist_msg.angular.z = (double)(get_msg->axes[2]);
    pub_ -> publish(twist_msg);
    RCLCPP_INFO(this->get_logger(), "I heard x:%lf , y:%lf" , get_msg->axes[0] , get_msg->axes[1]);
  }

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<joy2vel>());
  rclcpp::shutdown();
  return 0;
}