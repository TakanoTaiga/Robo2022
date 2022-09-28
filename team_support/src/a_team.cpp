#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

// This node is support a team hardware.

using std::placeholders::_1;
using namespace std::chrono_literals;

class a_team : public rclcpp::Node
{
    public:
    a_team(): Node("a_team_support")
    {
        sub_cmd_vel_fir = this->create_subscription<geometry_msgs::msg::Twist>(
            "robo2022/cmd_vel/a/fir" , 4 , std::bind(&a_team::topic_callback_A_team_support_fir, this ,_1)
        );

        pub_power = this->create_publisher<std_msgs::msg::Float32MultiArray>("robo2022util/a/cmd_pwr" , 4);
    }

    private:
    void
    topic_callback_A_team_support_fir(
        const geometry_msgs::msg::Twist::SharedPtr msg
    ){
        if(msg->linear.x > 0){
            auto pub_msg = std_msgs::msg::Float32MultiArray();
            pub_msg.data.push_back(msg->linear.x * 10);
            pub_msg.data.push_back(msg->linear.x * 10);
            pub_power->publish(pub_msg);

            rclcpp::sleep_for(100ms);

            std::fill(pub_msg.data.begin() , pub_msg.data.end() , 0);
            pub_power->publish(pub_msg);
        }else{
            auto pub_msg = std_msgs::msg::Float32MultiArray();
            pub_msg.data.push_back(msg->linear.x * 10);
            pub_msg.data.push_back(msg->linear.x * 10);
            pub_power->publish(pub_msg);
        }
        

    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_vel_fir;

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_power;
};

int main(int argc , char * argv[])
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<a_team>());
    rclcpp::shutdown();
    return 0;
}