#include "clang/AST/AST.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"
#include "llvm/Support/CommandLine.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

static cl::OptionCategory ToolOptions("loop-wrapper options");

class LoopObserver : public MatchFinder::MatchCallback {
    Rewriter &TheRewriter;
public:
    LoopObserver(Rewriter &R) : TheRewriter(R) {}

    virtual void run(const MatchFinder::MatchResult &Result) override {
        const Stmt *LoopNode = Result.Nodes.getNodeAs<Stmt>("loop");
        if (LoopNode) {
            // Вставляем loop_start() перед циклом
            TheRewriter.InsertText(LoopNode->getBeginLoc(), "loop_start();\n", true, true);

            // Ищем конец цикла, чтобы вставить loop_end()
            SourceLocation EndLoc = LoopNode->getEndLoc();
            SourceLocation InsertLoc = Lexer::getLocForEndOfToken(EndLoc, 0, *Result.SourceManager, Result.Context->getLangOpts());

            if (isa<DoStmt>(LoopNode)) {
                std::optional<Token> NextTok = Lexer::findNextToken(EndLoc, *Result.SourceManager, Result.Context->getLangOpts());
                if (NextTok && NextTok->is(tok::semi)) {
                    InsertLoc = Lexer::getLocForEndOfToken(NextTok->getLocation(), 0, *Result.SourceManager, Result.Context->getLangOpts());
                }
            }

            TheRewriter.InsertText(InsertLoc, "\nloop_end();", true, true);
        }
    }
};

class RewriteAction : public ASTFrontendAction {
    Rewriter TheRewriter;
    MatchFinder Finder;
    LoopObserver *Observer;

public:
    RewriteAction() { Observer = nullptr; }

    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        Observer = new LoopObserver(TheRewriter);
        
        // Ищем циклы
        Finder.addMatcher(stmt(anyOf(forStmt(), whileStmt(), cxxForRangeStmt(), doStmt())).bind("loop"), Observer);
        
        return Finder.newASTConsumer();
    }

    virtual void EndSourceFileAction() override {
        SourceManager &SM = TheRewriter.getSourceMgr();
        FileID MainFileID = SM.getMainFileID();
        const RewriteBuffer *RewriteBuf = TheRewriter.getRewriteBufferFor(MainFileID);
        
        // Выводим измененный код 
        if (RewriteBuf) {
            llvm::outs() << std::string(RewriteBuf->begin(), RewriteBuf->end());
        } else {
            llvm::outs() << SM.getBufferData(MainFileID);
        }
        delete Observer;
    }
};

int main(int argc, const char **argv) {
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, ToolOptions);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    CommonOptionsParser &CmdParser = ExpectedParser.get();
    ClangTool ASTTool(CmdParser.getCompilations(), CmdParser.getSourcePathList());

    return ASTTool.run(newFrontendActionFactory<RewriteAction>().get());
}