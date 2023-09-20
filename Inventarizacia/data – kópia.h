#pragma once
#include <iostream>
#include <vector>

struct Data {
	std::vector<std::string> druh;
	std::vector<std::string> inv_cislo;
	std::vector<int> stredisko;
	std::vector<int> uct_skupina;
	std::vector<std::string> nazov;
	std::vector<int> mesiac_nadobudnutia;
	std::vector<int> rok_nadobudnutia;
	std::vector<float> uctovna_VC;
	std::vector<float> uctovne_opravky;
	std::vector<float> zcu;
	std::vector<std::string> vyrobne_cislo;
	std::vector<std::string> poznamka;

	std::vector<std::string> found;
	int ID_stredisko = 0;
} typedef Data;

struct Foundelsewhere {
	std::vector<int> triedin;
	std::vector<std::string> inv_cislo;
	std::vector<int> foundin;
} typedef Foundelsewhere;

enum class Foundstate {
	NOT_FOUND,
	FOUND,
	FOUND_ELSEWHERE
};


