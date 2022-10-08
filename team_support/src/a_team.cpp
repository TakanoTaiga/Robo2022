#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

#include "team_support/macro.hpp"

// This node is support a team hardware.

using std::placeholders::_1;
using namespace std::chrono_literals;

class a_team : public rclcpp::Node
{
    public:
    a_team(): Node("a_team_support")
    {
        sub_cmd_vel_fir = this->create_subscription<geometry_msgs::msg::Twist>(
            "robo2022/cmd_vel/a_team/fir" , 4 , std::bind(&a_team::topic_callback_A_team_support_fir, this ,_1)
        );

        pub_power = this->create_publisher<std_msgs::msg::Float32MultiArray>("robo2022util/a_team/cmd_pwr" , 4);
    }

    private:
    void
    topic_callback_A_team_support_fir(
        const geometry_msgs::msg::Twist::SharedPtr msg
    ){
        if(msg->linear.x  < 1){return;}
        
        if(sub_lock == TS_OPEN){
            sub_lock = TS_LOCK;
            auto pub_msg = std_msgs::msg::Float32MultiArray();
            pub_msg.data.push_back(msg->linear.x * -10);
            pub_power->publish(pub_msg);

            rclcpp::WallRate loop_rate(1000ms);
            loop_rate.sleep();

            std::fill(pub_msg.data.begin() , pub_msg.data.end() , 70);
            pub_power->publish(pub_msg);
            sub_lock = TS_OPEN;
        }
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_vel_fir;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_power;

    bool sub_lock = TS_OPEN;

};

int main(int argc , char * argv[])
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<a_team>());
    rclcpp::shutdown();
    return 0;
}