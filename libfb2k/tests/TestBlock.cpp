#include <unittest++/UnitTest++.h>
#include "libfb2k/libfb2k.h"
#include "libfb2k/Block.h"

SUITE(BlockParsing)
{
    TEST(BlockParse) {
        fb2k::Block blk = fb2k::Block("$if(true,then,else)");
        auto fn = blk.getFunctions();
        CHECK_EQUAL(1, fn.size());
        CHECK_EQUAL("if" , fn[0].name);
        CHECK_EQUAL(0 , fn[0].pos);
        CHECK_EQUAL(0 , fn[0].raw_pos);

        CHECK_EQUAL(fn[0].args.size(), 3);

        CHECK_EQUAL("true", fn[0].args[0].getStatement());
        CHECK_EQUAL("then", fn[0].args[1].getStatement());
        CHECK_EQUAL("else", fn[0].args[2].getStatement());

        CHECK_EQUAL("", blk.getFormattedText());
    }

    TEST(BlockParseExtra) {
        fb2k::Block blk = fb2k::Block("$if(true,then,else) other text");
        auto fn = blk.getFunctions();
        CHECK_EQUAL(1, fn.size());
        CHECK_EQUAL("if", fn[0].name);
        CHECK_EQUAL(0 , fn[0].pos);
        CHECK_EQUAL(0 , fn[0].raw_pos);

        CHECK_EQUAL(3, fn[0].args.size());

        CHECK_EQUAL("true", fn[0].args[0].getStatement());
        CHECK_EQUAL("then", fn[0].args[1].getStatement());
        CHECK_EQUAL("else", fn[0].args[2].getStatement());


        CHECK_EQUAL(" other text", blk.getFormattedText());
    }

    TEST(BlockParseMulti) {
        fb2k::Block blk = fb2k::Block("$if(true,then,else) $if3(false,not,stuff)");
        auto fn = blk.getFunctions();
        CHECK_EQUAL(2, fn.size());

        CHECK_EQUAL("if", fn[0].name);
        CHECK_EQUAL(3, fn[0].args.size());
        CHECK_EQUAL(0 , fn[0].pos);
        CHECK_EQUAL(0 , fn[0].raw_pos);

        CHECK_EQUAL("true", fn[0].args[0].getStatement());
        CHECK_EQUAL("then", fn[0].args[1].getStatement());
        CHECK_EQUAL("else", fn[0].args[2].getStatement());

        CHECK_EQUAL("if3", fn[1].name);
        CHECK_EQUAL(3, fn[1].args.size());
        CHECK_EQUAL(1 , fn[1].pos);
        CHECK_EQUAL(20 , fn[1].raw_pos);

        CHECK_EQUAL("false", fn[1].args[0].getStatement());
        CHECK_EQUAL("not", fn[1].args[1].getStatement());
        CHECK_EQUAL("stuff", fn[1].args[2].getStatement());


        CHECK_EQUAL(" ", blk.getFormattedText());
    }

    TEST(BlockUnicodeParse) {
        fb2k::Block blk = fb2k::Block("$if(♡,馬鹿,バカ)");
        auto fn = blk.getFunctions();
        CHECK_EQUAL(1, fn.size());
        CHECK_EQUAL("if", fn[0].name);
        CHECK_EQUAL(0 , fn[0].pos);
        CHECK_EQUAL(0 , fn[0].raw_pos);

        CHECK_EQUAL(3, fn[0].args.size());

        CHECK_EQUAL("♡", fn[0].args[0].getStatement());
        CHECK_EQUAL("馬鹿", fn[0].args[1].getStatement());
        CHECK_EQUAL("バカ", fn[0].args[2].getStatement());

        CHECK_EQUAL("", blk.getFormattedText());
    }

    TEST(BlockUnicodeParseExtra) {
        fb2k::Block blk = fb2k::Block("$if(♡,馬鹿,バカ) 友");
        auto fn = blk.getFunctions();
        CHECK_EQUAL(1, fn.size());
        CHECK_EQUAL("if", fn[0].name);
        CHECK_EQUAL(0 , fn[0].pos);
        CHECK_EQUAL(0 , fn[0].raw_pos);

        CHECK_EQUAL(3, fn[0].args.size());

        CHECK_EQUAL("♡", fn[0].args[0].getStatement());
        CHECK_EQUAL("馬鹿", fn[0].args[1].getStatement());
        CHECK_EQUAL("バカ", fn[0].args[2].getStatement());

        CHECK_EQUAL(" 友", blk.getFormattedText());
    }

    TEST(BlockParseImplicIf) {
        fb2k::Block blk = fb2k::Block("[true]");
        auto fn = blk.getFunctions();
        CHECK_EQUAL(1, fn.size());
        CHECK_EQUAL("[" , fn[0].name);
        CHECK_EQUAL(0 , fn[0].pos);
        CHECK_EQUAL(0 , fn[0].raw_pos);

        CHECK_EQUAL(fn[0].args.size(), 1);

        CHECK_EQUAL("true", fn[0].args[0].getStatement());

        CHECK_EQUAL("", blk.getFormattedText());
    }

    TEST(BlockParseVariable) {
        fb2k::Block blk = fb2k::Block("%variable%");
        auto fn = blk.getFunctions();
        auto var = blk.getVariables();
        CHECK_EQUAL(0, fn.size());

        CHECK_EQUAL(1, var.size());

        CHECK_EQUAL("variable", var[0].name);
        CHECK_EQUAL(0, var[0].pos);
        CHECK_EQUAL(0 , var[0].raw_pos);

        CHECK_EQUAL("", blk.getFormattedText());
    }

    TEST(BlockParseMultiVariable) {
        fb2k::Block blk = fb2k::Block("%v1% %v2% %v3%");
        auto fn = blk.getFunctions();
        auto var = blk.getVariables();
        CHECK_EQUAL(0, fn.size());

        CHECK_EQUAL(3, var.size());

        CHECK_EQUAL("v1", var[0].name);
        CHECK_EQUAL(0, var[0].pos);
        CHECK_EQUAL(0 , var[0].raw_pos);

        CHECK_EQUAL("v2", var[1].name);
        CHECK_EQUAL(1, var[1].pos);
        CHECK_EQUAL(5 , var[1].raw_pos);

        CHECK_EQUAL("v3", var[2].name);
        CHECK_EQUAL(2, var[2].pos);
        CHECK_EQUAL(10 , var[2].raw_pos);

        CHECK_EQUAL("  ", blk.getFormattedText());
    }

    TEST(BlockParseVariableFunction) {
        fb2k::Block blk = fb2k::Block("$if(true,false) %var%");
        auto fn = blk.getFunctions();
        auto var = blk.getVariables();
        CHECK_EQUAL(1, fn.size());

        CHECK_EQUAL("if", fn[0].name);
        CHECK_EQUAL(2, fn[0].args.size());
        CHECK_EQUAL(0, fn[0].pos);
        CHECK_EQUAL(0, fn[0].raw_pos);

        CHECK_EQUAL("true", fn[0].args[0].getStatement());
        CHECK_EQUAL("false", fn[0].args[1].getStatement());

        CHECK_EQUAL(1, var.size());

        CHECK_EQUAL("var", var[0].name);
        CHECK_EQUAL(1, var[0].pos);
        CHECK_EQUAL(16 , var[0].raw_pos);

        CHECK_EQUAL(" ", blk.getFormattedText());
    }

    TEST(BlockParseAll) {
        fb2k::Block blk = fb2k::Block("$if(true,false) %var% \\$ text and 友");
        auto fn = blk.getFunctions();
        auto var = blk.getVariables();
        CHECK_EQUAL(1, fn.size());

        CHECK_EQUAL("if", fn[0].name);
        CHECK_EQUAL(2, fn[0].args.size());
        CHECK_EQUAL(0, fn[0].pos);

        CHECK_EQUAL("true", fn[0].args[0].getStatement());
        CHECK_EQUAL("false", fn[0].args[1].getStatement());

        CHECK_EQUAL(1, var.size());

        CHECK_EQUAL("var", var[0].name);
        CHECK_EQUAL(1, var[0].pos);

        CHECK_EQUAL("  $ text and 友", blk.getFormattedText());
    }

    TEST(BlockSpeicalChars) {
        // Add more speical characters or escape characters
        // TODO : Support $$ -> $ when parsed?
        fb2k::Block blk = fb2k::Block("\\$");
        CHECK_EQUAL("$", blk.getFormattedText());

        blk = fb2k::Block("\\(\\)");
        CHECK_EQUAL("()", blk.getFormattedText());

        blk = fb2k::Block("\\\\");
        CHECK_EQUAL("\\", blk.getFormattedText());

        blk = fb2k::Block("\\,");
        CHECK_EQUAL(",", blk.getFormattedText());

        blk = fb2k::Block("\\t\\n");
        CHECK_EQUAL("\t\n", blk.getFormattedText());
    }

}

SUITE(BlockErrorDetection)
{
    TEST(BlockParseInvaildName) {
        CHECK_THROW(fb2k::Block("$f a i l m e ()") , fb2k::InvalidFunctionName);
        CHECK_THROW(fb2k::Block("$友()") , fb2k::InvalidFunctionName);
        CHECK_THROW(fb2k::Block("$.xX~~420^Blaze^it~~Xx.(") , fb2k::InvalidFunctionName);
    }

    TEST(BlockParseInvaildBrackets) {
        CHECK_THROW(fb2k::Block("$if(errro") , fb2k::SyntaxError);
        CHECK_THROW(fb2k::Block("$if(e(rrro") , fb2k::SyntaxError);
        CHECK_THROW(fb2k::Block("$if(errro") , fb2k::SyntaxError);
    }

    TEST(BlockParseInvaildEscapeChars) {
        // Unsupported Escape Characters
        CHECK_THROW(fb2k::Block("\\0") , fb2k::SyntaxError);
    }

    TEST(BlockParseUnFinished) {
        CHECK_THROW(fb2k::Block("$almost") , fb2k::SyntaxError);
        CHECK_THROW(fb2k::Block("$") , fb2k::SyntaxError);
    }
}

SUITE(BlockEval)
{
    TEST(BlockVarEval) {
        fb2k::Block blk = fb2k::Block("%artist%");

        TagLib::PropertyMap metadata;
        TagLib::StringList ls;
        ls.append("Foobar");
        metadata.insert("artist", ls);

        auto result = blk.eval(metadata);
        CHECK_EQUAL("Foobar", result.result);
        CHECK_EQUAL(0, result.value);
    }

    TEST(BlockVarEvalUnicode) {
        fb2k::Block blk = fb2k::Block("%artist%");

        TagLib::PropertyMap metadata;
        TagLib::StringList ls;
        ls.append("友");
        metadata.insert("artist", ls);

        auto result = blk.eval(metadata);
        CHECK_EQUAL("友", result.result);
        CHECK_EQUAL(0, result.value);
    }

    TEST(BlockVarEvalMultiple) {
        fb2k::Block blk = fb2k::Block("%artist% %artist% %artist%");

        TagLib::PropertyMap metadata;
        TagLib::StringList ls;
        ls.append("FOOBAR");
        metadata.insert("artist", ls);

        auto result = blk.eval(metadata);
        CHECK_EQUAL("FOOBAR FOOBAR FOOBAR", result.result);
        CHECK_EQUAL(0, result.value);
    }
    TEST(BlockFuncEvalUnicode) {
        fb2k::Block blk = fb2k::Block("$if(expr,true,友)");

        TagLib::PropertyMap metadata;
        TagLib::StringList ls;
        ls.append("Foobar");
        metadata.insert("artist", ls);

        auto result = blk.eval(metadata);
        CHECK_EQUAL("友", result.result);
        CHECK_EQUAL(0, result.value);
    }

    TEST(BlockFuncVarEval) {
        fb2k::Block blk = fb2k::Block("$if(%artist%,true,false)");

        TagLib::PropertyMap metadata;
        TagLib::StringList ls;
        ls.append("Foobar");
        metadata.insert("artist", ls);

        auto result = blk.eval(metadata);
        CHECK_EQUAL("true", result.result);
        CHECK_EQUAL(0, result.value);
    }

    TEST(BlockFuncVarEvalCloseCase) {
        fb2k::Block blk = fb2k::Block("$if(%artist%,true,false)%artist%");

        TagLib::PropertyMap metadata;
        TagLib::StringList ls;
        ls.append("Foobar");
        metadata.insert("artist", ls);

        auto result = blk.eval(metadata);
        CHECK_EQUAL("trueFoobar", result.result);
        CHECK_EQUAL(0, result.value);
    }

    TEST(BlockEvalFileRed) {
        fb2k::Block blk = fb2k::Block("%filename% %artist% %title% %album% %channels% %length% %bitrate% %samplerate%");

        TagLib::FileRef file("test.flac");
        // FIXME: I do not trust the output of channels, length and samplerate. They all seem to be sections of each other
        CHECK_EQUAL("test.flac FOO BAR TEST mono 10 10107 1010744100", blk.eval(file).result);
    }

    TEST(BlockEvalInt) {
        TagLib::PropertyMap metadata;
        fb2k::Block blk;

        blk = fb2k::Block("10");
        auto result = blk.eval(metadata);
        CHECK_EQUAL("10", result.result);
        CHECK_EQUAL(10, result.value);

        blk = fb2k::Block("-10");
        result = blk.eval(metadata);
        CHECK_EQUAL("-10", result.result);
        CHECK_EQUAL(-10, result.value);

        blk = fb2k::Block("- 10");
        result = blk.eval(metadata);
        CHECK_EQUAL("- 10", result.result);
        CHECK_EQUAL(0, result.value);

        blk = fb2k::Block("10.1");
        result = blk.eval(metadata);
        CHECK_EQUAL("10.1", result.result);
        CHECK_EQUAL(10, result.value);

        blk = fb2k::Block("c3p0");
        result = blk.eval(metadata);
        CHECK_EQUAL("c3p0", result.result);
        CHECK_EQUAL(0, result.value);

        blk = fb2k::Block("   10");
        result = blk.eval(metadata);
        CHECK_EQUAL("   10", result.result);
        CHECK_EQUAL(10, result.value);
    }
}
