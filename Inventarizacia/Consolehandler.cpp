#include "Consolehandler.h"

Consolehandler::Consolehandler(Data* data) {
	this->datahandle = data;
	setcenter();
	mainmenu();
}

void Consolehandler::mainmenu() {
	
	bool running = true;
	do
	{
		std::cout << "-----------------------------------------" << std::endl;
		std::cout << "Main Menu" << std::endl;
		std::cout << "\n";
		std::cout << "Vybrane stredisko: " << datahandle->working.ID_stredisko <<std::endl;
		std::cout << "momentalny stav najdene/celkovo: " << datahandle->working.found.size() << "/" << datahandle->working.inv_cislo.size() << std::endl;
		std::cout << "\n";
		std::cout << "1. scan" << std::endl;
		std::cout << "2. zmen stredisko" << std::endl;
		std::cout << "3. list najdene vsetky strediska" << std::endl;
		std::cout << "4. list chybajuce z vybraneho strediska" << std::endl;
		std::cout << "5. list najdene na inom stredisku" << std::endl;
		std::cout << "6. list nenajdene nikde" << std::endl;
		std::cout << "7. flush najdene na inom stredisku" << std::endl;
		std::cout << "8. flush nenajdene" << std::endl;
		std::cout << "9. load from save" << std::endl;
		std::cout << "0. save and end" << std::endl;
		std::cout << "-----------------------------------------" << std::endl;
		std::string in;
		std::cin >> in;
		if (!Tools::isintparsable(in)) continue;
		switch (std::stoi(in.c_str())) {
		case(1):
			scan();
			break;
		case(2):
			setcenter();
			break;
		case(3):
			listtoconsole(datahandle->storage);
			break;
		case(4):
			listtoconsole(datahandle->referrence,CONSOLEH_ALL_MISSING);
			break;
		case(5):
			listtoconsole(datahandle->foundelsewhere);
			break;
		case(6):
			listtoconsole(datahandle->not_found);
			break;
		case(7):
			datahandle->flushfoundelsewhere();
			break;
		case(8):
			datahandle->flushnotfound();
			break;
		case(9):
			datahandle->loadstorage();
			break;
		case(0):
			datahandle->savestorage();
			running = false;
			break;
		}
	} while (running);
}

void Consolehandler::setcenter() {
	if (!datahandle->working.inv_cislo.empty()) {
		datahandle->append_working_to_storage();
	}
	std::string in;
	std::cout << "vyber si stredisko: \n";
	std::cin >> in;

	if (!Tools::isintparsable(in)) {
		std::cout << "stredisko musi byt cislo\n";
		return;
	}
	int stredisko = std::stoi(in.c_str());
	if (datahandle->load_to_working(stredisko)) return;
	if (!datahandle->filter_to_working(stredisko)) {
		std::cout << "stredisko neexistuje\n";
	}
}

void Consolehandler::scan() {
	bool running = true;
	Foundstate status;
	do {
		std::string in = "";
		std::cout << "naskenuj bar kod alebo daj q/Q na koniec: \n";
		std::cin >> in;
		
		if (in == "q" || in == "Q") {
			std::cout << "pocet threadov pustenych tento scan: " << datahandle->threadslaunchedcount << std::endl;
			running = false;
			datahandle->append_working_to_storage();
			continue;
		}

		status = datahandle->scan_and_process(in);
		datahandle->update_stats(in);
		listtoconsole(datahandle->stats);
		switch (status) {
		case Foundstate::NOT_FOUND: std::cout << "status: NOT FOUND\n"; break;
		case Foundstate::FOUND: std::cout << "status: FOUND\n"; break;
		case Foundstate::FOUND_ELSEWHERE: std::cout << "status: FOUND ELSEWHERE\n"; break;
		}


	} while (running);

}

void Consolehandler::listtoconsole(stats& source) {
	std::cout << "----------------------------\n";
	std::cout << " STATS: \n";
	std::cout << "-------------------------------------------------------------\n";
	std::cout << "\n";
	std::cout << "pocet najdenych na inom stredisku(celkovo): " << source.found_elsewhere_count << std::endl;
	std::cout << "najdene/celkovo(na vybranom stredisku): " << source.found_in_working_count << "/" << source.working_size << std::endl;
	std::cout << "\n";
	std::cout << "posledny najdeny kod: " << source.last_found << std::endl;
	std::cout << "posledny nacitany kod: " << source.last_scanned << std::endl;
	std::cout << "-------------------------------------------------------------\n";
}
void listtoconsole(data& source) {

}
void Consolehandler::listtoconsole(std::vector<std::string>& source) {
	std::cout << "vsetky nenajdene scany: " << std::endl;
	for (std::string& entry : source) {
		std::cout << "scan è: " << entry << std::endl;
	}
}

void Consolehandler::listtoconsole(std::vector<data>& source) {
	std::cout << "vsetky najdene po strediskach: " << std::endl;
	for (data& stredisko : source) {
		std::cout << "-------------------------------------------------------------\n";
		std::cout << "stredisko: " << stredisko.ID_stredisko << std::endl;
		std::cout << "najdene/celkovo: " << stredisko.found.size() << "/" << stredisko.inv_cislo.size() << std::endl;
		for (std::string entry : stredisko.found) {
			for (int i = 0; i < stredisko.inv_cislo.size(); i++) {
				if (entry == stredisko.inv_cislo[i]) {
					std::cout << "nazov: " << stredisko.nazov[i] << " inv.c: " << stredisko.inv_cislo[i] << std::endl;
				}
			}
		}
	}
}

void Consolehandler::listtoconsole(found& source) {
	for(int i = 0; i < source.inv_cislo.size(); i++) {
		std::cout << "najdene na stredisku: " << source.triedin[i] << " inv.c: " << source.inv_cislo[i] << " patri na stredisko: " << source.foundin[i] << std::endl;
	}
}

void Consolehandler::listtoconsole(data& source, int type) {
	switch (type) {
	case(1):	// MISSING
		std::cout << "chybajuce: ";
		
		if (datahandle->working.found.empty()) {
			std::cout << "VSETKY\n";
			return;
		}
		std::cout << std::endl;


		int count = 0;
		for (int i = 0; i < source.inv_cislo.size(); i++) {

			if (source.stredisko[i] != datahandle->working.ID_stredisko) continue;
			bool found = false;
			for (int j = 0; j < datahandle->working.found.size(); j++) {
				if (source.inv_cislo[i] == datahandle->working.found[j]) {
					found = true;
					break;
				}
			}
			if (!found) {
				count++;
				std::cout << " inv c: " << source.inv_cislo[i] << " nazov: " << source.nazov[i] << std::endl;
			}
		}
		std::cout << "----------------------\n";
		std::cout << "celkovo chybajucich: " << count << std::endl;
	break;
	}
}
