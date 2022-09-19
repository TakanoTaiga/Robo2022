from launch import LaunchDescription , actions
from launch_ros.actions import Node


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
            executable='joy2vel4b',
            remappings=[
                ('/joy' , '/sc_client/joy'),
                ('/SmartUI' , '/sc_client/SmartUI')
            ],
            on_exit=actions.Shutdown(),
        ),
        Node(
            package='robo2022',
            executable='b_team',
            remappings=[
                ('/joy' , '/sc_client/joy'),
                ('/SmartUI' , '/sc_client/SmartUI')
            ],
            on_exit=actions.Shutdown(),
        ),
        Node(
            package='robo2022',
            executable='errorChecker',
            on_exit=actions.Shutdown(),
        ),
    ])
