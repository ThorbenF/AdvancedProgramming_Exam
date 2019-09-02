/** @file Performance.cpp
 * The file contains a procedure to investigate the performance of the lookup in unbalanced and balanced BST() via member function find() compared to std::map.
   Average lookup-times are measured using std::chrono::high_resolution_clock and normalised with respect to the number of nodes present.
   The performance is tested for node numbers in the range of 10000 to 10000000.
   The output is a AverageLookupTimes.txt which is containing lookup-times in nanoseconds
   in the format 'std::map : BST(unbalanced) : BST(balanced)'.
 */

#include "BST_performance.h"
#include <map>
#include <algorithm>
#include <chrono>
#include <fstream>

int main(){
        /** Vector to store the lookup-times is constructed. And the node range is defined. */
        std::vector<std::vector<double> > lookup_times;
        int nodes=10000;
        int nodes_max=10000001;
        int stepsize=10000;

        for (; nodes < nodes_max; nodes+=stepsize) {
                std::vector<double> times_node_number;

                std::map<int, int> map;
                BST<int, int> unbalanced_BST;
                BST<int, int> balanced_BST;

                /** Vector for keys is constructed and contains keys in the node range. */
                std::vector<int> keys;
                for (auto i=0; i < nodes; ++i)
                        keys.push_back(i);

                /** Vector for input keys is constructed and contains the keys, but in a random order. */
                std::vector<int> input_keys = keys;
                std::random_shuffle (input_keys.begin(), input_keys.end());
                /** Vector for lookup keys is constructed and contains the keys, but in randomised lookup order. */
                std::vector<int> find_keys = keys;
                std::random_shuffle (find_keys.begin(), find_keys.end());


                /** A std::map is populated with the input keys. */
                for (auto elem : input_keys)
                        map.insert({elem, elem});

                /** The time to lookup all keys in the randomised lookup order is measured in nanoseconds.*/
                auto start_time_map = std::chrono::high_resolution_clock::now();

                for (const auto elem : find_keys)
                        map.find(elem);

                auto end_time_map = std::chrono::high_resolution_clock::now();

                auto total_time_map = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_map-start_time_map).count(); // nanoseconds
                auto map_ave_lookup_time= total_time_map/double(nodes);
                /** Average lookup-times are stored.*/
                times_node_number.push_back(map_ave_lookup_time);

                /** A BST() is populated with the input keys. */
                for (auto elem : input_keys)
                        unbalanced_BST.insert(elem, elem);

                /** The time to lookup all keys in the randomised lookup order is measured in nanoseconds.*/
                auto start_time_unbalanced = std::chrono::high_resolution_clock::now();

                for (const auto elem : find_keys)
                        unbalanced_BST.find(elem);

                auto end_time_unbalanced = std::chrono::high_resolution_clock::now();

                auto total_time_unbalanced = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_unbalanced-start_time_unbalanced).count();
                auto unbalanced_BST_ave_lookup_time=total_time_unbalanced/double(nodes);
                /** Average lookup-times are stored.*/
                times_node_number.push_back(unbalanced_BST_ave_lookup_time);

                /** The unbalanced BST() is copied and balanced afterwards. */
                balanced_BST = unbalanced_BST;
                balanced_BST.balance();

                /** The time to lookup all keys in the randomised lookup order is measured in nanoseconds.*/
                auto start_time_balanced = std::chrono::high_resolution_clock::now();

                for (const auto elem : find_keys)
                        balanced_BST.find(elem);

                auto end_time_balanced = std::chrono::high_resolution_clock::now();

                auto total_time_balanced = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_balanced-start_time_balanced).count();
                auto balanced_BST_ave_lookup_time=total_time_balanced/double(nodes);
                /** Average lookup-times are stored.*/
                times_node_number.push_back(balanced_BST_ave_lookup_time);

                lookup_times.push_back(times_node_number);
        }
        /** The output-file is generated.*/
        std::ofstream results;
        results.open ("AverageLookupTimes.txt");
        results << "Average lookup times in: nanoseconds" << std::endl;
        results << std::endl;
        results << "std::map" << " " << "BST(unbalanced)" << " "<<"BST(balanced)" << std::endl;
        for (auto elem : lookup_times) {
                for (auto e : elem)
                        results << e << " ";
                results << std::endl;
        }
        results.close();
}
