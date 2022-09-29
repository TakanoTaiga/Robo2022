#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/bool.hpp"

#define RANGE 200

using std::placeholders::_1;

class safeLidar : public rclcpp::Node{
    public:
    safeLidar(): Node("safeLidar")
    {
        laser_sub = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "/scan" , 10 , std::bind(&safeLidar::laser_topic_callback , this , _1)
        );
        
        pub_error = this->create_publisher<std_msgs::msg::String>("sc_client/error" , 10); 
        pub_safe_wall = this->create_publisher<std_msgs::msg::Bool>("safe_wall/lock" , 10);
    }

    private:

    void
    laser_topic_callback(
        const sensor_msgs::msg::LaserScan::SharedPtr msg
    ){
        int ragesSize = (int)msg->ranges.size();

        int counterGoodRange = 1;
        int counterSafeRange = 1;

        for(int i = ragesSize / 2 - RANGE / 2 ; i < ragesSize / 2 + RANGE / 2 ; i++){
            if (msg->ranges[i] < msg->range_max && msg->ranges[i] > msg->range_min)
            {
                counterGoodRange++;
                if(msg->ranges[i] > 1.3){
                    counterSafeRange++;
                }
            }    
        }
        int safePercent = ((float)counterSafeRange / (float)counterGoodRange) * 100.0;

        if(safePercent > 90){
            if(onecePubOK){
                auto pub_msg = std_msgs::msg::String();
                pub_error->publish(pub_msg);
                onecePubOK = false;

                auto pub_msg_2 = std_msgs::msg::Bool();
                pub_msg_2.data = false;
                pub_safe_wall->publish(pub_msg_2);
            }
            onecePubOK2 = true;
        }else{
            auto pub_msg = std_msgs::msg::String();
            pub_msg.data = "Stay away from the robot!";
            pub_error->publish(pub_msg);            
            onecePubOK = true;

            auto pub_msg_2 = std_msgs::msg::Bool();
            pub_msg_2.data = true;
            pub_safe_wall->publish(pub_msg_2);
        }
    }

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_error;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pub_safe_wall;


    bool onecePubOK = true;
    bool onecePubOK2 = true;

};

int main(int argc , char * argv[])
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<safeLidar>());
    rclcpp::shutdown();
    return 0;
}