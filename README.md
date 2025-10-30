 DSA_Projects

This repository contains **real-world C++ projects** based on **Data Structures and Algorithms (DSA)**.  
These projects simulate real-life problems while showcasing efficient data structure usage, algorithmic logic, and clean C++ implementation.

---

# Projects Included

# 🏥 1. Hospital Patient Queue Management System
 #**Core Concepts:** Queue, Priority Queue (Heap)

 Description:
A simulation of how patients are treated in hospitals based on **urgency and arrival time**.  
Each patient has:
- Name  
- Severity level (1–5)  
- Arrival time  

The system uses a **Priority Queue** to always treat the most critical patient first.  
It also allows comparison between **normal queue (FIFO)** and **priority-based** systems, showing waiting time differences.

 Features:
- Add new patients with severity and arrival details  
- Serve patients based on severity (max-heap priority queue)  
- Show live queue status  
- Compare normal queue vs priority-based performance  

 DSA Used:
- **Queue**
- **Priority Queue (Max-Heap)**
- **Structures / Classes**

# Real-world Analogy:
Emergency room triage in hospitals.

---

# 2. Warehouse Inventory Tracker
**Core Concepts:** Stack, Hash Map (unordered_map)

Description:
Simulates a warehouse system where each **rack** acts like a **stack (LIFO)**.  
Every product has a **unique ID**, **name**, and **expiry date**.  
Supports adding, removing, and searching for items efficiently.

 Features:
- Add new items to warehouse racks  
- Dispatch (pop) items in LIFO order  
- View and search items using ID  
- Optional: Manage expiry-based removal using a **min-heap**

# DSA Used:
- **Stack**
- **Hash Map**
- **(Optional: Min-Heap)**

#Real-world Analogy:
Warehouse systems like **Amazon or Flipkart**, where newer items are stored on top for easy access.

---


   git clone https://github.com/Sravani-22112004/DSA_Projects.git
   cd DSA_Projects
