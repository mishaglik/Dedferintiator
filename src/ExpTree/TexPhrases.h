
#define CHOOSE(arr) arr[rand() % (arr ## _SZ)]
#define PROBAB(k, n) ((rand() % n) < k)

const char TEX_INIT[] = "\\input{tex/preambula.tex}"; //TODO: Red infernal scripts

const char TEX_FINAL[] = "\\input{tex/Bibliography.tex}\n\\end{document}";

const int   TEX_DIF_VAR_SZ = 4;
const char* TEX_DIF_VAR[TEX_DIF_VAR_SZ] = {
    "Берем производную по $ %c $:\\\\\n",
    "Рассмотрим данную функцию как от $ %c $:\\\\ \n",
    "Теперь нужно лишь взять производную по $ %c $:\\\\\n",
    "Не забывайте, о существовании $ %c $\\\\\n",
};

const int   TEX_DIFF_SZ = 19;
const char* TEX_DIFF[TEX_DIFF_SZ] = {
    "Теперь исследуем данную функцию:\n",
    "Очевидно что \n",
    "Как уже показано ранее:\n",
    "Можно заметить, что \n",
    "Из того, что с==3 следует",
    "",
    "По известному соотношению\n",
    "Ни для кого ни секрет, что\n",
    "Как было сказано в \\ref{DerTable}\n",
    "Как было сказано в \\ref{What}\n",
    "Можно было и не упомянать, что\n",
    "",
    "Lorem ipsum dolores\n",
    "Если вы дочитали, то уже вам несложно видеть, что это всего лишь\n",
    "А здесь немного магии из предыдущего параграфа\n",
    "Несложно видеть, что это очевидным образом преобразуется в \n",
    "А доказательство этого вы можете найти в \\cite{Proof}\n",
    "Счёт производной мы не бросим 1488\n",
    "Могли бы уже землю обогнуть, а тут производная\n",
};

const int   TEX_DIFF_START_SZ = 1;
const char* TEX_DIFF_START[TEX_DIFF_START_SZ] = {
"",
};


const int   TEX_RENAME_SZ = 9;
const char* TEX_RENAME[TEX_RENAME_SZ] = {
    "Обзозначим за",
    "Пусть:",
    "Положим:",
    "(0o0)",
    "",
    "Удобно взять ",
    "Ввиду громоздкости объявим",
    "Как завещал нам ДЕД\\cite{DED}:",
    "Букв много не бывает, так что пусть:"
};



const char TEX_CHAPTER_1[] = "\\include{tex/Chapter1.tex}\n";

const char* TEX_PARTS[] = {
    "\\input{tex/Chapter2Part1.tex}\n",
    "\\input{tex/Chapter2Part2.tex}\n",
    "\\input{tex/Chapter2Part3.tex}\n",
    "\\input{tex/Chapter2Part4.tex}\n",
    "\\input{tex/Chapter3Part2.tex}\n",
};

const char* TEX_VARS[] = {
    "\\alpha",
    "\\beta",
    "\\gamma",
    "\\delta",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "\\mu",
    "\\nu",
    "\\psi",
    "\\epsilon",
    "end",
    "\\alpha_1",
    "\\alpha_2",
    "\\alpha_3",
    "\\alpha_4",
    "\\alpha_5",
    "\\alpha_6",
    "\\alpha_7",
    "\\alpha_8",
    "\\alpha_9",
    "\\alpha_{10}",
    "\\alpha_{11}",
    "\\alpha_{12}",
    "\\alpha_{13}",
    "\\alpha_{14}",
    "\\alpha_{15}",
    "\\alpha_{16}",
    "\\alpha_{17}",
    "\\alpha_{18}",
};