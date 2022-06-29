# 2022Robokon A team

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
  NWListener --> ROSInfomation
  GameController --> NWConnection
  GameController --> ControllerInfomationWidget
  GameController --> DeviceInfoWidget
  
  }
  state ROS2{
  ROS2(JoyMessage)
  }
 
  

```

## システム概要図-2
ロボット定義&動作定義

```mermaid
stateDiagram-v2
  ROS2 --> STM32
  STM32 --> Motordriver

```

## システム概要図-3
シュミレーション
