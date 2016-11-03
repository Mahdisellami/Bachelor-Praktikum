#ifndef H_Generator_hpp
#define H_Generator_hpp

#include <exception>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include "Schema.hpp"

struct Generator {
	Schema schema;
   Generator(Schema& s) : schema(s) {}
   ~Generator() {};
   void generate();
private:
   void generateRelations();
   void generateRowAttribute(Schema::Relation::Attribute& att);
   void generatePrimaryKey(Schema::Relation& rel);
   void generateIndexes(Schema::Relation& rel);
   void generateGettersAndSetters(Schema::Relation& rel);
   void generateFind(Schema::Relation& rel);
   void gnerateInsert(Schema::Relation& rel);
   void generateRelationVariable(Schema::Relation& rel);
   void gnerateDelete(Schema::Relation& rel);
};

#endif
