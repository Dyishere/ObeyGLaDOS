#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <string>
#include "Robot.h"
#include "Workbench.h"
#include "Task.h"

using namespace std;


vector<Workbench> workbenchs;
vector<Robot> robots;
int frameID , money = 0 , workBenchNum = 0;
char map[100][100];

bool readUntilOK() {
    char line[1024];
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }

    }
    return false;
}

void readMap() {
    for(int i = 0; i < 100; i++) {
        for(int j = 0; j < 100; j++) {
            scanf("%c" , &map[i][j]);
        }
        // 读取换行符
        char tmp;
        scanf("%c" , &tmp);
    }

    // read OK
    char line[1024];
    //fgets(line, sizeof line, stdin);
    fgets(line, sizeof line, stdin);

}

void initWorkbenchs() {

    for(int i = 0; i < workBenchNum; i++) {
        Workbench workbench = Workbench();
        workbench.workbenchId = i;
        workbenchs.push_back(workbench);
    }


}

void initRobots(){

    // 初始化四个机器人
    for(int i = 0; i < 4; i++) {
        Robot robot =  Robot();
        robot.robotId = i;
        robot.targetWorkbenchId = -1;
        robot.isWorking = false;
        robots.push_back(robot); 
    }

    // 下面是测试，将任务加入到任务队列当中
    Task task1 = Task();
    task1.workbenchId = 0;
    task1.commands.push_back("buy");
    task1.workbench_x = 1.25;
    task1.workbench_y = 48.75;
    robots[0].tasks.push(task1);

    Task task3 = Task();
    task3.workbenchId = 20;
    task3.commands.push_back("sell");
    task3.workbench_x = 36.75;
    task3.workbench_y = 16.25;
    robots[0].tasks.push(task3);

    Task task2 = Task();
    task2.workbenchId = 1;
    task2.commands.push_back("buy");
    task2.workbench_x = 3.25;
    task2.workbench_y = 48.75;
    robots[1].tasks.push(task2);

    Task task4 = Task();
    task4.workbenchId = 20;
    task4.workbench_x = 36.75;
    task4.workbench_y = 16.25;
    task4.commands.push_back("sell");
    robots[1].tasks.push(task4);

}

void readWorkbenchsInfo() {

    if(frameID == 1) {
        initWorkbenchs();
    }
    // 读取工作台信息
        int tmp_workbenchType , tmp_remainProductTime , tmp_materialGridStatus , tmp_productGridStatus;
        double tmp_workBenchX , tmp_workBenchY;
        for(int i = 0; i < workBenchNum; i ++) {   
            scanf("%d %lf %lf %d %d %d" , &tmp_workbenchType , &tmp_workBenchX , &tmp_workBenchY , &tmp_remainProductTime , &tmp_materialGridStatus , &tmp_productGridStatus);
            workbenchs[i].workbenchType = tmp_workbenchType;
            workbenchs[i].x = tmp_workBenchX;
            workbenchs[i].y = tmp_workBenchY;
            workbenchs[i].remainProductTime = tmp_remainProductTime;
            workbenchs[i].materialGridStatus = tmp_materialGridStatus;
            workbenchs[i].productGridStatus = tmp_productGridStatus;
            workbenchs[i].workbenchId = i;
        }
}

void readRobotsInfo() {
    int workbenchId,itemType;
    double timeValue,collisionValue,angleSpeed,lineSpeed_x , lineSpeed_y,towards,x , y;
        // 再读取4个机器人信息
        for(int i = 0;i < 4;i ++) {
            scanf("%d %d %lf %lf %lf %lf %lf %lf %lf %lf", &workbenchId, &itemType, &timeValue, &collisionValue, &angleSpeed, &lineSpeed_x, &lineSpeed_y, &towards, &x , &y);
            robots[i].currentWorkbenchId = workbenchId;
            robots[i].itemType = itemType;
            robots[i].timeValue = timeValue;
            robots[i].collisionValue = collisionValue;
            robots[i].angleSpeed = angleSpeed;
            robots[i].v_x = lineSpeed_x;
            robots[i].v_y = lineSpeed_y;
            robots[i].towards = towards;
            robots[i].x = x;
            robots[i].y = y;  
        }

}

void readOK() {
    char line[1024];
    fgets(line, sizeof line, stdin);
    fgets(line, sizeof line, stdin);
}

int main() {

    // 初始化机器人
    initRobots();

    readMap();
    puts("OK");
    fflush(stdout);
    

    while (scanf("%d", &frameID) != EOF) {

        // 读取当前的金钱数
        scanf("%d", &money);
        // 读取当前的工作台数
        scanf("%d", &workBenchNum);   
        // 读取工作台信息
        readWorkbenchsInfo();
        // 读取机器人信息
        readRobotsInfo();
        // 读取OK 
        readOK();


        //---------------------------------  以上是读取数据，以下是输出信息  ---------------------------------//
        printf("%d\n", frameID);
        // 机器人执行任务
        for(int i = 0; i < 4; i++) {
           robots[i].doWork();
        }
        // 输出机器人的动作
        for(int i = 0; i < 4; i++) {
            robots[i].output();
        }
        // 输出OK
        printf("OK\n", frameID);
        fflush(stdout);

    }

    return 0;
}
