#ifndef _TASK_H
#define _TASK_H

#pragma once
#include <iostream>
#include <vector>
#include <cstring>


class Task {
public:
    int workbenchId;
    double workbench_x, workbench_y;
    std::vector<std::string> commands;
};

#endif