#include <memory>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <math.h>


#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>

#define PI 3.14159265359
#define MAX_BUF_SIZE 128

using std::placeholders::_1;

class mdc2022Connect : public rclcpp::Node
{
    public:
    mdc2022Connect() : Node("mdc2022Connect")
    {
        this->declare_parameter<std::string>("device_file" , "/dev/ttyACM0");
        this->get_parameter("device_file" , deviceName_str);
        this->declare_parameter<bool>("debug" , false);
        this->get_parameter("debug" , param_debug);


        char deviceName[64];
        strcpy(deviceName , deviceName_str.c_str());
        this->fd1 = this->open_serial(deviceName);
        if(this->fd1 < 0){
            RCLCPP_INFO(this->get_logger() , "Serial port failed to execute script esptool");
            rclcpp::shutdown();
        }else{
            RCLCPP_INFO(this->get_logger(), "Serial port was connected");
        }

        sub_ = this->create_subscription<std_msgs::msg::Float32MultiArray>(
            "robo2022uil/cmd_pwr" , 10 , std::bind(&mdc2022Connect::topic_callback, this , _1)
        );
    }

    private:

    void topic_callback(const std_msgs::msg::Float32MultiArray::SharedPtr get_msg)
    {
        char buf[MAX_BUF_SIZE] = "M:";
        size_t offset = strlen(buf);

        for(size_t i = 0 ; i < get_msg->data.size() ; i++){
            if(get_msg->data[i] > 1000){
                get_msg->data[i] = 0;
            }
            int flag = 1;
            if(get_msg->data[i] < 0){
                get_msg->data[i] *= -1;
                flag = 0;
            }

            char data_str[12];
            snprintf(data_str , 12 , "%d:%d:" , flag , (int)(get_msg->data[i]));
            memmove(buf+offset , data_str , strlen(data_str));
            offset += strlen(data_str);
        }
        memmove(buf+offset , "E" , 2);

        int error = write(fd1,buf,strlen(buf));
        if(error < 0){
            RCLCPP_INFO(this->get_logger(), "Serial connection faild");
            rclcpp::shutdown();
        }
        
        if(param_debug){
            RCLCPP_INFO(this->get_logger(), "sended data :'%s'" , buf);
        }
    }

    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr sub_;

    std::string deviceName_str;

    bool param_debug;

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
    rclcpp::spin(std::make_shared<mdc2022Connect>());
    rclcpp::shutdown();
    return 0;
}