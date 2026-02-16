#pragma once
#include "DocumentRepository.hpp"

class FileDocumentRepository : public DocumentRepository {
  void save(const Document& doc) override;
  std::optional<Document> retrieve(const std::string& id) override;
  bool remove(const std::string& id) override;
};