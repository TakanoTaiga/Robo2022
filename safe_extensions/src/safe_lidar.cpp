#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/string.hpp"

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
            }
            onecePubOK2 = true;
        }else{
            auto pub_msg = std_msgs::msg::String();
            pub_msg.data = "Stay away from the robot!";
            pub_error->publish(pub_msg);            
            onecePubOK = true;
        }
    }

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_error;

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