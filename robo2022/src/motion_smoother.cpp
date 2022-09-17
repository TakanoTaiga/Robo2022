#include <memory>
#include <chrono>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include "robo2022/robo2022uitls.hpp"

using std::placeholders::_1;
using namespace std::chrono_literals;

class motion_smoother : public rclcpp::Node
{
  public:
  motion_smoother() : Node("motion_smoother")
  {
    sub_vel = this->create_subscription<geometry_msgs::msg::Twist>(
      "/cmd_vel/in" , 10 , std::bind(&motion_smoother::topic_callback , this , _1)
    );

    pub_vel = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel/out" , 10);

    timer = this->create_wall_timer(20ms , [this](){
      this->timer_callback();
    });

    target = RESET_TWIST;
    histry = RESET_TWIST;
  }

  private:

  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_vel;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_vel;
  rclcpp::TimerBase::SharedPtr timer;

  geometry_msgs::msg::Twist target;
  geometry_msgs::msg::Twist histry;


  void
  topic_callback(const geometry_msgs::msg::Twist::SharedPtr get_msg){
    target.linear = get_msg->linear;
    target.angular = get_msg->angular;
  }

  void
  timer_callback(){
    auto rtVec = RESET_TWIST;
  //5.2 4
  //4 5.2
    double vec = target.linear.x - histry.linear.x;
    vec = std::sqrt(vec * vec);
    if(vec > 0.1){
      //vector error
      if(target.linear.x > histry.linear.x){
        rtVec.linear.x = histry.linear.x + 0.1;
      }else{
        rtVec.linear.x = histry.linear.x - 0.1;
      }

    }else{
      rtVec.linear.x = target.linear.x;
    }



    histry.linear.x = rtVec.linear.x;
    pub_vel->publish(rtVec);
  }
};

int main(int argc , char * argv[])
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<motion_smoother>());
    rclcpp::shutdown();
    return 0;
}