# 2022Robokon A team

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
git clone https://github.com/TakanoTaiga/2022Robokon_A_Team.git
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
ros2 launch ~/robo2022_ws/src/2022Robokon_A_Team/launch_real.py
```
Virtual
```
source ~/robo2022_ws/install/setup.sh
ros2 launch ~/robo2022_ws/src/2022Robokon_A_Team/launch_virtual.py
```
