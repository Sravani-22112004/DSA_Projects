// warehouse_tracker.cpp
// Warehouse Inventory Tracker using stacks (racks) and hash map
// Build: g++ -std=c++17 warehouse_tracker.cpp -O2 -o warehouse_tracker
// Run: ./warehouse_tracker

#include <bits/stdc++.h>
using namespace std;

// Uses date strings in format "YYYY-MM-DD" which compare lexicographically correctly.

struct Item {
    string id;
    string name;
    string expiry; // "YYYY-MM-DD"
    string rackId; // which rack it's in
};

class Warehouse {
private:
    // Each rack is represented by a stack of item IDs.
    unordered_map<string, stack<string>> racks; // rackId -> stack of item ids
    unordered_map<string, Item> itemMap;        // itemId -> Item (quick lookup)
    // Min-heap for expiries: pair<expiry, itemId>
    priority_queue<pair<string, string>, vector<pair<string, string>>, greater<pair<string,string>>> expiryHeap;

public:
    // Create a rack if missing
    void ensureRack(const string &rackId) {
        if (racks.find(rackId) == racks.end()) {
            racks[rackId] = stack<string>();
        }
    }

    void addItem(const string &rackId, const string &id, const string &name, const string &expiry) {
        ensureRack(rackId);
        Item it{ id, name, expiry, rackId };
        racks[rackId].push(id);
        itemMap[id] = it;
        expiryHeap.push({ expiry, id });
        cout << "Added item " << name << " (ID:" << id << ") to rack " << rackId << " with expiry " << expiry << "\n";
    }

    void dispatchFromRack(const string &rackId) {
        if (racks.find(rackId) == racks.end() || racks[rackId].empty()) {
            cout << "Rack " << rackId << " is empty or does not exist.\n";
            return;
        }
        string topId = racks[rackId].top(); racks[rackId].pop();
        if (itemMap.find(topId) != itemMap.end()) {
            Item it = itemMap[topId];
            cout << "Dispatched item " << it.name << " (ID:" << it.id << ") from rack " << rackId << "\n";
            itemMap.erase(topId);
        } else {
            cout << "Top item not found in records (already removed?)\n";
        }
    }

    void findItem(const string &id) {
        if (itemMap.find(id) == itemMap.end()) {
            cout << "Item ID " << id << " not found or already dispatched.\n";
            return;
        }
        Item it = itemMap[id];
        cout << "Item: " << it.name << " (ID:" << it.id << "), Expiry: " << it.expiry << ", Rack: " << it.rackId << "\n";
    }

    void showRack(const string &rackId) {
        if (racks.find(rackId) == racks.end()) {
            cout << "Rack " << rackId << " does not exist.\n";
            return;
        }
        stack<string> tmp = racks[rackId];
        cout << "Contents of rack " << rackId << " from top to bottom:\n";
        while (!tmp.empty()) {
            string id = tmp.top(); tmp.pop();
            if (itemMap.find(id) != itemMap.end()) {
                Item it = itemMap[id];
                cout << it.name << " (ID:" << it.id << ", expiry: " << it.expiry << ")\n";
            } else {
                cout << id << " (dispatched/removed)\n";
            }
        }
    }

    void removeExpired(const string &currentDate) {
        // Pop from expiryHeap while expiry <= currentDate
        while (!expiryHeap.empty() && expiryHeap.top().first <= currentDate) {
            auto pr = expiryHeap.top(); expiryHeap.pop();
            string expiry = pr.first;
            string id = pr.second;
            // If item still exists in itemMap, remove it
            if (itemMap.find(id) != itemMap.end()) {
                Item it = itemMap[id];
                // We cannot easily remove from the middle of a stack; we'll mark it removed from itemMap only
                itemMap.erase(id);
                cout << "Removed expired item " << it.name << " (ID:" << id << ") expired on " << expiry << "\n";
            }
            // else it was already dispatched; just continue
        }
    }

    void listAllItems() {
        cout << "All items currently recorded (unordered):\n";
        for (auto &kv : itemMap) {
            Item it = kv.second;
            cout << it.name << " (ID:" << it.id << ", expiry: " << it.expiry << ", rack: " << it.rackId << ")\n";
        }
    }
};

void printMenu() {
    cout << "\n--- Warehouse Tracker Menu ---\n";
    cout << "1) Add item to rack\n";
    cout << "2) Dispatch top item from rack\n";
    cout << "3) Find item by ID\n";
    cout << "4) Show rack contents\n";
    cout << "5) Remove expired items (provide current date YYYY-MM-DD)\n";
    cout << "6) List all items\n";
    cout << "0) Exit\n";
    cout << "Choose: ";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Warehouse wh;
    int choice;
    while (true) {
        printMenu();
        if (!(cin >> choice)) break;
        if (choice == 0) break;
        if (choice == 1) {
            string rack, id, name, expiry;
            cout << "Enter rack ID: "; cin >> rack;
            cout << "Enter item ID: "; cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter item name: "; getline(cin, name);
            cout << "Enter expiry (YYYY-MM-DD): "; cin >> expiry;
            wh.addItem(rack, id, name, expiry);
        } else if (choice == 2) {
            string rack; cout << "Enter rack ID to dispatch from: "; cin >> rack;
            wh.dispatchFromRack(rack);
        } else if (choice == 3) {
            string id; cout << "Enter item ID: "; cin >> id;
            wh.findItem(id);
        } else if (choice == 4) {
            string rack; cout << "Enter rack ID: "; cin >> rack;
            wh.showRack(rack);
        } else if (choice == 5) {
            string date; cout << "Enter current date (YYYY-MM-DD): "; cin >> date;
            wh.removeExpired(date);
        } else if (choice == 6) {
            wh.listAllItems();
        } else {
            cout << "Invalid choice\n";
        }
    }
    cout << "Exiting Warehouse Tracker. Goodbye!\n";
    return 0;
}
