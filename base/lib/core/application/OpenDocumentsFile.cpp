// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <QFile>
#include <QStandardPaths>
#include <QStringList>

#include "OpenDocumentsFile.hpp"

namespace iscore
{
QString OpenDocumentsFile::path()
{
  static QString path = [] {
    auto paths
        = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
    return paths.first() + "/iscore_open_docs";
  }();

  return path;
}

bool OpenDocumentsFile::exists()
{
  return QFile::exists(OpenDocumentsFile::path());
}
}
