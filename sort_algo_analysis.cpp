#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cmath>
#include <numeric>
#include <functional>

using namespace std;
using namespace std::chrono;

class StringGenerator {
private:
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-.";
    mt19937 gen;
    uniform_int_distribution<> char_dist;
    uniform_int_distribution<> len_dist;

public:
    StringGenerator() : gen(random_device{}()), 
                       char_dist(0, chars.size() - 1), 
                       len_dist(10, 200) {}

    string generate_string() {
        int length = len_dist(gen);
        string s;
        s.reserve(length);
        for (int i = 0; i < length; ++i) {
            s += chars[char_dist(gen)];
        }
        return s;
    }

    vector<string> generate_random_array(int size) {
        vector<string> arr;
        arr.reserve(size);
        for (int i = 0; i < size; ++i) {
            arr.push_back(generate_string());
        }
        return arr;
    }

    vector<string> generate_reverse_sorted_array(int size) {
        vector<string> arr = generate_random_array(size);
        sort(arr.begin(), arr.end(), greater<string>());
        return arr;
    }

    vector<string> generate_almost_sorted_array(int size) {
        vector<string> arr = generate_random_array(size);
        sort(arr.begin(), arr.end());

        int swaps = max(1, static_cast<int>(size * 0.1));
        uniform_int_distribution<> idx_dist(0, size - 2);
        
        for (int i = 0; i < swaps; ++i) {
            int idx = idx_dist(gen);
            swap(arr[idx], arr[idx + 1]);
        }
        
        return arr;
    }
};

class StringSortTester {
private:
    int comparisons;
    bool counting_comparisons;

    void standard_quicksort(vector<string>& arr, int left, int right) {
        if (left >= right) return;
        
        int pivot = partition(arr, left, right);
        standard_quicksort(arr, left, pivot - 1);
        standard_quicksort(arr, pivot + 1, right);
    }

    int partition(vector<string>& arr, int left, int right) {
        string pivot = arr[right];
        int i = left - 1;
        
        for (int j = left; j < right; ++j) {
            if (counting_comparisons) comparisons++;
            if (arr[j] <= pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        
        swap(arr[i + 1], arr[right]);
        return i + 1;
    }

    void standard_mergesort(vector<string>& arr, int left, int right) {
        if (left >= right) return;
        
        int mid = left + (right - left) / 2;
        standard_mergesort(arr, left, mid);
        standard_mergesort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

    void merge(vector<string>& arr, int left, int mid, int right) {
        vector<string> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        
        while (i <= mid && j <= right) {
            if (counting_comparisons) comparisons++;
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];
        
        for (int p = 0; p < k; ++p) {
            arr[left + p] = temp[p];
        }
    }

    void string_quicksort(vector<string>& arr, int left, int right, int d = 0) {
        if (left >= right) return;
        
        int pivot = string_partition(arr, left, right, d);
        string_quicksort(arr, left, pivot - 1, d);
        
        if (d < arr[pivot].length()) {
            string_quicksort(arr, pivot, right, d + 1);
        }
    }

    int string_partition(vector<string>& arr, int left, int right, int d) {
        string pivot = arr[right];
        int i = left - 1;
        
        for (int j = left; j < right; ++j) {
            bool less = false;
            
            if (d < arr[j].length() && d < pivot.length()) {
                if (counting_comparisons) comparisons++;
                less = arr[j][d] < pivot[d];
            } else if (d >= arr[j].length() && d < pivot.length()) {
                less = true;
            } else if (d < arr[j].length() && d >= pivot.length()) {
                less = false;
            } else {
                less = false;
            }
            
            if (less) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        
        swap(arr[i + 1], arr[right]);
        return i + 1;
    }

    void string_mergesort(vector<string>& arr, int left, int right, int d = 0) {
        if (left >= right) return;
        
        int mid = left + (right - left) / 2;
        string_mergesort(arr, left, mid, d);
        string_mergesort(arr, mid + 1, right, d);
        string_merge(arr, left, mid, right, d);
    }

    void string_merge(vector<string>& arr, int left, int mid, int right, int d) {
        vector<string> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        
        while (i <= mid && j <= right) {
            bool less = false;
            
            if (d < arr[i].length() && d < arr[j].length()) {
                if (counting_comparisons) comparisons++;
                less = arr[i][d] < arr[j][d];
            } else if (d >= arr[i].length() && d < arr[j].length()) {
                less = true;
            } else if (d < arr[i].length() && d >= arr[j].length()) {
                less = false;
            } else {
                if (counting_comparisons) comparisons++;
                less = arr[i] < arr[j];
            }
            
            if (less) {
                temp[k++] = arr[i++];
            } else {
                temp[k++] = arr[j++];
            }
        }
        
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];
        
        for (int p = 0; p < k; ++p) {
            arr[left + p] = temp[p];
        }
    }

    void msd_radixsort(vector<string>& arr, int left, int right, int d = 0, bool switch_to_quicksort = false) {
        if (left >= right) return;
        
        if (switch_to_quicksort && right - left + 1 < 74) {
            string_quicksort(arr, left, right, d);
            return;
        }
        
        const int R = 256;
        vector<int> count(R + 2, 0);
        vector<string> aux(right - left + 1);
        
        for (int i = left; i <= right; ++i) {
            int c = (d < arr[i].length()) ? arr[i][d] + 1 : 0;
            count[c + 1]++;
        }
        
        for (int r = 0; r < R + 1; ++r) {
            count[r + 1] += count[r];
        }
        
        for (int i = left; i <= right; ++i) {
            int c = (d < arr[i].length()) ? arr[i][d] + 1 : 0;
            aux[count[c]++] = arr[i];
        }
        
        for (int i = left; i <= right; ++i) {
            arr[i] = aux[i - left];
        }
        
        for (int r = 0; r < R; ++r) {
            msd_radixsort(arr, left + count[r], left + count[r + 1] - 1, d + 1, switch_to_quicksort);
        }
    }

public:
    StringSortTester() : comparisons(0), counting_comparisons(false) {}

    struct SortResult {
        double avg_time_ms;
        double avg_comparisons;        
    };

    SortResult run_multiple_tests(function<void(vector<string>&)> sort_func, vector<string> arr, int runs = 5) {
        vector<double> times;
        vector<double> comps;
        
        for (int i = 0; i < runs; ++i) {
            vector<string> arr_copy = arr;
            comparisons = 0;
            counting_comparisons = true;
            
            auto start = high_resolution_clock::now();
            sort_func(arr_copy);
            auto end = high_resolution_clock::now();
            
            double time = duration_cast<microseconds>(end - start).count() / 1000.0;
            times.push_back(time);
            comps.push_back(static_cast<double>(comparisons));
        }
        
        double avg_time = accumulate(times.begin(), times.end(), 0.0) / runs;
        double avg_comp = accumulate(comps.begin(), comps.end(), 0.0) / runs;
        
        double time_var = 0.0, comp_var = 0.0;
        for (int i = 0; i < runs; ++i) {
            time_var += pow(times[i] - avg_time, 2);
            comp_var += pow(comps[i] - avg_comp, 2);
        }
        time_var /= runs;
        comp_var /= runs;
        
        return {
            avg_time,
            avg_comp,
        };
    }

    void test_algorithm(const string& name, function<void(vector<string>&)> sort_func, 
                       const vector<string>& arr, const string& array_type, int size, int runs = 50) {
        auto res = run_multiple_tests(sort_func, arr, runs);
        cout << name << "," << array_type << "," << size << ","
             << res.avg_time_ms << "," << res.avg_comparisons << endl;
    }

    void run_all_tests(const vector<string>& arr, const string& array_type, int size, int runs = 50) {                
        test_algorithm("Standard Quicksort", 
            [this](vector<string>& a) { standard_quicksort(a, 0, a.size() - 1); }, 
            arr, array_type, size, runs);
            
        test_algorithm("Standard Mergesort", 
            [this](vector<string>& a) { standard_mergesort(a, 0, a.size() - 1); }, 
            arr, array_type, size, runs);
            
        test_algorithm("String Quicksort", 
            [this](vector<string>& a) { string_quicksort(a, 0, a.size() - 1); }, 
            arr, array_type, size, runs);
            
        test_algorithm("String Mergesort", 
            [this](vector<string>& a) { string_mergesort(a, 0, a.size() - 1); }, 
            arr, array_type, size, runs);
            
        test_algorithm("MSD Radixsort", 
            [this](vector<string>& a) { msd_radixsort(a, 0, a.size() - 1, 0, false); }, 
            arr, array_type, size, runs);
            
        test_algorithm("MSD Radixsort (with switch)", 
            [this](vector<string>& a) { msd_radixsort(a, 0, a.size() - 1, 0, true); }, 
            arr, array_type, size, runs);
    }
};

int main() {
    StringGenerator generator;
    StringSortTester tester;
    
    const int MAX_SIZE = 3000;
    const int STEP = 100;
    const int RUNS = 5;

    cout << "algorithm,array_type,size,avg_time_ms,avg_comparisons" << endl;
    
    vector<string> random_arr = generator.generate_random_array(MAX_SIZE);
    vector<string> reverse_arr = generator.generate_reverse_sorted_array(MAX_SIZE);
    vector<string> almost_arr = generator.generate_almost_sorted_array(MAX_SIZE);
    
    for (int size = 100; size <= MAX_SIZE; size += STEP) {
        vector<string> random_sub(random_arr.begin(), random_arr.begin() + size);
        vector<string> reverse_sub(reverse_arr.begin(), reverse_arr.begin() + size);
        vector<string> almost_sub(almost_arr.begin(), almost_arr.begin() + size);
        
        tester.run_all_tests(random_sub, "random", size, RUNS);
        tester.run_all_tests(reverse_sub, "reverse sorted", size, RUNS);
        tester.run_all_tests(almost_sub, "almost sorted", size, RUNS);
    }
    
    return 0;
}