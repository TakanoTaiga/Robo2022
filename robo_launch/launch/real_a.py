from launch import LaunchDescription , actions
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os.path

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='robo2022',
            executable='joy2vel',
            remappings=[
                ('/joy' , '/sc_client/joy'),
                ('/SmartUI' , '/sc_client/SmartUI')
            ],
            on_exit=actions.Shutdown(),
        ),
        Node(
            package='sc_client',
            executable='server',
            parameters=[{'nic' : 'wlp3s0'} , {'c1' : 'Up'} , {'c2' : 'Down'} , {'s1' : 'Fir power'} , {'s2' : 'N/A'} , {'debug' : False}],
            on_exit=actions.Shutdown(),
        ),
        Node(
            package='joy',
            executable='joy_node',
            remappings=[
                ('/joy' , '/sc_client/joy')
            ],
        ),
        Node(
            package='robo2022',
            executable='robo2022',
            on_exit=actions.Shutdown(),
        ),
        Node(
            package='robo2022',
            executable='errorChecker',
            on_exit=actions.Shutdown(),
        ),
        Node(
            package='robo2022',
            executable='mdc2022Connect',
            on_exit=actions.Shutdown(),
            parameters=[{'device_file' , '/dev/ttyACM1'}],
            remappings=[
                ('robo2022util/team/cmd_pwr' , 'robo2022util/a/cmd_pwr'),
            ],
        ),
        Node(
            package='robo2022',
            executable='safetyLidar',
            remappings=[
                ('/scan' , '/robo2022/scan'),
            ],
        ),
        Node(
            package='urg_node',
            executable='urg_node_driver',
            parameters=[{'serial_port' : '/dev/ttyACM0'}],
            remappings=[
                ('/scan' , '/robo2022/scan'),
            ]
        ),
        Node(
            package='rviz2',
            namespace='',
            executable='rviz2',
            name='rviz2',
            arguments=['-d' , os.path.join(get_package_share_directory('robo_launch'), 'rviz', 'lidarView.rviz')]
        )
    ])
