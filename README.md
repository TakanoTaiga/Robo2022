![main workflow](https://github.com/TakanoTaiga/Robo2022/actions/workflows/main.yml/badge.svg)

# 2022Robokon A team
document --> [TakanoTaiga/robo2022doc](https://github.com/TakanoTaiga/robo2022doc)

## Build

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
ros2 launch robo_launch real_a.py
```
Sim
```
source ~/robo2022_ws/install/setup.sh
ros2 launch robo_launch sim_a.py
```
