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

struct bidirectional_map {
	my_map forward_map;
	my_map backward_map;
	int meeting_point;

	bidirectional_map(int start, int fin) : forward_map(start, fin), backward_map(start, fin) {
		meeting_point = 0;
	}
};


tuple<my_map, unsigned int, microseconds> solve_straight(int start, int fin, const OPERATION ops[], size_t ops_size) {
	auto start_time = high_resolution_clock::now();
	my_map map;
	queue<int> q;
	unsigned int cnt_steps;

	for (unsigned int iter = 0; iter < ITERATIONS; iter++){
		cnt_steps = 0;
		map = my_map(start, fin);

		q.push(start);
		bool found = false;
		while (!q.empty() && !found) {
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
						found = true;
						break;
					}
				}
			}
		}

		while (!q.empty()) {
			q.pop();
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
		cnt_steps = 0;
		map = my_map(start, fin);
		
		q.push(fin);
		bool found = false;
		while (!q.empty() && !found) {
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
						found = true;
						break;
					}
				}
			}
		}

		while (!q.empty()) {
			q.pop();
		}

	}

	auto end_time = high_resolution_clock::now();
	auto computation_time = duration_cast<microseconds>(end_time - start_time) / ITERATIONS;

	return make_tuple(map, cnt_steps, computation_time);
}


tuple<bidirectional_map, unsigned int, microseconds> solve_bidirectional(int start, int fin, const OPERATION ops[], size_t ops_size) {
	auto start_time = high_resolution_clock::now();
	bidirectional_map map(start, fin);
	queue<int> forward_q, backward_q;
	unsigned int cnt_steps;

	for (unsigned int iter = 0; iter < ITERATIONS; iter++) {
		cnt_steps = 0;
		map = bidirectional_map(start, fin);
		bool found = false;

		forward_q.push(start);
		backward_q.push(fin);

		if (start == fin) {
			map.meeting_point = start;
			found = true;
		}

		while (!found && (!forward_q.empty() || !backward_q.empty())) {
			// с начала
			int parent = forward_q.front();
			cnt_steps += 1;
			forward_q.pop();

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

				if (map.backward_map.get_op(child) != OPERATION::NO_OP) {
					found = true;
					map.meeting_point = child;
					map.forward_map.set_op(child, map.backward_map.get_op(child));
					break;
				}

				if (map.forward_map.get_op(child) == OPERATION::NO_OP) {
					forward_q.push(child);
					map.forward_map.set_op(child, ops[i]);
				}
			}

			if (found) {
				break;
			}

			// с конца

			parent = backward_q.front();
			cnt_steps += 1;
			backward_q.pop();

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


				if (map.forward_map.get_op(child) != OPERATION::NO_OP) {
					map.meeting_point = child;
					found = true;
					map.backward_map.set_op(child, map.forward_map.get_op(child));
					break;
				}

				if (map.backward_map.get_op(child) == OPERATION::NO_OP) {
					backward_q.push(child);
					map.backward_map.set_op(child, ops[i]);
				}
			}
		}

		while (!forward_q.empty()) forward_q.pop();
		while (!backward_q.empty()) backward_q.pop();
	}

	auto end_time = high_resolution_clock::now();
	auto computation_time = duration_cast<microseconds>(end_time - start_time) / ITERATIONS;

	return make_tuple(map, cnt_steps, computation_time);
}


void print_solve_straight(int start, int fin, my_map map, int cnt_steps, microseconds time) {
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


void print_solve_bidirectional(int start, int fin, bidirectional_map map, int cnt_steps, microseconds time) {
	printf("---------------------------------------------------------------------\n");

	if (map.forward_map.get_op(map.meeting_point) == OPERATION::NO_OP &&
		map.backward_map.get_op(map.meeting_point) == OPERATION::NO_OP) {
		printf("no solution\n");
	}
	else {
		queue<OPERATION> q;
		int s = start;
		int meeting_point = map.meeting_point;

		while (s != meeting_point) {
			switch (map.forward_map.get_op(s)) {
			case OPERATION::ADD3: q.push(OPERATION::ADD3); s += 3; break;
			case OPERATION::MUL2: q.push(OPERATION::MUL2); s *= 2; break;
			case OPERATION::SUB2: q.push(OPERATION::SUB2); s -= 2; break;
			}
		}

		while (s != fin) {
			switch (map.backward_map.get_op(s)) {
			case OPERATION::ADD3: q.push(OPERATION::ADD3); s += 3; break;
			case OPERATION::MUL2: q.push(OPERATION::MUL2); s *= 2; break;
			case OPERATION::SUB2: q.push(OPERATION::SUB2); s -= 2; break;
			}
		}

		int step = 0;
		while (!q.empty()) {
			step++;
			switch (q.front())
			{
			case OPERATION::ADD3: printf("%2d. %d + 3 = %d\n", step, start, start + 3); start += 3; break;
			case OPERATION::MUL2: printf("%2d. %d * 2 = %d\n", step, start, start * 2); start *= 2; break;
			case OPERATION::SUB2: printf("%2d. %d - 2 = %d\n", step, start, start - 2); start -= 2; break;
			}
			q.pop();
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

		auto res = solve_bidirectional(start, fin, OPS, cnt_ops);
		print_solve_bidirectional(start, fin, get<0>(res), get<1>(res), get<2>(res));
	}


	return 0;
}