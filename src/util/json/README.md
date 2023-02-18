# ObjArchive - easy json serialization

## Usage

```c++
#include "string2.h"
#include "obj_archive.h"

struct Point2 : ISerializable {
	int x, y;
	
	void save(ObjArchive& ar) const {
		ar.put("x", &x);
		ar.put("Value_y", &y);
	}
	
	void load(const ObjArchive& ar) {
		ar.get("x", &x);
		ar.get("Value_y", &y);
	}
};

int main() {
	Point2 p;
	p.x = 10;
	p.y = 15;
	string2 p_as_json = ObjArchive::to_str(&p); // object -> json
	
	string2 p2_as_json = R"(
		{
			"x": 22,
			"Value_y": -50
		}
	)";
	Point2 p2 = ObjArchive::from_str<Point2>(p2_as_json); // json -> object
	
	return 0;
}
```

To make your class serializable:
1) Inherit from `ISerializable`.
2) Override `save(ObjArchive&)` and `load(ObjArchive&)` using `put` and `get`.

To (de)serialize your class:
1) Serialize with `ObjArchive::to_str(&obj);`.
2) Deserialize with `MyObj obj = ObjArchive::from_str<MyObj>(json_string);`

## Design choices

The primary focus `ObjArchive` is having an easy, unobtrusive API. `ObjArchive` is meant for end-to-end serialization and not directly manipulating an `ObjArchive` instance (though there are methods providing this functionality, such as `get` and `put`).

## Inner workings

```c++
struct ObjArchive {
	std::map<string2, string2> map;
	std::vector<string2> array;
	string2 literal;
	int type;
};
```

The archive object functions as a sort of union (not a C `union`) between the 3 groups of values in json - `literal`, `list`, `dictionary`.

When you feed some object into `ObjArchive` through `put(key, val)`, `insert(val)`, `set(val)`, it'll automatically change its `type` to accommodate.

When `ObjArchive` parses a json string, it'll infer the type based on the structure of the string.

To support generic conversion to/from strings, template functions are used. There are several "groups" of template functions needed:
1) For types satisfying `std::is_arithmetic` (int, long, float, double, ...), conversion is done using `stringstream`.
2) For types inheriting from `ISerializable`, `ISerializable::load()`, `ISerializable::save()` is called.
3) For `string2`, conversion is trivial (except for adding, omitting double quotes).
4) For lists (`std::vector`), each element is generically converted based on the underlying type.