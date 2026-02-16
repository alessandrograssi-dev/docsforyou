#pragma once
#include <string>
#include <optional>

struct Document {
  std::string id;
  std::string author;
  std::string content;
};

class DocumentRepository {
public:
  virtual void save(const Document& doc) = 0;
  virtual std::optional<Document> retrieve(const std::string& id) = 0;
  virtual bool remove(const std::string& id) = 0;
  virtual ~DocumentRepository() = default;
};