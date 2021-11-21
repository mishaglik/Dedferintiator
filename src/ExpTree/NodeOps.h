#ifndef DEDFTR_TREE_NODE_OPS_H
#define DEDFTR_TREE_NODE_OPS_H

/*      Cats lays on sick place.
      |\      _,,,---,,_                                     _._     _,-'""`-._
ZZZzz /,`.-'`'    -.  ;-;;,_            |\__/,|   (`\       (,-.`._,'(       |\`-/|
     |,4-  ) )-,_. ,\ (  `'-'         _.|o o  |_   ) )          `-.-' \ )-`( , o o)
    '---''(_/--'  `-'\_)             -(((---(((--------               `-    \`_`"'-  */
#define ADD(left, right) newNode(ExprNodeType::OPERATOR, {.opr = Operator::ADD}, left, right)
#define SUB(left, right) newNode(ExprNodeType::OPERATOR, {.opr = Operator::SUB}, left, right)
#define MUL(left, right) newNode(ExprNodeType::OPERATOR, {.opr = Operator::MUL}, left, right)
#define DIV(left, right) newNode(ExprNodeType::OPERATOR, {.opr = Operator::DIV}, left, right)
#define POW(left, right) newNode(ExprNodeType::OPERATOR, {.opr = Operator::POW}, left, right)

#define SIN(right) newNode(ExprNodeType::OPERATOR, {.opr = Operator::SIN}, NULL, right)
#define COS(right) newNode(ExprNodeType::OPERATOR, {.opr = Operator::COS}, NULL, right)
#define TAN(right) newNode(ExprNodeType::OPERATOR, {.opr = Operator::TAN}, NULL, right)
#define COT(right) newNode(ExprNodeType::OPERATOR, {.opr = Operator::COT}, NULL, right)

#define LN(right)  newNode(ExprNodeType::OPERATOR, {.opr = Operator::LN }, NULL, right)

#define ABS(right) newNode(ExprNodeType::OPERATOR, {.opr = Operator::ABS}, NULL, right)

#define CONST(x)  newNode(ExprNodeType::NUMBER  , {.num = x}, NULL, NULL)
#define VAR(x)    newNode(ExprNodeType::VARIABLE, {.var = x}, NULL, NULL)
#endif
