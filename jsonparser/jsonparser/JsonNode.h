#pragma once
#include <string>
#include <memory>
#include <vector>
class JsonNode {
public:
    virtual ~JsonNode() = default;
    virtual std::string toString(int indent = 0) const = 0;
    virtual std::shared_ptr<JsonNode> getByPath(const std::vector<std::string>& path, size_t index = 0) const { return nullptr; }
    virtual bool createByPath(const std::vector<std::string>& path, const std::shared_ptr<JsonNode>& newValue, size_t index = 0) { return false; }
    virtual bool setByPath(const std::vector<std::string>& path, const std::shared_ptr<JsonNode>& newValue, size_t index = 0) { return false; }
    virtual bool deleteByPath(const std::vector<std::string>& path, size_t index = 0) { return false; }
    virtual void search(const std::string& key, std::vector<std::shared_ptr<JsonNode>>& results) const {}
};