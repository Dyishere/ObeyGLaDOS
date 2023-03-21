#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <string>
#include "Robot.h"
#include "Workbench.h"
#include "Task.h"
#include "ObeyGLaDOS.h"
//#include "ObeyGLaDOS.cpp"

using namespace std;

vector<Workbench> workbenchs;
vector<Robot> robots;
target_queue tq;

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


double calDist(double x1 , double y1 , double x2 , double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double getDegAngle2d(double forward_x, double forward_y, double target_x, double target_y)
{
    double theta = atan2(target_y, target_x) - atan2(forward_y, forward_x); //弧度
    if (theta > M_PI)
    {
        theta -= 2*M_PI;
    }
    if (theta < -M_PI)
    {
        theta += 2*M_PI;
    }
    return abs(theta * 180/M_PI); //角度
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
        robot.workbenches = &workbenchs;
        robots.push_back(robot); 
    }


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
            //cout << "workbenchType: " << workbenchs[i].workbenchType << endl;
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
    int avl[Num_generator] = {-1};
    int type[Num_generator] = {-1};
    double robots_state[NUM_ATELAS*3];
    double generator_coor[Num_generator*2];
    int edges[MAX_ID*MAX_ID] = {
        0, 0, 0, 1, 1, 0, 0, 0, 1,  // 1
        0, 0, 0, 1, 0, 1, 0, 0, 1,  // 2
        0, 0, 0, 0, 1, 1, 0, 0, 1,  // 3
        -1, -1, 0, 0, 0, 0, 1, 0, 1,  // 4
        -1, 0, -1, 0, 0, 0, 1, 0, 1,  // 5
        0, -1, -1, 0, 0, 0, 1, 0, 1,  // 6
        0, 0, 0, -1, -1, -1, 0, 1, 1,  // 7
        0, 0, 0, 0, 0, 0, -1, 0, 0,  // 8
        -1, -1, -1, -1, -1, -1, -1, 0, 0   // 9
    };
    int edges_for_generator[Num_generator*Num_generator] = {0};
    // int v[MAX_ID] = {
    //     (6000-3000)*1 , (7600-4400)*1, (9200-5800)*1, (22500-15400)*10, (25000-17200)*10, (27500-19200)*10, (105000-76000)*100, 1, 1
    // };
    int v[MAX_ID] = {
        (6000-3000)*1 , (7600-4400)*1, (9200-5800)*1, (22500-15400)*1, (25000-17200)*1, (27500-19200)*1, (105000-76000)*1, 1, 1
    };
    int v_for_generator[Num_generator] = {0};
    int Atelas_state[NUM_ATELAS*2] = {4, -1, 4, -1, 4, -1, 4, -1};  // 任务执行状态，上一次关联物品
    GLaDOS* G = NULL;



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

        if(frameID ==1) {
            initRobots();
        }

        // 读取机器人信息
        readRobotsInfo();
        // 读取OK 
        readOK();
        
        if(frameID == 1) {



            for(int i=0;i<NUM_ATELAS;i++){
                robots_state[i*3] = robots[i].x;
                robots_state[i*3 + 1] = robots[i].y;
                robots_state[i*3 + 2] = robots[i].isWorking ? 0.0 : 1.0;
            }
            for(int i=0; i<workBenchNum; i++){     // 第i个实例工作台
           
                type[i] = workbenchs[i].workbenchType;
                generator_coor[i*2] = workbenchs[i].x;
                generator_coor[i*2+1] = workbenchs[i].y;
                v_for_generator[i] = v[workbenchs[i].workbenchType-1];
                for(int j=0; j<workBenchNum; j++){
                
                    edges_for_generator[i*Num_generator+j] = edges[(workbenchs[i].workbenchType-1)*MAX_ID + (workbenchs[j].workbenchType-1)];
                    // edges_for_generator[j*Num_generator+i] = -edges[workbenchs[i].workbenchType, workbenchs[j].workbenchType];
                }
            }
            G = new GLaDOS(&tq, robots_state, edges_for_generator, generator_coor, type, v_for_generator, workBenchNum);
        }


        //---------------------------------  以上是读取数据，以下是输出信息  ---------------------------------//
        printf("%d\n", frameID);
        // 机器人执行任务
        for(int i = 0; i < 4; i++) {
           Atelas_state[i*2] = robots[i].doWork();
           Atelas_state[i*2+1] = robots[i].lastworkbenchId;
        }




        // 输出机器人的动作
        for(int i = 0; i < 4; i++) {
            robots[i].output();
        }
        // 输出OK
        printf("OK\n", frameID);
        fflush(stdout);

        // GLaDOS! Help me!
        for (int i = 0; i < NUM_ATELAS; i++)
        {
            /*
            此方法会自动从任务队列中取出要执行的任务并完成。
            返回值:
            0 代表正在执行一个任务
            1 代表已经完成一个buy任务
            2 代表已经完成一个sell任务
            3 代表已经完成一个des任务
            4 代表当前空闲了,没有要执行的任务
            */
            for(int i = 0; i < workBenchNum; i++){
                if(workbenchs[i].productGridStatus==1)avl[i] = 1;
                else if(workbenchs[i].remainProductTime != -1)avl[i] = 2;
                else avl[i] = 0;
            }
            // 遍历每一个机器人的状态，更新GLaDOS的状态
            switch (Atelas_state[i*2])
            {
            case 0:
                continue;
            case 1:
                G->free_node(Atelas_state[i*2+1]);
            case 2:
                //G->feed_node(Atelas_state[i*2+1], robots[i].lastworkbenchId);
                continue;
            default:
                continue;
            }
        }

        for(int i=0;i<NUM_ATELAS;i++){
            robots_state[i*3] = robots[i].x;
            robots_state[i*3 + 1] = robots[i].y;
            robots_state[i*3 + 2] = robots[i].isWorking ? 0.0 : 1.0;
        }

        G->update_state(avl, robots_state, v_for_generator);



        // 遍历tq
        while(tq.size()) {
            auto tmpFront = tq.front();
            tq.pop();

            Task t = Task();

            t.workbenchId = tmpFront.target_id;
            t.workbench_x = workbenchs[t.workbenchId].x;
            t.workbench_y = workbenchs[t.workbenchId].y;
            if(tmpFront.action == 1) {
                t.commands.push_back("buy");
               
            }else{
                t.commands.push_back("sell");
            }
            robots[tmpFront.atelas].tasks.push(t);
        }


        
    }

    return 0;
}
