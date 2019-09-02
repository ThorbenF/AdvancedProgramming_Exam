#include "BST_performance.h"
#include <map>
#include <algorithm>
#include <chrono>
#include <fstream>

int main(){
        std::vector<std::vector<double> > lookup_times;
        int nodes=10000;
        int nodes_max=10000001;
        int stepsize=10000;

        for (; nodes < nodes_max; nodes+=stepsize) {
                std::vector<double> times_node_number;
                //Setup inputs and lookups
                std::map<int, int> map;
                BST<int, int> unbalanced_BST;
                BST<int, int> balanced_BST;

                std::vector<int> keys;
                for (auto i=0; i < nodes; ++i)
                        keys.push_back(i);

                std::vector<int> input_keys = keys;
                std::random_shuffle (input_keys.begin(), input_keys.end());

                std::vector<int> find_keys = keys;
                std::random_shuffle (find_keys.begin(), find_keys.end());


                //Execute map
                for (auto elem : input_keys)
                        map.insert({elem, elem});

                auto start_time_map = std::chrono::high_resolution_clock::now();

                for (const auto elem : find_keys)
                        map.find(elem);

                auto end_time_map = std::chrono::high_resolution_clock::now();

                auto total_time_map = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_map-start_time_map).count(); // nanoseconds
                auto map_ave_lookup_time= total_time_map/double(nodes);
                times_node_number.push_back(map_ave_lookup_time);

                //Execute unbalanced BST
                for (auto elem : input_keys)
                        unbalanced_BST.insert(elem, elem);

                auto start_time_unbalanced = std::chrono::high_resolution_clock::now();

                for (const auto elem : find_keys)
                        unbalanced_BST.find(elem);

                auto end_time_unbalanced = std::chrono::high_resolution_clock::now();

                auto total_time_unbalanced = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_unbalanced-start_time_unbalanced).count();
                auto unbalanced_BST_ave_lookup_time=total_time_unbalanced/double(nodes);
                times_node_number.push_back(unbalanced_BST_ave_lookup_time);

                //Execute balanced BST
                balanced_BST = unbalanced_BST;
                balanced_BST.balance();

                auto start_time_balanced = std::chrono::high_resolution_clock::now();

                for (const auto elem : find_keys)
                        balanced_BST.find(elem);

                auto end_time_balanced = std::chrono::high_resolution_clock::now();

                auto total_time_balanced = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_balanced-start_time_balanced).count();
                auto balanced_BST_ave_lookup_time=total_time_balanced/double(nodes);
                times_node_number.push_back(balanced_BST_ave_lookup_time);

                lookup_times.push_back(times_node_number);
        }
        std::ofstream results;
        results.open ("AverageLookupTimes.txt");
        results << "Average lookup times in: nanoseconds" << std::endl;
        results << std::endl;
        results << "map" << " " << "BST(unbalanced)" << " "<<"BST(balanced)" << std::endl;
        for (auto elem : lookup_times) {
                for (auto e : elem)
                        results << e << " ";
                results << std::endl;
        }
        results.close();
}
