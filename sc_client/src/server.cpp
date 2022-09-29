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
#include "std_msgs/msg/string.hpp"

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
    this->declare_parameter<std::string>("c1" , "c1");
    this->declare_parameter<std::string>("c2" , "c2");
    this->declare_parameter<std::string>("s1" , "s1");
    this->declare_parameter<std::string>("s2" , "s2");

    this->get_parameter("port" , param_port);
    this->get_parameter("nic" , param_nic);
    this->get_parameter("debug" , param_debug);
    this->get_parameter("c1" , param_c1_Label);
    this->get_parameter("c2" , param_c2_Label);
    this->get_parameter("s1" , param_s1_Label);
    this->get_parameter("s2" , param_s2_Label);

    get_ip(param_nic);

    pub_joy_ = this->create_publisher<sensor_msgs::msg::Joy>("sc_client/joy" , 1);
    pub_smart_ui_ = this->create_publisher<sensor_msgs::msg::Joy>("sc_client/SmartUI" , 1);

    sub_smartui_info = this->create_subscription<std_msgs::msg::String>(
      "sc_client/info" , 10 , std::bind(&server::smartui_info_callback, this , _1)
    );
    sub_smartui_error = this->create_subscription<std_msgs::msg::String>(
      "sc_client/error" , 10 , std::bind(&server::smartui_error_callback, this , _1)
    );

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
    
    timer_ = this->create_wall_timer(1ms , [this](){
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
          snprintf(buf_send , 16 + IF_NAMESIZE , "MYNODEIP%s" ,param_addr_c);
          send_addr.sin_addr = from_addr.sin_addr;
          sendto(send_socket, buf_send, strlen(buf_send), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
          RCLCPP_INFO(this->get_logger(), "Device connected");
        }

        char cmpData_whoAreYou[] = "WHOAREYOU";
        if(strcmp(buf , cmpData_whoAreYou) == 0){
          char buf_send[48];
          char hostname[32];
          gethostname(hostname, sizeof(hostname));
          snprintf(buf_send , 48,  "MYNAMEIS%s" ,hostname);
          sendto(send_socket, buf_send, strlen(buf_send), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
        }

        char cmpData_whatIsYourNode[] = "REQNODEPARAM";
        if(strcmp(buf , cmpData_whatIsYourNode) == 0){
          char buf_send[] = "MYNODEISsc_client/server";
          sendto(send_socket, buf_send, strlen(buf_send), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));

          rclcpp::sleep_for(100ms);

          char buf_send_c1[64];
          snprintf(buf_send_c1 , 64 , "C1LABEL%s" , param_c1_Label.c_str());
          sendto(send_socket, buf_send_c1, strlen(buf_send_c1), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));

          rclcpp::sleep_for(100ms);

          char buf_send_c2[64];
          snprintf(buf_send_c2 , 64 , "C2LABEL%s" , param_c2_Label.c_str());
          sendto(send_socket, buf_send_c2, strlen(buf_send_c2), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));

          rclcpp::sleep_for(100ms);

          char buf_send_s1[64];
          snprintf(buf_send_s1 , 64 , "S1LABEL%s" , param_s1_Label.c_str());
          sendto(send_socket, buf_send_s1, strlen(buf_send_s1), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));

          rclcpp::sleep_for(100ms);

          char buf_send_s2[64];
          snprintf(buf_send_s2 , 64 , "S2LABEL%s" , param_s2_Label.c_str());
          sendto(send_socket, buf_send_s2, strlen(buf_send_s2), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
        }

        char cmpData_ping[] = "PING";
        if(strcmp(buf , cmpData_ping) == 0){
          char buf_send[] = "CHECK";
          sendto(send_socket, buf_send, strlen(buf_send), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));

          rclcpp::sleep_for(100ms);

          smartui_msg_callback();
        }

        if(create_msg_controller(buf) > 0){
          auto pub_joy_msg = sensor_msgs::msg::Joy();
          pub_joy_msg.axes = axesData;
          pub_joy_msg.buttons = buttonData;
          pub_joy_->publish(pub_joy_msg);

          auto pub_smartUI_msg = sensor_msgs::msg::Joy();
          pub_smartUI_msg.axes = customSliderData;
          pub_smartUI_msg.buttons = customButtonData;
          pub_smart_ui_->publish(pub_smartUI_msg);

          axesData.resize(0);
          buttonData.resize(0);
          customButtonData.resize(0);
          customSliderData.resize(0);
        }
    });
  }

private:
    rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr pub_joy_;
    rclcpp::Publisher<sensor_msgs::msg::Joy>::SharedPtr pub_smart_ui_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_smartui_info;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_smartui_error;
    rclcpp::TimerBase::SharedPtr timer_;

    rclcpp::TimerBase::SharedPtr smartuiTimer;


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
    std::string param_c1_Label;
    std::string param_c2_Label;
    std::string param_s1_Label;
    std::string param_s2_Label;


    std::vector<float> axesData;
    std::vector<int> buttonData;
    std::vector<int> customButtonData;
    std::vector<float> customSliderData;

    std_msgs::msg::String::SharedPtr smartui_info_msg;
    std_msgs::msg::String::SharedPtr smartui_error_msg;

    void
    smartui_info_callback(
      const std_msgs::msg::String::SharedPtr msg){
        smartui_info_msg = msg;
    }

    void
    smartui_error_callback(
      const std_msgs::msg::String::SharedPtr msg){
        smartui_error_msg = msg;
    }

    void
    smartui_msg_callback(){
      if(smartui_error_msg != NULL){
        char buf_smartui_[64];
        snprintf(buf_smartui_ , 64 , "SEMER%s" , smartui_error_msg->data.c_str());
        sendto(send_socket, buf_smartui_, strlen(buf_smartui_), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
      }
      

      if(smartui_info_msg != NULL){
        rclcpp::sleep_for(100ms);

        char buf_smartui_2[64];
        snprintf(buf_smartui_2 , 64 , "SINFO%s" , smartui_info_msg->data.c_str());
        sendto(send_socket, buf_smartui_2, strlen(buf_smartui_2), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));
      }
    }

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

    void my_atof2(char *data){
        char buf[16];
        snprintf(buf, 16, "%s", data+2);
        customSliderData.push_back(atof(buf));
    }

    void my_atob(char *data){
        if (data[2] == '1') {
          buttonData.push_back(1);
        }else if(data[2] == '0') {
          buttonData.push_back(0);
        }
    }

    void my_atob2(char *data){
        if (data[2] == '1') {
          customButtonData.push_back(1);
        }else if(data[2] == '0') {
          customButtonData.push_back(0);
        }
    }

    int create_msg_controller(char *data){
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
                }else if(header == 'B'){
                  my_atob(packetData);
                }else if(header == 'C'){
                  my_atob2(packetData);
                }else if(header == 'S'){
                  my_atof2(packetData);
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
