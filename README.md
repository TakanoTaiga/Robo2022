# 2022Robokon A team
## 開発環境
- Jetson AGX Xavier
  - Jetpack 5.0.1
  - ROS2 Foxy
- MacBook Pro
  - M1 Max
  - Docker(Ubuntu 20.04)
  - ROS2 Foxy
- Surface Go
  - Ubuntu 22.04LTS
  - ROS2 Humble
- Jetson Nano
  - Jetpack 4.6.1
  - ROS2 Dashing
  
## 本番環境(予定)
- Jetson AGX Xavier
  - Jetpack 5.0.1
  - ROS2 Foxy

- Jetson Nano(Second Support)
  - Jetpack 4.6.1
  - ROS2 Dashing

  

## システム概要図-1

コントローラー周り
```mermaid
flowchart TD

id1(PhysicalGameController) -- blutooth --> id2(GCContoller)
subgraph SmartControllerApp3
id2 --> id3(GameControllerClass)
id3 -- TX --> id4(NWConnection)
id3 --> DeviceInfoWidget
id3 --> ControllerInfomationWidget
id5(ROSConnect) -- tx --> id4
id4 -- rx --> id5
id5 --> ROSInfomation
end

subgraph smart_contoller3
id4 -- UDP --> id6(rn2)
end

subgraph robo2022
id6 -- sensor_msgs/Joy Message --> id7(jo2vel)
id7 -- geometry_msgs/Twist Message --> id8(omni)
end

id8 -- USB Serial --> Arduino

```

## システム概要図-2
ロボット動作

```mermaid
stateDiagram-v2
  RobotNetwork2 --> ROS2
  ROS2 --> STM32
  STM32 --> ROS2
  STM32 --> Hardwawre
  Hardwawre --> STM32

  state RobotNetwork2{
    pythonSocket --> rclpy
  }
  
  state ROS2{
  vel_msg --> PWRManager
  odom_msg
  }
  
  state STM32{
    UART --> HAL
  }
  
  state Hardwawre{
    Motor
    Encoder
  }

```

## システム概要図-3
センサー
