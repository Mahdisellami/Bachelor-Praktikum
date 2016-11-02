#include "Schema.hpp"

#include <sstream>

static std::string type(const Schema::Relation::Attribute& attr) {
   Types::Tag type = attr.type;
   switch(type) {
      case Types::Tag::Integer:
         return "Integer";
      case Types::Tag::Varchar: {
         std::stringstream ss;
         ss << "Varchar(" << attr.len1 << ")";
         return ss.str();
      }
      //Varchar, Char, Numeric, Date, Timestamp
      case Types::Tag::Char: {
         std::stringstream ss;
         ss << "Char(" << attr.len1 << ")";
         return ss.str();
      }
      // Must find solution for Numeric
      case Types::Tag::Numeric: {
         std::stringstream ss;
         ss << "Numeric(" << attr.len1 << ", " << attr.len2 << ")";
         return ss.str();
      }
      case Types::Tag::Date:
         return "Date";
      case Types::Tag::Timestamp:
         return "Timestamp";
   }
   throw;
}

std::string Schema::toString() const {
   std::stringstream out;
   for (const Schema::Relation& rel : relations) {
      out << rel.name << std::endl;
      out << "\tPrimary Key:";
      for (unsigned keyId : rel.primaryKey)
         out << ' ' << rel.attributes[keyId].name;
      out << std::endl;
      out << "\tIndexes:" << std::endl;
      for (const Schema::Relation::Index& ind : rel.indexes){
          out << "\t\t" << ind.name << ":";
          for (unsigned keyId : ind.attributes)
             out << ' ' << rel.attributes[keyId].name;
          out << std::endl;
      }
      out << "\tAttributes:" << std::endl;
      for (const auto& attr : rel.attributes)
         out << "\t\t" << attr.name << '\t' << type(attr) << (attr.notNull ? " not null" : "") << std::endl;
   }
   return out.str();
}
