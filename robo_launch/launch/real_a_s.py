from launch import LaunchDescription , actions
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os.path


def generate_launch_description():
    MACRO_MDC_ASYNC = False
    MACRO_MDC_SYNC = True
    return LaunchDescription([
        # Physical Controller
        Node(
            package='sc_client',
            executable='server',
            parameters=[{'nic' : 'enx606d3cd52e75'} , 
                        {'c1' : 'Up'} , 
                        {'c2' : 'Down'} , 
                        {'s1' : 'Fir power'} , 
                        {'s2' : "🦅 Eagle"} , 
                        {'debug' : False}],
            on_exit=actions.Shutdown(),
            remappings=[
                ('sc_client/error' , 'safe_extensions/error'),
            ]
        ),
        Node(
            package='joy',
            executable='joy_node',
            remappings=[
                ('/joy' , '/sc_client/joy')
            ],
        ),

        # Joysic to vector
        Node(
            package='robo2022',
            executable='joy2vel',
            remappings=[
                ('/joy' , '/sc_client/joy'),
                ('/SmartUI' , '/sc_client/SmartUI'),
            ],
            on_exit=actions.Shutdown(),
        ),
        Node(
            package='team_support',
            executable='joy2vel4a',
            remappings=[
                ('/joy' , '/sc_client/joy'),
                ('/SmartUI' , '/sc_client/SmartUI')
            ],
            on_exit=actions.Shutdown(),
        ),
        
        # Smoothers
        Node(
            package='robo2022',
            executable='motion_smoother',
            remappings=[
                ('/cmd_vel/in' ,  '/robo2022/cmd_vel/updown'),
                ('/cmd_vel/out' , '/smoothed_cmd_vel/main/updown')
            ],
            parameters=[{'gain' : 0.2}],
            name='updown'
        ),
        Node(
            package='robo2022',
            executable='motion_smoother',
            remappings=[
                ('/cmd_vel/in' ,  '/robo2022/cmd_vel/rover'),
                ('/cmd_vel/out' , '/smoothed_cmd_vel/main/rover')
            ],
            parameters=[{'gain' : 0.05}],
            name='main'
        ),

        # Main block
        Node(
            package='robo2022',
            executable='robo2022',
            remappings=[
                ('/robo2022/cmd_vel/updown' , '/smoothed_cmd_vel/main/updown'),
                ('/robo2022/cmd_vel/rover' , '/smoothed_cmd_vel/main/rover')
            ]
            
        ),
        Node(
            package='team_support',
            executable='a_team',
            remappings=[
                ('/joy' , '/sc_client/joy'),
                ('/SmartUI' , '/sc_client/SmartUI'),
                ('/robo2022/cmd_vel/a/fir' ,  'safe_extensions/cmd_vel/a/fir'),
            ],
        ),
        
        # Hardware connector
        Node(
            package='robo2022',
            executable='mdc2022Connect',
            on_exit=actions.Shutdown(),
            parameters=[{'device_file' : '/dev/ttyACM0'} , {'debug' : False},{'async' : MACRO_MDC_ASYNC}],
            remappings=[
                ('robo2022util/team/cmd_pwr' , 'robo2022util/a/cmd_pwr'),
            ],
        ),

        # Safe tools
        Node(
            package='robo2022',
            executable='errorChecker',
            on_exit=actions.Shutdown(),
        ),
        Node(
            package='safe_extensions',
            executable='safeLidar',
            remappings=[
                ('/scan' , '/safe_extensions/scan'),
                ('sc_client/error' , 'safe_extensions/error'),
            ],
        ),
        Node(
            package='urg_node',
            executable='urg_node_driver',
            parameters=[{'serial_port' : '/dev/ttyACM1'}],
            remappings=[
                ('/scan' , '/safe_extensions/scan'),
                ('sc_client/error' , 'safe_extensions/error'),
            ]
        ),
        Node(
            package='rviz2',
            namespace='',
            executable='rviz2',
            name='rviz2',
            arguments=['-d' , os.path.join(get_package_share_directory('robo_launch'), 'rviz', 'lidarView.rviz')]
        ),
        Node(
            package='safe_extensions',
            executable='safeWall',
            remappings=[
                ('/cmd_vel/in' , '/robo2022/cmd_vel/a/fir'),
                ('/cmd_vel/out' , 'safe_extensions/cmd_vel/a/fir'),
            ],
        ),
        
    ])
