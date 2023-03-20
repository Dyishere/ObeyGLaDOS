#include <queue>
#include <vector>
#include <string>
#include <cmath>
#include "Task.cpp"
#include "Robot.h"


void Robot::output() {
        printf("forward %d %lf\n", robotId, this->forwardSpeed);
        printf("rotate %d %lf\n", robotId, this->rotateSpeed);

        // 如果动作不为空，那么输出动作
        if(!actions.empty()) {
            for(int i = 0;i < actions.size();i++)
                printf("%s %d\n", actions[i].c_str() , robotId);
            actions.clear();
        }

    }

void Robot::setTarget(double tmp_x, double tmp_y) {
        target_x = tmp_x;
        target_y = tmp_y; 
        // 计算机器人与目标点之间的距离和角度
        double dx = target_x - x;
        double dy = target_y - y;
        double distance = std::sqrt(dx*dx + dy*dy);
        double angle = std::atan2(dy, dx);
        
        // 如果身处工作台，那么停止
        if(this->currentWorkbenchId == targetWorkbenchId) {
            return ;
        }

        // 计算机器人需要旋转的角度和方向
        double error_angle = angle - towards;

        while (error_angle > M_PI) {
            error_angle -= 2 * M_PI;
        }
        while (error_angle < -M_PI) {
            error_angle += 2 * M_PI;
        }

        int sign = (error_angle > 0) ? 1 : -1;
        
        // 如果机器人转到正确的方向，那么直接前进
        if (std::abs(error_angle) < 0.01) {
            rotateSpeed = 0;
             //如果距离超过10m，那么速度为6m/s
            if(distance >= 1)
                forwardSpeed = 6;
            else forwardSpeed = 4;
        }
        // 否则旋转
        else {
            forwardSpeed = 0;
            
            if(abs(error_angle) >= 2)
                rotateSpeed = 3*sign;
            else if(abs(error_angle) >= 1)
                rotateSpeed = 3*sign;
            else if(abs(error_angle) >= 0.5)
                rotateSpeed = 2*sign;
            else
                rotateSpeed = 1*sign;
        }




        


        // 限制前进速度和旋转速度的范围
        if (forwardSpeed > 6.0) {
            forwardSpeed = 6.0;
        }
        if (forwardSpeed < -2.0) {
            forwardSpeed = -2.0;
        }
        if (rotateSpeed > 1.0 * M_PI) {
            rotateSpeed = 1.0 * M_PI;
        }
        if (rotateSpeed < -1.0 * M_PI) {
            rotateSpeed = -1.0 * M_PI;
        }
    }


/*
    设置目标工作台及要完成的任务
    0 代表正在执行一个任务
    1 代表已经完成一个buy任务
    2 代表已经完成一个sell任务
    3 代表已经完成一个des任务
*/ 
int Robot::setTargetWorkbench(int workbenchId , double workbench_x ,double workbench_y, std::vector<std::string> commands) {
        this->targetWorkbenchId = workbenchId;
        this->commands.assign(commands.begin(), commands.end());
        this->targetWorkbench_x = workbench_x;
        this->targetWorkbench_y = workbench_y;
        /*
        通过判断机器人当前所处的工作台id来判断是否到达目标工作台
        如果到达目标工作台，那么执行指令并将目标工作台id设置为-1表示已经完成当前任务
        */
       if(currentWorkbenchId == workbenchId) {
            // 如果要买且当前工作台有产品，那么买
            if(commands[0] == "buy") {
                if(workbenches->at(currentWorkbenchId).productGridStatus == 1) {
                    // 将目标工作台id设置为-1表示已经完成当前任务
                    targetWorkbenchId = -1;
                    // 执行指令就是将commands中的指令加入到actions中
                    // 然后output函数中会输出actions中的指令
                    actions.push_back(commands[0]);
                    lastworkbenchId = currentWorkbenchId;
                    return 1;

                }else {
                    return 0;
                }
            }else if(commands[0] == "sell") {

                // 如果要卖，那么检查当前工作台是否收购这个产品
                if(!haveThisMaterial(itemType , workbenches->at(currentWorkbenchId).materialGridStatus)) {
                    // 将目标工作台id设置为-1表示已经完成当前任务
                    targetWorkbenchId = -1;
                    // 执行指令就是将commands中的指令加入到actions中
                    // 然后output函数中会输出actions中的指令
                    actions.push_back(commands[0]);
                    lastworkbenchId = currentWorkbenchId;
                    return 2;
                }else {
                    return 0;
                }

            }

        }
        
        setTarget(workbench_x ,  workbench_y);
        return 0;
        
    };


    // 每次检查当前是否正在执行任务
    // 如果正在执行任务，那么继续执行任务
    // 如果没有正在执行任务，那么执行下一个任务,从任务队列当中取出一个任务
    // 如果任务队列为空，那么停在原地
    /*
        设置目标工作台及要完成的任务
        0 代表正在执行一个任务
        1 代表已经完成一个buy任务
        2 代表已经完成一个sell任务
        3 代表已经完成一个des任务
        4 代表当前空闲了
    */
    
int Robot::doWork() {
        // 如果没有执行命令
        if(targetWorkbenchId == -1) {
            // 如果任务队列不为空
            if(!tasks.empty()) {
            
                // 取出任务队列的第一个任务
                // 将该任务的目标工作台设置为当前目标工作台'

            
                Task tmp_task = tasks.front();
                tasks.pop();
                // 开始一个新任务
                isWorking = true;
                return setTargetWorkbench(tmp_task.workbenchId , tmp_task.workbench_x ,tmp_task.workbench_y , tmp_task.commands);

            }else {
                // 如果任务队列为空，那么停在原地
                forwardSpeed = 0;
                rotateSpeed = 0;
                // 将该机器人的状态设置空闲状态
                isWorking = false;
                return 4;
            }

        // 如果正在执行任务，那么继续执行任务
        }else {
            return setTargetWorkbench(this->targetWorkbenchId ,this->targetWorkbench_x,this->targetWorkbench_y, this->commands);
        }

}

bool Robot::haveThisMaterial(int materialType, int materialGridStatus) {
  // 将materialType转换成对应的位索引
  int bitIndex = materialType;
  // 将1左移bitIndex位，得到一个只有第bitIndex位是1的数
  int bitMask = 1 << bitIndex;
  // 如果materialGridStatus与bitMask按位与的结果不为0，则说明该位为1，表示拥有这个物品
  return (materialGridStatus & bitMask) != 0;
}

