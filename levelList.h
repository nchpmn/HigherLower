// levelList.h

#ifndef LEVEL_LIST_H
#define LEVEL_LIST_H

// Create the struct levelList, with internal variables declared
struct levelList {
    const char* name;
    int attempts;
};

// Declare the levels array
extern levelList levels[];

#endif