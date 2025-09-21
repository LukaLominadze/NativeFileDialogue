#include <iostream>
#include "NativeFileDialogue.h"
#include <vector>

int main() {
	// File dialogue with filters
	std::string result = NFD::OpenFileDialogue("*.exe;*.bat;*");
	if (result.empty()) {
		std::cout << "Empty selection" << std::endl;
	}
	std::cout << result << std::endl;

	// Multiple files
	std::vector<std::string> results = NFD::OpenFilesDialogue();
	if (results.empty()) {
		std::cout << "Empty selections" << std::endl;
	}
	for (const std::string& res : results) {
		std::cout << res << std::endl;
	}
	
	// Multiple files with filters
	results = NFD::OpenFilesDialogue("*.png;*");
	if (results.empty()) {
		std::cout << "Empty selections" << std::endl;
	}
	for (const std::string& res : results) {
		std::cout << res << std::endl;
	}

	// Folder Dialogue
	result = NFD::OpenFolderDialogue();
	if (result.empty()) {
		std::cout << "Empty selection" << std::endl;
	}
	std::cout << result << std::endl;

	// Multiple Folders
	results = NFD::OpenFoldersDialogue();
	if (results.empty()) {
		std::cout << "Empty selections" << std::endl;
	}
	for (const std::string& res : results) {
		std::cout << res << std::endl;
	}

	// Save multiple files dialogue with filters
	result = NFD::OpenSaveFileDialogue("*.exe;*.bat;*");
	if (result.empty()) {
		std::cout << "Empty selection" << std::endl;
	}
	std::cout << result << std::endl;

	// Save folder dialogue
	result = NFD::OpenSaveFolderDialogue();
	if (result.empty()) {
		std::cout << "Empty selection" << std::endl;
	}
	std::cout << result << std::endl;
}