#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include "robo2022test/robo2022uitls.hpp"


using std::placeholders::_1;
using namespace std::chrono_literals;

#define FREQ 10ms

class vec_monitor : public rclcpp::Node , public robo2022utils
{
    public:
    vec_monitor() : Node("vec_monitor")
    {
        this->fileName = getDatetimeStr() + ".csv";

        sub_input_vel1 = this->create_subscription<geometry_msgs::msg::Twist>(
            "/vel_1" , 10 , std::bind(&vec_monitor::record_vec1 , this , _1)
        );
        sub_input_vel2 = this->create_subscription<geometry_msgs::msg::Twist>(
            "/vel_2" , 10 , std::bind(&vec_monitor::record_vec2 , this , _1)
        );

        std::ofstream file;
        file.open(this->fileName , std::ios::app);
        file << "Counter,";
        file << "V1 Linear X,V1 Linear Y,V1 Linear Z,";
        file << "V1 angular X,V1 angular Y,V1 angular Z,";
        file << "V1 linear scaler, V1 angular scaler,";

        file << "V2 Linear X,V2 Linear Y,V2 Linear Z,";
        file << "V2 angular X,V2 angular Y,V2 angular Z,";
        file << "V2 linear scaler, V2 angular scaler\n";     
        file.close();   


        callback_timer = this->create_wall_timer(FREQ , [this](){
            this->callback_func();
        });
    }

    private:

    void
    record_vec1(const geometry_msgs::msg::Twist::SharedPtr msg){
        archiveTwist1.linear = msg->linear;
        archiveTwist1.angular = msg->angular;
    }

    void
    record_vec2(const geometry_msgs::msg::Twist::SharedPtr msg){
        archiveTwist2.linear = msg->linear;
        archiveTwist2.angular = msg->angular;
    }

    void
    callback_func(){
        std::ofstream file;
        file.open(fileName , std::ios::app);
        file << counter << ",";
        file << archiveTwist1.linear.x << ","<< archiveTwist1.linear.y << ","<< archiveTwist1.linear.z << ",";
        file << archiveTwist1.angular.x << ","<< archiveTwist1.angular.y << ","<< archiveTwist1.angular.z << ",";
        file << makeScalerFromVector3(archiveTwist1.linear) << "," << makeScalerFromVector3(archiveTwist1.linear) << ",";
        file << archiveTwist2.linear.x << ","<< archiveTwist2.linear.y << ","<< archiveTwist2.linear.z << ",";
        file << archiveTwist2.angular.x << ","<< archiveTwist2.angular.y << ","<< archiveTwist2.angular.z << ",";
        file << makeScalerFromVector3(archiveTwist2.linear) << "," << makeScalerFromVector3(archiveTwist2.linear) << ",\n";
        file.close();

        counter += 1;
    }


    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_input_vel1;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_input_vel2;

    rclcpp::TimerBase::SharedPtr callback_timer;

    geometry_msgs::msg::Twist archiveTwist1;
    geometry_msgs::msg::Twist archiveTwist2;


    std::string fileName;

    int64_t counter = 0;
};

int main(int argc , char * argv[])
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<vec_monitor>());
    rclcpp::shutdown();
    return 0;
}