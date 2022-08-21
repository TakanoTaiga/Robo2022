from math import fabs
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='robo2022',
            executable='vel2motor',
            parameters=[{'motor_position':'V1'}],
            remappings=[
                ('robo2022/motor_power' , 'robo2022/motor_power/V1')
            ]
        ),
        Node(
            package='robo2022',
            executable='vel2motor',
            parameters=[{'motor_position':'V2'}],
            remappings=[
                ('robo2022/motor_power' , 'robo2022/motor_power/V2')
            ]
        ),
        Node(
            package='robo2022',
            executable='vel2motor',
            parameters=[{'motor_position':'V3'}],
            remappings=[
                ('robo2022/motor_power' , 'robo2022/motor_power/V3')
            ]
        ),
        Node(
            package='robo2022',
            executable='vel2motor',
            parameters=[{'motor_position':'V4'}],
            remappings=[
                ('robo2022/motor_power' , 'robo2022/motor_power/V4')
            ]
        ),
        Node(
            package='robo2022',
            executable='joy2vel',
            remappings=[
                ('/joy' , '/sc_client/joy')
            ]
        ),
        Node(
            package='sc_client',
            executable='server',
            parameters=[{'nic' : 'eno1'} , {'c1' : 'Up'} , {'c2' : 'Down'} , {'s1' : 'Unused'} , {'s2' : 'Shot Power'} , {'debug' : False}]
        ),
        Node(
            package='joy',
            executable='joy_node',
            remappings=[
                ('/joy' , '/sc_client/joy')
            ]
        ),

    ])
