#ifndef __VARIABLE_H__
#define __VARIABLE_H__
#include <string>

namespace LogicRegression
{

typedef int VariableID;

struct Variable
{
    Variable() {}
    Variable(std::string s) { _name = s; }
    ~Variable() {}

    void SetName(std::string s) { _name = s; }
    void SetID(VariableID i) { _id = i; }

    std::string _name;
    VariableID  _id;
};

}

#endif /* __VARIABLE_H__ */
