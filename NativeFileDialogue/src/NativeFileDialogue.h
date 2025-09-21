#pragma once

#include <string>
#include <iostream>
#include <vector>
namespace NFD 
{
    namespace Utils
    {
        std::vector<std::string> SplitString(const std::string& input, const std::string& seperator);
        std::vector<std::wstring> SplitStringW(const std::string& input, const std::string& seperator);
    }

    namespace ERROR_RESULT
    {
        constexpr const char* NOT_ENOUGH_FILTERS = "Not Enough Filters! - There need to be more than 1 filter";
    }

    std::string OpenFileDialogue();
    // Choose filters to exclude other file types
    // For example: ("*.exe;*.bat;*")
    // Format: *.(filter)  |  ; - seperator  |  * - all files
    std::string OpenFileDialogue(const char* filters);
    std::vector<std::string> OpenFilesDialogue();
    // Choose filters to exclude other file types
    // For example: ("*.exe;*.bat;*")
    // Format: *.(filter)  |  ; - seperator  |  * - all files
    std::vector<std::string> OpenFilesDialogue(const char* filters);
    std::string OpenFolderDialogue();
    std::vector<std::string> OpenFoldersDialogue();

    std::string OpenSaveFileDialogue();
    // Choose filters to exclude other file types
    // For example: ("*.exe;*.bat;*")
    // Format: *.(filter)  |  ; - seperator  |  * - all files
    std::string OpenSaveFileDialogue(const char* filters);
    std::string OpenSaveFolderDialogue();
}