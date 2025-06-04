#pragma once
#include "JsonNode.h"
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <regex>

class JsonObject : public JsonNode {
private:
	std::vector<std::pair<std::string, std::shared_ptr<JsonNode>>> members;
public:
	void add(const std::string& key, std::shared_ptr<JsonNode> value);
	
	std::string toString(int indent = 0) const override;

	std::shared_ptr<JsonNode> getByPath(const std::vector<std::string>& path, size_t index = 0) const override;

	bool createByPath(const std::vector<std::string>& path, const std::shared_ptr<JsonNode>& newValue, size_t index = 0) override;

	bool setByPath(const std::vector<std::string>& path, const std::shared_ptr<JsonNode>& newValue, size_t index = 0) override;

	bool deleteByPath(const std::vector<std::string>& path, size_t index = 0) override;

	void search(const std::string& key, std::vector<std::shared_ptr<JsonNode>>& results) const override;
};