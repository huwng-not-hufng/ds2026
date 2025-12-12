#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

// Cấu trúc Key-Value đơn giản
struct KeyValue {
    std::string key;
    int value;
};

// --- PHẦN 1: USER DEFINED FUNCTIONS (Người dùng viết) ---

// Mapper: Nhận vào một dòng text -> Trả về danh sách <từ, 1>
std::vector<KeyValue> map_func(const std::string& text) {
    std::vector<KeyValue> emitted_pairs;
    std::stringstream ss(text);
    std::string word;
    
    while (ss >> word) {
        // Xử lý đơn giản: bỏ dấu câu nếu cần (ở đây giữ nguyên theo slide)
        emitted_pairs.push_back({word, 1});
    }
    return emitted_pairs;
}

// Reducer: Nhận vào (từ, danh sách số 1) -> Trả về tổng
int reduce_func(const std::string& key, const std::vector<int>& values) {
    int sum = 0;
    for (int v : values) {
        sum += v;
    }
    return sum;
}

// --- PHẦN 2: MAPREDUCE FRAMEWORK (Engine tự chế) ---

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./wordcount <input_file>" << std::endl;
        return 1;
    }

    // 1. INPUT PHASE: Đọc file
    std::ifstream infile(argv[1]);
    if (!infile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line;
    std::vector<KeyValue> all_mapped_data;

    // 2. MAP PHASE: Gọi hàm map() cho từng dòng
    std::cout << "[Framework] Starting MAP phase..." << std::endl;
    while (std::getline(infile, line)) {
        std::vector<KeyValue> mapped = map_func(line);
        all_mapped_data.insert(all_mapped_data.end(), mapped.begin(), mapped.end());
    }
    infile.close();

    // 3. SHUFFLE & SORT PHASE: Gom nhóm theo Key
    // Dùng std::map để tự động sắp xếp và gom nhóm
    std::cout << "[Framework] Starting SHUFFLE phase..." << std::endl;
    std::map<std::string, std::vector<int>> shuffled_data;
    
    for (const auto& kv : all_mapped_data) {
        shuffled_data[kv.key].push_back(kv.value);
    }

    // 4. REDUCE PHASE: Gọi hàm reduce() cho từng Key
    std::cout << "[Framework] Starting REDUCE phase..." << std::endl;
    std::cout << "\n--- FINAL OUTPUT ---\n";
    
    for (const auto& entry : shuffled_data) {
        int result = reduce_func(entry.first, entry.second);
        std::cout << entry.first << ": " << result << std::endl;
    }

    return 0;
}