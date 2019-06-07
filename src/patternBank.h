#ifndef __PATTERN_BANK__
#define __PATTERN_BANK__

#include <vector>
#include <string>

namespace LogicRegression
{

enum status { MERGE, NONE, IDENTICAL };

class PatternBank
{
public:
    PatternBank() {}
    ~PatternBank() {}

    void   insert(const std::string&);
    void   merge(std::vector<std::string>::iterator&, const std::string&);
    void   visualize(status, std::string, std::string);
    void   show() const;
    void   random_sample();
    int    count_X(const std::string&);
    status check_stat(const std::string&, const std::string&);

private:
    std::vector<std::string> _patterns;
};

}

#endif /* __PATTERN_BANK__ */
