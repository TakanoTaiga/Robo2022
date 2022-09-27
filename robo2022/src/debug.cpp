#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

using std::placeholders::_1;
using namespace std::chrono_literals;

class debug : public rclcpp::Node
{
    public:
    debug(): Node("debug_tool")
    {
        pub_motor = this->create_publisher<std_msgs::msg::Float32MultiArray>("robo2022util/cmd_pwr" , 10);


        std::cout << "Type 'help' for more infomation." << std::endl;
        while(true){
            std::cout << ">>";
            std::string msg;
            std::cin >> msg;

            if(msg.find("help") != std::string::npos){
                std::cout << "command : mean" << std::endl;
                std::cout << "----------------------" << std::endl;
                std::cout << "exit  : exit this tool" << std::endl;
                std::cout << "motor : control power per motor" << std::endl;
                std::cout << "mset  : reset motor power" << std::endl;
                continue;
            }

            if(msg.find("exit") != std::string::npos){
                RCLCPP_INFO(this->get_logger(), "robo2022/debuger is shutdown");
                break;
            }

            if(msg.find("motor") != std::string::npos){
                std::cout << "\tmotor number >>";
                std::string motorNumber;
                std::cin >> motorNumber;
                if(std::stoi(motorNumber) > 5){
                    std::cout << "out of index range" << std::endl;
                    continue;
                }

                std::cout << "\tmotor power >>";
                std::string motorPower;
                std::cin >> motorPower;
                if(std::stoi(motorPower) > 1000){
                    std::cout << "out of index range" << std::endl;
                    continue;
                }

                
                auto pub_msg = std_msgs::msg::Float32MultiArray();
                pub_msg.data.resize(5);
                pub_msg.data[std::stoi(motorNumber)] = std::stoi(motorPower);
                pub_motor->publish(pub_msg);
                std::cout << "published";

                continue;
            }

            if(msg.find("mset") != std::string::npos){
                auto pub_msg = std_msgs::msg::Float32MultiArray();
                pub_msg.data.resize(5);
                pub_motor->publish(pub_msg);
            }



            std::cout << msg << ": command not found" << std::endl;
        }

        rclcpp::shutdown();
    }

    private:

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_motor;
};

int main(int argc , char * argv[])
{
    rclcpp::init(argc , argv);
    rclcpp::spin(std::make_shared<debug>());
    rclcpp::shutdown();
    return 0;
}