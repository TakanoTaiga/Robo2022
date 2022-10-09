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
            parameters=[{'nic' : 'enxa0cec8d372fa'} , 
                        {'c1' : '🍤'} , 
                        {'c2' : '永飛舞落射'} , 
                        {'s1' : '発射機構の出力'} , 
                        {'s2' : '90以上で常時回転'} ,
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
            executable='joy2vel4b',
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
                ('/cmd_vel/in' ,  '/robo2022/cmd_vel/b/belt'),
                ('/cmd_vel/out' , '/smoothed_cmd_vel/b/belt')
            ],
            parameters=[{'gain' : 2.0}],
            name='belt'
        ),
        Node(
            package='robo2022',
            executable='motion_smoother',
            remappings=[
                ('/cmd_vel/in' ,  '/robo2022/cmd_vel/b/fir'),
                ('/cmd_vel/out' , '/smoothed_cmd_vel/b/fir')
            ],
            parameters=[{'gain' : 10.0}],
            name='fir'
        ),
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
            executable='b_team',
            remappings=[
                ('/joy' , '/sc_client/joy'),
                ('/SmartUI' , '/sc_client/SmartUI'),
                ('/robo2022/cmd_vel/b/belt' ,  '/smoothed_cmd_vel/b/belt'),
                ('/robo2022/cmd_vel/b/fir' ,  '/smoothed_cmd_vel/b/fir'),
            ],
        ),
        
        # Hardware connector
        Node(
            package='robo2022',
            executable='mdc2022Connect',
            on_exit=actions.Shutdown(),
            parameters=[{'device_file' : '/dev/ttyACM0'} , 
                        {'debug' : True} , 
                        {'async' : MACRO_MDC_SYNC}],
            remappings=[
                ('robo2022util/team/cmd_pwr' , 'robo2022util/b/cmd_pwr'),
            ],
        ),

        # Safe tools
        Node(
            package='robo2022',
            executable='errorChecker',
            on_exit=actions.Shutdown(),
        ),
    ])
