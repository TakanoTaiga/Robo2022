#include <memory>
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <math.h>
#include <vector>


#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>

#include "robo2022/macros.hpp"

#define MAX_BUF_SIZE 128

#define ASYNC_SYNC ASYNC

using std::placeholders::_1;

class mdc2022Connect : public rclcpp::Node
{
    public:
    mdc2022Connect() : Node("mdc2022Connect")
    {
        this->declare_parameter<std::string>("device_file" , "/dev/ttyACM0");
        this->declare_parameter<bool>("debug" , false);
        this->declare_parameter<bool>("async" , SYNC);

        this->get_parameter("device_file" , deviceName_str);
        this->get_parameter("debug" , param_debug);
        this->get_parameter("async" , is_async);

        if(is_async != ASYNC_SYNC){
            RCLCPP_ERROR(this->get_logger() , "Async/Sync mode is not mach.");
            rclcpp::shutdown();
        }

        char deviceName[64];
        strcpy(deviceName , deviceName_str.c_str());
        this->fd1 = this->open_serial(deviceName);
        if(this->fd1 < 0){
            RCLCPP_ERROR(this->get_logger() , "Serial port failed to execute script esptool");
        }else{
            RCLCPP_INFO(this->get_logger(), "Serial port was connected");
        }

        sub_sp = this->create_subscription<std_msgs::msg::Float32MultiArray>(
            "robo2022util/main/cmd_pwr" , 10 , std::bind(&mdc2022Connect::topic_callback_sp, this , _1)
        );

        sub_team_support = this->create_subscription<std_msgs::msg::Float32MultiArray>(
            "robo2022util/team/cmd_pwr" , 10 , std::bind(&mdc2022Connect::topic_callback_team_support,this,_1)
        );

    }

    private:

    void topic_callback_sp(const std_msgs::msg::Float32MultiArray::SharedPtr msg){
        if(msg->data.size() != 5){
            RCLCPP_ERROR(this->get_logger() , "callback-sp msg data count error");
            return;
        }
        vec_sp_history.resize(0);
        vec_sp_history = msg->data;
#if ASYNC_SYNC
        is_sp_come = true;

        if(is_ts_come){
#endif
            auto vec_data = vec_sp_history;
            vec_data.insert(vec_data.end() , vec_ts_history.begin() , vec_ts_history.end());
            topic_compose(vec_data);
#if ASYNC_SYNC
            is_sp_come = false;
            is_ts_come = false;
        }
#endif
    }
    
    void topic_callback_team_support(const std_msgs::msg::Float32MultiArray::SharedPtr msg){
        if(msg->data.size() == 0){
            RCLCPP_ERROR(this->get_logger() , "callback_team_support msg data count error");
            return;
        }
        vec_ts_history.resize(0);
        vec_ts_history = msg->data;

#if ASYNC_SYNC
        is_ts_come = true;

        if(is_sp_come){
#endif
            auto vec_data = vec_sp_history;
            vec_data.insert(vec_data.end() , vec_ts_history.begin() , vec_ts_history.end());
            topic_compose(vec_data);

#if ASYNC_SYNC
            is_sp_come = false;
            is_ts_come = false;
        }
#endif
    }

    void topic_compose(const std::vector<float> vec_data)
    {
        auto vec = vec_data;
        char buf[MAX_BUF_SIZE] = "M:";
        size_t offset = strlen(buf);

        for(size_t i = 0 ; i < vec.size() ; i++){
            if(vec[i] > 1000){
                vec[i] = 0;
            }
            int flag = 1;
            if(vec[i] < 0){
                vec[i] *= -1;
                flag = 0;
            }

            char data_str[12];
            snprintf(data_str , 12 , "%d:%d:" , flag , (int)(vec[i]));
            memmove(buf+offset , data_str , strlen(data_str));
            offset += strlen(data_str);
        }
        memmove(buf+offset , "E" , 2);

        int error = write(fd1,buf,strlen(buf));
        if(error < 0){
            RCLCPP_ERROR(this->get_logger(), "Serial connection faild");
        }
        
        if(param_debug){
            RCLCPP_INFO(this->get_logger(), "sended data :'%s'" , buf);
        }
    }

    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr sub_sp;
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr sub_team_support;

    std::vector<float> vec_sp_history;
    std::vector<float> vec_ts_history;
    bool is_sp_come = false;
    bool is_ts_come = false;

    bool is_async = ASYNC;

    std::string deviceName_str;

    bool param_debug;
    // bool param_is_sync;

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