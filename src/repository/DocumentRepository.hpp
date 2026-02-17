#pragma once
#include <optional>
#include <string>
#include <string_view>

struct Document {
  std::string id;
  std::string author;
  std::string content;
  static inline constexpr std::string_view extension = ".DFY";
};

class DocumentRepository {
public:
  virtual void save(const Document& doc) = 0;
  virtual std::optional<Document> retrieve(const std::string& id) = 0;
  virtual bool remove(const std::string& id) = 0;
  virtual ~DocumentRepository() = default;
};