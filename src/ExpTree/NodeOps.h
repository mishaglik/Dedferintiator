#ifndef DEDFTR_TREE_NODE_OPS_H
#define DEDFTR_TREE_NODE_OPS_H

/*      Cats lays on sick place.
      |\      _,,,---,,_                                     _._     _,-'""`-._
ZZZzz /,`.-'`'    -.  ;-;;,_            |\__/,|   (`\       (,-.`._,'(       |\`-/|
     |,4-  ) )-,_. ,\ (  `'-'         _.|o o  |_   ) )          `-.-' \ )-`( , o o)
    '---''(_/--'  `-'\_)             -(((---(((--------               `-    \`_`"'-  */
#define ADD(left, right) newNode(ExprNodeType::OPERATOR, Operator::ADD, left, right)
#define SUB(left, right) newNode(ExprNodeType::OPERATOR, Operator::SUB, left, right)
#define MUL(left, right) newNode(ExprNodeType::OPERATOR, Operator::MUL, left, right)
#define DIV(left, right) newNode(ExprNodeType::OPERATOR, Operator::DIV, left, right)

#define SIN(left, right) newNode(ExprNodeType::OPERATOR, Operator::SIN, left, right)
#define COS(left, right) newNode(ExprNodeType::OPERATOR, Operator::COS, left, right)
#define TAN(left, right) newNode(ExprNodeType::OPERATOR, Operator::TAN, left, right)
#define COT(left, right) newNode(ExprNodeType::OPERATOR, Operator::COT, left, right)

#define ABS(left, right) newNode(ExprNodeType::OPERATOR, Operator::ABS, left, right)
#endif
