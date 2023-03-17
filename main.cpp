#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <string>
#include "Robot.h"
#include "Workbench.h"
#include "Task.h"
#include "ObeyGLaDOS.h"
// #include "ObeyGLaDOS.cpp"

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

    // // 下面是测试，将任务加入到任务队列当中
    // Task task1 = Task();
    // task1.workbenchId = 0;
    // task1.commands.push_back("buy");
    // task1.workbench_x = 1.25;
    // task1.workbench_y = 48.75;
    // robots[0].tasks.push(task1);

    // Task task3 = Task();
    // task3.workbenchId = 20;
    // task3.commands.push_back("sell");
    // task3.workbench_x = 36.75;
    // task3.workbench_y = 16.25;
    // robots[0].tasks.push(task3);

    // Task task2 = Task();
    // task2.workbenchId = 1;
    // task2.commands.push_back("buy");
    // task2.workbench_x = 3.25;
    // task2.workbench_y = 48.75;
    // robots[1].tasks.push(task2);

    // Task task4 = Task();
    // task4.workbenchId = 20;
    // task4.workbench_x = 36.75;
    // task4.workbench_y = 16.25;
    // task4.commands.push_back("sell");
    // robots[1].tasks.push(task4);

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
    int avl[MAX_Generator] = {-1};
    int type[MAX_Generator] = {-1};
    double robots_state[NUM_ATELAS*3];
    double generator_coor[MAX_Generator*2];
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
    int edges_for_generator[MAX_Generator*MAX_Generator] = {0};
    int v[MAX_ID] = {
        (6000-3000)*1, (7600-4400)*1, (9200-5800)*1, (22500-15400)*10, (25000-17200)*10, (27500-19200)*10, (105000-76000)*100, 1, 1
    };
    int v_for_generator[MAX_Generator] = {0};
    int Atelas_state[NUM_ATELAS*2] = {4, -1, 4, -1, 4, -1, 4, -1};  // 任务执行状态，上一次关联物品
    GLaDOS* G = NULL;

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
        
        if(frameID == 1) {
            for(int i=0;i<NUM_ATELAS;i++){
                robots_state[i*3] = robots[i].x;
                robots_state[i*3 + 1] = robots[i].y;
                robots_state[i*3 + 2] = robots[i].isWorking ? 0.0 : 1.0;
            }
            for(int i=0; i<MAX_Generator; i++){     // 第i个实例工作台
                type[i] = workbenchs[i].workbenchType;
                generator_coor[i*2] = workbenchs[i].x;
                generator_coor[i*2+1] = workbenchs[i].y;
                v_for_generator[i] = v[workbenchs[i].workbenchType-1];
                for(int j=0; j<MAX_Generator; j++){
                    edges_for_generator[i*MAX_Generator+j] = edges[(workbenchs[i].workbenchType-1)*MAX_ID + (workbenchs[j].workbenchType-1)];
                    // edges_for_generator[j*MAX_Generator+i] = -edges[workbenchs[i].workbenchType, workbenchs[j].workbenchType];
                }
            }
            G = new GLaDOS(&tq, robots_state, edges_for_generator, generator_coor, type, v_for_generator);
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
            for(int i = 0; i < MAX_Generator; i++){
                if(workbenchs[i].productGridStatus==1)avl[i] = 1;
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

        G->update_state(avl, robots_state);



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
