//physicalMDC.cpp

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

class physicalMDC : public rclcpp::Node
{
  public:
  physicalMDC() : Node("physicalMDC")
  {
    sub_[0] = this->create_subscription<std_msgs::msg::Float32>(
      "robo2022/motor_power/FR" , 10 , std::bind(&physicalMDC::topic_callback_FR, this , _1)
    );
    sub_[1] = this->create_subscription<std_msgs::msg::Float32>(
      "robo2022/motor_power/FL" , 10 , std::bind(&physicalMDC::topic_callback_FL, this , _1)
    );
    sub_[2] = this->create_subscription<std_msgs::msg::Float32>(
      "robo2022/motor_power/RR" , 10 , std::bind(&physicalMDC::topic_callback_RR, this , _1)
    );
    sub_[3] = this->create_subscription<std_msgs::msg::Float32>(
      "robo2022/motor_power/RL" , 10 , std::bind(&physicalMDC::topic_callback_RL, this , _1)
    );

    pub_ = this->create_publisher<std_msgs::msg::String>("robo2022/serial_output" , 10);

    motor_power_sub_checker[0] = false;
    motor_power_sub_checker[1] = false;
    motor_power_sub_checker[2] = false;
    motor_power_sub_checker[3] = false;
  }

  private:
  void topic_callback_FR(const std_msgs::msg::Float32::SharedPtr get_msg)
  {
    motor_power_sub_data[0] = get_msg->data;
    motor_power_sub_checker[0] = true;
    sendSerialData();
  }
  void topic_callback_FL(const std_msgs::msg::Float32::SharedPtr get_msg)
  {
    motor_power_sub_data[1] = get_msg->data;
    motor_power_sub_checker[1] = true;
    sendSerialData();
  }
  void topic_callback_RR(const std_msgs::msg::Float32::SharedPtr get_msg)
  {
    motor_power_sub_data[2] = get_msg->data;
    motor_power_sub_checker[2] = true;
    sendSerialData();
  }
  void topic_callback_RL(const std_msgs::msg::Float32::SharedPtr get_msg)
  {
    motor_power_sub_data[3] = get_msg->data;
    motor_power_sub_checker[3] = true;
    sendSerialData();
  }

  void sendSerialData(){
    //check to see if all has been received
    for(uint8_t i = 0;i < 4 ; i++){
        if(!motor_power_sub_checker[i])
            return;
    }

    //create motor power message
    std::string motor_power_messages = "M";
    for(uint8_t i = 0;i < 4 ; i++){
        if(motor_power_sub_data[i] >= 0){
            if(motor_power_sub_data[i] > 1.5){
                return;
            }
            motor_power_messages += ":1:" + std::to_string((uint)((motor_power_sub_data[i] / 1.5) * 1000.0));
        }else{
            if(motor_power_sub_data[i] < -1.5){
                return;
            }
            motor_power_messages += ":0:" + std::to_string((uint)((motor_power_sub_data[i] / 1.5) * 1000.0 * -1));
        }

    }
    motor_power_messages += ":E:";
    auto pubMsg = std_msgs::msg::String();
    pubMsg.data = motor_power_messages;
    pub_->publish(pubMsg);
  }

  double motor_power_sub_data[4];
  bool motor_power_sub_checker[4];

  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_[4];

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<physicalMDC>());
  rclcpp::shutdown();
  return 0;
}