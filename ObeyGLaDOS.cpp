// ObeyGLaDOS.cpp: 定义应用程序的入口点。
//

#include "ObeyGLaDOS.h"

using namespace std;

double coor::getDistance(coor c) {
	return (x - c.x) * (x - c.x) + (y - c.y) * (y - c.y);
};

// void Node::GetCost(Node n[MAX_Generator], float sigma) {
// 	for (int i=0; i < MAX_Generator; i++) {
// 		Cost[i] = Wait + c.getDistance(n[i].c)*sigma;
// 	}
// };

//int Node::GetJump(Node n, float sigma) {
//	Jump = c.getDistance(n.c);
//	return Jump;
//};

void Node::init(double x, double y, int Value, int Type){
	c.x = x;
	c.y = y;
	this->Value = Value;
	this->Type = Type;
}

int GLaDOS::get_valueable_generator(coor atelas_coor){
	int sell_available = 0;
	float max = -100000.0;
	int max_id = -1;
	for (int i = 0; i < MAX_Generator; i++) {
		if(available[i] != 1)continue;
		sell_available = 0;
		for(int j = 0; j < MAX_Generator; j++){
			if(dynamic_Edge[i*Num_generator+j] == 1){
				sell_available = 1;
				break;
			}
		}

		if(sell_available == 1){
			if (N[i].Value / N[i].c.getDistance(atelas_coor) > max) {
				max = N[i].Value / N[i].c.getDistance(atelas_coor);
				max_id = i;
			}
		}
	}

	return max_id;
}

int GLaDOS::get_nearest_node(int generator_id){
	float max = -100000.0;
	int max_id = -1;
	// for(int i = 0; i<MAX_Generator; i++){
	// 	cout << dynamic_Edge[generator_id * MAX_Generator + i];
	// }
	// cout << endl;

	for (int i = 0; i < MAX_Generator; i++) {
		if (dynamic_Edge[generator_id * Num_generator + i] == 1) {
			if (N[i].Value / N[i].c.getDistance(N[generator_id].c) > max) {
				max = N[i].Value / N[i].c.getDistance(N[generator_id].c);
				max_id = i;
			}
		}
	}
	return max_id;
};

void GLaDOS::distribute(task t) {
	this->q->push(t);
};

void GLaDOS::command_sell(int target_id, int atelas_id, int food_id) {
	task t = { target_id, atelas_id, -1 };
	this->distribute(t);
	this->feed_node(target_id, food_id);
};

void GLaDOS::command_buy(int target_id, int atelas_id) {
	task t = { target_id, atelas_id, 1 };
	this->distribute(t);
	this->atelas[atelas_id].active = 0.0;
	this->atelas[atelas_id].target = target_id;				// 将Atelas的目标设置为生产机器
	this->available[target_id] = -1;					// 将生产机器的状态设置为不可用
};

int GLaDOS::Maximun_Value_Search() {
	// success: 返回当前可用最高价值生产机器的最优前置条件
	// failed: 时返回-1
	//cout << "Maximun_Value_Search" << endl;

	int target = -1;
	float tmp_value = -99999.0;
	
	for (int i = 0; i < MAX_Generator; i++) {
		if(this->available[i] == -1 || this->available[i] == 2 || this->hungry(i) == false)continue;
		if (this->N[i].Value > tmp_value) {
			tmp_value = N[i].Value;
			target = i;
		}
	}

	if (target != -1) {
		if (available[target] == 1)
			return target;
		else	// available[target] == 0时，进行前置条件搜索
			target = this->backward(target);	// 对得到的target进行前置条件搜索
	}

	return target;
};

int GLaDOS::backward(int target_Node_ID) {
	//cout << "backward" << endl;
	if(N[target_Node_ID].Type <= 3 || available[target_Node_ID] == 1)return target_Node_ID;

	float min_cost = 99999.0;

	for (int i = 0; i < MAX_Generator; i++) {
		if(available[i] == -1 || this->hungry(i) == false)continue;
		if (this->dynamic_Edge[target_Node_ID * Num_generator + i] == -1 && N[target_Node_ID].c.getDistance(N[i].c) < min_cost) {
			min_cost = N[target_Node_ID].c.getDistance(N[i].c);
			target_Node_ID = i;
		}
	}
	if(min_cost != 99999.0){
		if (available[target_Node_ID] == 1)
			return target_Node_ID;
		else if(available[target_Node_ID] == 0)
			target_Node_ID = this->backward(target_Node_ID);
	}

	if(available[target_Node_ID] == 1)return target_Node_ID;
	else return -1;
};

bool GLaDOS::hungry(int Node_ID){
	if(N[Node_ID].Type <= 3)return true;
	for(int i = 0; i < MAX_Generator; i++){
		if(dynamic_Edge[Node_ID * Num_generator + i] == -1)return true;
	}
	return false;
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
	if(N[Node_ID].Type == recycle_bin)return;
	for (int i = 0; i < MAX_Generator; i++) {
		if (N[i].Type == N[food].Type) {
			dynamic_Edge[Node_ID * Num_generator + i] = 0;
			dynamic_Edge[i * Num_generator + Node_ID] = 0;
		}
	}
};

// 购买后调用
void GLaDOS::free_node(int Node_ID) {
	available[Node_ID] = 0;
	for (int i = 0; i < MAX_Generator; i++) {
		if(static_Edge[Node_ID*Num_generator+i] == -1){
			dynamic_Edge[Node_ID * Num_generator + i] = -1;
			dynamic_Edge[i * Num_generator + Node_ID] = 1;
		}
	}
};

// 每帧调用
void GLaDOS::update_state(int avl[Num_generator], double atelas_coor[NUM_ATELAS * 3], int static_value[Num_generator]) {
	// 更新载货机器人状态
	freeze = 0.0;
	for (int i = 0; i < NUM_ATELAS; i++) {
		atelas[i].c.x = atelas_coor[i * 3];
		atelas[i].c.y = atelas_coor[i * 3 + 1];
		atelas[i].active = atelas_coor[i * 3 + 2];
		if(atelas[i].active == 1.0)
			atelas[i].target = -1;
		freeze += atelas[i].active;
	}

	// 更新节点状态
	for (int i = 0; i < MAX_Generator; i++) {
		available[i] = avl[i];
	}
	for(int i = 0; i<NUM_ATELAS; i++){
		if(atelas[i].target != -1)		// 若有正在运送的货物，则将其目标设置为不可用
			available[atelas[i].target] = -1;
	}
	for (int i = 0; i < MAX_Generator; i++) {
		for(int j = 0; j < MAX_Generator; j++)
			if(dynamic_Edge[i*MAX_Generator+j] == -1)
				this->N[i].Value += static_value[j]*dynamic_Edge[i*MAX_Generator+j];
	}

	// 调用决策
	generator();
};

void GLaDOS::generator() {
	// 
	for (int i = 0; i < NUM_ATELAS; i++) {	// 运载机器编号：i
		if (freeze <= 0.0)break;		// 检查是否还有可用机器人
		else if(atelas[i].active == 1.0){
			// for (int j = 0; j < NUM_ATELAS; j++) {	// 载货机器人编号：j
			// 	if (atelas[j].active == 0.0)continue;
			// 	tmp_value = atelas[j].c.getDistance(N[i].c);
			// 	if (tmp_value < min_value) {
			// 		min_value = tmp_value;
			// 		min_id = j;
			// 	}
			// }

			// 单次循环得到：购买目标机器，对应出售目标机器
			int max_id = this->Maximun_Value_Search();
			// cout << "Max_id: "	<< max_id << endl;
			if (max_id != -1) {

				// 计算卖到哪里价值更高
				// max_target=-1, max_weight=-9999.0;

				// for (int x = 0; x < MAX_Generator; x++) {
				// 	if (dynamic_Edge[i * MAX_Generator + x] < 1)continue;
				// 	if (N[x].Value / N[x].c.getDistance(N[i].c) > max_weight) {
				// 		max_weight = N[x].Value / N[x].c.getDistance(N[i].c);
				// 		max_target = x;
				// 	}
				// }
				// if (max_target != -1){
				// 	this->distribute(t = { max_target, min_id, -1 });
				// 	this->feed_node(i, max_target);
				// }
				int max_target = this->get_nearest_node(max_id);
				// cout << "Max_target: "	<< max_target << endl;
				if(max_target != -1){
					freeze = freeze - 1.0;
					// 分配购买指令: 
					// // target_id: max_id
					// // atelas_id: i
					// // action: buy
					this->command_buy(max_id, i);

					// 分配卖出指令: 
					// // target_id: max_target
					// // atelas_id: i
					// // food_id: max_id
					// // action: sell
					this->command_sell(max_target, i, max_id);
				}
			}
		}
	}
	// 没有则跳过
};

GLaDOS::GLaDOS(target_queue* tq, double atelas_coor[NUM_ATELAS * 3], int edge[Num_generator * Num_generator], double generator_coor[Num_generator * 2], int Type[Num_generator], int value[Num_generator], int max_generator) {
	this->q = tq;
	this->MAX_Generator = max_generator;
	for (int i = 0; i < NUM_ATELAS; i++) {
		this->atelas[i].c.x = atelas_coor[i * 3];
		this->atelas[i].c.y = atelas_coor[i * 3 + 1];
		this->atelas[i].active = atelas_coor[i * 3 + 2];
	}
	for (int i = 0; i < Num_generator*Num_generator; i++) {
		this->static_Edge[i] = edge[i];
		this->dynamic_Edge[i] = edge[i];
	}
	for (int i = 0; i < MAX_Generator; i++) {
		this->N[i].Type = Type[i];
		this->N[i].c.x = generator_coor[i * 2];
		this->N[i].c.y = generator_coor[i * 2 + 1];
		this->N[i].Value = value[i];
		for(int j = 0; j < MAX_Generator; j++)
			if(dynamic_Edge[i*MAX_Generator+j] == -1)
				this->N[i].Value += value[j]*dynamic_Edge[i*MAX_Generator+j];
	}
};

// GLaDOS::GLaDOS() {
// 	q = NULL;
// 	for (int i = 0; i < NUM_ATELAS; i++) {
// 		atelas[i].c.x = 0.0;
// 		atelas[i].c.y = 0.0;
// 		atelas[i].active = 0;
// 	}
// 	for (int i = 0; i < MAX_Generator*MAX_Generator; i++) {
// 		static_Edge[i] = 0;
// 		dynamic_Edge[i] = 0;
// 	}
// 	// for (int i = 0; i < MAX_Generator; i++) {
// 	// 	N[i].c.x = 0.0;
// 	// 	N[i].c.y = 0.0;
// 	// 	N[i].Value = 0;
// 	// }
// };