#include<list>
#include<iostream>
#include<cstdlib>
#include<tuple>
#include "main.h"

int genPID(std::list<std::pair<int, int>> processes) {
	int pid = 0;
	do {
		pid = (int)(rand() % 100000 + 1);	// arbitrarily large process id range chosen to be greater than 10,000 due to iterations done later
	} while (contains(pid, processes));  // repeat if the process already exists
	return pid;
}

bool contains(int pid, std::list<std::pair<int, int>> processes) {
	for (auto i = processes.begin(); i != processes.end(); i++){
		if (i->first == pid)
			return true;
		}
	return false;
}

int genMemoryAllocationSize() {
	return (int)(rand() % 8 + 3);	//assign a memory space between 3 and 10 for the process to take up
}

std::list<std::pair<int, int>>::iterator genProcess(std::list<std::pair<int, int>> *processes) {
	processes->push_back(std::make_pair<int, int>(genPID(*processes), genMemoryAllocationSize()));
	std::list<std::pair<int, int>>::iterator process_it = processes->begin();
	advance(process_it, (processes->size() - 1));
	return process_it;
}

int allocate(int algorithm_id, int next_fit_start_index, std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it) {
	int local_next_fit_start_index = next_fit_start_index;
	switch (algorithm_id) {
		case 1: allocate_1(memory, process_it); break;
		case 2: local_next_fit_start_index = allocate_2(next_fit_start_index, memory, process_it); break;
		case 3: allocate_3(memory, process_it); break;
		case 4: allocate_4(memory, process_it); break;
	}
	return local_next_fit_start_index;
}

// first fit
void allocate_1(std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it) {
	int p_id = process_it->first;
	int process_size = process_it->second;
	int segment_start = -1;
	int segment_length = 0;
	int index = 0;
	for (auto i = memory->begin(); i != memory->end(); i++) {
		if (*i == -1) {		// if the memory "block" is empty, start recording a segment
			if (segment_start == -1)
				segment_start = index;
			segment_length++;
			if (segment_length == process_size) {	//if the segment is a fit, allocate the memory and break
				std::list<int>::iterator memory_it = memory->begin();
				advance(memory_it, segment_start);
				for (int j = 0; j < process_size; j++) {
					*memory_it = p_id;
					advance(memory_it, 1);
				}
				break;
			}
		}
		else {
			segment_start = -1;
			segment_length = 0;
		}
		index++;
	}
}

// next fit
int allocate_2(int next_fit_start_index, std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it) {
	int local_next_fit_start_index = next_fit_start_index;
	int p_id = process_it->first;
	int process_size = process_it->second;
	int segment_start = -1;
	int segment_length = 0;
	int index = 0;
	std::list<int>::iterator i = memory->begin();
	advance(memory, local_next_fit_start_index);
	while (i != memory->end()){
		if (*i == -1) {		// if the memory "block" is empty, start recording a segment
			if (segment_start == -1)
				segment_start = index;
			segment_length++;
			if (segment_length == process_size) {	//if the segment is a fit, allocate the memory and break
				std::list<int>::iterator memory_it = memory->begin();
				advance(memory_it, segment_start);
				for (int j = 0; j < process_size; j++) {
					*memory_it = p_id;
					advance(memory_it, 1);
				}
				break;
			}
		}
		else {
			segment_start = -1;
			segment_length = 0;
		}
		index++;
		i++;
	}
	return local_next_fit_start_index;
}



// best fit
void allocate_3(std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it) {
	int p_id = process_it->first;
	int process_size = process_it->second;
	int segment_start = -1;
	int segment_length = 0;
	int index = 0;
	std::list<std::pair<int, int>> segment_list;				// <segment_start, segment_length>
	for (auto i = memory->begin(); i != memory->end(); i++) {
		if (*i == -1) {											// if the memory "block" is empty, start recording a segment
			if (segment_start == -1)
				segment_start = index;
			segment_length++;
			if (index == memory->size() - 1) {
				if(segment_length >= process_size)												// if the segment is large enough to be valid for the process size
					segment_list.push_back(std::make_pair(segment_start, segment_length));		// if we're at the last memory block, then store the segment
			}
		}
		else {																				// if the memory is already allocated...
			if (segment_start != -1) {														// and there was a segment beforehand...
				if(segment_length >= process_size)										    // and the segment is large enough to be valid for the process size
					segment_list.push_back(std::make_pair(segment_start, segment_length));  // store the segment block in the list of segments
			}
			segment_start = -1;
			segment_length = 0;
		}
		index++;
	}

	int best_segment_start = -1;													// initialize variables to keep track of the best segment
	int best_segment_length = -1;
	if(!segment_list.empty()){														// if we found good segments...
		for (auto i = segment_list.begin(); i != segment_list.end(); i++) {			// loop through the list...
			if (i->second < best_segment_length || best_segment_length == -1) {		// if this segment is a better fit than the last or best_segment_length hasn't been initialized yet...
				best_segment_length = i->second;									// note that this is the new best segment.
				best_segment_start = i->first;
			}
		}
		std::list<int>::iterator memory_it = memory->begin();
		advance(memory_it, best_segment_start);					// initialize the iterator to begin allocating
		for (int j = 0; j < process_size; j++) {
			*memory_it = p_id;									// then allocate the memory the appropriate length.
			advance(memory_it, 1);
		}
	}
}

// worst fit
void allocate_4(std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it) {
	int p_id = process_it->first;
	int process_size = process_it->second;
	int segment_start = -1;
	int segment_length = 0;
	int index = 0;
	std::list<std::pair<int, int>> segment_list; // <segment_start, segment_length>
	for (auto i = memory->begin(); i != memory->end(); i++) {
		if (*i == -1 && index != memory->size()) {		// if the memory "block" is empty, start recording a segment
			if (segment_start == -1)
				segment_start = index;
			segment_length++;
			if (index == memory->size() - 1) {
				if (segment_length >= process_size)												// if the segment is large enough to be valid for the process size
					segment_list.push_back(std::make_pair(segment_start, segment_length));		// if we're at the last memory block, then store the segment
			}
		}
		else {	// if the memory is already allocated...
			if (segment_start != -1) {	// and there was a segment beforehand...
				if (segment_length >= process_size)	// and the segment is large enough to be valid for the process size
					segment_list.push_back(std::make_pair(segment_start, segment_length)); // store the segment block in the list of segments
			}
			segment_start = -1;
			segment_length = 0;
		}
		index++;
	}

	int worst_segment_start = -1;
	int worst_segment_length = -1;
	if (!segment_list.empty()) {
		for (auto i = segment_list.begin(); i != segment_list.end(); i++) {				
			if (i->second > worst_segment_length || worst_segment_length == -1) {	// same process as best fit, except find the largest fit
				worst_segment_length = i->second;
				worst_segment_start = i->first;
			}
		}
		std::list<int>::iterator memory_it = memory->begin();
		advance(memory_it, worst_segment_start);
		for (int j = 0; j < process_size; j++) {				// iterate and allocate
			*memory_it = p_id;
			advance(memory_it, 1);
		}
	}
}

// clear the memory
void deallocate(std::list<int> *memory, std::list<std::pair<int, int>> *processes) {
	if (processes->size() > 0) {
		int selection = rand() % (processes->size());	//generate a random number between 0 and the size of the list - 1 to deallocate
		std::list<std::pair<int, int>>::iterator process_it = processes->begin();
		std::advance(process_it, selection);
		int process_id = process_it->first;
		for (auto i = memory->begin(); i != memory->end(); i++) {
			if (process_id == (int)*i) {
				*i = -1;
			}
		}
		processes->erase(process_it);
	}
}

// initializes a theoretical 256k memory block
void memoryInitialize(std::list<int> *memory) {
	for (int i = 0; i < 128; i++) {
		memory->push_back(-1);
	}
}

// resets the memory
void memoryReset(std::list<int> *memory) {
	for (auto i = memory->begin(); i != memory ->end(); i++) {
		*i = -1;
	}
}

int genRequest(int algorithm_id, int next_fit_start_index, std::list<int> *memory, std::list<std::pair<int, int>> *processes) {
	int choice = (int)(rand() % 2 + 1);
	std::list<std::pair<int, int>>::iterator process_it;
	switch (choice) {
		case 1	: process_it = genProcess(processes);
				  allocate(algorithm_id, next_fit_start_index, memory, process_it);
				  break;

		case 2	: deallocate(memory, processes);
				  break;

		default : std::cout << "An error occured." << std::endl;
	}
	return next_fit_start_index;
}

void printStatistics(std::list<int> memory) {
	std::list<int>::iterator i = memory.begin();
	while (i != memory.end()) {
		std::cout << *i << "\n";
		i++;
	}
}

int main() {
	std::list<int> memory;						// 256KB memory
	memoryInitialize(&memory);
	std::list<std::pair<int, int>> processes;	// list of processes and memory allocation size
	int next_fit_start_index = 0;
	std::cout << "Algorithm 1:\n";
	for (int i = 0; i < 10000; i++)
		next_fit_start_index = genRequest(1, next_fit_start_index, &memory, &processes);
	printStatistics(memory);
	std::cout << "===================================================================\n";
	memoryReset(&memory);
	processes.clear();

	std::cout << "Algorithm 2:\n";
	for (int i = 0; i < 10000; i++)
		next_fit_start_index = genRequest(2, next_fit_start_index, &memory, &processes);
	printStatistics(memory);
	std::cout << "===================================================================\n";
	memoryReset(&memory);
	processes.clear();

	std::cout << "Algorithm 3:\n";
	for (int i = 0; i < 10000; i++)
		next_fit_start_index = genRequest(3, next_fit_start_index, &memory, &processes);
	printStatistics(memory);
	std::cout << "===================================================================\n";
	memoryReset(&memory);
	processes.clear();

	std::cout << "Algorithm 4:\n";
	for (int i = 0; i < 10000; i++)
		next_fit_start_index = genRequest(4, next_fit_start_index, &memory, &processes);
	printStatistics(memory);
	std::cout << "===================================================================\n";
	memoryReset(&memory);
	processes.clear();


	std::cin.get();
	return 0;
}
