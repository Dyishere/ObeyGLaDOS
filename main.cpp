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




using namespace std;

bool readUntilOK() {

    char line[1024];
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }

    }
    return false;
}


FILE *fp;

void init(){

    // 初始化四个机器人
    for(int i = 0; i < 4; i++) {
        Robot robot =  Robot();
        robot.robotId = i;
        robot.targetWorkbenchId = -1;
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
    task2.workbenchId = 7;
    task2.commands.push_back("buy");
    task2.workbench_x = 48.75;
    task2.workbench_y = 48.75;
    robots[1].tasks.push(task2);

    Task task4 = Task();
    task4.workbenchId = 5;
    task4.workbench_x = 44.75;
    task4.workbench_y = 48.75;
    task4.commands.push_back("sell");
    robots[1].tasks.push(task4);

}

int main() {

    init();


    readUntilOK();
    puts("OK");
    fflush(stdout);

    int frameID;
    int money;
    int workBenchNum = 0;
    while (scanf("%d", &frameID) != EOF) {

        scanf("%d", &money);
        scanf("%d", &workBenchNum);
        // 如果工作台vector为空，那么说明是第一帧输入，需要初始化
        if(workbenchs.empty()) {

            for(int i = 0; i < workBenchNum; i++) {
                Workbench workbench = Workbench();
                workbench.workbenchId = i;
                workbenchs.push_back(workbench);
            }
        }
        
        char line[1024];
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

            // 输出读入的7个数据
            //cout << tmp_workbenchType << " " << tmp_workBenchX << " " << tmp_workBenchY << " " << tmp_remainProductTime << " " << tmp_materialGridStatus << " " << tmp_productGridStatus << endl;
        }

        //cout << "workbench over ......" << endl;


            int workbenchId;
            int itemType;
            double timeValue;
            double collisionValue;
            double angleSpeed;
            double lineSpeed_x , lineSpeed_y;
            double towards;
            double x , y;
        // 再读取4个机器人信息
        for(int i = 0;i < 4;i ++) {
            // 读取第一个机器人信息
            /*
            第一个数为所处工作台id
            携带物品类型
            时间价值系数
            碰撞价值系数
            角速度
            线速度
            朝向
            坐标
            */
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

                //cout << "i = " << i << " over ..." << endl;    
                // 将输入的十个数字都输出
                //cout << workbenchId << " " << itemType << " " << timeValue << " " << collisionValue << " " << angleSpeed << " " << lineSpeed_x << " " << lineSpeed_y << " " << towards << " " << x << " " << y << endl; 
        }

       
        // 读取OK 
        fgets(line, sizeof line, stdin);
        fgets(line, sizeof line, stdin);

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
