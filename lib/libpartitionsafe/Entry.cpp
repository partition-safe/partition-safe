//
// Created by Youri Lefers on 24-11-16.
//

#include "Entry.h"
#include "Common.h"

Entry::Entry(const FILINFO filinfo, const std::string directory):
    name(Common::tCharToStdString(filinfo.fname, sizeof(filinfo.fname))),
    altName(Common::tCharToStdString(filinfo.fname, sizeof(filinfo.altname))),
    size(filinfo.fsize), date(filinfo.fdate), time(filinfo.ftime),
    attribute(filinfo.fattrib), isDirectory(filinfo.fattrib & AM_DIR > 0),
    directory(directory) {}

std::string Entry::getFullPath() {
    return (directory.compare("/") > 0 ? directory : "") + "/" + name;
}
