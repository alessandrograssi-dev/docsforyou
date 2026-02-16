#include "FileDocumentRepository.hpp"
#include <fstream>
#include <string>
#include <exception>
#include <filesystem>

void FileDocumentRepository::save(const Document& doc){
  std::string filename = doc.id + ".txt";
  std::ofstream fs(filename);
  if (!fs.is_open())
    throw std::runtime_error("Could not open file to write");
  fs << doc.author << '\n';
  fs << "---\n";
  fs << doc.content << '\n';
  fs.close();
}

std::optional<Document> FileDocumentRepository::retrieve(const std::string& id){
  std::ifstream fs(id + ".txt");
    if (!fs) return std::nullopt;

    Document doc;
    doc.id = id;

    std::getline(fs, doc.author);
    std::string separator;
    std::getline(fs, separator);

    std::string line;
    while (std::getline(fs, line)) {
        doc.content += line + "\n";
    }

    return doc;
}

bool FileDocumentRepository::remove(const std::string& id){
  std::string filename = id + ".txt";
  return std::filesystem::remove(filename);
}