#pragma once
#include "../repository/DocumentRepository.hpp"
#include <cstdint>
#include <mutex>
#include <unordered_set>

class DocumentService {
public:
  explicit DocumentService(DocumentRepository& repo);

  std::string store(const std::string& name, const std::string& content);
  std::optional<Document> get(const std::string& id);
  bool remove(const std::string& id);

private:
  DocumentRepository& m_repo;
  std::unordered_set<std::uint32_t> m_used_ids{};
  std::uint32_t m_max_id{0};
  std::mutex m_mutex;

  void initialize();
};