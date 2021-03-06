#ifndef LIBFB2K_BLOCK_H
#define LIBFB2K_BLOCK_H

#include <string>
#include <vector>

#include <taglib.h>
#include <tpropertymap.h> //Only aviable v1.8+
#include <fileref.h>

#include "libfb2k/error.h"


#define LIBFB2K_CMD_START         '$'
#define LIBFB2K_ARGS_START        '('
#define LIBFB2K_ARGS_END          ')'
#define LIBFB2K_VAR_START         '%'
#define LIBFB2K_VAR_END           '%'
#define LIBFB2K_IMPLICIT_IF_START '['
#define LIBFB2K_IMPLICIT_IF_END   ']'

namespace fb2k
{
struct BlockResult {
    std::string result;
    bool success;
    int value;

    BlockResult() {
        this->result = "";
        success = false;
        value = 0;
    }
};

class Block
{
public:
    struct Function {
        std::string name;
        std::vector<Block> args;
        unsigned int pos;
        unsigned int raw_pos;
    };
    struct Variable {
        std::string name;
        unsigned int pos;
        unsigned int raw_pos;
    };

    Block();
    Block ( std::string statement );
    virtual ~Block();

    void parse ( std::string statement );
    BlockResult eval ( const TagLib::PropertyMap& metadata );
    BlockResult eval ( const TagLib::FileRef& fileref );

    std::string getStatement();
    std::string getFormattedText();
    std::vector<Function> getFunctions();
    std::vector<Variable> getVariables();

    bool isParsed();
private:
    std::vector<Function> functions;
    std::vector<Variable> variables;

    std::string raw_statement;
    std::string parsed_statement;

    bool parsed;
};
}
#endif // LIBFB2K_BLOCK_H
