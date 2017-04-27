#include<list>
#ifndef _main_h
#define _main_h

int genPID(std::list<std::pair<int, int>> processes);
bool contains(int pid, std::list<std::pair<int, int>> processes);
int genMemoryAllocationSize();
std::list<std::pair<int, int>>::iterator genProcess(std::list<std::pair<int, int>> *processes);
int allocate(int algorithm_id, int next_fit_start_index, std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it);
void allocate_1(std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it);
int allocate_2(int next_fit_start_index, std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it);
void allocate_3(std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it);
void allocate_4(std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it);
void deallocate(std::list<int> *memory, std::list<std::pair<int, int>> *processes);
void memoryInitialize(std::list<int> *memory);
void memoryReset(std::list<int> *memory);
int genRequest(int algorithm_id, int next_fit_start_index, std::list<int> *memory, std::list<std::pair<int, int>> *processes);
void printStatistics(std::list<int> memory);

#endif
