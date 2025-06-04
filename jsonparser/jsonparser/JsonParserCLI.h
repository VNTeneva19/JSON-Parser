#pragma once
#include "JsonParser.h"
#include <fstream>
#include <iostream>

class JsonParserCLI {
private:
	std::shared_ptr<JsonNode> currentJson;
	std::string currentFile;

	std::vector<std::string> splitPath(const std::string& pathStr);
public:
	void open(const std::string& filename);

	void save();

	void saveAs(const std::string& filename);

	bool validate(const std::string& input);

	void print();

	void search(const std::string& key);

	void deleteKey(const std::string& pathStr);

	void setValue(const std::string& pathStr, const std::string& value);

	void createValue(const std::string& pathStr, const std::string& valueStr);

	void help();

	void repl();
};