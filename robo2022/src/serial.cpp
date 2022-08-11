#include <memory>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <math.h>


#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#define PI 3.14159265359

using std::placeholders::_1;

class serial : public rclcpp::Node
{
    public:
    serial() : Node("serial")
    {
        this->declare_parameter<std::string>("device_file");
        this->get_parameter("device_file" , deviceName_str);
        char deviceName[64];
        strcpy(deviceName , deviceName_str.c_str());
        this -> fd1 = this->open_serial(deviceName);
        if(this->fd1 < 0){
            std::cout << "ERROR";
            rclcpp::shutdown();
        }else{
            RCLCPP_INFO(this->get_logger(), "Serial connected");
        }

        sub_ = this->create_subscription<std_msgs::msg::String>(
            "robo2022/serial_output" , 10 , std::bind(&serial::topic_callback, this , _1)
        );
    }

    private:

    void topic_callback(const std_msgs::msg::String::SharedPtr get_msg)
    {
        int len = get_msg->data.size();
        int error = write(fd1,get_msg->data.c_str(),len);
        if(error < 0){
            RCLCPP_INFO(this->get_logger(), "Serial connection faild");
            rclcpp::shutdown();
        }
        //RCLCPP_INFO(this->get_logger(), "sended data :'%s'" , get_msg->data.c_str());
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;

    std::string deviceName_str;

    int open_serial(char *device_name)
    {
        int fd=open(device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
        fcntl(fd, F_SETFL,0);
        struct termios conf_tio;
        tcgetattr(fd,&conf_tio);
        speed_t BAUDRATE = B115200;
        cfsetispeed(&conf_tio, BAUDRATE);
        cfsetospeed(&conf_tio, BAUDRATE);
        conf_tio.c_lflag &= ~(ECHO | ICANON);
        conf_tio.c_cc[VMIN]=0;
        conf_tio.c_cc[VTIME]=0;
        tcsetattr(fd,TCSANOW,&conf_tio);
        return fd;
    }

    int fd1;
};

int main(int argc , char * argv[])
{
    rclcpp::init(argc , argv);
    rclcpp::spin(std::make_shared<serial>());
    rclcpp::shutdown();
    return 0;
}