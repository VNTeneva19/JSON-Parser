#include "JsonValue.h"

std::string JsonValue::toString(int indent) const {
	if (value.front() == '"' && value.back() == '"') {
		return value;
	}
	return value;
}
