// hospital_queue.cpp
// Hospital Patient Queue Management System
// Build: g++ -std=c++17 hospital_queue.cpp -O2 -o hospital_queue
// Run: ./hospital_queue

#include <bits/stdc++.h>
using namespace std;

struct Patient {
    string id;        // unique id (or name)
    string name;
    int severity;     // 1 (low) - 5 (critical)
    int arrivalOrder; // incremental arrival counter
    int arrivalTime;  // simulated time (int) — can be arrivalOrder for simplicity
};

// Priority comparator: higher severity first. If equal, earlier arrival first.
struct PriorityCompare {
    bool operator()(const Patient &a, const Patient &b) const {
        if (a.severity != b.severity) return a.severity < b.severity;
        return a.arrivalOrder > b.arrivalOrder;
    }
};

class HospitalQueueSystem {
private:
    queue<Patient> normalQueue;
    priority_queue<Patient, vector<Patient>, PriorityCompare> priorityQueue;
    unordered_map<string, Patient> record; // quick lookup by id
    int arrivalCounter = 0; // increments with each new patient

    // For waiting time simulation (we'll track time when treated)
    long long totalWaitNormal = 0;
    long long totalWaitPriority = 0;
    int treatedNormalCount = 0;
    int treatedPriorityCount = 0;

public:
    void addPatient(const string &id, const string &name, int severity) {
        Patient p;
        p.id = id;
        p.name = name;
        p.severity = severity;
        p.arrivalOrder = ++arrivalCounter;
        p.arrivalTime = p.arrivalOrder; // simple simulated timestamp
        // push to both queues
        normalQueue.push(p);
        priorityQueue.push(p);
        record[id] = p;
        cout << "Added patient: " << name << " (ID: " << id << ", severity: " << severity << ")\n";
    }

    void treatNextNormal() {
        if (normalQueue.empty()) {
            cout << "Normal queue is empty.\n";
            return;
        }
        Patient p = normalQueue.front(); normalQueue.pop();
        // We must also remove this patient from priority queue — lazy deletion approach:
        // We'll mark as treated in record and skip when popping from priority later.
        if (record.find(p.id) != record.end()) {
            // compute wait = currentTime - arrivalTime
            int currentTime = ++arrivalCounter; // advance time for simulation
            totalWaitNormal += (currentTime - p.arrivalTime);
            treatedNormalCount++;
            cout << "Treated (FIFO): " << p.name << " (ID: " << p.id << ", severity: " << p.severity << ")\n";
            record.erase(p.id);
        } else {
            // Already treated via priority earlier; skip and recurse
            treatNextNormal();
        }
        // Clean priority queue top if needed
        cleanPriorityTop();
    }

    void treatNextPriority() {
        if (priorityQueue.empty()) {
            cout << "Priority queue is empty.\n";
            return;
        }
        Patient p = priorityQueue.top(); priorityQueue.pop();
        // skip if already treated (not in record)
        while (record.find(p.id) == record.end()) {
            if (priorityQueue.empty()) {
                cout << "Priority queue is empty.\n";
                return;
            }
            p = priorityQueue.top(); priorityQueue.pop();
        }
        int currentTime = ++arrivalCounter; // advance time
        totalWaitPriority += (currentTime - p.arrivalTime);
        treatedPriorityCount++;
        cout << "Treated (PRIORITY): " << p.name << " (ID: " << p.id << ", severity: " << p.severity << ")\n";
        // remove from normal queue lazily (mark as treated)
        record.erase(p.id);
        cleanNormalFront();
    }

    void showQueues() {
        cout << "----- Current Normal Queue (FIFO order) -----\n";
        // copy to temp queue to display without modifying
        queue<Patient> tmp = normalQueue;
        while (!tmp.empty()) {
            Patient p = tmp.front(); tmp.pop();
            // only show if still not treated
            if (record.find(p.id) != record.end())
                cout << p.name << " (ID:" << p.id << ", sev:" << p.severity << ")\n";
        }
        cout << "----- Current Priority Queue (highest severity first) -----\n";
        // copy priority queue
        auto tmpPQ = priorityQueue;
        while (!tmpPQ.empty()) {
            Patient p = tmpPQ.top(); tmpPQ.pop();
            if (record.find(p.id) != record.end())
                cout << p.name << " (ID:" << p.id << ", sev:" << p.severity << ")\n";
        }
    }

    void simulateCompareAverageWait() {
        // This function prints current averages for treated patients so far.
        cout << "Simulation stats so far:\n";
        if (treatedNormalCount > 0) {
            double avgN = (double)totalWaitNormal / treatedNormalCount;
            cout << "Average wait (when treated by normal FIFO): " << avgN << " time units\n";
        } else cout << "No patients treated in FIFO mode yet.\n";
        if (treatedPriorityCount > 0) {
            double avgP = (double)totalWaitPriority / treatedPriorityCount;
            cout << "Average wait (when treated by priority): " << avgP << " time units\n";
        } else cout << "No patients treated in PRIORITY mode yet.\n";
    }

    // lazy cleaning helpers
    void cleanPriorityTop() {
        while (!priorityQueue.empty()) {
            Patient p = priorityQueue.top();
            if (record.find(p.id) == record.end()) priorityQueue.pop();
            else break;
        }
    }
    void cleanNormalFront() {
        while (!normalQueue.empty()) {
            Patient p = normalQueue.front();
            if (record.find(p.id) == record.end()) normalQueue.pop();
            else break;
        }
    }

    void findPatient(const string &id) {
        if (record.find(id) != record.end()) {
            Patient p = record[id];
            cout << "Patient found: " << p.name << " (ID:" << p.id << ", sev:" << p.severity << ", arrival:" << p.arrivalOrder << ")\n";
        } else {
            cout << "Patient ID " << id << " not found (may have been treated already).\n";
        }
    }
};

void printMenu() {
    cout << "\n--- Hospital Queue Menu ---\n";
    cout << "1) Add patient\n";
    cout << "2) Treat next (FIFO - normal)\n";
    cout << "3) Treat next (PRIORITY - severity)\n";
    cout << "4) Show queues\n";
    cout << "5) Find patient by ID\n";
    cout << "6) Show simulation stats (avg wait)\n";
    cout << "0) Exit\n";
    cout << "Choose: ";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    HospitalQueueSystem sys;
    int choice;
    while (true) {
        printMenu();
        if (!(cin >> choice)) break;
        if (choice == 0) break;
        if (choice == 1) {
            string id, name;
            int severity;
            cout << "Enter ID (unique): "; cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter name: "; getline(cin, name);
            cout << "Enter severity (1-5): "; cin >> severity;
            if (severity < 1) severity = 1;
            if (severity > 5) severity = 5;
            sys.addPatient(id, name, severity);
        } else if (choice == 2) {
            sys.treatNextNormal();
        } else if (choice == 3) {
            sys.treatNextPriority();
        } else if (choice == 4) {
            sys.showQueues();
        } else if (choice == 5) {
            string id; cout << "Enter ID: "; cin >> id;
            sys.findPatient(id);
        } else if (choice == 6) {
            sys.simulateCompareAverageWait();
        } else {
            cout << "Invalid choice\n";
        }
    }
    cout << "Exiting Hospital Queue System. Goodbye!\n";
    return 0;
}
