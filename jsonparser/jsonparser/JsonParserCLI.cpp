#include "JsonParserCLI.h"

void JsonParserCLI::open(const std::string& filename) {
	std::ifstream file(filename);
	if (filename.empty() || filename.find_first_not_of(" \t\n\r") == std::string::npos) {
		std::cout << "Error: No filename specified.\n";
		return;
	}
	if (file) {
		std::stringstream buffer;
		buffer << file.rdbuf();
		JsonParser parser;
		if (parser.parse(buffer.str())) {
			currentJson = parser.getRoot();
			currentFile = filename;
			std::cout << "File opened successfully.\n";
		}
		else {
			std::cout << "Failed to parse JSON.\n";
		}
	}
	else {
		std::cout << "Could not open file.\n";
	}
}

void JsonParserCLI::save() {
	if (currentFile.empty() || !currentJson) {
		std::cout << "No file loaded.\n";
		return;
	}
	std::ofstream file(currentFile);
	file << currentJson->toString();
	std::cout << "File saved.\n";
}

void JsonParserCLI::saveAs(const std::string& filename) {
	if (filename.empty() || filename.find_first_not_of(" \t\n\r") == std::string::npos) {
		std::cout << "Error: No filename specified.\n";
		return;
	}

	if (!currentJson) {
		std::cout << "No JSON to save.\n";
		return;
	}

	std::ofstream outFile(filename);
	if (!outFile) {
		std::cout << "Could not open file for writing.\n";
		return;
	}

	outFile << currentJson->toString();
	currentFile = filename;
	std::cout << "File saved as " << filename << ".\n";
}

bool JsonParserCLI::validate(const std::string& input) {
	JsonParser parser;
	if (parser.parse(input)) {
		std::cout << "JSON is valid.\n";
		return true;
	}
	else {
		std::cout << "Error: invalid JSON syntax.\n";
		return false;
	}
}

void JsonParserCLI::print() {
	if (!currentJson) {
		std::cout << "No file loaded.\n";
		return;
	}
	std::cout << currentJson->toString() << "\n";
}

void JsonParserCLI::search(const std::string& key) {
	if (key.empty()) {
		std::cout << "Cannot search blank key.\n";
		return;
	}
	if (!currentJson) {
		std::cout << "No file loaded.\n";
		return;
	}

	std::vector<std::shared_ptr<JsonNode>> results;
	currentJson->search(key, results);

	std::cout << "[\n";
	for (size_t i = 0; i < results.size(); ++i) {
		std::cout << "  " << results[i]->toString();
		if (i + 1 != results.size()) std::cout << ",";
		std::cout << "\n";
	}
	std::cout << "]\n";
}

void JsonParserCLI::deleteKey(const std::string& pathStr) {
	if (!currentJson) return;
	auto path = splitPath(pathStr);
	if (currentJson->deleteByPath(path)) std::cout << "Deleted.\n";
	else std::cout << "Not found.\n";
}

void JsonParserCLI::setValue(const std::string& pathStr, const std::string& valueStr) {
	if (!currentJson) {
		std::cout << "No file loaded.\n";
		return;
	}
	auto path = splitPath(pathStr);

	JsonParser parser;
	if (!parser.parse(valueStr)) {
		std::cout << "Invalid JSON value.\n";
		return;
	}
	auto newValue = parser.getRoot();
	if (!newValue) {
		std::cout << "Invalid JSON value.\n";
		return;
	}

	if (currentJson->setByPath(path, newValue)) {
		std::cout << "Value set.\n";
	}
	else {
		std::cout << "Path not found or could not set value.\n";
	}
}

void JsonParserCLI::createValue(const std::string& pathStr, const std::string& valueStr) {
	if (!currentJson) {
		std::cout << "No JSON loaded.\n";
		return;
	}

	auto path = splitPath(pathStr);

	JsonParser parser;
	if (!parser.parse(valueStr)) {
		std::cout << "Invalid JSON value: " << valueStr << "\n";
		return;
	}
	auto newNode = parser.getRoot();

	if (currentJson->createByPath(path, newNode)) {
		std::cout << "Element created successfully.\n";
	}
	else {
		std::cout << "Element already exists or could not create element.\n";
	}
}


void JsonParserCLI::help() {
	std::cout << "Commands: open <file>, save, saveas <file>, print, validate, search <key>, create <path> <string>, set <path> <string>, delete <path>, help, exit\n";
}

void JsonParserCLI::repl() {
	std::string line;
	while (true) {
		std::cout << "> ";
		std::getline(std::cin, line);
		std::istringstream iss(line);
		std::string cmd;
		iss >> cmd;
		if (cmd == "open") {
			std::string file; iss >> file; open(file);
		}
		else if (cmd == "save") save();
		else if (cmd == "saveas") {
			std::string file; iss >> file; saveAs(file);
		}
		else if (cmd == "print") print();
		else if (cmd == "validate") {
			if (!currentJson) {
				std::cout << "No file loaded.\n";
			}
			else {
				validate(currentJson->toString());
			}
		}
		else if (cmd == "search") {
			std::string key; iss >> key; search(key);
		}
		else if (cmd == "set") {
			std::string path, val; iss >> path >> val; setValue(path, val);
		}
		else if (cmd == "create") {
			std::string path, val; iss >> path >> val; createValue(path, val);
		}
		else if (cmd == "delete") {
			std::string path; iss >> path; deleteKey(path);
		}
		else if (cmd == "help") help();
		else if (cmd == "exit") break;
		else std::cout << "Unknown command.\n";
	}
}
std::vector<std::string> JsonParserCLI::splitPath(const std::string& pathStr) {
	std::vector<std::string> path;
	std::string segment;
	for (size_t i = 0; i < pathStr.size(); ++i) {
		if (pathStr[i] == '/') {
			if (!segment.empty()) {
				path.push_back(segment);
				segment.clear();
			}
		}
		else {
			segment.push_back(pathStr[i]);
		}
	}
	if (!segment.empty()) path.push_back(segment);
	return path;
}

