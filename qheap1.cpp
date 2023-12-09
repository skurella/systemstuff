#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

struct MinHeap {
    std::vector<int> v;

    void add(int el) {
        auto pos = v.size();
        v.push_back(el);
        while (true) {
            auto parent_pos = pos / 2;
            if (parent_pos == pos) break;
            auto is_min_heap = v.at(parent_pos) < v.at(pos);
            if (is_min_heap) break;
            std::swap(v.at(parent_pos), v.at(pos));
        }
    }
    void del(int el) {
        // TODO: optimize with an index?
        int pos = 0;
        while (v.at(pos) != el) ++pos;
        if (pos + 1 == v.size()) {
            v.pop_back();
            return;
        }
        std::swap(v.at(pos), v.at(v.size() - 1));
        v.pop_back();
        while (true) {
            int left_child_pos = pos * 2;
            int right_child_pos = pos * 2 + 1;
            
            if (left_child_pos >= v.size()) break;
            
            if (right_child_pos >= v.size()) {
                bool is_min_heap = v.at(pos) < v.at(left_child_pos);
                if (is_min_heap) break;
                std::swap(v.at(pos), v.at(left_child_pos));
                break;
            }
            
            // Both children contain values, replace the smaller child.
            if (v.at(left_child_pos) < v.at(right_child_pos)) {
                std::swap(v.at(pos), v.at(left_child_pos));
                pos = left_child_pos;
            } else {
                std::swap(v.at(pos), v.at(right_child_pos));
                pos = right_child_pos;
            }
        }
    }

    int min() { return v.at(0); }
};

int main() {
    MinHeap heap;
    int q;
    std::cin >> q;
    for (auto i = 0; i < q; ++i) {
        int command;
        int element;
        std::cin >> command;
        switch (command) {
        case 1:
            std::cin >> element;
            heap.add(element);
            break;
        case 2:
            std::cin >> element;
            heap.del(element);
            break;
        case 3:
            std::cout << heap.min() << '\n';
        }
    }
    return 0;
}

