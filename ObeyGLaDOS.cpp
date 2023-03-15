// ObeyGLaDOS.cpp: 定义应用程序的入口点。
//

#include "ObeyGLaDOS.h"

using namespace std;

double coor::getDistance(coor c) {
	return (x - c.x) * (x - c.x) + (y - c.y) * (y - c.y);
};

// void Node::GetCost(Node n[MAX_ID], float sigma) {
// 	for (int i=0; i < MAX_ID; i++) {
// 		Cost[i] = Wait + c.getDistance(n[i].c)*sigma;
// 	}
// };

//int Node::GetJump(Node n, float sigma) {
//	Jump = c.getDistance(n.c);
//	return Jump;
//};

void Node::init(double x, double y, int Value){
	c.x = x;
	c.y = y;
	this->Value = Value;
}

void GLaDOS::distribute(task t) {
	this->q->push(t);
};

int GLaDOS::backward(int target_Node_ID) {
	for (int i = 0; i < target_Node_ID; i++) {
		if (static_Edge[target_Node_ID * MAX_ID + i] == 1) {
			return i;
		}
	}
	return target_Node_ID;
};

//void GLaDOS::clean_s() {
//	task pop_task;
//
//	while (!_s.empty()) {
//		if (available[_s.top().target_id] != 0)
//			break;
//		else
//		{
//			pop_task = _s.top();
//
//		}
//	}
//};

// 卖出后调用
void GLaDOS::feed_node(int Node_ID, int food) {
	dynamic_Edge[Node_ID * MAX_ID + food] = 0;
	dynamic_Edge[food * MAX_ID + Node_ID] = 0;
};

// 购买后调用
void GLaDOS::free_node(int Node_ID) {
	for (int i = 0; i < MAX_ID; i++) {
		dynamic_Edge[Node_ID * MAX_ID + i] = static_Edge[Node_ID * MAX_ID + i];
		dynamic_Edge[i * MAX_ID + Node_ID] = static_Edge[i * MAX_ID + Node_ID];
	}
};

// 每帧调用
void GLaDOS::update_state(int avl[MAX_ID], double atelas_coor[NUM_ATELAS * 3]) {
	freeze = 0.0;
	for (int i = 0; i < MAX_ID; i++) {
		available[i] = avl[i];
	}
	for (int i = 0; i < NUM_ATELAS; i++) {
		atelas[i].c.x = atelas_coor[i * 3];
		atelas[i].c.y = atelas_coor[i * 3 + 1];
		atelas[i].active = atelas_coor[i * 3 + 2];
		freeze += atelas[i].active;
	}
	generator();
};

void GLaDOS::generator() {
	float tmp_value, min_value = 9999.0, max_weight=-9999.0;
	int min_id=-1, max_target=-1;
	task t;

	// 先检查有没有制造好的物品，有则分配购买命令，并接上一条卖出命令
	while(freeze > 0.0)
		for (int i = MAX_ID-1; i >= 0; i--) {	// 生产机器编号：i
			// 初始化临时变量
			min_value = 9999.0, max_weight=-9999.0;
			min_id=-1, max_target=-1;

			if (freeze == 0.0)break;

			if (available[i] == 0) {
				for (int j = 0; j < NUM_ATELAS; j++) {	// 载货机器人编号：j
					if (atelas[j].active == 0.0)continue;
					tmp_value = atelas[j].c.getDistance(N[i].c);
					if (tmp_value < min_value) {
						min_value = tmp_value;
						min_id = j;
					}
				}
				if (min_id != -1) {
					freeze = freeze - 1.0;
					this->distribute(t = { i, min_id, 1 });
				}
				// 计算卖到哪里价值更高
				for (int x = i + 1; x < MAX_ID; x++) {
					if (dynamic_Edge[i * MAX_ID + i + x] == 0)continue;
					if (N[x].Value / N[x].c.getDistance(N[i].c) > max_weight) {
						max_weight = N[x].Value / N[x].c.getDistance(N[i].c);
						max_target = x;
					}
					if (max_target != -1)
						this->distribute(t = { x, min_id, -1 });
				}
			}
		}
	// 没有则跳过
};

GLaDOS::GLaDOS(target_queue* tq, double atelas_coor[NUM_ATELAS * 3], int edge[MAX_ID * MAX_ID], double generator_coor[MAX_ID * 2], int value[MAX_ID]) {
	this->q = tq;
	for (int i = 0; i < NUM_ATELAS; i++) {
		this->atelas[i].c.x = atelas_coor[i * 3];
		this->atelas[i].c.y = atelas_coor[i * 3 + 1];
		this->atelas[i].active = atelas_coor[i * 3 + 2];
	}
	for (int i = 0; i < MAX_ID*MAX_ID; i++) {
		this->static_Edge[i] = edge[i];
		this->dynamic_Edge[i] = edge[i];
	}
	for (int i = 0; i < MAX_ID; i++) {
		this->N[i].c.x = generator_coor[i * 2];
		this->N[i].c.y = generator_coor[i * 2 + 1];
		this->N[i].Value = value[i];
	}
};

// GLaDOS::GLaDOS() {
// 	q = NULL;
// 	for (int i = 0; i < NUM_ATELAS; i++) {
// 		atelas[i].c.x = 0.0;
// 		atelas[i].c.y = 0.0;
// 		atelas[i].active = 0;
// 	}
// 	for (int i = 0; i < MAX_ID*MAX_ID; i++) {
// 		static_Edge[i] = 0;
// 		dynamic_Edge[i] = 0;
// 	}
// 	// for (int i = 0; i < MAX_ID; i++) {
// 	// 	N[i].c.x = 0.0;
// 	// 	N[i].c.y = 0.0;
// 	// 	N[i].Value = 0;
// 	// }
// };