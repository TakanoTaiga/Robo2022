#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

#define ROBO2022UTILS_L 0.565685

using std::placeholders::_1;
using namespace std::chrono_literals;

class robo2022utils
{
    public:

    std::vector<float>
    vel2motor(
        const geometry_msgs::msg::Twist::SharedPtr get_msg)
    {
        static std::vector<float> vec(4);
        vec[0] = -0.707106781 * get_msg->linear.z + 0.707106781 * get_msg->linear.x + ROBO2022UTILS_L * get_msg->angular.y;
        vec[1] = -0.707106781 * get_msg->linear.z - 0.707106781 * get_msg->linear.x + ROBO2022UTILS_L * get_msg->angular.y;
        vec[2] =  0.707106781 * get_msg->linear.z - 0.707106781 * get_msg->linear.x + ROBO2022UTILS_L * get_msg->angular.y;
        vec[3] =  0.707106781 * get_msg->linear.z + 0.707106781 * get_msg->linear.x + ROBO2022UTILS_L * get_msg->angular.y;

        return vec;
    }

    // double
    // makeScalerFromVector3(
    //     geometry_msgs::msg::Vector3 vec
    // ){
    //     return std::sqrt(
    //         std::pow(vec.x , 2) +
    //         std::pow(vec.y , 2) +
    //         std::pow(vec.z , 2)
    //         );
    // }

    // double
    // vector3toScaler(
    //     const geometry_msgs::msg::Vector3 history_vec3,
    //     const geometry_msgs::msg::Vector3 target_vec3
    // ){
    //     auto distance_vec = geometry_msgs::msg::Vector3();
    //     distance_vec.x = history_vec3.x - target_vec3.x;
    //     distance_vec.y = history_vec3.y - target_vec3.y;
    //     distance_vec.z = history_vec3.z - target_vec3.z;

    //     return makeScalerFromVector3(distance_vec);
    // }

    // geometry_msgs::msg::Vector3
    // vectorFixer(
    //     geometry_msgs::msg::Vector3 get_vec
    // ){
        
    // }

    // geometry_msgs::msg::Vector3
    // safeVecCreater(
    //    const geometry_msgs::msg::Vector3 history_vec3,
    //     const geometry_msgs::msg::Vector3 target_vec3
    // ){
    //     double scale = 
    //     makeScalerFromVector3(history_vec3) /
    //     makeScalerFromVector3(target_vec3);


    // }
};

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

        sub_cmd_vel_bomb = this->create_subscription<geometry_msgs::msg::Twist>(
            "robo2022/cmd_vel/bomb" , 4 , std::bind(&robo2022::topic_callback_cmd_vel_bomb , this , _1) // use linear z (y-up&right-hand)
        );

        pub_power = this->create_publisher<std_msgs::msg::Float32MultiArray>("robo2022/cmd_pwr" , 4);

        // vec_histry_rover = geometry_msgs::msg::Twist();
        // vec_histry_up_down = geometry_msgs::msg::Twist();
        // vec_histry_bomb = geometry_msgs::msg::Twist();

        // this->declare_parameter<double>("max_vel" , 1.0);
        // this->get_parameter("max_vel" , param_max_velocity);

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
    topic_callback_cmd_vel_bomb(
        const geometry_msgs::msg::Twist::SharedPtr get_msg)
    {
        vec_target_bomb = get_msg;
        pwr_callback_func();
    }

    void
    pwr_callback_func(){
        // auto just_time_cmd_vel_rover = geometry_msgs::msg::Twist();
        // auto just_time_cmd_vel_up_down = geometry_msgs::msg::Twist();
        // auto just_time_cmd_vel_bomb = geometry_msgs::msg::Twist();

        // if(
        // param_max_velocity > 
        // vector3toScaler(vec_histry_rover.linear , vec_target_rover->linear))
        // {
        //     just_time_cmd_vel_rover.linear = vec_target_rover->linear;
        // }else{

        // }

        if(vec_target_rover == NULL){return;}
        if(vec_target_up_down == NULL){return;}
        if(vec_target_bomb == NULL){return;}

        auto motorPower = vel2motor(vec_target_rover);
        auto pub_msg = std_msgs::msg::Float32MultiArray();
        pub_msg.data.insert(
            pub_msg.data.begin(),
            motorPower.begin(),
            motorPower.end()
        );
        pub_msg.data.push_back(vec_target_up_down->linear.y);
        pub_msg.data.push_back(vec_target_bomb->linear.x);

        pub_power->publish(pub_msg);

    }


    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_vel_rover;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_vel_up_down;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_vel_bomb;

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_power;

    geometry_msgs::msg::Twist::SharedPtr vec_target_rover;
    geometry_msgs::msg::Twist::SharedPtr vec_target_up_down;
    geometry_msgs::msg::Twist::SharedPtr vec_target_bomb;

//     geometry_msgs::msg::Twist vec_histry_rover;
//     geometry_msgs::msg::Twist vec_histry_up_down;
//     geometry_msgs::msg::Twist vec_histry_bomb;

    rclcpp::TimerBase::SharedPtr pwr_callback_timer;

//     double param_max_velocity;
};

int main(int argc , char * argv[])
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<robo2022>());
    rclcpp::shutdown();
    return 0;
}