#pragma once
#include <fstream>
#include <vector>
#include <algorithm>

class RecordTable {
public:
    RecordTable(const std::string& filename, int maxRecords);
    void addRecord(const std::string& name, int score);
    void displayRecords() const;

private:
    std::string filename;
    int maxRecords;
    std::vector<Record> records;

    struct Record {
        std::string name;
        int score;
    };

    void loadRecords();
    void saveRecords();
};


