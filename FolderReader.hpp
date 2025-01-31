//
//  FolderReader.hpp
//  Database5
//
//  Created by rick gessner on 4/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef FolderReader_h
#define FolderReader_h

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace ECE141
{

class FolderListener
{
public:
  virtual bool operator()(const std::string &aName) = 0;
};

class FolderReader
{
public:
  FolderReader(const char *aPath) : path(aPath) {}
  virtual ~FolderReader() {}
  virtual bool exists(const std::string &aPath)
  {
    //STUDENT: add logic to see if FOLDER at given path exists.
    return fs::exists(aPath);
  }

  virtual void each(FolderListener &aListener, const std::string &anExtension) const
  {
    //STUDENT: iterate db's, pass the name of each to listener
    for (auto &p : fs::directory_iterator(path))
      aListener(p.path().u8string());
  }

  std::string path;
};

} // namespace ECE141

#endif /* FolderReader_h */
