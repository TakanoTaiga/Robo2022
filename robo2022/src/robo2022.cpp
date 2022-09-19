#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

#include "robo2022/robo2022uitls.hpp"

using std::placeholders::_1;
using namespace std::chrono_literals;


class robo2022 : public rclcpp::Node , public robo2022utils
{
    public:
    robo2022() : Node("robo2022")
    {
        sub_cmd_vel_rover = this->create_subscription<geometry_msgs::msg::Twist>(
            "robo2022/cmd_vel/rover" , 4 , std::bind(&robo2022::topic_callback_cmd_vel_rover , this , _1) // use linear x,z & anguler y(y-up&right-hand)
        );

        sub_cmd_vel_up_down = this->create_subscription<geometry_msgs::msg::Twist>(
            "robo2022/cmd_vel/updown" , 4 , std::bind(&robo2022::topic_callback_cmd_vel_up_down , this , _1) // use linear y (y-up&right-hand)
        );


        pub_power = this->create_publisher<std_msgs::msg::Float32MultiArray>("robo2022util/cmd_pwr" , 4);
    }

    private:

    void 
    topic_callback_cmd_vel_rover(
        const geometry_msgs::msg::Twist::SharedPtr get_msg)
    {
        vec_target_rover = get_msg;
        pwr_callback_func();
    }

    void 
    topic_callback_cmd_vel_up_down(
        const geometry_msgs::msg::Twist::SharedPtr get_msg)
    {
        vec_target_up_down = get_msg;
        pwr_callback_func();
    }


    void
    pwr_callback_func(){
        if(vec_target_rover == NULL){return;}
        if(vec_target_up_down == NULL){return;}

        auto motorPower = vel2motor(vec_target_rover);
        auto pub_msg = std_msgs::msg::Float32MultiArray();
        pub_msg.data.insert(
            pub_msg.data.begin(),
            motorPower.begin(),
            motorPower.end()
        );
        pub_msg.data.push_back(vec_target_up_down->linear.y * 100);

        pub_power->publish(pub_msg);

    }


    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_vel_rover;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_vel_up_down;

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_power;

    geometry_msgs::msg::Twist::SharedPtr vec_target_rover;
    geometry_msgs::msg::Twist::SharedPtr vec_target_up_down;

    rclcpp::TimerBase::SharedPtr pwr_callback_timer;

};

int main(int argc , char * argv[])
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<robo2022>());
    rclcpp::shutdown();
    return 0;
}