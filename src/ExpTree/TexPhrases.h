
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

const int   TEX_DIFF_SZ = 17;
const char* TEX_DIFF[TEX_DIFF_SZ] = {
    "Теперь исследуем данную функцию:\n",
    "Очевидно что \n",
    "Как уже показано ранее:\n",
    "Можно заметить, что \n",
    "",
    "",
    "По известному соотношению\n",
    "Ни для кого ни секрет, что\n",
    "Как было сказано в \\ref{DerTable}\n",
    "Как было сказано в \\ref{What}\n",
    "Можно было и не упомянать, что\n",
    "Из вышесказанного следует\n",
    "Lorem ipsum dolores\n",
    "Если вы дочитали, то уже вам несложно видеть, что это всего лишь\n",
    "А здесь немного магии из предыдущего параграфа\n",
    "Несложно видеть, что это очевидным образом преобразуется в \n",
    "А доказательство этого вы можете найти в \\cite{Proof}\n"
};

const int   TEX_DIFF_START_SZ = 1;
const char* TEX_DIFF_START[TEX_DIFF_START_SZ] = {
"",
};






const char TEX_CHAPTER_1[] = "\\include{tex/Chapter1.tex}\n";

const char* TEX_PARTS[] = {
    "\\input{tex/Chapter2Part1.tex}\n",
    "\\input{tex/Chapter2Part2.tex}\n",
    "\\input{tex/Chapter2Part3.tex}\n",
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


};