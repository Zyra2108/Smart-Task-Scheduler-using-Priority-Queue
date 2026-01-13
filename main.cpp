#include <bits/stdc++.h>
using namespace std;

class Task {
public:
    int task_id;
    int urgency;
    int deadline;
    int exec_time;
    int created_at;
    double priority;

    Task(int id, int u, int d, int e, int c)
        : task_id(id), urgency(u), deadline(d),
          exec_time(e), created_at(c), priority(0) {}

    string repr() const {
        stringstream ss;
        ss << "Task(id=" << task_id << ", priority=" 
           << fixed << setprecision(2) << priority << ")";
        return ss.str();
    }
};

class MinHeap {
public:
    vector<Task*> data;

    void push(Task* item) {
        data.push_back(item);
        heapify_up(data.size() - 1);
    }

    Task* pop() {
        if (data.empty()) return nullptr;
        swap(data[0], data.back());
        Task* item = data.back();
        data.pop_back();
        heapify_down(0);
        return item;
    }

private:
    void heapify_up(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (data[i]->priority < data[p]->priority) {
                swap(data[i], data[p]);
                i = p;
            } else break;
        }
    }

    void heapify_down(int i) {
        int smallest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        if (l < data.size() && data[l]->priority < data[smallest]->priority)
            smallest = l;
        if (r < data.size() && data[r]->priority < data[smallest]->priority)
            smallest = r;

        if (smallest != i) {
            swap(data[i], data[smallest]);
            heapify_down(smallest);
        }
    }
};

double calculate_priority(Task* task, int current_time) {
    return (
        task->urgency * 10.0
        + (current_time - task->created_at) * 1.5
        - max(0, task->deadline - current_time) * 2.0
        - task->exec_time * 1.2
    );
}

class Scheduler {
public:
    MinHeap heap;
    int time = 0;

    void add_task(Task* task) {
        task->priority = calculate_priority(task, time);
        heap.push(task);
    }

    void run() {
        cout << "\n--- Task Execution Order ---\n\n";
        if (heap.data.empty()) {
            cout << "No tasks to execute.\n";
            return;
        }

        while (true) {
            Task* task = heap.pop();
            if (!task) break;
            time += task->exec_time;
            cout << "[Time " << time << "] Executing "
                 << task->repr() << "\n";
        }
    }
};

/* =========================
   Input Utilities
   ========================= */

int read_positive_int(string prompt, int min_val = 1, int max_val = -1) {
    int val;
    while (true) {
        cout << prompt;
        cin >> val;
        if (cin.fail() || val < min_val || (max_val != -1 && val > max_val)) {
            cout << "Invalid input. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        } else {
            return val;
        }
    }
}

int main() {
    Scheduler scheduler;

    int n = read_positive_int("Enter number of tasks: ");

    for (int i = 0; i < n; i++) {
        cout << "\nTask " << i + 1 << "\n";
        int urgency = read_positive_int("Urgency (1–10): ", 1, 10);
        int deadline = read_positive_int("Deadline (time units): ");
        int exec_time = read_positive_int("Execution time: ");

        Task* task = new Task(
            i + 1, urgency, deadline, exec_time, scheduler.time
        );
        scheduler.add_task(task);
    }

    scheduler.run();
    return 0;
}
