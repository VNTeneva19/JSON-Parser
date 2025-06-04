#include "JsonArray.h"

void JsonArray::add(std::shared_ptr<JsonNode> value) {
    elements.push_back(value);
}

std::string JsonArray::toString(int indent) const {
    std::ostringstream oss;
    oss << "[\n";
    for (size_t i = 0; i < elements.size(); ++i) {
        oss << std::string(indent + 2, ' ')
            << elements[i]->toString(indent + 2);
        if (i != elements.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << std::string(indent, ' ') << "]";
    return oss.str();
}