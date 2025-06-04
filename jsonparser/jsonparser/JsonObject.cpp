#include "JsonObject.h"
#include <sstream>

void JsonObject::add(const std::string& key, std::shared_ptr<JsonNode> value) {
	for (auto& pair : members) {
		if (pair.first == key) {
			pair.second = value;
			return;
		}
	}
	members.emplace_back(key, value);
}

std::string JsonObject::toString(int indent) const {
	std::ostringstream oss;
	oss << "{\n";
	for (size_t i = 0; i < members.size(); ++i) {
		oss << std::string(indent + 2, ' ')
			<< "\"" << members[i].first << "\": "
			<< members[i].second->toString(indent + 2);
		if (i != members.size() - 1) oss << ",";
		oss << "\n";
	}
	oss << std::string(indent, ' ') << "}";
	return oss.str();
}

std::shared_ptr<JsonNode> JsonObject::getByPath(const std::vector<std::string>& path, size_t index) const {
	if (index >= path.size()) return nullptr;
	for (const auto& pair : members) {
		if (pair.first == path[index]) {
			if (index == path.size() - 1) return pair.second;
			return pair.second->getByPath(path, index + 1);
		}
	}
	return nullptr;
}

bool JsonObject::createByPath(const std::vector<std::string>& path, const std::shared_ptr<JsonNode>& newValue, size_t index) {
	if (index >= path.size()) return false;

	for (auto& pair : members) {
		if (pair.first == path[index]) {
			if (index == path.size() - 1) {
				return false;
			}
			return pair.second->createByPath(path, newValue, index + 1);
		}
	}

	if (index == path.size() - 1) {
		members.emplace_back(path[index], newValue);
		return true;
	}
	else {
		auto newObj = std::make_shared<JsonObject>();
		members.emplace_back(path[index], newObj);
		return newObj->createByPath(path, newValue, index + 1);
	}
}


bool JsonObject::setByPath(const std::vector<std::string>& path, const std::shared_ptr<JsonNode>& newValue, size_t index) {
	if (index >= path.size()) return false;

	for (auto& pair : members) {
		if (pair.first == path[index]) {
			if (index == path.size() - 1) {
				pair.second = newValue; // замени
				return true;
			}
			return pair.second->setByPath(path, newValue, index + 1);
		}
	}
	return false;
}


bool JsonObject::deleteByPath(const std::vector<std::string>& path, size_t index) {
	if (index >= path.size()) return false;
	if (index == path.size() - 1) {
		for (auto it = members.begin(); it != members.end(); ++it) {
			if (it->first == path[index]) {
				members.erase(it);
				return true;
			}
		}
		return false;
	}
	for (auto& pair : members) {
		if (pair.first == path[index]) {
			return pair.second->deleteByPath(path, index + 1);
		}
	}
	return false;
}

void JsonObject::search(const std::string& key, std::vector<std::shared_ptr<JsonNode>>& results) const {
	std::regex pattern;
	bool isRegex = false;

	try {
		if (key.find('*') != std::string::npos) {
			std::string regexPattern = std::regex_replace(key, std::regex("\\*"), ".*");
			pattern = std::regex("^" + regexPattern + "$");
			isRegex = true;
		}
	}
	catch (...) {
		isRegex = false;
	}

	for (const auto& pair : members) {
		if ((isRegex && std::regex_match(pair.first, pattern)) || (!isRegex && pair.first == key)) {
			results.push_back(pair.second); 
		}
		pair.second->search(key, results); 
	}
}
