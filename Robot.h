#ifndef _ROBOT_H
#define _ROBOT_H

#include <vector>
#include <string>
#include <queue>
#include "Task.h"
class Robot {


    public:


    // 机器人的任务队列
    std::queue<Task> tasks;
    Robot() {}

    int robotId;
    // 所处工作台id
    int currentWorkbenchId;
    // 携带物品类型
    int itemType;
    // 时间价值系数
    double timeValue;
    // 碰撞价值系数
    double collisionValue;
    // 坐标
    double x , y;
    // 朝向
    double towards;
    // 线速度
    double v_x ,  v_y;
    // 角速度
    double angleSpeed;
    // 目标点
    double target_x , target_y;

    // 前进速度
    double forwardSpeed;
    // 旋转速度
    double rotateSpeed;
    // 距离阈值
    double dist_threshold = 1;

    int lastworkbenchId = -1;

    // 当前正在前往工作台id
    // -1表示没有前往任何工作台
    int targetWorkbenchId;
    double targetWorkbench_x, targetWorkbench_y;
    // 要执行的指令
    std::vector<std::string> commands;
    std::vector<std::string> actions;
    bool isWorking = false;
    
    //vector<Workbench> *workbenches;



    /*
        此方法输出机器人的两个速度
        而且输出机器人要完成的指令
    */
    void output();
    /*
        设置一个目标点，机器人将自动设置响应的速度
    */ 
    void setTarget(double tmp_x, double tmp_y); 
    /*
        设置一个目标工作台和需要在该工作台完成的指令
        返回值:
            0 代表正在执行一个任务
            1 代表已经完成一个buy任务
            2 代表已经完成一个sell任务
            3 代表已经完成一个des任务
    */
    int setTargetWorkbench(int workbenchId , double workbench_x ,double workbench_y, std::vector<std::string> commands); 
    /*
        此方法会自动从任务队列中取出要执行的任务并完成。
        返回值:
            0 代表正在执行一个任务
            1 代表已经完成一个buy任务
            2 代表已经完成一个sell任务
            3 代表已经完成一个des任务
            4 代表当前空闲了,没有要执行的任务
    */
    int doWork(); 
};

#endif