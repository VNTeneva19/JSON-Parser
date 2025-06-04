#pragma once
#include "JsonNode.h"
#include <vector>
#include <string>
#include <memory>
#include <sstream>

class JsonArray : public JsonNode {
private:
	std::vector<std::shared_ptr<JsonNode>> elements;
public:
	void add(std::shared_ptr<JsonNode> value);

	std::string toString(int indent = 0) const override;
};