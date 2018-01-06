#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <string>
#include <experimental\filesystem>

int main(int argc, char** argv) {

	bool bNarc = false;

	/*std::experimental::filesystem::path binaryFile("data/arm9.bin");
	std::experimental::filesystem::path narctool("data/narctool.exe");
	std::experimental::filesystem::path bgDir("data/BG_/");*/
	
	std::experimental::filesystem::path narctool;
	std::experimental::filesystem::path binaryFile;
	std::experimental::filesystem::path bgDir;

	if (argc == 1) {
		std::cout << "Usage:\n";

		std::cout << "./DigiGraphics [/path/to/arm9.bin] [/dir/to/extracted/graphics/]\n\n";

		std::cout << "If you also want each graphic to be archived in a NARC, please use the following command:\n";

		std::cout << "./DigiGraphics [/path/to/arm9.bin] [/path/to/narctool.exe] [/dir/to/extracted/graphics/]\n\n";
		return 0;
	}
	else if (argc == 2) {
		if (argv[1] == "-v") {
			std::cout << "version: 0.1\n";
			return 0;
		}
		else {
			std::cout << "Incorrect Number of arguments. \n";
			std::cout << "./DigiGraphics [/path/to/arm9.bin] [/dir/to/extracted/graphics/]\n\n";
			std::cout << "or ./DigiGraphics -n [/path/to/arm9.bin] [/path/to/narctool.exe] [/dir/to/extracted/graphics/]\n\n";
			return 1;
		}
	}
	else if (argc == 5) {
		std::string arg = argv[1];
		if (arg == "-n") {
			bNarc = true;
			binaryFile = std::experimental::filesystem::path(argv[2]);
			narctool = std::experimental::filesystem::path(argv[3]);
			bgDir = std::experimental::filesystem::path(argv[4]);
		}
		else {
			std::cout << "Unrecognized command " << argv[1] << "\n";
			return 1;
		}
	}
	else if (argc != 3 && argc != 5) {
		std::cout << "Incorrect Number of arguments. \n";
		std::cout << "./DigiGraphics [/path/to/arm9.bin] [/dir/to/extracted/graphics/]\n\n";
		return 1;
	}
	else {
		binaryFile = std::experimental::filesystem::path(argv[1]);
		bgDir = std::experimental::filesystem::path(argv[2]);
	}

	std::string bgDirStem = bgDir.parent_path().filename().string();

	std::uint32_t combinationOffset = 0x00151BF4;
	std::uint32_t endingBytes = 0x5f47422f;
	std::ifstream binFILE;
	binFILE.open(binaryFile, std::ios::binary);
	if (!binFILE.is_open()) {
		std::cout << "Couldn't find the arm9.bin file\n";
		return 1;
	}
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
		
		combo.emplace_back(nscr, ncgr, nclr);
		
	}
	binFILE.close();
	for (unsigned int c = 0; c < combo.size(); c++) {
		auto[nscr, ncgr, nclr] = combo[c];
		std::experimental::filesystem::path newDir = bgDir.parent_path().string() + std::to_string(c) + "/";
		std::experimental::filesystem::create_directory(newDir);

		std::experimental::filesystem::path ncgrFilenameFrom(bgDir.string() + bgDirStem + "NCGR" + std::to_string(ncgr) + ".bin");
		std::experimental::filesystem::path nscrFilenameFrom(bgDir.string() + bgDirStem + "NSCR" + std::to_string(nscr) + ".bin");
		std::experimental::filesystem::path nclrFilenameFrom(bgDir.string() + bgDirStem + "NCLR" + std::to_string(nclr) + ".bin");

		std::experimental::filesystem::path ncgrFilenameTo(newDir.string() + bgDirStem + std::to_string(ncgr) + ".NCGR");
		std::experimental::filesystem::path nscrFilenameTo(newDir.string() + bgDirStem + std::to_string(nscr) + ".NSCR");
		std::experimental::filesystem::path nclrFilenameTo(newDir.string() + bgDirStem + std::to_string(nclr) + ".NCLR");

		std::experimental::filesystem::copy(ncgrFilenameFrom, ncgrFilenameTo, std::experimental::filesystem::copy_options::overwrite_existing);
		std::experimental::filesystem::copy(nscrFilenameFrom, nscrFilenameTo, std::experimental::filesystem::copy_options::overwrite_existing);
		std::experimental::filesystem::copy(nclrFilenameFrom, nclrFilenameTo, std::experimental::filesystem::copy_options::overwrite_existing);

		if (bNarc) {
			std::string cmd = narctool.string() + " p " + newDir.string() + " " + newDir.string().substr(0, newDir.string().size() - 1) + ".narc";
			system(cmd.c_str());
		}
	}

	return 0;
}