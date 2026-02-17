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

    DocumentService::s_used_ids.insert(std::stoul(stem));
  }
  std::cout << "Found " << DocumentService::s_used_ids.size() << " files\n";
}

std::string DocumentService::store(const std::string& name, const std::string& content)
{
  Document doc;

  std::uint32_t i;
  for (i = 0; i < std::numeric_limits<std::uint32_t>::max(); ++i)
    if (DocumentService::s_used_ids.find(i) == DocumentService::s_used_ids.end()) {
      DocumentService::s_used_ids.emplace(i);
      doc.id = std::to_string(i);
      doc.author = name;
      doc.content = content;
      m_repo.save(doc);
      return std::to_string(i);
    }
  throw std::runtime_error("The number of files the service could handle has been reached!");
}

std::optional<Document> DocumentService::get(const std::string& id)
{
  return m_repo.retrieve(id);
}

bool DocumentService::remove(const std::string& id)
{
  try {
    DocumentService::s_used_ids.erase(std::stoul(id));
  }
  catch (const std::exception& e) {
    return false;
  }
  if (m_repo.remove(id)) {
    DocumentService::s_used_ids.erase(std::stoul(id));
    return true;
  }
  return false;
}