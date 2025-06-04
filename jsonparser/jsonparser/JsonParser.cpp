#include "JsonParser.h"

void JsonParser::skipWhitespace() {
	while (pos < json.size() && isspace(json[pos])) pos++;
}

char JsonParser::peek() const {
	return pos < json.size() ? json[pos] : '\0';
}

char JsonParser::consume() {
	return pos < json.size() ? json[pos++] : '\0';
}

std::string JsonParser::parseStringLiteral() {
	std::string result;
	consume(); 
	while (peek() != '"' && peek() != '\0') {
		if (peek() == '\\') {
			consume(); 
			char next = consume();
			result += (next == '"' ? '"' : next); 
		}
		else {
			result += consume();
		}
	}
	consume();
	return result;
}

std::shared_ptr<JsonNode> JsonParser::parseValue() {
	skipWhitespace();
	char c = peek();
	if (c == '{') return parseObject();
	else if (c == '[') return parseArray();
	else if (c == '"') return std::make_shared<JsonValue>("\"" + parseStringLiteral() + "\"");
	else return nullptr;
}

std::shared_ptr<JsonNode> JsonParser::parseObject() {
	consume();
	auto obj = std::make_shared<JsonObject>();
	skipWhitespace();
	if (peek() == '}') { consume(); return obj; }

	while (true) {
		skipWhitespace();
		if (peek() != '"') return nullptr;
		std::string key = parseStringLiteral();

		skipWhitespace();
		if (consume() != ':') return nullptr;

		skipWhitespace();
		auto value = parseValue();
		if (!value) return nullptr;

		obj->add(key, value);

		skipWhitespace();
		if (peek() == '}') { consume(); break; }
		if (consume() != ',') return nullptr;
	}

	return obj;
}

std::shared_ptr<JsonNode> JsonParser::parseArray() {
	consume();
	auto arr = std::make_shared<JsonArray>();
	skipWhitespace();
	if (peek() == ']') { consume(); return arr; }

	while (true) {
		skipWhitespace();
		auto val = parseValue();
		if (!val) return nullptr;

		arr->add(val);

		skipWhitespace();
		if (peek() == ']') { consume(); break; }
		if (consume() != ',') return nullptr;
	}

	return arr;
}

bool JsonParser::parse(const std::string& input) {
	json = input;
	pos = 0;
	root = parseValue();
	return root != nullptr;
}


std::shared_ptr<JsonNode> JsonParser::getRoot() const {
	return root;
}