#include "DocumentService.hpp"

DocumentService::DocumentService(DocumentRepository& repo) : m_repo(repo) {}

std::string DocumentService::store(const std::string& name, const std::string& content) {
  Document doc;
  doc.id = std::to_string(++m_max_id);
  doc.author = name;
  doc.content = content;
  m_repo.save(doc);
  return std::to_string(m_max_id);
}

std::optional<Document> DocumentService::get(const std::string& id) {
  return m_repo.retrieve(id);
}

bool DocumentService::remove(const std::string& id) {
  return m_repo.remove(id);
}