# 2022Robokon A team

more info --> [2022Robokon_A_Team/architecture.md](https://github.com/TakanoTaiga/2022Robokon_A_Team/blob/main/architecture.md)

## Build

install dependencies:
```
sudo apt install ros-foxy-joy-linux
sudo apt install ros-foxy-joy-linux-dbgsym
```

Create a workspace and clone repos:
```
mkdir -p ~/robo2022_ws/src
cd ~/robo2022_ws/src
git clone https://github.com/TakanoTaiga/Robo2022.git
```
Build the code in the workspace
```
cd ~/robo2022_ws
colcon build
```

## Launch
Real
```
source ~/robo2022_ws/install/setup.sh
ros2 launch robo_launch real.py
```
Sim
```
source ~/robo2022_ws/install/setup.sh
ros2 launch robo_launch sim.py
```
