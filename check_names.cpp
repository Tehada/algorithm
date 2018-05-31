#include <iostream>
#include <string>
#include <regex>

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

static llvm::cl::OptionCategory MyToolCategory("my-tool options");

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
            r = std::regex("(([A-Z]|[A-Z]{3,})[a-z]+)+([A-Z]|[A-Z]{3,})?|main|operator\\(\\)|operator==");
            break;
    }
    return std::regex_match(name, r);
}

class MyVisitor : public RecursiveASTVisitor<MyVisitor> {
public:
    explicit MyVisitor(ASTContext *Context): Context_(Context), ErrorStream_(Errors_) {}

    void CheckDecl(NamedDecl* d, Entity type) {
        if (Filename_.empty()) Filename_ = Context_->getSourceManager().getFilename(d->getLocStart());
        if (!CheckName(d->getNameAsString(), type)) {
            FullSourceLoc FullLocation = Context_->getFullLoc(d->getLocStart());
            BadName(type, d->getNameAsString(), Context_->getSourceManager().getFilename(d->getLocStart()), FullLocation.getSpellingLineNumber(), ErrorStream_);
            ++bad_names_;
        }
    };

    bool VisitVarDecl(VarDecl *d) {
        if (Context_->getSourceManager().isInMainFile(d->getLocStart())) {    
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
        if (Context_->getSourceManager().isInMainFile(d->getLocStart())) {
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
        if (Context_->getSourceManager().isInMainFile(d->getLocStart())) {
            Entity type = Entity::kType;
            CheckDecl(d, type);
        }
        return true;
    }

    bool VisitEnumDecl(EnumDecl *d) {
        if (Context_->getSourceManager().isInMainFile(d->getLocStart())) {
            Entity type = Entity::kType;
            CheckDecl(d, type);
        }
        return true;
    }

    bool VisitFunctionDecl(FunctionDecl *d) {
        if (Context_->getSourceManager().isInMainFile(d->getLocStart())) {
            Entity type = Entity::kFunction;
            CheckDecl(d, type);
        }
        return true;
    }

    bool VisitTypeAliasDecl(TypeDecl *d) {
        if (Context_->getSourceManager().isInMainFile(d->getLocStart())) {
            Entity type = Entity::kType;
            CheckDecl(d, type);
        }
        return true;
    }

    bool VisitStmt(Stmt *stmt) { 
        const SourceManager &SM = Context_->getSourceManager(); 
        SourceLocation startLoc = stmt->getLocStart(); 
        if (clang::Lexer::isAtStartOfMacroExpansion(startLoc, SM, Context_->getLangOpts())) { 
            std::cout << "is start of macro expansion" << std::endl;
        } 
        return true; 
    }

    int GetBadNames() const {
        return bad_names_;
    }

    std::string GetErrors() {
        ErrorStream_.flush();
        return Errors_;
    }

    void ResetErrors() {
        bad_names_ = 0;
        Errors_.clear();
    }

    std::string GetFilename() {
        return Filename_;
    }

private:
    ASTContext *Context_;
    std::string Errors_;
    llvm::raw_string_ostream ErrorStream_;
    int bad_names_ = 0;

    std::string Filename_;
};

class MyConsumer : public clang::ASTConsumer {
public:
    explicit MyConsumer(ASTContext *Context) : Visitor_(Context) {}

    virtual void HandleTranslationUnit(clang::ASTContext &Context) {
        Visitor_.TraverseDecl(Context.getTranslationUnitDecl());
        PrintStatistics(Visitor_.GetFilename(), Visitor_.GetBadNames(), 0);
        llvm::outs() << Visitor_.GetErrors();
        Visitor_.ResetErrors();
    }

private:
    MyVisitor Visitor_;
};

class MyAction : public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
        return std::unique_ptr<clang::ASTConsumer>(new MyConsumer(&Compiler.getASTContext()));
    }
};

int main(int argc, const char **argv) {
    CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
    //for (const auto& name : OptionsParser.getSourcePathList()) {
    //    std::cerr << "name: " << name << "\n";
    //}
    ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());
    return Tool.run(newFrontendActionFactory<MyAction>().get());
}

