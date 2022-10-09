#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"
#include "geometry_msgs/msg/twist.hpp"

using std::placeholders::_1;

class joy2vel4b : public rclcpp::Node
{
  public:
  joy2vel4b() : Node("joy2vel4b")
  {
    this->declare_parameter<double>("beltPower" , 10.0);
    this->get_parameter("beltPower" , param_belt);

    sub_joy = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy" , 10 , std::bind(&joy2vel4b::joy_topic_callback, this , _1)
    );

    sub_smartui = this->create_subscription<sensor_msgs::msg::Joy>(
      "/SmartUI" , 10 , std::bind(&joy2vel4b::smartui_callback , this , _1)
    );

    pub_cmd_vel_fir = this->create_publisher<geometry_msgs::msg::Twist>("robo2022/cmd_vel/b/fir" , 10);
    pub_cmd_vel_belt = this->create_publisher<geometry_msgs::msg::Twist>("robo2022/cmd_vel/b/belt" , 10);

  }

  private:

  void 
  joy_topic_callback(const sensor_msgs::msg::Joy::SharedPtr get_msg)
  {
    auto pub_msg_belt = geometry_msgs::msg::Twist();
    if(get_msg->axes[5] < 0)
    {
      //Nutoral
      pub_msg_belt.linear.x = param_belt;
    }
    else if(get_msg->buttons[4] > 0)
    {
      //Counter
      pub_msg_belt.linear.x = -1.0 * param_belt;
    }
    pub_cmd_vel_belt->publish(pub_msg_belt);
  }

  void
  smartui_callback(const sensor_msgs::msg::Joy::SharedPtr get_msg)
  {
    auto pub_msg_fir = geometry_msgs::msg::Twist();

    if(get_msg->axes[1] > 90.0){
      pub_msg_fir.linear.x = get_msg->axes[0];
    }else{
      pub_msg_fir.linear.x = 0;
    }
    pub_cmd_vel_fir->publish(pub_msg_fir);
  }

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_joy;
  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_smartui;

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel_fir;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel_belt;

  double param_belt;
  double fir_power;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<joy2vel4b>());
  rclcpp::shutdown();
  return 0;
}