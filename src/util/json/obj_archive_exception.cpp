#include "obj_archive_exception.h"

const char* ObjArchiveException::what() const noexcept {
    return msg;
}