/*
 * Generator.cpp
 *
 *  Created on: 2 Nov 2016
 *      Author: mahdisellami
 */
#include "Generator.hpp"
#include "Schema.hpp"

#include <iostream>

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

static std::string tuplePrimaryKey(const Schema::Relation& rel) {
	if(rel.primaryKey.size()==1){
		return type(rel.attributes[rel.primaryKey[0]]);
	} else {
		std::string s = "tuple<";
		for (const unsigned& keyId : rel.primaryKey){
			s.append(type(rel.attributes[keyId]) + ", ");
		}
		s.replace(s.length()-2,2,">");
		return s;
	}
}

static std::string primaryKeyListTypeAndName(const Schema::Relation& rel) {
		std::string s = "";
		for (const unsigned& keyId : rel.primaryKey){
			s.append(type(rel.attributes[keyId]) + " " + rel.attributes[keyId].name + ", ");
		}
		s.replace(s.length()-2,2,"");
		return s;
}

static std::string attributeListTypeAndName(const Schema::Relation& rel) {
		std::string s = "";
		for (Schema::Relation::Attribute att : rel.attributes){
			s.append(type(att) + " " + att.name + ", ");
		}
		s.replace(s.length()-2,2,"");
		return s;
}

static std::string primaryKeyListName(const Schema::Relation& rel) {
		std::string s = "";
		if (rel.primaryKey.size()>1)
			s.append("{");
		for (const unsigned& keyId : rel.primaryKey){
			s.append(rel.attributes[keyId].name + ", ");
		}
		s.replace(s.length()-2,2,"");
		if (rel.primaryKey.size()>1)
			s.append("}");
		return s;
}

static std::string attributeListName(const Schema::Relation& rel) {
		std::string s = "";
		for (Schema::Relation::Attribute att : rel.attributes){
			s.append(att.name + ", ");
		}
		s.replace(s.length()-2,2,"");
		return s;
}

void Generator::generate(){
	std::cout << "#ifndef H_Tpcc\n" <<
	"#define H_Tpcc\n" <<
	"//---------------------------------------------------------------------------\n"
	"#include ""Types.hpp""\n" <<
	"#include <vector>\n" <<
	"#include <unordered_map>\n" <<
	"#include <iostream>\n" <<
	"#include <fstream>\n" <<
	"\nusing namespace std;\n" <<
	"\nnamespace std {\n" <<
	"template<>\n" <<
	"struct hash<Integer> {\n" <<
	"\tsize_t operator()(const Integer& k) const {\n" <<
	"\t\treturn hash<int32_t>()(k.value);\n" <<
	"\t}\n" <<
	"};\n" <<
	"\ntemplate<>\n" <<
	"struct hash<tuple<Integer, Integer>> {\n" <<
	"\tsize_t operator()(tuple<Integer, Integer> const& k) const {\n" <<
	"\t\tsize_t h1 = hash<Integer>()(get<0>(k));\n" <<
	"\t\tsize_t h2 = hash<Integer>()(get<1>(k));\n" <<
	"\t\treturn h1 ^ (h2 << 1);\n" <<
	"\t}\n" <<
	"};\n" <<
	"\ntemplate<>\n" <<
	"struct hash<tuple<Integer, Integer, Integer>> {\n" <<
	"\tsize_t operator()(tuple<Integer, Integer, Integer> const& k) const {\n" <<
	"\t\tsize_t h1 = hash<Integer>()(get<0>(k));\n" <<
	"\t\tsize_t h2 = hash<Integer>()(get<1>(k));\n" <<
	"\t\tsize_t h3 = hash<Integer>()(get<2>(k));\n" <<
	"\t\treturn h1 ^ ((h2 ^ (h3 << 1)) << 1);\n" <<
	"\t}\n" <<
	"};\n" <<
	"\ntemplate<>\n" <<
	"struct hash<tuple<Integer, Integer, Integer, Integer>> {\n" <<
	"\tsize_t operator()(tuple<Integer, Integer, Integer, Integer> const& k) const {\n" <<
	"\t\tsize_t h1 = hash<Integer>()(get<0>(k));\n" <<
	"\t\tsize_t h2 = hash<Integer>()(get<1>(k));\n" <<
	"\t\tsize_t h3 = hash<Integer>()(get<2>(k));\n" <<
	"\t\tsize_t h4 = hash<Integer>()(get<3>(k));\n" <<
	"\t\treturn h1 ^ ((h2 ^ ((h3 ^ (h4 << 1)) << 1)) << 1);\n" <<
	"\t}\n" <<
	"};\n" <<
	"}\n" <<
	"//---------------------------------------------------------------------------\n" <<
	"//TPCC\n" <<
	"//---------------------------------------------------------------------------\n" <<
	"// TPCC class\n" <<
	"class TPCC {\n";
	Generator::generateRelations();

	std::cout <<"\npublic:\n\n";

	for (Schema::Relation rel : schema.relations)
		generateRelationVariable(rel);
	std::cout << "\n\tTPCC() {}\n};\n\n#endif\n";
}



void Generator::generateRelations(){
	for (Schema::Relation rel : schema.relations){
		std::string name = rel.name;
		name[0] = toupper(name[0]);
		std::cout << "\t//---------------------------------------------------------------------------\n" <<
				"\t//" << name <<"\n" <<
				"\t//---------------------------------------------------------------------------\n" <<
				"\t// " << name << " class\n" <<
				"\tclass " << name << " {\n" <<
				"\t\t/// " << name << " Row\n" <<
				"\t\tstruct " << name << "_Row {\n";
		for (Schema::Relation::Attribute att : rel.attributes)
			generateRowAttribute(att);
		std::cout << "\t\t};\n" <<
			"\tpublic:\n" <<
			"\t\t//" << name << "\n" <<
			"\t\tvector<" << name << "_Row> " << rel.name << "\n";
		generatePrimaryKey(rel);
		generateIndexes(rel);
		std::cout << "\n\t\t" << name << "() {}\n\n";
		gnerateInsert(rel);
		gnerateDelete(rel);
		generateFind(rel);
		generateGettersAndSetters(rel);
		std::cout << "\t};\n\n";
	}
}

void Generator::generateRelationVariable(Schema::Relation& rel){
	std::string className = rel.name;
	className[0] = toupper(className[0]);
	std::cout << "\t//" << className << "\n\t" << className << " " << rel.name << "\n";
}

void Generator::generateRowAttribute(Schema::Relation::Attribute& att){
	std::cout << "\t\t\t" << type(att) << " " << att.name << ";\n";
}

void Generator::generatePrimaryKey(Schema::Relation& rel){
	if(rel.primaryKey.size()){
	std::string name = rel.name;
	name[0] = toupper(name[0]);
	std::cout << "\t\t//" << name << " primary key\n" <<
	"\t\tunordered_map<" << 	tuplePrimaryKey(rel) <<
	",Tid> " << rel.name.at(0) << "_p_k;\n";
	}
}

void Generator::generateIndexes(Schema::Relation& rel){
	if (rel.indexes.size()){
		int i=0;
		for (Schema::Relation::Index ind : rel.indexes){
			i++;
			std::string name = ind.name;
			name[0] = toupper(name[0]);
			std::cout << "\t\t//Index: " << name << "\n" <<
			"\t\tunordered_multimap<";
			if(ind.attributes.size()==1){
				std::cout << type(rel.attributes[ind.attributes[0]]);
			} else {
				std::string s = "tuple<";
				for (const unsigned& keyId : ind.attributes){
					s.append(type(rel.attributes[keyId]) + ", ");
				}
				s.replace(s.length()-2,2,">");
				std::cout << s;
			}
			std::cout << ",Tid> " << ind.name << ";\n";

		}
	}
}

void Generator::generateGettersAndSetters(Schema::Relation& rel){
	for (Schema::Relation::Attribute att : rel.attributes){
		std::cout << "\t\tinline " << type(att) << " get_" << att.name <<
				"(Tid tid){ return " << rel.name << ".at(tid)." << att.name << ";}\n" <<
				"\t\tinline void set_" << att.name << "(Tid tid, " << type(att) << " " << att.name <<
				"){ " << rel.name << ".at(tid)." << att.name << " = " << att.name << ";}\n";
	}
}

void Generator::generateFind(Schema::Relation& rel){
	if (rel.primaryKey.size())
	std::cout << "\t\tinline uint64_t lookup(" << primaryKeyListTypeAndName(rel) << "){ return  " << rel.name.at(0) <<
			"_p_k.at(" <<  primaryKeyListName(rel) << ");}\n";
}

void Generator::gnerateInsert(Schema::Relation& rel){
	std::cout << "\t\tinline void insert(" << attributeListTypeAndName(rel) << ")\n" <<
	"\t\t{\n";
	std::string className = rel.name;
	className[0] = toupper(className[0]);
	if (rel.primaryKey.size()){
		std::cout << "\t\t\tif (" << rel.name[0] << "_p_k.find(" << primaryKeyListName(rel) <<
				") == " << rel.name[0] << "_p_k.end()) {\n\t\t\t\t" << rel.name[0] <<
				"_p_k.insert( { " << primaryKeyListName(rel) << ", " << rel.name << ".size() });\n" <<
				"\t\t\t\t" << className << "_Row " << rel.name[0] << " = {" << attributeListName(rel) << "};\n" <<
				"\t\t\t\t" << rel.name << ".push_back(" << rel.name[0] << ");\n" <<
				"\t\t\t} else {\n\t\t\t\tcerr << \"Primary key must be unique!\\n\";\n" <<
				"\t\t\t}\n";
	} else {
		std::cout << "\t\t\t" << className << "_Row " << rel.name[0] << " = {" << attributeListName(rel) << "};\n" <<
				"\t\t\t" << rel.name << ".push_back(" << rel.name[0] << ");\n";
	}
	std::cout << "\t\t}\n";
}


void Generator::gnerateDelete(Schema::Relation& rel){
	std::cout << "\t\tinline void delete(Tid tid)\n" <<
	"\t\t{\n";
	std::string className = rel.name;
	className[0] = toupper(className[0]);
	std::cout << "\t\t\tif (tid < " << rel.name << ".size()" << "){\n" <<
			"\t\t\t\t" << rel.name[0] << "_p_k.erase(" << rel.name[0] << "_p_k.begin() + tid);\n" <<
			"\t\t\t\t" << rel.name << ".erase(" << rel.name << ".begin() + tid);\n" <<
			"\t\t\t}\n\t\t}\n";

}
