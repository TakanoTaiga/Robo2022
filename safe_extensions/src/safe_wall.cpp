#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "geometry_msgs/msg/twist.hpp"

using std::placeholders::_1;

class safe_wall : public rclcpp::Node
{
    public:
    safe_wall(): Node("safe_wall")
    {
        sub_vel = this->create_subscription<geometry_msgs::msg::Twist>(
            "cmd_vel/in" , 4 , std::bind(&safe_wall::vel_sub_callback , this , _1)
        );

        sub_lock = this->create_subscription<std_msgs::msg::Bool>(
            "safe_wall/lock" , 4 , std::bind(&safe_wall::lock_sub_callback , this , _1)
        );

        pub_vel = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel/out" , 4);
    }

    private:

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_vel;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_lock;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_vel;

    bool lockStatus = false;

    void 
    vel_sub_callback(
        geometry_msgs::msg::Twist::SharedPtr msg
    ){
        if(lockStatus){
            auto pub_msg = geometry_msgs::msg::Twist();
            pub_vel->publish(pub_msg);
            return;
        }
        
        auto pub_msg = geometry_msgs::msg::Twist();
        pub_msg.angular = msg->angular;
        pub_msg.linear = msg->linear;
        pub_vel->publish(pub_msg);
    }

    void
    lock_sub_callback(
        std_msgs::msg::Bool::SharedPtr msg
    ){
        lockStatus = msg->data;
    }
};

int main(int argc , char * argv[])
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<safe_wall>());
    rclcpp::shutdown();
    return 0;
}