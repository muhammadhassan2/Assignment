#include<bits/stdc++.h>
#include <iostream>
#include "Myvector.h"
#include <chrono>
using namespace std;

int main()
{
        auto start_time = std::chrono::high_resolution_clock::now();
   Myvector<int> v(10);
   v.push_back(8);
   v.push_back(10);
   v.emplace_back(8);
    v.emplace_back(5);
        auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;
    return 0;
}
