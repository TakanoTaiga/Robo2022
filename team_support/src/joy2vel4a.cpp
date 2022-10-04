#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"
#include "geometry_msgs/msg/twist.hpp"

using std::placeholders::_1;

class joy2vel4a : public rclcpp::Node
{
  public:
  joy2vel4a() : Node("joy2vel4a")
  {
    sub_joy = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy" , 10 , std::bind(&joy2vel4a::joy_topic_callback, this , _1)
    );

    sub_smartui = this->create_subscription<sensor_msgs::msg::Joy>(
      "/SmartUI" , 10 , std::bind(&joy2vel4a::smartui_callback , this , _1)
    );

    pub_cmd_vel_fir = this->create_publisher<geometry_msgs::msg::Twist>("robo2022/cmd_vel/a/fir" , 10);
  }

  private:

  void 
  joy_topic_callback(const sensor_msgs::msg::Joy::SharedPtr get_msg)
  {
    auto pub_msg_fir = geometry_msgs::msg::Twist();
    if(get_msg->buttons[5] > 0){
      pub_msg_fir.linear.x = fir_power;
    }else{
      pub_msg_fir.linear.x = 0;
    }
    pub_cmd_vel_fir->publish(pub_msg_fir);
  }

  void
  smartui_callback(const sensor_msgs::msg::Joy::SharedPtr get_msg)
  {
    fir_power = get_msg->axes[0];    
  }

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_joy;
  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr sub_smartui;

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel_fir;

  double fir_power;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<joy2vel4a>());
  rclcpp::shutdown();
  return 0;
}