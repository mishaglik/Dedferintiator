#ifndef OP_DEF
#error OP_DEF NOT DEFINED
#else

// name, flags, strVal, Diff, TeX

/**
 * flags 0b0000000N
 *       0b1 - both arguments
 * 
 */


/*      Cats lays on sick place.
      |\      _,,,---,,_                                     _._     _,-'""`-._
ZZZzz /,`.-'`'    -.  ;-;;,_            |\__/,|   (`\       (,-.`._,'(       |\`-/|
     |,4-  ) )-,_. ,\ (  `'-'         _.|o o  |_   ) )          `-.-' \ )-`( , o o)
    '---''(_/--'  `-'\_)             -(((---(((--------               `-    \`_`"'-  */

OP_DEF(ADD, 0b100, '+', ADD(D(L), D(R)), "+")

OP_DEF(SUB, 0b100, '-', SUB(D(L), D(R)), "-")

OP_DEF(MUL, 0b111, '*', ADD(MUL(D(L), C(R)), MUL(C(L), D(R))), "*")

OP_DEF(DIV, 0b110, '/', DIV(SUB(MUL(D(L), C(R)), MUL(C(L), D(R))), POW(C(R), CONST(2))), "")

OP_DEF(POW, 0b110, '^', (
    isVariable(R, var) ?
        (MUL(C(node), ADD(MUL(D(R), LN(C(L))), MUL(DIV(D(L), C(L)), C(R))))) :
        (MUL(MUL(C(R), POW(C(L), SUB(C(R), CONST(1)))), D(L)))
                
), "^")

OP_DEF(SIN, 0b001, 0x6E6973 /* 'nis' */, MUL(COS(C(R)), D(R)), "\\sin")

OP_DEF(COS, 0b000, 0x736F63 /* 'soc' */, MUL(MUL(CONST(-1), SIN(C(R))), D(R)), "\\cos")

OP_DEF(TAN, 0b001, 0x006774 /* 'gt'  */, DIV(               D(R) , POW(COS(C(R)), CONST(2))), "\\tan")

OP_DEF(COT, 0b000, 0x677463 /* 'gtc' */, DIV(MUL(CONST(-1), D(R)), POW(SIN(C(R)), CONST(2))), "\\cot")

OP_DEF(ABS, 0b001, 0x736261 /* 'sba' */, MUL(DIV(ABS(C(R)), C(R)), D(R)), "")

OP_DEF(LN , 0b000, 0x006E6C /* 'nl'  */, DIV(D(R), C(R)), "\\ln")

OP_DEF(DF , 0b000, 'd', D(R), "'")

#endif