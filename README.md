# NativeFileDialogue
Utility library for opening file/folder/save dialogues on your native platform

## Platform
This project is primarly supported on Windows (MSVC compiler tested).
Linux (Ubuntu, Debian) and MacOs support is in development.

## C++
C++11 or above is recommended

## Dependencies
No dependencies

## Getting Started
Visual Studio 2022 (and above) is recommended.

<ins>**1. Downloading the repository:**</ins>

Start cloning the repository with ```git clone https://github.com/LukaLominadze/NativeFileDialogue```

<ins>**2. Building the project:**</ins>

- *With premake*

After cloning the repository, go to the ```Scripts``` directory and run the ```Setup.py``` file.

- *With cmake*

After cloning the repository, run ```cmake -B build -G "BUILD_TYPE" .``` where BUILD_TYPE specifies what system to build for. for ex. ```cmake -B build -G "Visual Studio 17 2022" .```

You can build the ```Test``` project to get familiar with the library.

## Easy include
If you want to just use the library right away, Copy the ```NativeFileDialogue.h``` and ```NativeFileDialogue.cpp``` files from https://github.com/LukaLominadze/NativeFileDialogue/tree/main/NativeFileDialogue/src directory

## Project goals
- Linux (Ubuntu and Debian) support
- MacOs support

## Writing code

The file dialogue library is a collection of simple functions that usually return strings or vectors of strings as a result. the ```<filesystem>``` module has been ignored for support on older versions of C++.

The functions always start with the suffix ```Open``` so it's easy to navigate through them, for ex.
```cpp
std::string OpenFileDialogue();
std::string OpenFolderDialogue();
std::string OpenSaveFileDialogue(const char* filters);
```

The library content is inside the namespace ```NFD```.
Usage ex.
```cpp
std::string result = NFD::OpenFileDialogue();
```

### Using filters in NFD
You might want to restrict, which files the user can use or save and for that we use filters.
to use filters on file dialogues, you will have to follow this format.
- All filters are inside a single string
- Each format will be written as such: "\*.exe", "\*.ttf", etc.
- Multiple formats will be seperated by ';': "\*.exe;\*.ttf"
- To be able to choose ALL files, either use no filters or define it as such "\*", "\*.exe;*"
```cpp
std::string result = NFD::OpenFileDialogue("*.exe;*.ttf;*");
```

### All dialogues

*File dialogue*
```cpp
std::string result = NFD::OpenFileDialogue();
if (result.empty()) {
	std::cout << "Empty selection" << std::endl;
}
std::cout << result << std::endl;
```

*File dialogue with filters*
```cpp
std::string result = NFD::OpenFileDialogue("*.exe;*.bat;*");
if (result.empty()) {
	std::cout << "Empty selection" << std::endl;
}
std::cout << result << std::endl;
```

*Multiple files dialogue*
```cpp
std::vector<std::string> results = NFD::OpenFilesDialogue();
if (results.empty()) {
	std::cout << "Empty selections" << std::endl;
}
for (const std::string& res : results) {
	std::cout << res << std::endl;
}
```

*Multiple files dialogue with filters*
```cpp
std::vector<std::string> results = NFD::OpenFilesDialogue("*.png;*.jpg");
if (results.empty()) {
	std::cout << "Empty selections" << std::endl;
}
for (const std::string& res : results) {
	std::cout << res << std::endl;
}
```

*Folder dialogue*
```cpp
std::string result = NFD::OpenFolderDialogue();
if (result.empty()) {
	std::cout << "Empty selection" << std::endl;
}
std::cout << result << std::endl;
```

*Save file dialogue*
```cpp
std::string result = NFD::OpenSaveFileDialogue();
if (result.empty()) {
	std::cout << "Empty selection" << std::endl;
}
std::cout << result << std::endl;
```

*Save file dialogue with filters*
```cpp
std::string result = NFD::OpenSaveFileDialogue("*.exe;*.bat;*");
if (result.empty()) {
	std::cout << "Empty selection" << std::endl;
}
std::cout << result << std::endl;
```

*Save folder dialogue*
```cpp
std::string result = NFD::OpenSaveFolderDialogue();
if (result.empty()) {
	std::cout << "Empty selection" << std::endl;
}
std::cout << result << std::endl;
```
