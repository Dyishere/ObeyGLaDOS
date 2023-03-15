// ObeyGLaDOS.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#ifndef OBEY_GLADOS_SRC
#define OBEY_GLADOS_SRC

#include <iostream>
#include <queue>
#include <tuple>
#include <stack>

#define task _task								// 修改为实际使用的任务结构体
#define MAX_ID 9								// 最大生产机器编号: 0 ~ MAX_ID-1
#define NUM_ATELAS 4							// 运输机器人数量
#define target_queue std::queue<_task>			// 任务队列，修改为实际使用的队列

struct _task {
	int target_id;	// 指定机器号
	int atelas;		// 指定机器人
	int action;		// 1购买、-1卖出
};

class coor {
public:
	double x;
	double y;
	double getDistance(coor c);
	coor(double x, double y) :x(x), y(y) {};
};

class Node {
public:
	int Value;				// 产生物品价值
	coor c;
	// int Jump;					// 与回收站之间的路程
	// int GetJump(Node n, float sigma);
	void init(double x, double y, int Value);
	Node():c(0, 0), Value(0) {};
};

class Atelas {
public:
	coor c;
	//coor target_c;			// 坐标
	double active;		// 激活状态: 0.0不可用，1.0可用
	Atelas() :c(0.0, 0.0), active(0.0) {};
};

target_queue t_queue;			// 任务队列

class GLaDOS {
public:
	//stack<task> _s;

	Node N[MAX_ID];
	int static_Edge[MAX_ID * MAX_ID];		
	int dynamic_Edge[MAX_ID * MAX_ID];	// 当工作台某个需要的物品已满足，则将对应边置0，未满足则置1
	int available[MAX_ID];				// 表明工作台状态：-1工作占用、0空闲、1生产完毕
	Atelas atelas[NUM_ATELAS];			// 存储运输机器人状态
	float freeze;
	target_queue* q;

	// 决策工具
	void distribute(task t);				// 分配任务	
	int backward(int target_Node_ID);		// 获取目标节点的前置条件

	// 状态更新
	//void clean_s();						// 清理暂存队列
	void feed_node(int Node_ID, int food);
	void free_node(int Node_ID);
	void update_state(int avl[MAX_ID], double atelas_coor[NUM_ATELAS * 3]);	// 更新生产机器状态和运载机器人坐标
	void generator();
	GLaDOS(target_queue* tq, double atelas_coor[NUM_ATELAS * 3], int edge[MAX_ID * MAX_ID], double generator_coor[MAX_ID*2], int value[MAX_ID]);
};

#endif // OBEY_GLADOS_SRC
// TODO: 在此处引用程序需要的其他标头。
