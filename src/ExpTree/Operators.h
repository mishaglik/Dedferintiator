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

OP_DEF(DF , 0b000, 'd', D(R), "'", R + 1)

#endif
