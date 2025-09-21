#include "NativeFileDialogue.h"
#ifdef _WIN32
#include <windows.h>
#include <shobjidl.h> 

namespace NFD
{
namespace Utils
{
    std::vector<std::string> SplitString(const std::string& input, const std::string& seperator) {
        std::vector<std::string> tokens;

        size_t offset = 0;
        size_t index = input.find_first_of(seperator, offset);
        // we will use this to determine how much space to allocate for all tokens
        size_t count = 1;

        // if there's no conditions for splitting, return the result
        if (index == std::string::npos) {
            tokens.emplace_back(input);
            return tokens;
        }

        // we will find all the tokens first
        while (index != std::string::npos) {
            count++;
            offset = (index + seperator.size());
            index = input.find_first_of(seperator, offset);
        }

        // reserve and pre-allocate space for all tokens
        tokens.reserve(count);

        offset = 0;
        index = input.find_first_of(seperator, offset);

        // push all tokens into the vector
        while (index != std::string::npos) {
            size_t size = index - offset;
            tokens.emplace_back(input.substr(offset, size));

            offset = (index + seperator.size());
            index = input.find_first_of(seperator, offset);
        }

        // insert the last token into the vector
        size_t size = input.size() - offset;
        if (size != 0) {
            tokens.emplace_back(input.substr(offset, size));
        }

        return tokens;
    }

    std::vector<std::wstring> SplitStringW(const std::string& input, const std::string& seperator) {
        std::vector<std::wstring> tokens;

        wchar_t* wpInput = (wchar_t*)malloc(sizeof(wchar_t) * (input.size() + 1));
        mbstowcs(wpInput, input.data(), input.size());
        *(wpInput + input.size()) = L'\0';
        std::wstring wInput(wpInput);
        free(wpInput);

        wchar_t* wpSeperator = (wchar_t*)malloc(sizeof(wchar_t) * (seperator.size() + 1));
        mbstowcs(wpSeperator, seperator.data(), seperator.size());
        *(wpSeperator + seperator.size()) = L'\0';
        std::wstring wSeperator(wpSeperator);
        free(wpSeperator);

        size_t offset = 0;
        size_t index = input.find_first_of(seperator, offset);
        // we will use this to determine how much space to allocate for all tokens
        size_t count = 1;

        // if there's no conditions for splitting, return the result
        if (index == std::wstring::npos) {
            tokens.emplace_back(wInput);
            return tokens;
        }

        // we will find all the tokens first
        while (index != std::string::npos) {
            count++;
            offset = (index + seperator.size());
            index = input.find_first_of(seperator, offset);
        }

        // reserve the space for all tokens and pre-allocate space
        // for all tokens in a contigious block of memory
        tokens.reserve(count);

        offset = 0;
        index = input.find_first_of(seperator, offset);

        // push all tokens into the vector
        while (index != std::string::npos) {
            size_t count = (index - offset);
            tokens.emplace_back(wInput.substr(offset, count));

            offset = (index + seperator.size());
            index = input.find_first_of(seperator, offset);
        }

        // insert the last token into the vector
        count = (input.size() - offset);
        if (count != 0) {
            tokens.emplace_back(wInput.substr(offset, count));
        }

        return tokens;
    }
}


std::string OpenFileDialogue()
{
    std::string strFilePath;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Get the name as a result
                    if (SUCCEEDED(hr))
                    {
                        size_t pszFilePathLen = wcslen(pszFilePath) + 1;

                        char* pcFilePath = (char*)malloc(sizeof(char) * pszFilePathLen);
                        wcstombs(pcFilePath, pszFilePath, pszFilePathLen);

                        strFilePath = std::string(pcFilePath);

                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return strFilePath;
}

std::string OpenFileDialogue(const char* filters)
{
    std::string strFilePath;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        std::vector<std::wstring> wFilters = Utils::SplitStringW(filters, ";");
        if (wFilters.size() <= 1) {
            CoUninitialize();
            return ERROR_RESULT::NOT_ENOUGH_FILTERS;
        }

        std::vector<COMDLG_FILTERSPEC> ComDlgFilterSpecs;
        ComDlgFilterSpecs.reserve(wFilters.size());
        for (std::wstring& filter : wFilters) {
            ComDlgFilterSpecs.push_back({ filter.c_str(), filter.c_str() });
        }
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            pFileOpen->SetFileTypes(ComDlgFilterSpecs.size(), ComDlgFilterSpecs.data());

            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Get the name as a result
                    if (SUCCEEDED(hr))
                    {
                        size_t pszFilePathLen = wcslen(pszFilePath) + 1;

                        char* pcFilePath = (char*)malloc(sizeof(char) * pszFilePathLen);
                        wcstombs(pcFilePath, pszFilePath, pszFilePathLen);

                        strFilePath = std::string(pcFilePath);

                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }

        CoUninitialize();
    }
    return strFilePath;
}

std::vector<std::string> OpenFilesDialogue()
{
    std::vector<std::string> strFilePaths;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            FILEOPENDIALOGOPTIONS mOptions = FOS_ALLOWMULTISELECT;
            pFileOpen->SetOptions(mOptions);

            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItemArray* pItems;
                hr = pFileOpen->GetResults(&pItems);
                if (SUCCEEDED(hr))
                {
                    DWORD mItemCount;
                    hr = pItems->GetCount(&mItemCount);
                    if (SUCCEEDED(hr)) {
                        IShellItem* pItem;
                        for (DWORD i = 0; i < mItemCount; i++) {
                            hr = pItems->GetItemAt(i, &pItem);
                            if (SUCCEEDED(hr)) {
                                PWSTR pszFilePath;
                                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                // Get the name as a result
                                if (SUCCEEDED(hr))
                                {
                                    size_t pszFilePathLen = wcslen(pszFilePath) + 1;

                                    char* pcFilePath = (char*)malloc(sizeof(char) * pszFilePathLen);
                                    wcstombs(pcFilePath, pszFilePath, pszFilePathLen);

                                    strFilePaths.emplace_back(pcFilePath);

                                    CoTaskMemFree(pszFilePath);
                                }
                                pItem->Release();
                            }
                        }
                    }
                }
            }
            pFileOpen->Release();
        }

        CoUninitialize();
    }
    return strFilePaths;
}

std::vector<std::string> OpenFilesDialogue(const char* filters)
{
    std::vector<std::string> strFilePaths;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        std::vector<std::wstring> wFilters = Utils::SplitStringW(filters, ";");
        if (wFilters.size() <= 1) {
            CoUninitialize();
            return strFilePaths;
        }

        std::vector<COMDLG_FILTERSPEC> ComDlgFilterSpecs;
        ComDlgFilterSpecs.reserve(wFilters.size());
        for (std::wstring& filter : wFilters) {
            ComDlgFilterSpecs.push_back({ filter.c_str(), filter.c_str() });
        }
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            FILEOPENDIALOGOPTIONS mOptions = FOS_ALLOWMULTISELECT;
            pFileOpen->SetOptions(mOptions);
            pFileOpen->SetFileTypes(ComDlgFilterSpecs.size(), ComDlgFilterSpecs.data());

            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItemArray* pItems;
                hr = pFileOpen->GetResults(&pItems);
                if (SUCCEEDED(hr))
                {
                    DWORD mItemCount;
                    hr = pItems->GetCount(&mItemCount);
                    if (SUCCEEDED(hr)) {
                        IShellItem* pItem;
                        for (DWORD i = 0; i < mItemCount; i++) {
                            hr = pItems->GetItemAt(i, &pItem);
                            if (SUCCEEDED(hr)) {
                                PWSTR pszFilePath;
                                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                // Get the name as a result
                                if (SUCCEEDED(hr))
                                {
                                    size_t pszFilePathLen = wcslen(pszFilePath) + 1;

                                    char* pcFilePath = (char*)malloc(sizeof(char) * pszFilePathLen);
                                    wcstombs(pcFilePath, pszFilePath, pszFilePathLen);

                                    strFilePaths.emplace_back(pcFilePath);

                                    CoTaskMemFree(pszFilePath);
                                }
                                pItem->Release();
                            }
                        }
                    }
                }
            }
            pFileOpen->Release();
        }

        CoUninitialize();
    }
    return strFilePaths;
}

std::string OpenFolderDialogue()
{
    std::string strFilePath;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            FILEOPENDIALOGOPTIONS options = FOS_PICKFOLDERS;
            pFileOpen->SetOptions(options);

            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Get the name as a result
                    if (SUCCEEDED(hr))
                    {
                        size_t pszFilePathLen = wcslen(pszFilePath) + 1;

                        char* pcFilePath = (char*)malloc(sizeof(char) * pszFilePathLen);
                        wcstombs(pcFilePath, pszFilePath, pszFilePathLen);

                        strFilePath = std::string(pcFilePath);

                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return strFilePath;
}

std::vector<std::string> OpenFoldersDialogue()
{
    std::vector<std::string> strFolderPaths;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            FILEOPENDIALOGOPTIONS mOptions = FOS_PICKFOLDERS | FOS_ALLOWMULTISELECT;
            pFileOpen->SetOptions(mOptions);

            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItemArray* pItems;
                hr = pFileOpen->GetResults(&pItems);
                if (SUCCEEDED(hr))
                {
                    DWORD mItemCount;
                    hr = pItems->GetCount(&mItemCount);
                    if (SUCCEEDED(hr)) {
                        IShellItem* pItem;
                        for (DWORD i = 0; i < mItemCount; i++) {
                            hr = pItems->GetItemAt(i, &pItem);
                            if (SUCCEEDED(hr)) {
                                PWSTR pszFilePath;
                                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                                // Get the name as a result
                                if (SUCCEEDED(hr))
                                {
                                    size_t pszFilePathLen = wcslen(pszFilePath) + 1;

                                    char* pcFilePath = (char*)malloc(sizeof(char) * pszFilePathLen);
                                    wcstombs(pcFilePath, pszFilePath, pszFilePathLen);

                                    strFolderPaths.emplace_back(pcFilePath);

                                    CoTaskMemFree(pszFilePath);
                                }
                                pItem->Release();
                            }
                        }
                    }
                }
            }
            pFileOpen->Release();
        }

        CoUninitialize();
    }
    return strFolderPaths;
}

std::string OpenSaveFileDialogue()
{
    std::string strFilePath;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileSaveDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
            IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Get the name as a result
                    if (SUCCEEDED(hr))
                    {
                        size_t pszFilePathLen = wcslen(pszFilePath) + 1;

                        char* pcFilePath = (char*)malloc(sizeof(char) * pszFilePathLen);
                        wcstombs(pcFilePath, pszFilePath, pszFilePathLen);

                        strFilePath = std::string(pcFilePath);

                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return strFilePath;
}

std::string OpenSaveFileDialogue(const char* filters)
{
    std::string strFilePath;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        std::vector<std::wstring> wFilters = Utils::SplitStringW(filters, ";");
        if (wFilters.size() <= 1) {
            CoUninitialize();
            return ERROR_RESULT::NOT_ENOUGH_FILTERS;
        }

        std::vector<COMDLG_FILTERSPEC> ComDlgFilterSpecs;
        ComDlgFilterSpecs.reserve(wFilters.size());
        for (std::wstring& filter : wFilters) {
            ComDlgFilterSpecs.push_back({ filter.c_str(), filter.c_str() });
        }
        IFileSaveDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
            IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            pFileOpen->SetFileTypes(ComDlgFilterSpecs.size(), ComDlgFilterSpecs.data());

            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Get the name as a result
                    if (SUCCEEDED(hr))
                    {
                        size_t pszFilePathLen = wcslen(pszFilePath) + 1;

                        char* pcFilePath = (char*)malloc(sizeof(char) * pszFilePathLen);
                        wcstombs(pcFilePath, pszFilePath, pszFilePathLen);

                        strFilePath = std::string(pcFilePath);

                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }

        CoUninitialize();
    }
    return strFilePath;
}

std::string OpenSaveFolderDialogue()
{
    std::string strFilePath;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileSaveDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
            IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            FILEOPENDIALOGOPTIONS options = FOS_PICKFOLDERS;
            pFileOpen->SetOptions(options);

            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Get the name as a result
                    if (SUCCEEDED(hr))
                    {
                        size_t pszFilePathLen = wcslen(pszFilePath) + 1;

                        char* pcFilePath = (char*)malloc(sizeof(char) * pszFilePathLen);
                        wcstombs(pcFilePath, pszFilePath, pszFilePathLen);

                        strFilePath = std::string(pcFilePath);

                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return strFilePath;
}
#endif
}