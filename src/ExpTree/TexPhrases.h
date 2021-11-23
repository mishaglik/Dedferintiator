
#define CHOOSE(arr) arr[rand() % (arr ## _SZ)]
#define PROBAB(k, n) ((rand() % n) < k)

const char TEX_INIT[] = "\\documentclass{report}\n"
                        "\\title{\\textbf{\\Huge Матан?}\\\\ {\\LARGE Символьные методы дифференциального исчисления над множеством функций многих переменных.}\\\\ {\\small 1.5 Производные по цене 1}}\n"
                        "\\usepackage[utf8]{inputenc}\n"
                        "\\usepackage[T2A]{fontenc}\n"
                        "\\usepackage[russian]{babel}\n"
                        "\\usepackage{amssymb}\n"
                        "\\usepackage[auto]{chappg}"
                        "\\usepackage{epigraph}"
                        "\\usepackage[normalem]{ulem}\n"
                        "\\usepackage{amsmath}\n"
                        "\\usepackage{geometry}\n"
                        "\\usepackage{enumitem}\n"
                        "%\\newgeometry{vmargin={15mm}, hmargin={12mm,17mm}}\n"
                        "\\author{Глинский Михаил Б05-133 }\n"
                        "\\date{Ноябрь. Осень. Дождь. Снег. Мокрый снег. 2021}\n"
                        "\\begin{document}\n"
                        "\\maketitle\n"
                        "\\vspace*{\\fill}\n"
                        "\\begin{center}\n"
                        "\\section*{Предисловие}\n"
                        "\\end{center}\n"
                        "\\vspace*{\\fill}\n"
                        "Дорогой читатель! Читая эту ***[Расшифровка эвфеминистических выражений предоставляется читателю в качестве упражнения. - Авт.] ты и представить себе не можешь сколько \\sout{боли и страданий} удовольствия ты получишь в результате прочтения!\n"
                        "Я, великий автор книги, ставил целью максимально просто и понятно донести до читателя основы излагаемого материала.\n"
                        "Но как бы то ни было, нельзя вот так взять и понять матан. В математике царит баланс. И для обретения знаний нужно потерять что-то равноценное. \\textit{Душу, например}.\n"
                        "\n"
                        "Прочтя эту \\%!@* ты осознаешь, почему не стоит трать время впустую... Итак, вперёд, читатель!!!\n"
                        "\n"
                        "\\begin{flushright}\n"
                        "\\textit{C уважением, Автор}\n"
                        "\\end{flushright}\n"
                        "\\vspace*{\\fill}\n"
                        "\\tableofcontents\n"
                        ;//TODO: Red infernal scripts
	


const char TEX_FINAL[] = "\\end{document}";

const int   TEX_DIF_VAR_SZ = 3;
const char* TEX_DIF_VAR[TEX_DIF_VAR_SZ] = {
    "Берем производную по $ %c $:\n",
    "Рассмотрим данную функцию как от $ %c $:\n",
    "Теперь нужно лишь взять производную по $ %c $:\n",
};

const int   TEX_DIFF_SZ = 7;
const char* TEX_DIFF[TEX_DIFF_SZ] = {
    "Теперь исследуем данную функцию:\n",
    "Очевидно что \n",
    "Как уже показано ранее:\n",
    "Можно заметить, что \n",
    "",
    "",
    "По известному соотношению\n"
};

const int   TEX_DIFF_START_SZ = 1;
const char* TEX_DIFF_START[TEX_DIFF_START_SZ] = {
"Что ж нам дали функцию, так что для начала пронализируем входные данные, с целью \\textit{\\sout{гадания на БРС}} определения класса задачи.\n",
};











const char TEX_CHAPTER_1[] = "\\chapter{Матчасть}\n"
                             "\\epigraph{{А прежде всего матчасть.}}{\\textit{Народная мудрость}}\n"
                             "\\section{О числах}\n"
                             "\\subsection{Целые числа}\n"
                             "Все вы знаете целые числа. А вот вам вопрос $ -1/12 $ целое ли число?\n"
                             "Теперь всё сложно ведь:\n"
                             "$$\n"
                             "\\sum_{n=1}^{\\infty} n = -\\frac{1}{12}\n"
                             "$$\n"
                             "Слева целое число, как сумма целых. А вот справа нет. Па-ла-докс!\n"
                             "Вообще нам за глаза хватает целых чисел. Но вот открою вам страшную тайну. Нет никакой необходимости вводить другие.\n"
                             "\\subsection{Вещественные числа}\n"
                             "Их нет.\n"
                             "\\chapter{Производная}\n"
                             "\\epigraph{\\textit{Дифференцировал дифференцировал, да не выдифференцировал...}}{Любой студент тех вуза.}\n"
                             ;