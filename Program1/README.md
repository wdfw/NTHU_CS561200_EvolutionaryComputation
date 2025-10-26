參數表:
| Options       | Description | Default |
| ------------- | ----------- | ------- |
| `-n, --dimension` | The dimension of Schwefel function | 10 |
| `-r, --representation` | The representation: binary or real-valued | binary |
| `-p, --population_size` | Population size | 100 |
| `-u, --uniform_crossover` | Use uniform crossover (1) or not (0). If not: binary GA → 2-point crossover, real GA → whole arithmetic crossover | 0 |
| `-c, --pc` | Crossover probability \(p_c\) | 0.9 |
| `-m, --pm` | Mutation probability \(p_m\) | 0.1 |
| `-g, --generations` | Max number of generations | 500 |
| `-g, --generations` | Max number of generations | 500 |
| `-d, --debug` | Enable debug prints | false |
| `-k, --tournament_size` | Tournament size in parent selection **Additional parameter** | 2 | 
| `--a1, --alpha1` | Probability of swapping in uniform crossover **Additional parameter** | 0.5 |
| `--a2, --alpha2` | Weight of whole arithmetic crossover **Additional parameter** | 0.2 |

編譯方法:
  1. 在本資料夾輸入
    $ make
  2. 接著就會在同個資料夾底下產生執行檔 main

執行方法
  1. 根據參數表指定要使用的基因演算法參數，例如輸入
    $ ./main -n 10 -r binary -p 100 -u 0 -c 0.9 -m 0.1 -g 500 -k 2 --a1 0.5 --a2 0.2 
     代表 使用 Binary 2-point crossover以參數 (維度10, 群體數量100, 交配機率0.9, 變異機率0.1, 競爭者數量2, Uniform crossover交換機率0.5, Whole arithmetic crossover權重0.2) 的設定來進行求解
