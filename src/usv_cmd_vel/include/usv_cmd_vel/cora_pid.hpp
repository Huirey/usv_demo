#ifndef _PID_WAMV_H
#define _PID_WAMV_H
#include <math.h>

namespace control
{
    //定义PID控制对象
    class pid_wamv
    {
    private:
        double err,last_err,detla_err,last_time=0,intergral_error=0;
        double ut;
        int count = 0;
    public:
       double pid_control(double desire,double actual,double kp,double kd,double ki);
    };
    //PID控制函数
   double pid_wamv::pid_control(double desire,double actual,double kp,double kd,double ki)
   {
        err = desire - actual;
        intergral_error +=err;
        detla_err=err-last_err;

        //Ki限幅
        detla_err=detla_err>=2 ? 2:detla_err;
        detla_err=detla_err<=-2 ? -2:detla_err;       

        ut = kp*err + ki*intergral_error + kd*detla_err;
        if(count%3==0)//每三次收到数据更新一次PID
        {
            last_err = err;
        }
        count++;
        return ut;
   }
} 


#endif