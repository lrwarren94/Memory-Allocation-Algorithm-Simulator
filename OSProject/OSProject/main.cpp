#include<list>
#include<iostream>
#include<cstdlib>
#include<tuple>

int genPID(std::list<std::pair<int, int>> processes) {
	int pid = 0;
	do {
		pid = (int)(rand() % 100000 + 1);	// arbitrarily large process id range chosen to be greater than 10,000 due to iterations done later
	} while (contains(pid, processes));  // repeat if the process already exists
	return pid;
}

bool contains(int pid, std::list<std::pair<int, int>> processes) {
	for (auto i = processes.begin(); i != processes.end(); i++)
		if (i->first == pid)
			return true;
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
					*memory_it == p_id;
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
	std::list<int>::iterator 
	for (auto i = memory->begin(); i != memory->end(); i++) {
		if (*i == -1) {		// if the memory "block" is empty, start recording a segment
			if (segment_start == -1)
				segment_start = index;
			segment_length++;
			if (segment_length == process_size) {	//if the segment is a fit, allocate the memory and break
				std::list<int>::iterator memory_it = memory->begin();
				advance(memory_it, segment_start);
				for (int j = 0; j < process_size; j++) {
					*memory_it == p_id;
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
	return local_next_fit_start_index;
}

void allocate_3(std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it) {

}

void allocate_4(std::list<int> *memory, std::list<std::pair<int, int>>::iterator process_it) {

}

void deallocate(std::list<int> *memory, std::list<std::pair<int, int>> *processes) {
	if (processes->size() > 0) {
		int selection = rand() % (processes->size() - 1);	//generate a random number between 0 and the size of the list - 1 to deallocate
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

void memoryInitialize(std::list<int> *memory) {
	for (int i = 1; i <= 128; i++) {
		memory->push_back(-1);
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


int main() {
	std::list<int> memory;						// 256KB memory
	memoryInitialize(&memory);
	std::list<std::pair<int, int>> processes;	// list of processes and memory allocation size
	int next_fit_start_index = 0;
	next_fit_start_index = genRequest(2, next_fit_start_index, &memory, &processes);

	
	return 0;
}
