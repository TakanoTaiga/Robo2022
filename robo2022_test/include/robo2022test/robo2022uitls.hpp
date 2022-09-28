#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>
#include <iomanip>


#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

#define ROBO2022UTILS_L 0.565685
#define VEL2MOTOR_SCALER 500

#define RESET_TWIST geometry_msgs::msg::Twist()

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

        vec[0] *= (float)(VEL2MOTOR_SCALER);
        vec[1] *= (float)(VEL2MOTOR_SCALER);
        vec[2] *= (float)(VEL2MOTOR_SCALER);
        vec[3] *= (float)(VEL2MOTOR_SCALER);

        return vec;
    }

    double
    makeScalerFromVector3(
        geometry_msgs::msg::Vector3 vec
    ){
        return std::sqrt(
            std::pow(vec.x , 2) +
            std::pow(vec.y , 2) +
            std::pow(vec.z , 2)
            );
    }

    std::string 
    getDatetimeStr() {
    time_t t = time(nullptr);
    const tm* localTime = localtime(&t);
    std::stringstream s;
    s << localTime->tm_year + 1900;
    s << std::setw(2) << std::setfill('0') << localTime->tm_mon + 1;
    s << std::setw(2) << std::setfill('0') << localTime->tm_mday;
    s << std::setw(2) << std::setfill('0') << localTime->tm_hour;
    s << std::setw(2) << std::setfill('0') << localTime->tm_min;
    s << std::setw(2) << std::setfill('0') << localTime->tm_sec;
    return s.str();
    }
};
