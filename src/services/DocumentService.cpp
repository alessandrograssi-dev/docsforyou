#include "DocumentService.hpp"
#include <filesystem>
#include <iostream>

DocumentService::DocumentService(DocumentRepository& repo) : m_repo(repo)
{
  initialize();
}

void DocumentService::initialize()
{
  const auto dir = std::filesystem::current_path();

  for (const auto& entry : std::filesystem::directory_iterator(dir)) {
    if (!entry.is_regular_file()) {
      continue;
    }

    const auto path = entry.path();
    if (path.extension() != Document::extension) {
      continue;
    }

    const auto stem = path.stem().string();
    if (stem.empty() || stem.find_first_not_of("0123456789") != std::string::npos) {
      continue;
    }

    std::uint32_t id = std::stoul(stem);
    if (id > m_max_id)
      m_max_id = id;

    DocumentService::m_used_ids.insert(id);
  }
  std::cout << "Found " << DocumentService::m_used_ids.size() << " files\n";
}

std::string DocumentService::store(const std::string& name, const std::string& content)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  while (m_used_ids.find(m_max_id) != m_used_ids.end())
    ++m_max_id;
  
  Document doc;
  doc.id = std::to_string(m_max_id);
  doc.author = name;
  doc.content = content;
  m_repo.save(doc);
  m_used_ids.emplace(m_max_id);
  return std::to_string(m_max_id++);
}

std::optional<Document> DocumentService::get(const std::string& id)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_repo.retrieve(id);
}

bool DocumentService::remove(const std::string& id)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  std::uint32_t numeric_id;
  try {
    numeric_id = std::stoul(id);
  }
  catch (const std::exception& e) {
    return false;
  }
  if (m_repo.remove(id)) {
    DocumentService::m_used_ids.erase(numeric_id);
    return true;
  }
  return false;
}