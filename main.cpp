#include <iostream>
#include <unordered_map>
#include <stack>
#include <queue>
#include <tuple>
#include <chrono>

using namespace std;
using namespace std::chrono;

enum class OPERATION {
	NO_OP,
	ADD3,
	MUL2,
	SUB2
};

const OPERATION OPS[] = { OPERATION::ADD3, OPERATION::MUL2, OPERATION::SUB2 };
const unsigned int ITERATIONS = 100;
const  int DELTA = 5;

struct my_map {
private:
	vector<OPERATION> vect;
	int start, fin;

public:
	my_map() {
		start = fin = 0;
	}

	my_map(int start, int fin) {
		this->vect = vector<OPERATION>(abs(fin) + 1 + 2 * DELTA);
		this->start = start;
		this->fin = fin;
	}

	void set_op(int num, OPERATION op) {
		if (start < fin) {
			this->vect[num + DELTA] = op;
		}
		else {
			this->vect[-num + DELTA] = op;
		}
	}

	OPERATION get_op(int num) {
		if (start < fin) {
			return this->vect[num + DELTA];
		}
		else {
			return this->vect[-num + DELTA];
		}
	}
};


tuple<my_map, unsigned int, microseconds> solve_straight(int start, int fin, const OPERATION ops[], size_t ops_size) {
	auto start_time = high_resolution_clock::now();
	my_map map;
	queue<int> q;
	unsigned int cnt_steps;

	for (unsigned int iter = 0; iter < ITERATIONS; iter++){
		cnt_steps = 1;
		map = my_map(start, fin);
		for (size_t i = 0; i < ops_size; i++) {
			if (ops[i] == OPERATION::MUL2 && fin == 0) {
				continue;
			}

			int child;
			switch (ops[i]) {
			case OPERATION::ADD3: child = start + 3; break;
			case OPERATION::MUL2: child = start * 2; break;
			case OPERATION::SUB2: child = start - 2; break;
			}


			if (fin > start) {
				if (child < start - DELTA || child > fin + DELTA) {
					continue;
				}
			}
			else {
				if (child < fin - DELTA || child > start + DELTA) {
					continue;
				}
			}

			q.push(child);
			map.set_op(child, ops[i]);

			if (child == fin) {
				while (!q.empty()) {
					q.pop();
				}
				break;
			}
		}

		while (!q.empty()) {
			int parent = q.front();
			cnt_steps += 1;
			q.pop();

			for (size_t i = 0; i < ops_size; i++) {
				if (ops[i] == OPERATION::MUL2 && parent == 0) {
					continue;
				}

				int child;
				switch (ops[i]) {
				case OPERATION::ADD3: child = parent + 3; break;
				case OPERATION::MUL2: child = parent * 2; break;
				case OPERATION::SUB2: child = parent - 2; break;
				}


				if (fin > start) {
					if (child < start - DELTA || child > fin + DELTA) {
						continue;
					}
				}
				else {
					if (child < fin - DELTA || child > start + DELTA) {
						continue;
					}
				}

				if (map.get_op(child) == OPERATION::NO_OP) {
					q.push(child);
					map.set_op(child, ops[i]);

					if (child == fin) {
						while (!q.empty()) {
							q.pop();
						}
						break;
					}
				}
			}
		}
	}
	
	auto end_time = high_resolution_clock::now();
	auto computation_time = duration_cast<microseconds>(end_time - start_time) / ITERATIONS;

	return make_tuple(map, cnt_steps, computation_time);
}


tuple<my_map, unsigned int, microseconds> solve_reverse(int start, int fin, const OPERATION ops[], size_t ops_size) {
	auto start_time = high_resolution_clock::now();
	my_map map;
	queue<int> q;
	unsigned int cnt_steps;

	for (unsigned int iter = 0; iter < ITERATIONS; iter++) {
		cnt_steps = 1;
		map = my_map(start, fin);
		for (size_t i = 0; i < ops_size; i++) {
			if (ops[i] == OPERATION::MUL2 && (fin % 2 != 0 || fin == 0)) {
				continue;
			}

			int child;
			switch (ops[i]) {
			case OPERATION::ADD3: child = fin - 3; break;
			case OPERATION::MUL2: child = fin / 2; break;
			case OPERATION::SUB2: child = fin + 2; break;
			}

			if (fin > start) {
				if (child < start - DELTA || child > fin + DELTA) {
					continue;
				}
			}
			else {
				if (child < fin - DELTA || child > start + DELTA) {
					continue;
				}
			}

			q.push(child);
			map.set_op(child, ops[i]);

			if (child == start) {
				while (!q.empty()) {
					q.pop();
				}
				break;
			}
		}

		while (!q.empty()) {
			int parent = q.front();
			cnt_steps += 1;
			q.pop();

			for (size_t i = 0; i < ops_size; i++) {
				if (ops[i] == OPERATION::MUL2 && (parent % 2 != 0 || parent == 0)) {
					continue;
				}

				int child;
				switch (ops[i]) {
				case OPERATION::ADD3: child = parent - 3; break;
				case OPERATION::MUL2: child = parent / 2; break;
				case OPERATION::SUB2: child = parent + 2; break;
				}

				if (fin > start) {
					if (child < start - DELTA || child > fin + DELTA) {
						continue;
					}
				}
				else {
					if (child < fin - DELTA || child > start + DELTA) {
						continue;
					}
				}


				if (map.get_op(child) == OPERATION::NO_OP) {
					q.push(child);
					map.set_op(child, ops[i]);

					if (child == start) {
						while (!q.empty()) {
							q.pop();
						}
						break;
					}
				}
			}
		}
	}

	auto end_time = high_resolution_clock::now();
	auto computation_time = duration_cast<microseconds>(end_time - start_time) / ITERATIONS;

	return make_tuple(map, cnt_steps, computation_time);
}


void print_solve(int start, int fin, my_map map, int cnt_steps, microseconds time) {
	printf("---------------------------------------------------------------------\n");
	stack<OPERATION> res;

	if (map.get_op(fin) == OPERATION::NO_OP) {
		printf("no solution\n");
	}
	else {
		while (fin != start) {
			switch (map.get_op(fin)) {
			case OPERATION::ADD3: res.push(OPERATION::ADD3); fin -= 3; break;
			case OPERATION::MUL2: res.push(OPERATION::MUL2); fin /= 2; break;
			case OPERATION::SUB2: res.push(OPERATION::SUB2); fin += 2; break;
			}
		}

		int step = 0;
		while (!res.empty()) {
			step++;
			switch (res.top())
			{
			case OPERATION::ADD3: printf("%2d. %d + 3 = %d\n", step, fin, fin + 3); fin += 3; break;
			case OPERATION::MUL2: printf("%2d. %d * 2 = %d\n", step, fin, fin * 2); fin *= 2; break;
			case OPERATION::SUB2: printf("%2d. %d - 2 = %d\n", step, fin, fin - 2); fin -= 2; break;
			}
			res.pop();
		}
		printf("\nsolution length: %d; nodes considered: %d;\n", step, cnt_steps);
		printf("computation time: %.3f milliseconds\n", time.count() / 1000.0);
	}
	printf("---------------------------------------------------------------------\n");
}

void print_solve_reverse(int start, int fin, my_map map, int cnt_steps, microseconds time) {
	printf("---------------------------------------------------------------------\n");
	if (map.get_op(start) == OPERATION::NO_OP) {
		printf("no solution\n");
	}
	else {
		int step = 0;
		while (start != fin) {
			step++;
			switch (map.get_op(start)) {
			case OPERATION::ADD3: printf("%2d. %d + 3 = %d\n", step, start, start + 3);; start += 3; break;
			case OPERATION::MUL2: printf("%2d. %d * 2 = %d\n", step, start, start * 2);; start *= 2; break;
			case OPERATION::SUB2: printf("%2d. %d - 2 = %d\n", step, start, start - 2);; start -= 2;; break;
			}
		}
		printf("\nsolution length: %d; nodes considered: %d;\n", step, cnt_steps);
		printf("computation time: %.3f milliseconds\n", time.count() / 1000.0);
	}
	printf("---------------------------------------------------------------------\n");
}

int main() {
	while (true) {
		int start, fin, cnt_ops;
		cout << "enter start num: ";
		cin >> start;
		

		cout << "enter fin num: ";
		cin >> fin;

		cout << "enter num of operations: ";
		cin >> cnt_ops;

		if (cnt_ops < 2) {
			break;
		}

		auto res = solve_reverse(start, fin, OPS, cnt_ops);
		print_solve_reverse(start, fin, get<0>(res), get<1>(res), get<2>(res));
	}


	return 0;
}