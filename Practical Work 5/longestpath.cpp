#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

 
struct KeyValue {
    std::string key;
    std::string value;
};

 

 
std::vector<KeyValue> map_func(const std::string& path) {
    std::vector<KeyValue> emitted_pairs;
    if (!path.empty()) {
         
        emitted_pairs.push_back({"MAX", path});
    }
    return emitted_pairs;
}


std::string reduce_func(const std::string& key, const std::vector<std::string>& values) {
    std::string max_path = "";
    for (const std::string& path : values) {
        if (path.length() > max_path.length()) {
            max_path = path;
        }
    }
    return max_path;
}



int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./longestpath <input_file>" << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1]);
    if (!infile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line;
    std::vector<KeyValue> all_mapped_data;

   
    std::cout << "[Framework] Starting MAP phase..." << std::endl;
    while (std::getline(infile, line)) {
        std::vector<KeyValue> mapped = map_func(line);
        all_mapped_data.insert(all_mapped_data.end(), mapped.begin(), mapped.end());
    }
    infile.close();

    
    std::cout << "[Framework] Starting SHUFFLE phase..." << std::endl;
    std::map<std::string, std::vector<std::string>> shuffled_data;
    
    for (const auto& kv : all_mapped_data) {
        shuffled_data[kv.key].push_back(kv.value);
    }

 
    std::cout << "[Framework] Starting REDUCE phase..." << std::endl;
    std::cout << "\n--- FINAL RESULT ---\n";
    
    for (const auto& entry : shuffled_data) {
        std::string result = reduce_func(entry.first, entry.second);
        std::cout << "Longest Path Found: " << result << std::endl;
        std::cout << "Length: " << result.length() << std::endl;
    }

    return 0;
}