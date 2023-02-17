#pragma once

struct ObjArchive;

class ISerializable {
public:
    virtual void save(ObjArchive& ar) const = 0;
    virtual void load(const ObjArchive& ar) = 0;
};