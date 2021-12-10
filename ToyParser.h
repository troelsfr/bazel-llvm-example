/* parser/listener/visitor header section */

// Generated from /private/var/tmp/_bazel_tfr/eb219285a319690cc062d785a8a7d1bd/sandbox/darwin-sandbox/34/execroot/wonop/qir/cc/parser/ToyParser.g4 by ANTLR 4.8

#pragma once

/* parser precinclude section */

#include "antlr4-runtime.h"


/* parser postinclude section */
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


namespace toylang {

/* parser context section */

class  ToyParser : public antlr4::Parser {
public:
  enum {
    DUMMY = 1, Return = 2, Continue = 3, Break = 4, From = 5, Import = 6, 
    FunctionDecl = 7, ImutableVariableDecl = 8, MutVariableDecl = 9, ContextDecl = 10, 
    IntegerLiteralPostfix = 11, Integer = 12, Digit = 13, ID = 14, LessThan = 15, 
    GreaterThan = 16, Equal = 17, And = 18, Colon = 19, Semicolon = 20, 
    Plus = 21, Minus = 22, Multiply = 23, Divide = 24, Remainder = 25, OpenPar = 26, 
    ClosePar = 27, OpenCurly = 28, CloseCurly = 29, OpenSquare = 30, CloseSquare = 31, 
    QuestionMark = 32, Comma = 33, String = 34, Foo = 35, Bar = 36, Any = 37, 
    MultiLineComment = 38, SingleLineComment = 39, WS = 40, Dot = 41, DotDot = 42, 
    Dollar = 43, Ampersand = 44
  };

  enum {
    RuleMain = 0, RuleDivide = 1, RuleAnd_ = 2, RuleConquer = 3, RuleUnused = 4, 
    RuleFunctionDeclaration = 5, RuleFunctionArguments = 6, RuleSingleArgumentDeclaration = 7, 
    RuleUnused2 = 8, RuleStatement = 9, RuleArgumentInstances = 10, RuleAssignment = 11, 
    RuleDeclaration = 12, RuleArrayInitializer = 13, RuleKvInstances = 14, 
    RuleStructInitializer = 15, RuleKeyValuePair = 16, RuleExpression = 17, 
    RuleFlowControl = 18, RuleTypeName = 19, RuleId = 20, RuleArray = 21, 
    RuleIdarray = 22, RuleAny = 23
  };

  ToyParser(antlr4::TokenStream *input);
  ~ToyParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  /* public parser declarations/members section */
  bool myAction() { return true; }
  bool doesItBlend() { return true; }
  void cleanUp() {}
  void doInit() {}
  void doAfter() {}


  class MainContext;
  class DivideContext;
  class And_Context;
  class ConquerContext;
  class UnusedContext;
  class FunctionDeclarationContext;
  class FunctionArgumentsContext;
  class SingleArgumentDeclarationContext;
  class Unused2Context;
  class StatementContext;
  class ArgumentInstancesContext;
  class AssignmentContext;
  class DeclarationContext;
  class ArrayInitializerContext;
  class KvInstancesContext;
  class StructInitializerContext;
  class KeyValuePairContext;
  class ExpressionContext;
  class FlowControlContext;
  class TypeNameContext;
  class IdContext;
  class ArrayContext;
  class IdarrayContext;
  class AnyContext; 

  class  MainContext : public antlr4::ParserRuleContext {
  public:
    MainContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<FunctionDeclarationContext *> functionDeclaration();
    FunctionDeclarationContext* functionDeclaration(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MainContext* main();

  class  DivideContext : public antlr4::ParserRuleContext {
  public:
    DivideContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    And_Context *and_();
    antlr4::tree::TerminalNode *GreaterThan();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DivideContext* divide();

  class  And_Context : public antlr4::ParserRuleContext {
  public:
    And_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *And();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  And_Context* and_();

  class  ConquerContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *idToken = nullptr;;
    ConquerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DivideContext *> divide();
    DivideContext* divide(size_t i);
    And_Context *and_();
    antlr4::tree::TerminalNode *ID();
    std::vector<antlr4::tree::TerminalNode *> LessThan();
    antlr4::tree::TerminalNode* LessThan(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConquerContext* conquer();

  class  UnusedContext : public antlr4::ParserRuleContext {
  public:
    double input = 111;
    double calculated;
    int _a;
    double _b;
    int _c;
    UnusedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    UnusedContext(antlr4::ParserRuleContext *parent, size_t invokingState, double input = 111);
    virtual size_t getRuleIndex() const override;
    StatementContext *statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnusedContext* unused(double input = 111);

  class  FunctionDeclarationContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *name = nullptr;;
    ToyParser::TypeNameContext *returnType = nullptr;;
    FunctionDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *FunctionDecl();
    antlr4::tree::TerminalNode *OpenPar();
    antlr4::tree::TerminalNode *ClosePar();
    antlr4::tree::TerminalNode *Colon();
    antlr4::tree::TerminalNode *OpenCurly();
    antlr4::tree::TerminalNode *CloseCurly();
    antlr4::tree::TerminalNode *ID();
    TypeNameContext *typeName();
    FunctionArgumentsContext *functionArguments();
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionDeclarationContext* functionDeclaration();

  class  FunctionArgumentsContext : public antlr4::ParserRuleContext {
  public:
    FunctionArgumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SingleArgumentDeclarationContext *> singleArgumentDeclaration();
    SingleArgumentDeclarationContext* singleArgumentDeclaration(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionArgumentsContext* functionArguments();

  class  SingleArgumentDeclarationContext : public antlr4::ParserRuleContext {
  public:
    SingleArgumentDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();
    antlr4::tree::TerminalNode *Colon();
    TypeNameContext *typeName();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SingleArgumentDeclarationContext* singleArgumentDeclaration();

  class  Unused2Context : public antlr4::ParserRuleContext {
  public:
    Unused2Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> Semicolon();
    antlr4::tree::TerminalNode* Semicolon(size_t i);
    std::vector<UnusedContext *> unused();
    UnusedContext* unused(size_t i);
    antlr4::tree::TerminalNode *Colon();
    antlr4::tree::TerminalNode *Plus();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Unused2Context* unused2();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclarationContext *declaration();
    antlr4::tree::TerminalNode *Semicolon();
    AssignmentContext *assignment();
    FlowControlContext *flowControl();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementContext* statement();

  class  ArgumentInstancesContext : public antlr4::ParserRuleContext {
  public:
    ArgumentInstancesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArgumentInstancesContext* argumentInstances();

  class  AssignmentContext : public antlr4::ParserRuleContext {
  public:
    AssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    antlr4::tree::TerminalNode *Equal();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentContext* assignment();

  class  DeclarationContext : public antlr4::ParserRuleContext {
  public:
    ToyParser::TypeNameContext *type = nullptr;;
    ToyParser::ExpressionContext *init = nullptr;;
    DeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ImutableVariableDecl();
    IdContext *id();
    antlr4::tree::TerminalNode *Equal();
    antlr4::tree::TerminalNode *Colon();
    ExpressionContext *expression();
    TypeNameContext *typeName();
    antlr4::tree::TerminalNode *MutVariableDecl();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclarationContext* declaration();

  class  ArrayInitializerContext : public antlr4::ParserRuleContext {
  public:
    ArrayInitializerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenSquare();
    ArgumentInstancesContext *argumentInstances();
    antlr4::tree::TerminalNode *CloseSquare();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArrayInitializerContext* arrayInitializer();

  class  KvInstancesContext : public antlr4::ParserRuleContext {
  public:
    KvInstancesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<KeyValuePairContext *> keyValuePair();
    KeyValuePairContext* keyValuePair(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KvInstancesContext* kvInstances();

  class  StructInitializerContext : public antlr4::ParserRuleContext {
  public:
    StructInitializerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenSquare();
    KvInstancesContext *kvInstances();
    antlr4::tree::TerminalNode *CloseSquare();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StructInitializerContext* structInitializer();

  class  KeyValuePairContext : public antlr4::ParserRuleContext {
  public:
    ToyParser::IdContext *identifier = nullptr;;
    KeyValuePairContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Colon();
    ExpressionContext *expression();
    IdContext *id();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  KeyValuePairContext* keyValuePair();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ToyParser::ExpressionContext *lhs = nullptr;;
    ToyParser::IdContext *functionName = nullptr;;
    ToyParser::ArgumentInstancesContext *arguments = nullptr;;
    ToyParser::ExpressionContext *subexpression = nullptr;;
    ToyParser::IdContext *identifier = nullptr;;
    antlr4::Token *type = nullptr;;
    antlr4::Token *op = nullptr;;
    ToyParser::ExpressionContext *rhs = nullptr;;
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenPar();
    antlr4::tree::TerminalNode *ClosePar();
    IdContext *id();
    ArgumentInstancesContext *argumentInstances();
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    ArrayInitializerContext *arrayInitializer();
    antlr4::tree::TerminalNode *Integer();
    antlr4::tree::TerminalNode *IntegerLiteralPostfix();
    antlr4::tree::TerminalNode *String();
    antlr4::tree::TerminalNode *Multiply();
    antlr4::tree::TerminalNode *Divide();
    antlr4::tree::TerminalNode *Remainder();
    antlr4::tree::TerminalNode *Plus();
    antlr4::tree::TerminalNode *Minus();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionContext* expression();
  ExpressionContext* expression(int precedence);
  class  FlowControlContext : public antlr4::ParserRuleContext {
  public:
    FlowControlContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    FlowControlContext() = default;
    void copyFrom(FlowControlContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ReturnContext : public FlowControlContext {
  public:
    ReturnContext(FlowControlContext *ctx);

    antlr4::tree::TerminalNode *Return();
    ExpressionContext *expression();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ContinueContext : public FlowControlContext {
  public:
    ContinueContext(FlowControlContext *ctx);

    antlr4::tree::TerminalNode *Break();
    antlr4::tree::TerminalNode *Continue();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  FlowControlContext* flowControl();

  class  TypeNameContext : public antlr4::ParserRuleContext {
  public:
    TypeNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    IdContext *id();
    antlr4::tree::TerminalNode *LessThan();
    std::vector<TypeNameContext *> typeName();
    TypeNameContext* typeName(size_t i);
    antlr4::tree::TerminalNode *GreaterThan();
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeNameContext* typeName();

  class  IdContext : public antlr4::ParserRuleContext {
  public:
    IdContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdContext* id();

  class  ArrayContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *integerToken = nullptr;;
    std::vector<antlr4::Token *> el;;
    ArrayContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenCurly();
    antlr4::tree::TerminalNode *CloseCurly();
    std::vector<antlr4::tree::TerminalNode *> Integer();
    antlr4::tree::TerminalNode* Integer(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArrayContext* array();

  class  IdarrayContext : public antlr4::ParserRuleContext {
  public:
    ToyParser::IdContext *idContext = nullptr;;
    std::vector<IdContext *> element;;
    IdarrayContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OpenCurly();
    antlr4::tree::TerminalNode *CloseCurly();
    std::vector<IdContext *> id();
    IdContext* id(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdarrayContext* idarray();

  class  AnyContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *t = nullptr;;
    AnyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AnyContext* any();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool divideSempred(DivideContext *_localctx, size_t predicateIndex);
  bool conquerSempred(ConquerContext *_localctx, size_t predicateIndex);
  bool expressionSempred(ExpressionContext *_localctx, size_t predicateIndex);

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;

  /* private parser declarations section */

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

}  // namespace toylang
