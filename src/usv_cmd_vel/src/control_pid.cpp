#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float32.h>
#include <math.h>
#include <stdio.h>
#include <usv_cmd_vel/cora_pid.hpp>

geometry_msgs::Twist des_vel;
nav_msgs::Odometry actu_vel;
std_msgs::Float32 left__thrust;
std_msgs::Float32 right_thrust;
std_msgs::Float32 left_angle;
std_msgs::Float32 right_angle;
std_msgs::Float32 lateral__thrust;
//目前合适的数值 0.01 0.05 0 0 0 0
double kp,kd,ut_vx,ut_vy,ut_wz;
//回调函数，一旦接收到新数据就将其更新到des_vel
void desire_back(const geometry_msgs::Twist &desire_vel)
{
    des_vel = desire_vel;
}
//回调函数，一旦接收到新数据就将其更新到actu_vel
void actual_back(const nav_msgs::Odometry &actual_vel)
{
    actu_vel = actual_vel;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL,"");
    if(argc!=7)  ROS_INFO("请输入kp_vx,kd_vx,ki_vx,kp_wz,kd_wz,ki_wz");

    ros::init(argc,argv,"control_pid",ros::init_options::AnonymousName);
    ros::NodeHandle nh;
    //订阅速度控制话题，使用movebase发布的速度信息控制螺旋桨转速
    ros::Subscriber sub_desire = nh.subscribe("/cmd_vel",10,desire_back);
    //odom话题由GPS和IMU融合定位提供
    ros::Subscriber sub_actual = nh.subscribe("/cora/robot_localization/odometry/filtered",10,actual_back);
    //发布消息到左螺旋桨转速控制话题
    ros::Publisher pub_left_thrust = nh.advertise<std_msgs::Float32>("/cora/thrusters/left_thrust_cmd",10);
    //发布消息到右螺旋桨转速控制话题
    ros::Publisher pub_right_thrust = nh.advertise<std_msgs::Float32>("/cora/thrusters/right_thrust_cmd",10);
    
    ros::Rate r(50); 
    while (ros::ok())
    {
        control::pid_wamv pid;
        // liner.x x即是前进的方向
        ut_vx = pid.pid_control(des_vel.linear.x,actu_vel.twist.twist.linear.x,atof(argv[1]),atof(argv[2]),atof(argv[3]));
        // ut_vx = fabs(ut_vx);
        if(ut_vx>=1) ut_vx =1;
        if(ut_vx<=-1) ut_vx = -1; 
        left__thrust.data+=ut_vx;
        right_thrust.data+=ut_vx;

        
        // angle.z 对于cora模型，目前的控制方法是当angular.z不为0时就只转向
        if(des_vel.angular.z<0){
            left__thrust.data=0.1;
            right_thrust.data=-0.1;
        }
        if(des_vel.angular.z>0){
            left__thrust.data=-0.1;
            right_thrust.data=0.1;
        }

        pub_left_thrust.publish(left__thrust);
        pub_right_thrust.publish(right_thrust);

        r.sleep(); 
        ros::spinOnce();
    }
    

    return 0;
}