#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

#include <iostream>
#include <map>
#include <string>

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

std::map<std::string, std::map<std::pair<std::string, std::string>, int>> ConversionStats;

class CastObserver : public MatchFinder::MatchCallback {
public:
    virtual void run(const MatchFinder::MatchResult &Result) override {
        const ImplicitCastExpr *CastNode = Result.Nodes.getNodeAs<ImplicitCastExpr>("hidden_cast");
        const FunctionDecl *FuncNode = Result.Nodes.getNodeAs<FunctionDecl>("parent_func");

        if (CastNode && FuncNode) {
            // Берем чистые типы без const
            QualType FromType = CastNode->getSubExpr()->getType().getUnqualifiedType();
            QualType ToType = CastNode->getType().getUnqualifiedType();

            // Проверяем, что это встроенные базовые типы 
            if (FromType.getCanonicalType()->isBuiltinType() && 
                ToType.getCanonicalType()->isBuiltinType()) {
                
                PrintingPolicy PrintPol(Result.Context->getLangOpts());
                std::string FromStr = FromType.getAsString(PrintPol);
                std::string ToStr = ToType.getAsString(PrintPol);

                // Отсеиваем фиктивные преобразования, где тип не меняется 
                if (FromStr != ToStr) {
                    std::string FuncName = FuncNode->getNameInfo().getName().getAsString();
                    ConversionStats[FuncName][{FromStr, ToStr}]++;
                }
            }
        }
    }
};

static cl::OptionCategory ToolOptions("hidden-cast-counter options");

int main(int argc, const char **argv) {
    auto ExpectedParser = CommonOptionsParser::create(argc, argv, ToolOptions);
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }
    
    CommonOptionsParser &CmdParser = ExpectedParser.get();
    ClangTool ASTTool(CmdParser.getCompilations(), CmdParser.getSourcePathList());

    CastObserver Observer;
    MatchFinder Finder;

    // Ищем неявный каст внутри функции и привязываем к ним имена
    Finder.addMatcher(
        implicitCastExpr(hasAncestor(functionDecl().bind("parent_func"))).bind("hidden_cast"),
        &Observer
    );

    // Запуск парсинга
    int status = ASTTool.run(newFrontendActionFactory(&Finder).get());

    // Выводим результаты
    for (const auto &FuncData : ConversionStats) {
        std::cout << "Function " << FuncData.first << "\n";
        for (const auto &CastData : FuncData.second) {
            std::cout << CastData.first.first << " -> " << CastData.first.second 
                      << ": " << CastData.second << "\n";
        }
        std::cout << "\n";
    }

    return status;
}