#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

// This node is support b team hardware.

using std::placeholders::_1;
using namespace std::chrono_literals;

class b_team : public rclcpp::Node
{
    public:
    b_team(): Node("b_team_support")
    {
        sub_cmd_vel_belt = this->create_subscription<geometry_msgs::msg::Twist>(
            "robo2022/cmd_vel/b/belt" , 4  , std::bind(&b_team::topic_callback_B_team_support_belt, this ,_1)
        );

        sub_cmd_vel_fir = this->create_subscription<geometry_msgs::msg::Twist>(
            "robo2022/cmd_vel/b/fir" , 4 , std::bind(&b_team::topic_callback_B_team_support_fir, this ,_1)
        );

        pub_power = this->create_publisher<std_msgs::msg::Float32MultiArray>("robo2022util/b/cmd_pwr" , 4);
    }

    private:
    
    void
    topic_callback_B_team_support_belt(
        const geometry_msgs::msg::Twist::SharedPtr msg
    ){
        vec_record_belt = msg;
        pwr_callback();
    }

    void
    topic_callback_B_team_support_fir(
        const geometry_msgs::msg::Twist::SharedPtr msg
    ){
        vec_record_fir = msg;
        pwr_callback();
    }

    void 
    pwr_callback()
    {
        if(vec_record_belt == NULL){return;}
        if(vec_record_fir == NULL){return;}

        auto pub_msg = std_msgs::msg::Float32MultiArray();
        pub_msg.data.push_back(vec_record_belt->linear.x * 100);
        pub_msg.data.push_back(vec_record_fir->linear.x * -10);
        
        pub_power->publish(pub_msg);
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_vel_belt;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_vel_fir;

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_power;

    geometry_msgs::msg::Twist::SharedPtr vec_record_belt;
    geometry_msgs::msg::Twist::SharedPtr vec_record_fir;
    
};

int main(int argc , char * argv[])
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<b_team>());
    rclcpp::shutdown();
    return 0;
}