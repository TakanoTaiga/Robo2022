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
#include "sensor_msgs/msg/joy.hpp"

using std::placeholders::_1;
using namespace std::chrono_literals;


#define pub_size 256

class server : public rclcpp::Node
{
  public:
  server() : Node("server")
  {
    this->declare_parameter<int>("port" , 64201);
    this->declare_parameter<std::string>("nic" , "lo");
    this->declare_parameter<bool>("debug" , false);

    this->get_parameter("port" , param_port);
    this->get_parameter("nic" , param_nic);
    this->get_parameter("debug" , param_debug);

    get_ip(param_nic);

    pub_ = this->create_publisher<sensor_msgs::msg::Joy>("sc_client/joy" , 10);

    rcv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    send_socket = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    send_addr.sin_family = AF_INET;

    char rcvAddr[] = "0.0.0.0";
    addr.sin_addr.s_addr = inet_addr(rcvAddr);
    addr.sin_port = htons(param_port);
    send_addr.sin_port = htons(param_port);

    bind(rcv_sock, (const struct sockaddr *)&addr, sizeof(addr));
    static const int val = 1;
    ioctl(rcv_sock , FIONBIO , &val);
    
    timer_ = this->create_wall_timer(10ms , [this](){
      char buf[pub_size];
        
        memset(buf, 0, sizeof(buf));
        if(recvfrom(rcv_sock , buf , sizeof(buf) , 0 , (struct sockaddr *)&from_addr , &sin_size) < 0){
          return;
        }

        if(param_debug){
          RCLCPP_INFO(this->get_logger(), "%s:%s", inet_ntoa(from_addr.sin_addr), buf);
        }

        char cmpData[] = "WHATISNODEIP";
        if(strcmp(buf , cmpData) == 0){
          char buf_send[16 + IF_NAMESIZE];
          sprintf(buf_send , "MYNODEIP%s" ,param_addr_c);
          send_addr.sin_addr = from_addr.sin_addr;
          sendto(send_socket, buf_send, strlen(buf_send), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
        }

        char cmpData_whoAreYou[] = "WHOAREYOU";
        if(strcmp(buf , cmpData_whoAreYou) == 0){
          char buf_send[48];
          char hostname[32];
          gethostname(hostname, sizeof(hostname));
          sprintf(buf_send , "MYNAMEIS%s" ,hostname);
          sendto(send_socket, buf_send, strlen(buf_send), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
        }

        char cmpData_whatIsYourNode[] = "WHATISYORNODE";
        if(strcmp(buf , cmpData_whatIsYourNode) == 0){
          char buf_send[] = "MYNODEISsc_client/server";
          sendto(send_socket, buf_send, strlen(buf_send), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
        }

        char cmpData_ping[] = "PING";
        if(strcmp(buf , cmpData_ping) == 0){
          char buf_send[] = "CHECK";
          sendto(send_socket, buf_send, strlen(buf_send), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
        }

        if(create_msg(buf) > 0){
          auto pub_joy_msg = sensor_msgs::msg::Joy();
          pub_joy_msg.axes = axesData;
          pub_joy_msg.buttons = buttonData;
          pub_->publish(pub_joy_msg);

          axesData.resize(0);
          buttonData.resize(0);
        }
    });
  }

private:
    rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr pub_;
    rclcpp::TimerBase::SharedPtr timer_;

    int rcv_sock;
    int send_socket;
    struct sockaddr_in addr;
    struct sockaddr_in from_addr;
    struct sockaddr_in send_addr;
    socklen_t sin_size;

    int param_port;
    char param_addr_c[IF_NAMESIZE];
    std::string param_nic;
    bool param_debug;

    std::vector<float> axesData;
    std::vector<int> buttonData;

    void get_ip(std::string nic){
      int fd_gi;
      struct ifreq ifr_gi;

      fd_gi = socket(AF_INET,SOCK_DGRAM,0);

      ifr_gi.ifr_addr.sa_family = AF_INET;

      strncpy(ifr_gi.ifr_name , nic.c_str() , IF_NAMESIZE-1);
      ioctl(fd_gi,SIOCGIFADDR,&ifr_gi);
      close(fd_gi);

      sprintf(param_addr_c , "%s" , inet_ntoa(((struct sockaddr_in *)&ifr_gi.ifr_addr)->sin_addr));

      RCLCPP_INFO(this->get_logger(), "%s", param_addr_c);
    }

    void my_atofs(char *data , int size){
    char data_x[16];
    char data_y[16];
    int colonPos = 0;
    
    bool flag = false;
    for(int i = 0; i < size ; i++){
        if (data[i] == ':') {
            colonPos = i;
            if(flag)
                break;
            flag = true;
        }
    }
    
    snprintf(data_x, colonPos - 2, "%s" , data + 2);
    snprintf(data_y, size - colonPos, "%s" , data + colonPos + 1);
    
    axesData.push_back(atof(data_x));
    axesData.push_back(atof(data_y));    
    }

    void my_atof(char *data){
        char buf[16];
        snprintf(buf, 16, "%s", data+2);
        axesData.push_back(atof(buf));
    }

    void my_atob(char *data){
        if (data[2] == '1') {
          buttonData.push_back(1);
        }else{
          buttonData.push_back(0);
        }
    }


    int create_msg(char *data){
        char headerName[7];
        snprintf(headerName, 7, "%s", data);
        if(strcmp(headerName, "GCINFO")){
            return -1;
        }
        
        int end_pos = 0;
        
        for(int i = 0 ;i <= pub_size - 2 ; i++){
            if (data[i] == 'E' && data[i + 1] == 'N' && data[i + 2] == 'D') {
                end_pos = i;
                break;
            }
        }
        
        char csvData[pub_size];
        snprintf(csvData, end_pos - 6, "%s", data + 6);
        
        for (int i = 0; i < pub_size - 1; i++) {
            if (csvData[i] == ',') {
                char header = csvData[i + 1];
                int nextt=0;
                for (int j = i + 1; j < pub_size; j++) {
                    if (csvData[j] == ',') {
                        nextt = j;
                        break;
                    }
                }
                char packetData[32];
                snprintf(packetData, nextt - i, "%s" , &csvData[i+1]);
                
                if (header == 'J') {
                    my_atofs(packetData, nextt - i);
                }else if(header == 'T'){
                    my_atof(packetData);
                }else{
                    my_atob(packetData);
                }
            }
        }
        return 1;
    }

    
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<server>());
  rclcpp::shutdown();
  return 0;
}
