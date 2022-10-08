#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <vector>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using std::placeholders::_1;

using namespace std::chrono_literals;

#define BUFFER_SIZE 256



class errorChecker : public rclcpp::Node
{
    public:
    errorChecker() : Node("errorChecker")
    {
        sub = this->create_subscription<std_msgs::msg::String>(
            "/error" , 10 , std::bind(&errorChecker::sub_callback , this , _1)
        );

        rcv_sock = socket(AF_INET, SOCK_DGRAM, 0);
        char rcvAddr[] = "0.0.0.0";
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(rcvAddr);
        addr.sin_port = htons(64202);
        bind(rcv_sock, (const struct sockaddr *)&addr, sizeof(addr));
        static const int val = 1;
        ioctl(rcv_sock , FIONBIO , &val);

        timer_ = this->create_wall_timer(1ms , [this](){
            timerCallback();
        });
    }

    private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub;

    int rcv_sock;
    struct sockaddr_in addr;
    struct sockaddr_in from_addr;
    socklen_t sin_size;
    rclcpp::TimerBase::SharedPtr timer_;    

    void
    sub_callback(std_msgs::msg::String::SharedPtr msg){
        RCLCPP_ERROR(this->get_logger(), "%s", msg->data.c_str());
        rclcpp::shutdown();
    }

    void timerCallback(){
        char buf[BUFFER_SIZE];
        
        memset(buf, 0, sizeof(buf));
        if(recvfrom(rcv_sock , buf , sizeof(buf) , 0 , (struct sockaddr *)&from_addr , &sin_size) < 0){
          return;
        }

        char cmpData[] = "EMERGENCY";
        if(strcmp(buf , cmpData) == 0){
            RCLCPP_ERROR(this->get_logger(), "push from controller emergency button");
            rclcpp::shutdown();
        }
    }
};

int main(int argc , char * argv[])
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<errorChecker>());
    rclcpp::shutdown();
    return 0;
}