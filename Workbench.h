#ifndef _WORKBENCH_H
#define _WORKBENCH_H

#pragma once

class Workbench {

    public:
    int workbenchId;
    // 工作台类型，范围[1 , 9]
    int workbenchType;
    // 该工作台坐标
    double x , y;
    // 剩余生产时间，单位为帧数
    int remainProductTime;
    // 原材料格状态
    //二进制位表描述，例如 48(110000)表示拥有物品 4 和 5。
    int materialGridStatus;
    // 产品格状态 0无1有
    int productGridStatus;

};

#endif
