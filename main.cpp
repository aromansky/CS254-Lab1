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

struct my_map {
	sfs
};

void solve1(int start, int fin, OPERATION ops[], size_t ops_size) {
	auto start_time = high_resolution_clock::now();

	// в соответсвие числу ставится операция и кол-во шагов
	vector<OPERATION> map(abs(fin - start) + 1);
	queue<int> q;


	for (size_t i = 0; i < ops_size; i++) {
		int child;
		switch (ops[i]) {
		case OPERATION::ADD3: child = start + 3; break;
		case OPERATION::MUL2: child = start * 2; break;
		case OPERATION::SUB2: child = start - 2; break;
		}
		if (child != start && child <= fin) {
			q.push(child);
			map[abs(child - start) - 1] = ops[i];
		}
	}

	unsigned int cnt_steps = ops_size;
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


			if (child > fin || child == parent) {
				continue;
			}

			if (map[abs(child - start) - 1] == OPERATION::NOT_OP) {
				q.push(child);
				map[abs(child - start) - 1] = ops[i];
			}
			/*else {
				if (get<1>(map[parent]) + 1 < get<1>(map[child])) {
					map[child] = make_tuple(ops[i], get<1>(map[parent]) + 1);
				}
			}*/
		}
	}

	auto end_time = high_resolution_clock::now();
	auto computation_time = duration_cast<microseconds>(end_time - start_time);



	printf("---------------------------------------------------------------------\n");
	stack<OPERATION> res;

	if (map[abs(fin - start)] == OPERATION::NOT_OP) {
		printf("no solution\n");
	}
	else {
		while (map[abs(fin - start)] != OPERATION::NOT_OP) {
			switch (map[abs(fin - start)]) {
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
		printf("computation time: %.3f milliseconds\n", computation_time.count() / 1000.0);
	}
	printf("---------------------------------------------------------------------\n");
}

int main() {
	while (true) {
		int start, fin;
		cout << "enter start num: ";
		cin >> start;
		if (start == -1) {
			break;
		}

		cout << "enter fin num: ";
		cin >> fin;

		OPERATION ops[] = { OPERATION::ADD3, OPERATION::MUL2, OPERATION::SUB2 };
		solve1(start, fin, ops, 2);
	}


	return 0;
}