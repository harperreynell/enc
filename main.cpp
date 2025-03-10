// #include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>
namespace fs = std::filesystem;
using namespace std;

class Bill {
public: 
    void setPath(string);
    vector<fs::path> listFiles(string);
    void crypto(int);

    Bill(string path, string key) {
        this->path = path;
        this->key = this->sha256(key);
    }

private:
    string key;
    string path;
    vector<fs::path> files;

    int helper(string, string, int);
    int hash(string);
    string sha256(string);
};

void Bill::setPath(string path) {
    this->path = path;
}

vector<fs::path> Bill::listFiles(string path) {
    for(const auto &entry : fs::recursive_directory_iterator(path)) {
        this->files.push_back(entry.path());
    }

    return this->files;
}

int Bill::hash(string str) {
    int sum = 0;
    for (auto c : str) {
        sum += c;
    }

    return sum * 1368 % 14;
}

int Bill::helper(string file, string key, int operation) {
    int temp;
    std::fstream fs(file, std::fstream::in | std::fstream::out);
    if(fs.is_open()) {
        while(!fs.eof()) {
            char c = fs.get();
            fs.seekp(fs.tellp() - static_cast<std::streampos>(1));

            if(operation == 1) temp = (c + this->hash(key));
            else temp = (c - this->hash(key));
            
            fs.put((char)temp);
            fs.seekp(fs.tellp());
        }
        fs.close();
        return 0;
    } else {
        return -1;
    }
}

void Bill::crypto(int operation) {
    vector<fs::path> files;
    files = this->listFiles(this->path);
    string key = this->key;

    cout << "Starting encryption..." << endl;
    for(const auto file : files) {
        cout << "Encrypting " << file.filename() << " with key " << key << '\n';
        if(this->helper(file, key, operation) == -1)
        cout << "Failed to open file " << file.filename() << '\n';
    }
}

string Bill::sha256(string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(str.c_str()), str.size(), hash);
    
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}



int main(int argc, char* argv[]) {
    if (argc > 4) {
        cout << "Wrong argument usage";
        return -13;
    }
    Bill cypher(argv[1], argv[2]);
   
    if(strcmp(argv[3], "enc") == 0) cypher.crypto(1);
    else if(strcmp(argv[3], "dec") == 0) cypher.crypto(-1);
    else cout << "No operation like " << argv[3] << '\n' <<
        "Available operations are {enc} or {dec}\n";

    return 0;
}