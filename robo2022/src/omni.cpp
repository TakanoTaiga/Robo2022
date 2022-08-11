/*
This node has reached its EOL (end-of-life).
*/

#include <memory>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <math.h>


#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

#define PI 3.14159265359

using std::placeholders::_1;

class serial
{
    public:
    int open_serial(const char *device_name)
    {
        int fd=open(device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
        fcntl(fd, F_SETFL,0);
        //load configuration
        struct termios conf_tio;
        tcgetattr(fd,&conf_tio);
        //set baudrate
        speed_t BAUDRATE = B115200;
        cfsetispeed(&conf_tio, BAUDRATE);
        cfsetospeed(&conf_tio, BAUDRATE);
        //non canonical, non echo back
        conf_tio.c_lflag &= ~(ECHO | ICANON);
        //non blocking
        conf_tio.c_cc[VMIN]=0;
        conf_tio.c_cc[VTIME]=0;
        //store configuration
        tcsetattr(fd,TCSANOW,&conf_tio);
        return fd;
    }

    void serial_callback(std::string serial_msg)
    {
        int len = serial_msg.size();
        write(fd1,serial_msg.c_str(),len);
    }

    int fd1;

};

class msgCreate
{
    public:
    double L = 1.0;
    std::string creater(const geometry_msgs::msg::Twist::SharedPtr geo_msg)
    {
        double V1 = -1 * sin(PI / 4) * geo_msg->linear.x + cos(PI / 4) * geo_msg->linear.y + L * geo_msg->angular.z;
        double V2 = -1 * cos(PI / 4) * geo_msg->linear.x - sin(PI / 4) * geo_msg->linear.y + L * geo_msg->angular.z;
        double V3 =      sin(PI / 4) * geo_msg->linear.x - cos(PI / 4) * geo_msg->linear.y + L * geo_msg->angular.z;
        double V4 =      cos(PI / 4) * geo_msg->linear.x + sin(PI / 4) * geo_msg->linear.y + L * geo_msg->angular.z;

        double scaler = 50.0;
        V1 *= scaler;
        V2 *= scaler;
        V3 *= scaler;
        V4 *= scaler;

        std::string rtMessage = "FL" + std::to_string((int)V1);
        rtMessage += "FR" + std::to_string((int)V2);
        rtMessage += "RL" + std::to_string((int)V3);
        rtMessage += "RR" + std::to_string((int)V4);
        return rtMessage;
    }
};

class omnictr : public rclcpp::Node , public serial , public msgCreate
{
    public:
    omnictr() : Node("omnictr")
    {
        char deviceName[] = "/dev/ttyACM0";
        this -> fd1 = this->open_serial(deviceName);
        if(this->fd1 < 0){
            std::cout << "ERROR";
            rclcpp::shutdown();
        }
        sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "robo2022/cmd_vel" , 10 , std::bind(&omnictr::topic_callback, this , _1)
        );
    }

    private:

    void topic_callback(const geometry_msgs::msg::Twist::SharedPtr get_msg)
    {
    RCLCPP_INFO(this->get_logger(), "I heard x:%lf , y:%lf , z:%lf" , get_msg->linear.x , get_msg->linear.y ,get_msg->angular.z);

    std::string message = creater(get_msg);
    RCLCPP_INFO(this->get_logger(), message.c_str());
    serial_callback(message);
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_;
};

int main(int argc , char * argv[])
{
    rclcpp::init(argc , argv);
    rclcpp::spin(std::make_shared<omnictr>());
    rclcpp::shutdown();
    return 0;
}