#include "data.h"

Data::Data() {
	if (!load()){
		state = false;
	}
}

bool Data::load() {
	initialize_data(10000, referrence);

	std::ifstream fileIMA(IMA);
	std::ifstream fileDIM(DIM);
	bool fail = false;
	if (!fileIMA.is_open()) {
		std::cout << "failed to open IMA\n";
		return false;
	}
	if (!fileDIM.is_open()) {
		std::cout << "failed to open DIM\n";
		return false;
	}

	std::string line = "";
	unsigned int lineindex = 0;

	std::string skipfirstline;
	std::getline(fileIMA, skipfirstline);

	while (std::getline(fileIMA, line)) {
		unsigned int dataindex = 0;
		std::string fill = "";
		for (char& c : line) {
			if (c == ';' && !fill.empty()) {
				switch (dataindex) {
				case(0):
					referrence.inv_cislo[lineindex] = fill;
					break;
				case(1):
					referrence.stredisko[lineindex] = std::stoi(fill.c_str());
					break;
				case(3):
					referrence.nazov[lineindex] = fill;
					break;
				}
				fill = "";
				dataindex++;
			}
			else {
				fill += c;
			}
		}
		lineindex++;
	}
	fileIMA.close();

	
	line = "";

	std::getline(fileDIM, skipfirstline);

	while (std::getline(fileDIM, line)) {
		unsigned int dataindex = 0;
		std::string fill = "";
		for (char& c : line) {
			if (c == ';' && !fill.empty()) {
				switch (dataindex) {
				case(0):
					referrence.inv_cislo[lineindex] = fill;
					break;
				case(1):
					referrence.stredisko[lineindex] = std::stoi(fill.c_str());
					break;
				case(3):
					referrence.nazov[lineindex] = fill;
					break;
				}
				fill = "";
				dataindex++;
			}
			else {
				fill += c;
			}
		}
		lineindex++;
	}
	fileDIM.close();

	return true;
}


bool Data::filter_to_working(int& stredisko) {
	if (!working_exists(stredisko)) return false;
	mu.lock();
	working = data();
	unsigned int i = 0;
	for (int entry : referrence.stredisko) {
		if (entry == stredisko) {
			working.inv_cislo.push_back(referrence.inv_cislo[i]);
			working.stredisko.push_back(referrence.stredisko[i]);
			working.nazov.push_back(referrence.nazov[i]);
		}
		i++;
	}
	working.ID_stredisko = stredisko;
	stats.working_size = working.inv_cislo.size();
	mu.unlock();
	return true;
}
bool Data::get_from_storage(data& destination,int& stredisko) {
	for (data entry : storage) {
		if (entry.ID_stredisko == stredisko) {
			destination = entry;
			return true;
		}
	}
	return false;
}


bool Data::load_to_working(int& stredisko) {
	if (!working_exists(stredisko)) return false;
	if (!get_from_storage(working,stredisko)) return false;
	stats.working_size = working.inv_cislo.size();
	return true;
}


Foundstate Data::scan_and_process(std::string input) {

	for (std::string element : working.inv_cislo) {
		if (element == input) {
			working.found.push_back(input);
			//checkanderasefoundelsewhere(input);
			std::thread worker(&Data::TH_check_and_erase_foundelsewhere, this, input);
			worker.detach();
			return Foundstate::FOUND;
		}
	}
	for (int i = 0; i < referrence.inv_cislo.size(); i++) {
		if (referrence.inv_cislo[i] == input) {
			foundelsewhere.triedin.push_back(working.ID_stredisko);
			foundelsewhere.inv_cislo.push_back(input);
			foundelsewhere.foundin.push_back(referrence.stredisko[i]);
			return Foundstate::FOUND_ELSEWHERE;
		}
	}
	not_found.push_back(input);
	return Foundstate::NOT_FOUND;
}
void Data::TH_check_and_erase_foundelsewhere(std::string input) {
	mu.lock();
	threadslaunchedcount++;
	std::cout << "thread n: " << std::this_thread::get_id << std::endl;
	for (int i = 0; i < foundelsewhere.inv_cislo.size(); i++) {
		if (input == foundelsewhere.inv_cislo[i]) {
			foundelsewhere.foundin.erase(std::next(foundelsewhere.foundin.begin(), i));
			foundelsewhere.inv_cislo.erase(std::next(foundelsewhere.inv_cislo.begin(), i));
			foundelsewhere.triedin.erase(std::next(foundelsewhere.triedin.begin(), i));
			std::cout << "thread function finished found\n";
			break;
		}
	}

	std::cout << "thread function finished\n";
	mu.unlock();
}

void Data::initialize_data(int size, data& data) {
	data.inv_cislo = std::vector<std::string>(size, "");
	data.stredisko = std::vector<int>(size, 0);
	data.nazov = std::vector<std::string>(size, "");
}

bool Data::working_exists(int& input) {
	for (int element : referrence.stredisko) {
		if (element == input) {
			return true;
		}
	}
	std::cout << "stredisko neexistuje" << std::endl;
	return false;
}

void Data::update_stats(std::string& input) {
	stats.found_elsewhere_count = foundelsewhere.inv_cislo.size();
	stats.found_in_working_count = working.found.size();
	stats.last_scanned = input;
}
void Data::TH_append_working_to_storage() {
	std::thread worker(&Data::TH_append_working_to_storage,this);
	worker.detach();
}
void Data::append_working_to_storage() {
	mu.lock();
	bool found = false;
	for (data& entry : storage) {
		if (entry.ID_stredisko == working.ID_stredisko) {
			found = true;
			for (int i = 0; i < working.inv_cislo.size(); i++) {
				bool found_entry = false;
				for (int j =0; j < entry.inv_cislo.size(); j++) {
					if (entry.inv_cislo[j] == working.inv_cislo[i]) {
						found_entry = true;
						break;
					}
				}
				if (!found_entry) {
					entry.inv_cislo.push_back(working.inv_cislo[i]);
					entry.nazov.push_back(working.nazov[i]);
					entry.stredisko.push_back(working.stredisko[i]);
				}
			}
			for (int i = 0; i < working.found.size();i++) {
				bool found_enty = false;
				for (int j = 0; j < entry.found.size(); j++) {
					if (working.found[i] == entry.found[j]) {
						found_enty = true;
						break;
					}
				}
				if (!found_enty) {
					entry.found.push_back(working.found[i]);
				}
			}
		}
	}
	if (!found) 
		storage.push_back(working);
	
	mu.unlock();
}

void Data::flushnotfound() {
	not_found = std::vector < std::string >();
}
void Data::flushfoundelsewhere() {
	foundelsewhere = found();
}

void Data::savestorage() {	
	std::ofstream file(SAVE);
	if (!file.is_open()) {
		std::cout << "file failed to open\n";
	}


	std::string out;
	for (data& storage_el : storage) {
		out += "----\n";
		out += std::to_string(storage_el.ID_stredisko) + "\n";
		out += "++++\n";
		for (std::string found : storage_el.found) {
			out += found + "\n";
		}
	}
	file.write(out.c_str(), out.size());
	file.close();
}
void Data::loadstorage() {
	std::ifstream file(SAVE);
	if (!file.is_open()) {
		std::cout << "file failed to open\n";
	}

	int init = working.ID_stredisko;
	std::string line = "";
	int state = 0; // 1 = set stredisko 2 = read found
	while (std::getline(file, line)) {
		
		if (line == "----") {
			state = 1;
			continue;
		}
		

		if (line == "++++") {
			state = 2;
			continue;
		}
		int Iline = std::stoi(line);
		switch (state) {
		case(1):
			if (!working.inv_cislo.empty()) {
				append_working_to_storage();
			}
			filter_to_working(Iline);
			break;
		case(2):
			working.found.push_back(line);
			break;
		}
	}
	file.close();
	append_working_to_storage();
	filter_to_working(init);
}
/*
void Data::exporttocsv() {


	std::ifstream ima(IMA);
	std::ifstream dim(DIM);
	std::ofstream imao(IMAO);
	std::ofstream dimo(DIMO);

	std::string line;

} */

