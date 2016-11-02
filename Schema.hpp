#ifndef H_Schema_hpp
#define H_Schema_hpp

#include <vector>
#include <string>
#include "Types.hpp"

struct Schema {
   struct Relation {
      struct Attribute {
         std::string name;
         Types::Tag type;
         unsigned len1;
         unsigned len2;
         bool notNull;
         Attribute() : len1(~0), len2(~0), notNull(true) {}
      };
      struct Index {
    	  std::string name;
    	  std::vector<unsigned> attributes;
          Index(const std::string& name) : name(name) {}
      };
      std::string name;
      std::vector<Schema::Relation::Attribute> attributes;
      std::vector<unsigned> primaryKey;
      std::vector<Schema::Relation::Index> indexes;
      Relation(const std::string& name) : name(name) {}
   };
   std::vector<Schema::Relation> relations;
   std::string toString() const;
};
#endif
