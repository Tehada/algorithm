#include <iostream>
#include <string>
#include <regex>
#include <fstream>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include "clang/Tooling/CommonOptionsParser.h"

#include "clang/Lex/Lexer.h"

#include "print.h"

using namespace clang::tooling;
using namespace llvm;

static llvm::cl::OptionCategory my_tool_category("my-tool options");
static cl::opt<std::string> dictionary_path("dict");

std::vector<std::string> dictionary;

using namespace clang;

bool CheckName(const std::string& name, Entity type) {
    std::regex r;
    switch (type) {
        case Entity::kVariable:
            r = std::regex("[a-z]+(_([a-z]+))*");
            break;
        case Entity::kField:
            r = std::regex("(([a-z]*)_)*");
            break;
        case Entity::kType:
            r = std::regex("(([A-Z]|[A-Z]{3,})[a-z]+)+([A-Z]|[A-Z]{3,})?|");
            break;
        case Entity::kConst:
            r = std::regex("[a-z](([A-Z]|[A-Z]{3,})[a-z]+)+([A-Z]|[A-Z]{3,})?");
            break;
        case Entity::kFunction:
            r = std::regex("(([A-Z]|[A-Z]{3,})[a-z]+)+([A-Z]|[A-Z]{3,})?|main|operator\\(\\)|operator==|operator<<|operator>>");
            break;
    }
    return std::regex_match(name, r);
}

template<class T>
typename T::value_type LevenshteinDistance(const T & src, const T & dst) {
  const typename T::size_type mSize = src.size();
  const typename T::size_type nSize = dst.size();
  if (mSize == 0) {
    return nSize;
  }
  if (nSize == 0) {
    return mSize;
  }

  std::vector< std::vector<typename T::value_type> > matrix(mSize + 1);

  for (typename T::size_type i = 0; i <= mSize; ++i) {
    matrix[i].resize(nSize + 1);
    matrix[i][0] = i;
  }
  for (typename T::size_type i = 0; i <= nSize; ++i) {
    matrix[0][i] = i;
  }

  typename T::value_type above_cell, left_cell, diagonal_cell, cost;

  for (typename T::size_type i = 1; i <= mSize; ++i) {
    for(typename T::size_type j = 1; j <= nSize; ++j) {
      cost = src[i - 1] == dst[j - 1] ? 0 : 1;
      above_cell = matrix[i - 1][j];
      left_cell = matrix[i][j - 1];
      diagonal_cell = matrix[i - 1][j - 1];
      matrix[i][j] = std::min(std::min(above_cell + 1, left_cell + 1), diagonal_cell + cost);
    }
  }

  return matrix[mSize][nSize];
}

template<typename Out>
void SplitByDelim(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        std::transform(item.begin(), item.end(), item.begin(), [](unsigned char c){ return std::tolower(c); });
        //std::cerr << "\tDelim word: " << item << "\n";
        *(result++) = item;
    }
}

template<typename Out>
void SplitByUpperLetter(const std::string &s, Out result) {
    std::regex r("[a-z]+|[A-Z]([a-z]+)|[A-Z]{3,}([a-z]+)");
    auto words_begin = std::sregex_iterator(s.begin(), s.end(), r);
    auto words_end = std::sregex_iterator();
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string item = match.str();
        std::transform(item.begin(), item.end(), item.begin(), [](unsigned char c){ return std::tolower(c); });
        //std::cerr << "\tUpper word: " << item << "\n";
        *(result++) = item;
    }
}

std::vector<std::string> Split(const std::string &s, Entity type) {
    //std::cerr << "Split: " << s << "\n";
    std::vector<std::string> elems;
    if (type == Entity::kVariable)
        SplitByDelim(s, '_', std::back_inserter(elems));
    else
        SplitByUpperLetter(s, std::back_inserter(elems));
    return elems;
}


class MyVisitor : public RecursiveASTVisitor<MyVisitor> {
public:
    explicit MyVisitor(ASTContext *context): context_(context), error_stream_(errors_) {}

    bool VisitVarDecl(VarDecl *d) {
        if (context_->getSourceManager().isInMainFile(d->getLocStart())) {    
            Entity type = Entity::kVariable;
            if (d->getAccess() == clang::AccessSpecifier::AS_private) {
                type = Entity::kField;
            }
            if (d->getType().isConstQualified())
                type = Entity::kConst;

            CheckDecl(d, type);
        }
        return true;
    }

    bool VisitFieldDecl(FieldDecl *d) {
        if (context_->getSourceManager().isInMainFile(d->getLocStart())) {
            Entity type = Entity::kField;
            if (d->getAccess() != clang::AccessSpecifier::AS_private) {
                type = Entity::kVariable;
            }
            if (d->getType().isConstQualified())
                type = Entity::kConst;

            CheckDecl(d, type);
        }
        return true;
    }

    bool VisitCXXRecordDecl(CXXRecordDecl *d) {
        if (context_->getSourceManager().isInMainFile(d->getLocStart())) {
            Entity type = Entity::kType;
            CheckDecl(d, type);
        }
        return true;
    }

    bool VisitEnumDecl(EnumDecl *d) {
        if (context_->getSourceManager().isInMainFile(d->getLocStart())) {
            Entity type = Entity::kType;
            CheckDecl(d, type);
        }
        return true;
    }

    bool VisitFunctionDecl(FunctionDecl *d) {
        if (context_->getSourceManager().isInMainFile(d->getLocStart())) {
            Entity type = Entity::kFunction;
            CheckDecl(d, type);
        }
        return true;
    }

    bool VisitTypeAliasDecl(TypeDecl *d) {
        if (context_->getSourceManager().isInMainFile(d->getLocStart())) {
            Entity type = Entity::kType;
            CheckDecl(d, type);
        }
        return true;
    }

    bool VisitStmt(Stmt *stmt) { 
        const SourceManager &source_manager = context_->getSourceManager(); 
        SourceLocation start_loc = stmt->getLocStart(); 
        if (clang::Lexer::isAtStartOfMacroExpansion(start_loc, source_manager, context_->getLangOpts())) { 
            //std::cout << "is start of macro expansion" << std::endl;
        } 
        return true; 
    }

    int GetBadNames() const {
        return bad_names_;
    }

    int GetMistakes() const {
        return mistakes_;
    }

    std::string GetErrors() {
        error_stream_.flush();
        return errors_;
    }

    void ResetErrors() {
        bad_names_ = 0;
        mistakes_ = 0;
        errors_.clear();
    }

    std::string GetFilename() {
        return filename_;
    }

private:
    ASTContext *context_;
    std::string errors_;
    llvm::raw_string_ostream error_stream_;
    int bad_names_ = 0, mistakes_ = 0;

    std::string filename_;

    void CheckDecl(NamedDecl* d, Entity type) {
        if (filename_.empty()) filename_ = context_->getSourceManager().getFilename(d->getLocStart());
        FullSourceLoc full_location = context_->getFullLoc(d->getLocStart());
        if (!CheckName(d->getNameAsString(), type)) {
            BadName(type, d->getNameAsString(), context_->getSourceManager().getFilename(d->getLocStart()), full_location.getSpellingLineNumber(), error_stream_);
            ++bad_names_;
        } else if (d->getNameAsString().size() > 3) {
            std::vector<std::string> words = Split(d->getNameAsString(), type);
            //std::cerr << "\t\tSplit done:\n";
            //for (auto& w : words) std::cerr << "\t\t" << w << "\n";
            for (auto& word : words) {
                if (word.size() < 4) continue;

                size_t dist = 4, index_of_nearest = 0;
                for (size_t i = 0; i < dictionary.size(); ++i) {
                    size_t new_dist = LevenshteinDistance(word, dictionary[i]);
                    if (new_dist < 4 && new_dist < dist) {
                        dist = new_dist;
                        index_of_nearest = i;
                        if (dist == 0) break;
                    }
                }
                if (0 < dist && dist < 4) {
                    //std::cerr << "\t\t\t" << dictionary[index_of_nearest] << " is nearest to " << word << "\n";
                    Mistake(d->getNameAsString(), word, dictionary[index_of_nearest], filename_, full_location.getSpellingLineNumber(), error_stream_);
                    ++mistakes_;
                }
            }
        }
    }
};

class MyConsumer : public clang::ASTConsumer {
public:
    explicit MyConsumer(ASTContext *context) : visitor_(context) {}

    virtual void HandleTranslationUnit(clang::ASTContext &context) {
        visitor_.TraverseDecl(context.getTranslationUnitDecl());
        PrintStatistics(visitor_.GetFilename(), visitor_.GetBadNames(), visitor_.GetMistakes());
        llvm::outs() << visitor_.GetErrors();
        visitor_.ResetErrors();
    }

private:
    MyVisitor visitor_;
};

class MyAction : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef in_file) {
        return std::unique_ptr<clang::ASTConsumer>(new MyConsumer(&compiler.getASTContext()));
    }
};

std::vector<std::string> ReadDictionary(const std::string& path) {
    std::ifstream input_file(path);
    std::vector<std::string> res;
    std::string word;
    while (input_file >> word) {
        res.push_back(word);
    }
    return res;
}

int main(int argc, const char **argv) {
    CommonOptionsParser options_parser(argc, argv, my_tool_category);
    if (!dictionary_path.empty()) {
        dictionary = ReadDictionary(dictionary_path);
    }
    //for (const auto& name : options_parser.getSourcePathList()) {
    //    std::cerr << "name: " << name << "\n";
    //}
    ClangTool tool(options_parser.getCompilations(), options_parser.getSourcePathList());
    return tool.run(newFrontendActionFactory<MyAction>().get());
}

