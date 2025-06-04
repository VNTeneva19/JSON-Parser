#pragma once
#include "JsonNode.h"
#include "JsonArray.h"
#include "JsonValue.h"
#include "JsonObject.h"
#include <string>
#include <memory>

class JsonParser {
private:
	std::shared_ptr<JsonNode> root;
	std::string json;
	size_t pos = 0;

	void skipWhitespace();

	char peek() const;

	char consume();

	std::string parseStringLiteral();

	std::shared_ptr<JsonNode> parseValue();

	std::shared_ptr<JsonNode> parseObject();

	std::shared_ptr<JsonNode> parseArray();

public:
	bool parse(const std::string& input); 

	std::shared_ptr<JsonNode> getRoot() const;
};