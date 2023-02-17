#pragma once

struct ObjArchive;

struct ISerializable {
    virtual void save(ObjArchive& ar) const = 0;
    virtual void load(const ObjArchive& ar) = 0;
};