#include <iostream>
#include <vector>
#include "data.h"
#include <fstream>
#include <string>
#include <algorithm>




Data referrence;
Data working;
Foundelsewhere foundelsewhere;
std::vector<int> not_found;


void initializedata(int size, Data &data) {
	data.druh = std::vector<std::string>(size, "");
	data.inv_cislo = std::vector<std::string>(size, "");
	data.stredisko = std::vector<int>(size, 0);
	data.uct_skupina = std::vector<int>(size, 0);
	data.nazov = std::vector<std::string>(size, "");
	data.mesiac_nadobudnutia = std::vector<int>(size, 0);
	data.rok_nadobudnutia = std::vector<int>(size, 0);
	data.uctovna_VC = std::vector<float>(size, 0.0f);
	data.uctovne_opravky = std::vector<float>(size, 0.0f);
	data.zcu = std::vector<float>(size, 0.0f);
	data.vyrobne_cislo = std::vector<std::string>(size, "");
	data.poznamka = std::vector<std::string>(size, "");
}
void setbycenter(Data& source, Data& destination,int center) {
	unsigned int i = 0;
	for (int entry : source.stredisko) {
		if (entry == center) {
			destination.druh.push_back(source.druh[i]);
			destination.inv_cislo.push_back(source.inv_cislo[i]);
			destination.stredisko.push_back(source.stredisko[i]);
			destination.uct_skupina.push_back(source.uct_skupina[i]);
			destination.nazov.push_back(source.nazov[i]);
			destination.mesiac_nadobudnutia.push_back(source.mesiac_nadobudnutia[i]);
			destination.rok_nadobudnutia.push_back(source.rok_nadobudnutia[i]);
			destination.uctovna_VC.push_back(source.uctovna_VC[i]);
			destination.uctovne_opravky.push_back(source.uctovne_opravky[i]);
			destination.zcu.push_back(source.zcu[i]);
			destination.vyrobne_cislo.push_back(source.vyrobne_cislo[i]);
			destination.poznamka.push_back(source.poznamka[i]);
		}
		i++;
	}
	destination.ID_stredisko = center;
}
void loaddata(Data& referrence, std::string filepath) {
	if (filepath.empty()){ 
		std::cout << "filepath is empty \n";
		return;
	}
		
	std::ifstream file(filepath);
	if (file.is_open()) {
		std::string line;
		unsigned short lineindex = 0;
		std::string firstline;
		std::getline(file, firstline);
		while (std::getline(file, line)) {
			unsigned short dataindex = 0;
			std::string fill= "";
			for (char& c : line) {
				//if (c == ',') c = '.';
				if (c == ';' && !fill.empty() ) {
					switch (dataindex) {
					case(0):
						referrence.druh[lineindex] = fill;
						break;
					case(1):
						referrence.inv_cislo[lineindex] = fill;
						break;
					case(2):
						referrence.stredisko[lineindex] = std::stoi(fill.c_str());
						break;
					case(3):
						referrence.uct_skupina[lineindex] = std::stoi(fill.c_str());
						break;
					case(4):
						referrence.nazov[lineindex] = fill;
						break;
					case(5):
						referrence.mesiac_nadobudnutia[lineindex] = std::stoi(fill.c_str());
						break;
					case(6):
						referrence.rok_nadobudnutia[lineindex] = std::stoi(fill.c_str());
						break;
					case(7):
						referrence.uctovna_VC[lineindex] = std::stof(fill.c_str());
						break;
					case(8):
						referrence.uctovne_opravky[lineindex] = std::stof(fill.c_str());
						break;
					case(9):
						referrence.zcu[lineindex] = std::stof(fill.c_str());
						break;
					case(10):
						referrence.vyrobne_cislo[lineindex] = fill;
						break;
					case(11):
						referrence.poznamka[lineindex] = fill;
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
		file.close();
	}
}
/*void exporttocsv(Data &data) {
	std::ofstream IMA;
	std::ofstream DIM;
	std::ofstream* pworking;
	std::ofstream working;
	for (int i = 0; i < data.inv_cislo.size(); i++) {
		std::string out = data.druh[i] + ',' + data.
		if (data.druh[i] == "IMA") {
			pworking = &IMA;
		}
		else {
			pworking = &DIM;
		}

		pworking->open("IMA.csv");
		pworking->write()
	}
}*/
void debugdisplay(Data &data) {
	for (int i = 0; i < data.inv_cislo.size(); i++) {
		std::cout << data.inv_cislo[i] << " , " << data.stredisko[i] << " , " << data.nazov[i] << std::endl;
		std::cout << "i: " << i << std::endl;
	}
}
bool isintparsable(std::string& input) {
	if (!all_of(input.begin(), input.end(), ::isdigit)) {
		std::cout << "zly vstup, vstup musi byt cislo\n";
		return false;
	}
	return true;
}
bool findstredisko(Data &data,std::string& input) {
	if (!isintparsable(input)) return false;
	int in = std::stoi(input.c_str());
	for (int element : data.stredisko) {
		if (element == in) {
			return true;
		}
	}
	std::cout << "stredisko neexistuje" << std::endl;
	return false;
}

Foundstate checkandassignscanned(Data &ref,Foundelsewhere& foundelse,Data& work,std::vector<int> &not_found, std::string& input) {
	
	for (std::string element : work.inv_cislo) {
		if (element == input) {
			work.found.push_back(input);
			for (int i = 0; i < foundelse.inv_cislo.size(); i++) {
				if (input == foundelse.inv_cislo[i]) {
					foundelse.foundin.erase(std::next(foundelse.foundin.begin(), i));
					foundelse.inv_cislo.erase(std::next(foundelse.inv_cislo.begin(), i));
					foundelse.triedin.erase(std::next(foundelse.triedin.begin(), i));
					break;
				}
			}
			return Foundstate::FOUND;
		}
	}
	for (int i = 0; i < ref.inv_cislo.size();i++) {
		if (ref.inv_cislo[i] == input) {
			foundelse.triedin.push_back(work.ID_stredisko);
			foundelse.inv_cislo.push_back(input);
			foundelse.foundin.push_back(ref.stredisko[i]);
			return Foundstate::FOUND_ELSEWHERE;
		}
	}
	not_found.push_back(std::stoi(input));
	return Foundstate::NOT_FOUND;
}
void listfoundelsewhere(Foundelsewhere &data)
{
	for (int i = 0; i < data.inv_cislo.size(); i++) {
		std::cout << "nacitane na stredisku: " << data.triedin[i] << " inv cislo: " << data.inv_cislo[i] << "najdene na stredisku: " << data.foundin[i] << std::endl;
	}
}
void liststorage(std::vector<Data> &storage) {
	for (Data data : storage) {
		std::cout << "--------------------------------------------------\n";
		std::cout << "stredisko: " << data.ID_stredisko << std::endl;
		std::cout << "najduty inv: \n";
		std::cout << "--------------------------------------------------\n";
		for (std::string invc : data.found) {
			for (int i = 0; i < data.inv_cislo.size(); i++) {
				if (data.inv_cislo[i] == invc) {
					std::cout << "druh: " << data.druh[i];
					std::cout << " inv c: " << data.inv_cislo[i];
					std::cout << " stredisko: " << data.stredisko[i];
					std::cout << " uct skupina: " << data.uct_skupina[i];
					std::cout << " nazov: " << data.nazov[i];
					std::cout << " mesiac nadobud: " << data.mesiac_nadobudnutia[i];
					std::cout << " rok nadobud: " << data.rok_nadobudnutia[i];
					std::cout << " uctovna VC: " << data.uctovna_VC[i];
					std::cout << " uctovne opravky: " << data.uctovne_opravky[i];
					std::cout << " ZCU: " << data.zcu[i];
					std::cout << " vyrobne c: " << data.vyrobne_cislo[i];
					std::cout << " pozn: " << data.poznamka[i] << std::endl;
				}
			}
		}
		std::cout << "--------------------------------------------------\n";
	}
}
void updatefound(std::vector<std::string> &saved, std::vector<std::string> &update) {
	for (int i = 0; i < update.size(); i++) {
		bool found = false;
		
		for (int j = 0; j < saved.size(); j++) {
			if (update[i] == saved[j]) {
				found = true;
				break;
			}
		}
		if (!found) {
			saved.push_back(update[i]);
		}
	}
}
void listnotfound(std::vector<int>& source) {
	for (int entry : source) {
		std::cout << entry << std::endl;
	}
}
void flushnotfound(std::vector<int>& source) {
	source = std::vector<int>();
}
void flushfoundelsewhere(Foundelsewhere& source) {
	source = Foundelsewhere();
}

int main() {
	initializedata(5000, referrence);
	loaddata(referrence, "test.csv");
	std::vector<Data> storage;

	while (true)
	{
		{
			bool quit = false;
			bool contin = false;
			std::cout << "-----------------------------------------" << std::endl;
			std::cout << "Main Menu" << std::endl;
			std::cout << "\n";
			std::cout << "1. scan" << std::endl;
			std::cout << "2. list found elsewhere" << std::endl;
			std::cout << "3. list found storage" << std::endl;
			std::cout << "4. list not found scans" << std::endl;
			std::cout << "5. flush found elsewhere" << std::endl;
			std::cout << "6. flush not found" << std::endl;
			std::cout << "7. end" << std::endl;
			std::cout << "-----------------------------------------" << std::endl;
			std::string in;
			std::cin >> in;
			if(!isintparsable(in)) continue;
			switch (std::stoi(in.c_str())) {
			case(1): 

				break;
			case(2):
				listfoundelsewhere(foundelsewhere);
				contin = true;
				break;
			case(3):
				liststorage(storage);
				contin = true;
				break;
			case(4):
				listnotfound(not_found);
				contin = true;
				break;
			case(5):
				flushfoundelsewhere(foundelsewhere);
				contin = true;
				break;
			case(6):
				flushnotfound(not_found);
				contin = true;
				break;
			case(7):
				quit = true;
				break;
			default:
				contin = true;
				break;
			}
			if (quit == true) break;
			if (contin == true) continue;
		}
		{
			std::cout << "zadaj stredisko: \n";
			std::string in;
			std::cin >> in;
			if (!findstredisko(referrence, in)) {
				continue;
			}
			setbycenter(referrence, working, std::stoi(in.c_str()));
		}
		std::string input = "";
		Foundstate status = Foundstate::NOT_FOUND;
		std::cout << "nacitaj prvotny kod: \n";
		std::cin >> input;
		while (true) {
			status = checkandassignscanned(referrence, foundelsewhere, working,not_found, input);
			std::cout << "-------------------------------------------------------------\n";
			std::cout << "pocet najdenych na inom stredisku: " << foundelsewhere.inv_cislo.size() << std::endl;
			std::cout << "najdene/celkovo: " << working.found.size() << "/" << working.inv_cislo.size() << std::endl;
			std::cout << "\n";
			if (!working.found.empty()) {
				std::cout << "posledny najdeny kod: " << working.found.back() << std::endl;
			}
			std::cout << "posledny nacitany kod: " << input << std::endl;
			
			switch (status) {
			case(Foundstate::FOUND):
				std::cout << "status: FOUND\n";
				break;
			case(Foundstate::NOT_FOUND):
				std::cout << "status: NOT FOUND\n";
				break;
			case(Foundstate::FOUND_ELSEWHERE):
				std::cout << "status: FOUND ELSEWHERE\n";
				break;
			}
			std::cout << "\n";
			std::cout << "\n";
			std::cout << "nacitaj kod alebo vpis Q na ulozenie a navrat do hlavneho menu: \n";
			std::cin >> input;
			if (input == "q" || input == "Q") {
				bool found = false;
				int index = 0;
				for (int i = 0; i < storage.size();i++) {
					if (storage[i].ID_stredisko == working.ID_stredisko) {
						found = true;
						index = i;
					}
				}
				if (!found) {
					storage.push_back(working);
				}
				else {
					updatefound(storage[index].found, working.found);
				}
				break;
			}
			std::cout << "-------------------------------------------------------------\n";
		}
	}
	return 0;
}