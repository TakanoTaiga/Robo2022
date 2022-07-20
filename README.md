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
stateDiagram-v2
  PhysicalGameController --> SmartController3App(iOS)
  SmartController3App(iOS) --> RoboNetwork2
  RoboNetwork2 --> SmartController3App(iOS)
  RoboNetwork2 --> ROS2
  
  state PhysicalGameController{
  XboxOneController --> bluetooth
  Dualshock --> bluetooth
  OtherController --> bluetooth
  }

  state SmartController3App(iOS){
  NWListener --> ROSConnect
  ROSConnect --> NWConnection
  ROSConnect --> ROSInfomation
  GameController --> NWConnection
  GameController --> ControllerInfomationWidget
  GameController --> DeviceInfoWidget
  
  }
  state ROS2{
  rn2 --> joy2vel
  joy2vel --> omni
  }
 
  

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
