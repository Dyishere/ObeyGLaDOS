#ifndef _ROBOT_H
#define _ROBOT_H

#include <vector>
#include <string>
#include <queue>
#include "Task.h"
class Robot {


    public:
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

    // 当前正在前往工作台id
    // -1表示没有前往任何工作台
    int targetWorkbenchId;
    double targetWorkbench_x, targetWorkbench_y;
    // 要执行的指令
    std::vector<std::string> commands;
    std::vector<std::string> actions;

    void output(); 
    void setTarget(double tmp_x, double tmp_y); 
    void setTargetWorkbench(int workbenchId , double workbench_x ,double workbench_y, std::vector<std::string> commands); 
    void doWork(); 
};

#endif