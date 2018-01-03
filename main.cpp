#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <string>
#include <experimental\filesystem>

int main(int argc, char** argv) {


	/*if (argc != 2) {
		std::cerr << "Not enough arguments (2)\n";
	}*/
	std::string binaryFile = "data/arm9.bin";
	std::string narctool = "data\\narctool.exe";

	std::uint32_t combinationOffset = 0x00151BF4;
	std::uint32_t endingBytes = 0x5f47422f;
	std::ifstream binFILE;
	binFILE.open(binaryFile, std::ios::binary);
	binFILE.seekg(combinationOffset);
	std::vector<std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>> combo;
	while(1) {
		std::uint32_t nscr;
		std::uint32_t ncgr;
		std::uint32_t nclr;

		binFILE.read(reinterpret_cast<char*>(&nscr), sizeof(nscr));
		binFILE.read(reinterpret_cast<char*>(&ncgr), sizeof(ncgr));
		binFILE.read(reinterpret_cast<char*>(&nclr), sizeof(nclr));

		if (nscr == endingBytes) {
			break;
		}
		std::cout << std::hex << ncgr << " " << nclr << " " << nscr << std::endl;
		combo.emplace_back(nscr, ncgr, nclr);
		
	}

	std::cin.get();

	for (unsigned int c = 0; c < combo.size(); c++) {
		auto[nscr, ncgr, nclr] = combo[c];
		std::cout << ncgr << " " <<  nclr << " " << nscr << std::endl;
		std::string oldDir = "data/bg/";
		std::string newDir = "data/bg" + std::to_string(c) + "/";
		std::experimental::filesystem::create_directory(newDir);

		std::string ncgrFilename = "bg_" + std::to_string(ncgr) + ".NCGR";
		std::string nscrFilename = "bg_" + std::to_string(nscr) + ".NSCR";
		std::string nclrFilename = "bg_" + std::to_string(nclr) + ".NCLR";

		std::experimental::filesystem::copy(oldDir + ncgrFilename, newDir + ncgrFilename, std::experimental::filesystem::copy_options::overwrite_existing);
		std::experimental::filesystem::copy(oldDir + nscrFilename, newDir + nscrFilename, std::experimental::filesystem::copy_options::overwrite_existing);
		std::experimental::filesystem::copy(oldDir + nclrFilename, newDir + nclrFilename, std::experimental::filesystem::copy_options::overwrite_existing);

		std::string cmd = narctool + " p " + newDir + " " + newDir.substr(0, newDir.size() - 1) + ".narc";
		system(cmd.c_str());
	}

	binFILE.close();

	std::cin.get();

	return 0;
}