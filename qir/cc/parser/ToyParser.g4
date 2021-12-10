parser grammar ToyParser;

options {
  tokenVocab = ToyLexer;
}

// These are all supported parser sections:

// Parser file header. Appears at the top in all parser related files. Use e.g. for copyrights.
@parser::header {/* parser/listener/visitor header section */}

// Appears before any #include in h + cpp files.
@parser::preinclude {/* parser precinclude section */}

// Follows directly after the standard #includes in h + cpp files.
@parser::postinclude {
/* parser postinclude section */
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
}

// Directly preceeds the parser class declaration in the h file (e.g. for additional types etc.).
@parser::context {/* parser context section */}

// Appears in the private part of the parser in the h file.
// The function bodies could also appear in the definitions section, but I want to maximize
// Java compatibility, so we can also create a Java parser from this grammar.
// Still, some tweaking is necessary after the Java file generation (e.g. bool -> boolean).
@parser::members {
/* public parser declarations/members section */
bool myAction() { return true; }
bool doesItBlend() { return true; }
void cleanUp() {}
void doInit() {}
void doAfter() {}
}

// Appears in the public part of the parser in the h file.
@parser::declarations {/* private parser declarations section */}

// Appears in line with the other class member definitions in the cpp file.
@parser::definitions {/* parser definitions section */}

// Additionally there are similar sections for (base)listener and (base)visitor files.
@parser::listenerpreinclude {/* listener preinclude section */}
@parser::listenerpostinclude {/* listener postinclude section */}
@parser::listenerdeclarations {/* listener public declarations/members section */}
@parser::listenermembers {/* listener private declarations/members section */}
@parser::listenerdefinitions {/* listener definitions section */}

@parser::baselistenerpreinclude {/* base listener preinclude section */}
@parser::baselistenerpostinclude {/* base listener postinclude section */}
@parser::baselistenerdeclarations {/* base listener public declarations/members section */}
@parser::baselistenermembers {/* base listener private declarations/members section */}
@parser::baselistenerdefinitions {/* base listener definitions section */}

@parser::visitorpreinclude {/* visitor preinclude section */}
@parser::visitorpostinclude {/* visitor postinclude section */}
@parser::visitordeclarations {/* visitor public declarations/members section */}
@parser::visitormembers {/* visitor private declarations/members section */}
@parser::visitordefinitions {/* visitor definitions section */}

@parser::basevisitorpreinclude {/* base visitor preinclude section */}
@parser::basevisitorpostinclude {/* base visitor postinclude section */}
@parser::basevisitordeclarations {/* base visitor public declarations/members section */}
@parser::basevisitormembers {/* base visitor private declarations/members section */}
@parser::basevisitordefinitions {/* base visitor definitions section */}

// Actual grammar start.
main: functionDeclaration+ EOF;
divide : ID (and_ GreaterThan)? {doesItBlend()}?;
and_ @init{ doInit(); } @after { doAfter(); } : And ;

conquer:
  divide+
  | {doesItBlend()}? and_ { myAction(); }
  | ID (LessThan* divide)?? { $ID.text; }
;

// Unused rule to demonstrate some of the special features.
unused[double input = 111] returns [double calculated] locals [int _a, double _b, int _c] @init{ doInit(); } @after { doAfter(); } :
  statement
;
catch [...] {
  // Replaces the standard exception handling.
}
finally {
  cleanUp();
}

functionDeclaration: FunctionDecl name = ID OpenPar functionArguments? ClosePar Colon returnType=typeName
  OpenCurly 
      (statement)* 
  CloseCurly;

functionArguments: singleArgumentDeclaration (Comma singleArgumentDeclaration)*;

singleArgumentDeclaration: id Colon typeName;

unused2:
  (unused[1] .)+ (Colon | Semicolon | Plus)? ~Semicolon
;

/*
constDeclaration: ConstVariableDecl id (Colon type=typeName)? (Equal init=constExpression);
constExpression: lhs=constExpression op=(Multiply | Divide | Remainder) rhs=constExpression
    | lhs=constExpression op=(Plus | Minus) rhs=constExpression
    | functionName = id OpenPar arguments=argumentInstances  ClosePar
    | OpenPar subexpression=constExpression ClosePar
    | arrayInitializer    
    | identifier = id
    | Integer type=IntegerLiteralPostfix?
    | String
*/


statement: 
    declaration Semicolon 
  | assignment Semicolon
  | flowControl Semicolon
  | expression Semicolon
;

argumentInstances: expression (Comma expression)*;

assignment: lhs=expression Equal rhs=expression;

declaration: ImutableVariableDecl id (Colon type=typeName)? (Equal init=expression)
           | MutVariableDecl id (Colon type=typeName)? (Equal init=expression) ;




arrayInitializer: OpenSquare argumentInstances CloseSquare;

kvInstances: keyValuePair (Comma keyValuePair)*;
structInitializer: OpenSquare kvInstances CloseSquare;

keyValuePair: identifier = id Colon expression;

expression: arrayAddress = expression OpenSquare index = expression CloseSquare
    | lhs=expression op=(Multiply | Divide | Remainder) rhs=expression
    | lhs=expression op=(Plus | Minus) rhs=expression
    | functionName = id OpenPar arguments=argumentInstances  ClosePar
    | OpenPar subexpression=expression ClosePar  
    | identifier = id
    | Integer type=IntegerLiteralPostfix?
    | String
    | arrayInitializer  
    ;


flowControl:
  Return expression? # Return
  | Break # Continue
  | Continue # Continue
;

typeName : id (LessThan typeName (Comma typeName)* GreaterThan)?;

id: ID;
array : OpenCurly el += Integer (Comma el += Integer)* CloseCurly;
idarray : OpenCurly element += id (Comma element += id)* CloseCurly;
any: t = .;

