#pragma once
#include "JsonNode.h"
#include <string>

class JsonValue : public JsonNode {
private:
	std::string value;
public:
	JsonValue(const std::string& val) : value(val) {};
	std::string toString(int indent = 0) const override;
};