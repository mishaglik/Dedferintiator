#ifndef OP_DEF
#error OP_DEF NOT DEFINED
#else

// name, flags, strVal, Diff, TeX, Weight

/**
 * flags 0b000
 *       0b100 - both arguments
 *       0b010 - isLeftZero   f(0, x) = 0 ?
 *       0b001 - isRightZero  f(x, 0) = 0 ?
 * 
 */


/*      Cats lay on a sick places... :(
      |\      _,,,---,,_                                     _._     _,-'""`-._
ZZZzz /,`.-'`'    -.  ;-;;,_            |\__/,|   (`\       (,-.`._,'(       |\`-/|
     |,4-  ) )-,_. ,\ (  `'-'         _.|o o  |_   ) )          `-.-' \ )-`( , o o)
    '---''(_/--'  `-'\_)             -(((---(((--------               `-    \`_`"'-  */

OP_DEF(ADD, 0b100, '+', ADD(D(L), D(R)), "+", 3 * (L + R) + 1)

OP_DEF(SUB, 0b100, '-', SUB(D(L), D(R)), "-", 3 * (L + R) + 1)

OP_DEF(MUL, 0b111, '*', ADD(MUL(D(L), C(R)), MUL(C(L), D(R))), "*", 3 * (L + R) + 1)

OP_DEF(DIV, 0b110, '/', DIV(SUB(MUL(D(L), C(R)), MUL(C(L), D(R))), POW(C(R), CONST(2))), "", 2 * MAX(L , R) + 1)

OP_DEF(POW, 0b110, '^', (
    isVariable(R, var) ?
        (MUL(C(node), ADD(MUL(D(R), LN(C(L))), MUL(DIV(D(L), C(L)), C(R))))) :
        (MUL(MUL(C(R), POW(C(L), SUB(C(R), CONST(1)))), D(L)))
                
), "^", 8 * L + 2 * R + 1)

OP_DEF(SIN, 0b001, 0x6E6973 /* 'nis' */, MUL(COS(C(R)), D(R)), "\\sin", 2*R + 3)

OP_DEF(COS, 0b000, 0x736F63 /* 'soc' */, MUL(MUL(CONST(-1), SIN(C(R))), D(R)), "\\cos", 2*R + 3)

OP_DEF(TAN, 0b001, 0x006774 /* 'gt'  */, DIV(               D(R) , POW(COS(C(R)), CONST(2))), "\\tan", 2*R + 3)

OP_DEF(COT, 0b000, 0x677463 /* 'gtc' */, DIV(MUL(CONST(-1), D(R)), POW(SIN(C(R)), CONST(2))), "\\cot", 2*R + 3)

OP_DEF(ABS, 0b001, 0x736261 /* 'sba' */, MUL(DIV(ABS(C(R)), C(R)), D(R)), "", 2*R + 3)

OP_DEF(LN , 0b000, 0x006E6C /* 'nl'  */, DIV(D(R), C(R)), "\\ln", 2*R + 3)

OP_DEF(SH , 0b001, 0x006873 /* 'hs'  */, MUL(CH(C(R)), D(R)), "\\sh", 2*R + 2)

OP_DEF(CH , 0b000, 0x006863 /* 'hc'  */, MUL(SH(C(R)), D(R)), "\\ch", 2*R + 2)

OP_DEF(TH , 0b001, 0x006874 /* 'ht'  */, DIV(D(R), POW(CH(C(R)), CONST(2))), "\\th", 2*R + 2)

OP_DEF(CTH , 0b000, 0x687463 /* 'htc'  */, DIV(MUL(CONST(-1), D(R)), POW(SH(C(R)), CONST(2))), "\\cth", 2*R + 2)

OP_DEF(ASN, 0b001, 0x6E7361 /* 'nsa' */, DIV(D(R), POW( SUB(CONST(1), POW(C(R), CONST(2))) , DIV(CONST(1), CONST(2)))), "\\arcsin", 2*R+2)

OP_DEF(ACN, 0b000, 0x6E6361 /* 'nca' */, DIV(MUL(CONST(-1), D(R)), POW( SUB(CONST(1), POW(C(R), CONST(2))) , DIV(CONST(1), CONST(2)))), "\\arccos", 2*R+2)

OP_DEF(ATG, 0b001, 0x677461 /* 'gta' */, DIV(D(R), ADD(CONST(1), POW(C(R), CONST(2)))), "\\arctg", 2*R+2)

OP_DEF(ACT, 0b000, 0x746361 /* 'tca' */, DIV(MUL(CONST(-1),D(R)), ADD(CONST(1), POW(C(R), CONST(2)))), "\\arcctg", 2*R+2)

OP_DEF(DF , 0b000, 'd', D(R), "'", R + 1)

#endif
