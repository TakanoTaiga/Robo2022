# 2022Robokon A team

document --> [TakanoTaiga/robo2022doc](https://github.com/TakanoTaiga/robo2022doc)

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
colcon build --symlink-install
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
