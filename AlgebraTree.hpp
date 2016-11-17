/*
 * AlgebraTree.hpp
 *
 *  Created on: 16 Nov 2016
 *      Author: mahdisellami
 */

#ifndef ALGEBRATREE_HPP_
#define ALGEBRATREE_HPP_


#include <vector>
#include "Schema.hpp"

typedef string IU;


//---------------------------------------------------------------------------
// Node
//---------------------------------------------------------------------------
class Node
{
public:
    Node* input;
    Node* consumer;
    Node(Node* in, Node* out) : input(in), consumer(out) {}
    virtual void produce() const;
    virtual void consume(const Node* on) const;
    void setInput(Node* in);
    void setConsumer(Node* out);
    virtual vector<IU*> getProduced();
};

//---------------------------------------------------------------------------
void Node::produce() const {}
//---------------------------------------------------------------------------
void Node::consume(const Node* on) const {}
//---------------------------------------------------------------------------
void Node::setInput(Node* in)
{
	input = in;
}
//---------------------------------------------------------------------------
void Node::setConsumer(Node* out)
{
	consumer = out;
}
//---------------------------------------------------------------------------
vector<IU*> Node::getProduced()
{
}

//---------------------------------------------------------------------------
// Table Scan
//---------------------------------------------------------------------------
class Scan : public Node
{
public:
	std::vector<IU> IUs;
	Schema::Relation* relation;
    Scan(Node* in, Node* out, Schema::Relation* rel) : Node(in, out), relation(rel) {
    	for (Schema::Relation::Attribute a : rel->attributes){
    		IUs.push_back(a.name);
    	}
    }
    virtual void produce() const;
    IU* getIU(string columnName);
    virtual vector<IU*> getProduced();
};

//---------------------------------------------------------------------------
void Scan::produce() const
{
	std::string name = relation->name;
	name[0] = toupper(name[0]);
	std::string name2 = relation->name;
	std::cout << "for (auto tid=0; tid<"
			<< name2 << "." << name2 << ".size(); tid++) {" << std::endl;
	consumer->consume(this);
	std::cout << "}" << std::endl;
}

//---------------------------------------------------------------------------
IU* Scan::getIU(string columnName)
{
	for (int i=0; i<IUs.size(); i++){
		if (IUs[i]==columnName){
			return &IUs[i];
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------
vector<IU*> Scan::getProduced()
{
	vector<IU*> produced;
	for (int i=0;i<IUs.size(); i++){
		produced.push_back(getIU(IUs[i]));
	}
	return produced;
}

//---------------------------------------------------------------------------
// Selection
//---------------------------------------------------------------------------
class Selection : public Node
{
public:
	string parameter;
	int value;
    Selection(Node* in, Node* out, string par, int val) : Node(in, out), parameter(par), value(val) {
    }
    virtual void produce() const;
    virtual void consume(const Node* on) const;
    virtual vector<IU*> getProduced() const;
};

//---------------------------------------------------------------------------
void Selection::produce() const
{
	input->produce();
}
//---------------------------------------------------------------------------
void Selection::consume(const Node* on) const
{
	std::cout << "if (" << parameter << " = " << value << ") {" << std::endl;
	consumer->consume(this);
	std::cout << "}" << std::endl;
}

//---------------------------------------------------------------------------
vector<IU*> Selection::getProduced() const
{
	return input->getProduced();
}

//---------------------------------------------------------------------------
// Hash Join
//---------------------------------------------------------------------------
class HashJoin : public Node
{
public:
	Node* input2;
	HashJoin(Node* left, Node* out, Node* right) : Node(left, out), input2(right) {}
	virtual void produce() const;
	virtual void consume(const Node* on) const;
    void setInput2(Node* in);
    virtual vector<IU*> getProduced();
};

//---------------------------------------------------------------------------
void HashJoin::produce() const
{
	std::cout << "unordered_multimap<tuple<Integer,Integer,Integer>, TPCC::Customer::Customer_Row> IndexCustomer;" << std::endl;
	input->produce();
	input2->produce();
}
//---------------------------------------------------------------------------
void HashJoin::consume(const Node* on) const
{
	if (on==input){
		std::cout << "store t in HT" << std::endl;
	} else {
		std::cout << "for t in HT.lookup(t) {" << std::endl;
		consumer->consume(this);
		std::cout << "}" << std::endl;
	}
}//---------------------------------------------------------------------------
void HashJoin::setInput2(Node* in)
{
	input2 = in;
}

//---------------------------------------------------------------------------
vector<IU*> HashJoin::getProduced()
{
	vector<IU*> left = input->getProduced();
	vector<IU*> right = input2->getProduced();
	for (IU* iu : left) {
		right.push_back(iu);
	}
	return right;
}

//---------------------------------------------------------------------------
// Print
//---------------------------------------------------------------------------
class Print : public Node
{
public:
	Print(Node* in, Node* out) : Node(in, out) {}
	virtual void produce() const;
	virtual void consume(const Node* on) const;
    virtual vector<IU*> getProduced() const;
};

//---------------------------------------------------------------------------
void Print::produce() const
{
	input->produce();
}
//---------------------------------------------------------------------------
void Print::consume(const Node* on) const
{
	std::cout << "std::cout << result" << std::endl;
}

vector<IU*> Print::getProduced() const
{
	return input->getProduced();
}

#endif /* ALGEBRATREE_HPP_ */
