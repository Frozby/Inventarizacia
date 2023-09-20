#pragma once


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <thread>
#include <mutex>
#include "Tools.h"



#define DIM "DIM.csv"
#define IMA "IMA.csv"
#define SAVE "save.txt"
#define DIMO "DIMo.csv"
#define IMAO "IMAo.csv"




struct data {
	std::vector<std::string> inv_cislo;
	std::vector<int> stredisko;
	std::vector<std::string> nazov;
	std::vector<std::string> found;
	int ID_stredisko = 0;
} typedef data;

struct found {
	std::vector<int> triedin;
	std::vector<std::string> inv_cislo;
	std::vector<int> foundin;
} typedef found;

struct stats {
	int found_elsewhere_count = 0;
	int found_in_working_count = 0;
	int working_size = 0;
	std::string last_found = "";
	std::string last_scanned = "";
} typedef stats;

enum class Foundstate {
	NOT_FOUND,
	FOUND,
	FOUND_ELSEWHERE
};

class Data {
private:
	void initialize_data(int size, data& data); // GET RID OF
	void TH_check_and_erase_foundelsewhere(std::string input);
	
	bool working_exists(int& input);
	std::mutex mu;

public:
	Data();
	data referrence;
	data working;
	std::vector<data> storage;
	bool state = true;

	found foundelsewhere;
	std::vector<std::string> not_found;
	stats stats;

	int threadslaunchedcount = 0;

public:
	bool load();
	bool filter_to_working(int& stredisko);
	bool load_to_working(int& stredisko);
	bool get_from_storage(data& destination, int& stredisko);
	Foundstate scan_and_process(std::string input);
	
	void append_working_to_storage();
	void TH_append_working_to_storage();
	//void exporttocsv();
	void update_stats(std::string& input);

	void savestorage();
	void loadstorage();

public:
	void flushnotfound();
	void flushfoundelsewhere();
};
