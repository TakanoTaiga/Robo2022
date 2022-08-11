#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/float32.hpp"

#define PI 3.14159265359

using std::placeholders::_1;

class msgCreate
{
    public:

    double L = 0.565685;

    double vel2motor(const geometry_msgs::msg::Twist::SharedPtr geo_msg , std::string motorPosition)
    {
        double V1 = -0.707106781 * geo_msg->linear.x + 0.707106781 * geo_msg->linear.y + L * geo_msg->angular.z;
        double V2 = -0.707106781 * geo_msg->linear.x - 0.707106781 * geo_msg->linear.y + L * geo_msg->angular.z;
        double V3 =  0.707106781 * geo_msg->linear.x - 0.707106781 * geo_msg->linear.y + L * geo_msg->angular.z;
        double V4 =  0.707106781 * geo_msg->linear.x + 0.707106781 * geo_msg->linear.y + L * geo_msg->angular.z;

        if(motorPosition == "V1"){
            return V1;
        }else if(motorPosition == "V2"){
            return V2;
        }else if(motorPosition == "V3"){
            return V3;
        }else if(motorPosition == "V4"){
            return V4;
        }else if(motorPosition == "normal"){
          return geo_msg->angular.x;
        }else{
          return -1;
        }
    }
};

class joy2motor : public rclcpp::Node , public msgCreate
{
  public:
  joy2motor() : Node("joy2motor")
  {
    this->declare_parameter<std::string>("motor_position");
    this->get_parameter("motor_position", motorPosition_string_);

    sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
      "robo2022/cmd_vel" , 10 , std::bind(&joy2motor::topic_callback, this , _1)
    );

    pub_ = this->create_publisher<std_msgs::msg::Float32>("robo2022/motor_power" , 10);
  }

  private:
  void topic_callback(const geometry_msgs::msg::Twist::SharedPtr get_msg)
  {
    auto pubMsg = std_msgs::msg::Float32();
    pubMsg.data = vel2motor(get_msg , motorPosition_string_);
    pub_->publish(pubMsg);
  }

  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pub_;

  std::string motorPosition_string_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<joy2motor>());
  rclcpp::shutdown();
  return 0;
}