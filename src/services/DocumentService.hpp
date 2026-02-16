#pragma once
#include "../repository/DocumentRepository.hpp"
#include <cstdint>

class DocumentService {
public:
  explicit DocumentService(DocumentRepository& repo);

  std::string store(const std::string& name, const std::string& content);
  std::optional<Document> get(const std::string& id);
  bool remove(const std::string& id);

private:
  DocumentRepository& m_repo;
  uint32_t m_max_id{0};

};