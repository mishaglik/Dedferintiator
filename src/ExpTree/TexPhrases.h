
#define CHOOSE(arr) arr[rand() % (arr ## _SZ)]

const char TEX_INIT[] = "\\documentclass{article}"
                        "\\title{{\\Huge Матан?}\\\\ {\\LARGE Символьные методы дифференциального исчисления над множеством функций многих переменных.}\\\\ {\\small 1.5 Производные по цене 1}}\n"
                        "\\usepackage[utf8]{inputenc}\n"
                        "\\usepackage[T2A]{fontenc}\n"
                        "\\usepackage[russian]{babel}\n"
                        "\\usepackage{amssymb}\n"
                        "\\usepackage{amsmath}\n"
                        "\\usepackage{geometry}\n"
                        "\\usepackage{enumitem}\n"
                        "\\newgeometry{vmargin={15mm}, hmargin={12mm,17mm}}\n"
                        "\\author{Глинский Михаил Б05-133 }\n"
                        "\\date{Ноябрь 2021}\n"
                        "\\begin{document}\n"
                        "\\maketitle\n";
	


const char TEX_FINAL[] = "\\end{document}";;