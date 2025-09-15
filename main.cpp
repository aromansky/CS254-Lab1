#include <iostream>
#include <unordered_map>
#include <stack>
#include <queue>
#include <tuple>
#include <chrono>

using namespace std;
using namespace std::chrono;

enum class OPERATION {
	NOT_OP,
	ADD3,
	MUL2,
	SUB2
};

const OPERATION OPS[] = { OPERATION::ADD3, OPERATION::MUL2, OPERATION::SUB2 };
const unsigned int ITERATIONS = 100;

struct my_map {
private:
	vector<OPERATION> vect;

public:
	my_map() {

	}

	my_map(int start, int fin) {
		this->vect = vector<OPERATION>(3 * (abs(fin) + 1) + 1);
	}

	void set_op(int num, OPERATION op) {
		this->vect[num + vect.size() / 2 + 1] = op;
	}

	OPERATION get_op(int num) {
		return this->vect[num + vect.size() / 2 + 1];
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
			int child;
			switch (ops[i]) {
			case OPERATION::ADD3: child = start + 3; break;
			case OPERATION::MUL2: child = start * 2; break;
			case OPERATION::SUB2: child = start - 2; break;
			}
			if (child != start && abs(child * 1.0 / fin) <= 1.5) {
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

		while (!q.empty()) {
			int parent = q.front();
			cnt_steps += 1;
			q.pop();

			for (size_t i = 0; i < ops_size; i++) {
				int child;
				switch (ops[i]) {
				case OPERATION::ADD3: child = parent + 3; break;
				case OPERATION::MUL2: child = parent * 2; break;
				case OPERATION::SUB2: child = parent - 2; break;
				}


				if (abs(child * 1.0 / fin) > 1.5 || child == parent) {
					continue;
				}


				if (map.get_op(child) == OPERATION::NOT_OP) {
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

void print_solve(int start, int fin, my_map map, int cnt_steps, microseconds time) {
	printf("---------------------------------------------------------------------\n");
	stack<OPERATION> res;

	if (map.get_op(fin) == OPERATION::NOT_OP) {
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

int main() {
	while (true) {
		int start, fin, cnt_ops;
		cout << "enter start num: ";
		cin >> start;
		

		cout << "enter fin num: ";
		cin >> fin;

		cout << "enter num of operations: ";
		cin >> cnt_ops;

		auto res = solve_straight(start, fin, OPS, cnt_ops);
		print_solve(start, fin, get<0>(res), get<1>(res), get<2>(res));

		if (start < 2) {
			break;
		}
	}


	return 0;
}