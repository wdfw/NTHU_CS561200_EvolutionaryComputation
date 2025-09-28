# EC_HW_2025

## Table of Contents
- [Latest Announcement](#latest-announcement)
- [Sample Code Usage](#sample-code-usage)
- [Input/Output Format](#inputoutput-format)
  - [Input](#input)
  - [Output](#output)
  - [Judger](#judger)
- [Implementation Hints](#implementation-hints)
  - [Representation](#representation)
  - [Parameters](#parameters)
  - [Operators](#operators)
- [Grading](#grading)
- [Submission Guidelines](#submission-guidelines)

---

## Latest Announcement
(09/22) If you encounter any issues or have any questions regarding this repository, please feel free to open an issue or ask on eeclass.

---

## Sample Code Usage

Clone this repository to your local machine:
```bash
git clone https://github.com/CILAB715/EC_HW_2025.git
cd EC_HW_2025
```

C++:
```bash
cd cpp
make
./main -n 10 -r binary -p 100 -u 0 -c 0.9 -m 0.1 -g 500 -d
```

Python:
```bash
cd py
python main.py -n 10 -r binary -p 100 -u 0 -c 0.9 -m 0.1 -g 500 -d
```

Example output:
```
-------------------------------------------
|Parameter           |Value               |
-------------------------------------------
|dimension           |10                  |
|representation      |binary              |
|population_size     |100                 |
|uniform_crossover   |false               |
|crossover_method    |2-point             |
|cross_prob          |0.9                 |
|mut_prob            |0.1                 |
|num_generations     |500                 |
-------------------------------------------
0.00145984
```

---

## Input/Output Format

We provide sample parser code in both C++ and Python.  

You may write your own parser, but it must accept the following parameters, and your program must support at least the following 8 options. You may add more options for convenience.

| Options       | Description | Default |
| ------------- | ----------- | ------- |
| `-n, --dimension` | The dimension of Schwefel function | 10 |
| `-r, --representation` | The representation: binary or real-valued | binary |
| `-p, --population_size` | Population size | 100 |
| `-u, --uniform_crossover` | Use uniform crossover (1) or not (0). If not: binary GA → 2-point crossover, real GA → whole arithmetic crossover | 0 |
| `-c, --pc` | Crossover probability \(p_c\) | 0.9 |
| `-m, --pm` | Mutation probability \(p_m\) | 0.1 |
| `-g, --generations` | Max number of generations | 500 |
| `-d, --debug` | Enable debug prints | false |

### Input
Example testcase (`testcase/01.in`):

C++:
```bash
./main -n 10 -r binary -p 100 -u 0 -c 0.9 -m 0.1 -g 500
```

Python:
```bash
python ./main.py -n 10 -r binary -p 100 -u 0 -c 0.9 -m 0.1 -g 500
```

The first line of the `.in` file contains the parameters.  
The second line shows the configuration table, printed only if `-d` is enabled.

### Output
- Only print the configuration table when `-d` or `--debug` is used.  
- Always output the best fitness of the final generation.

Example:
```
50.5991
```

`testcase/*.out` files contain:
1. Mean best fitness (120 TA trials)  
2. Standard deviation  

Your code will be run for 30 trials. If the mean fitness is within two standard deviations, it is correct.

### Judger
- Due to the stochastic nature of evolutionary computation, TAs will modifies line 6 in `judger.py` to set `test_num = 30` in grading process. This means each program will be executed 30 times, and the average fitness is checked against the expected range.  
  - If you notice that with `test_num = 1`, your results sometimes pass and sometimes fail, try increasing `test_num` to examine the averaged performance.

To run judger, place your executable and `judger.py` in the same directory, then run:
```bash
python judger.py
```

Each testcase will run several trials as `test_num` set, so the process may take a while.

---

## Implementation Hints

Below are some common issues students may face when working on this assignment. We also encourage you to ask further questions on the eeclass discussion forum for timely responses from the TAs.

### Representation
In the assignment specification, the variable $c_i$ is equivalent to $x_i$.

### Parameters
- $p_c, p_m$: The default settings in the spec are $p_c = 0.9$ and $p_m = 1/l$. However, since the judger validates results under different parameter settings, please follow the parameter values given in each testcase. For your report, you are encouraged to try different settings.
- $\mu, \lambda$: In survivor selection using $\mu+\lambda$, typically $\lambda = \mu$. Please use this setting in this assignment.
- **Tournament selection**: The $n$-tournament selection mentioned in the spec is equivalent to $k$-tournament selection. Please set $k = 2$.
- $\alpha$:
  - For whole arithmetic crossover, set $\alpha = 0.2$.
  - For uniform crossover, set the probability of swapping at each position to 0.5.

### Operators
For binary representation, treat the individual as a bit array of length $10 \times \text{dimension}$ (e.g., for 10 dimensions, 100 bits). Perform crossover at arbitrary bit positions (e.g., 43, 81) instead of using dimension-level units.


---

## Grading
### **Coding (30%)**
- Simple test: each failed testcase −1 point (10%)
- Parent Selection (3%)
- Crossover (8%)
  - 2% for each method 
- Mutation (6%)
  - 3% for each representation
- Survivor Selection (3%)
- Penalties:
  - Compilation failure with Makefile / Raise error when running judger / Incorrect input/output format: −5
  - Incorrect filename: −3  
    - Executable: `main` or `main.py`  
    - Report: `report.pdf`
  - Lack of code efficiency: -3
  
### **Report (70%)**
#### I. Introduction (12%)
- Clear explanation of the GA implementation process. **(3%)**
- Correct description of each operator and its function. **(3%)**
- Introduction of both Binary GA and Real-Valued GA representations. **(3%)**
- Parameter table: includes all standard parameters (population size, pc, pm, α, n, generations). **(3%)**
#### II. Experiments (37%)
1. **Crossover Method Comparison under the Same Parameter Settings (12%)**
   - Binary GA: Uniform Crossover **(3%)**, Two-point Crossover **(3%)**
   - Real-Valued GA: Uniform Crossover **(3%)**, Whole Arithmetic Crossover **(3%)**
2. **Representation Comparison (5%)**
   - Compare Binary vs. Real-Valued representation and provide analysis. **(5%)**
3. **Parameter Adjustment Experiments (20%)**
   - **Crossover rate (pc):** Test different pc values under both representations and crossover methods. **(8%, each 2%)**
   - **Mutation rate (pm):** Test different pm values under both representations. **(4%)**
   - **Tournament size (n):** Test the effect of different n values under both representations. **(4%)**
   - **Analysis:** Discuss how parameter adjustments affect different representations and crossover methods. **(4%)**
#### III. Extended Experiments (13%)
- **Large-Scale Problem Test (N = 100):** Repeat the crossover method comparison under the large-scale setting. **(8%, each 2%)**
  - **Notes:** Use the same crossover operators as in Section II.1. Compare how increasing problem size impacts convergence behavior.
- **Comparison with Baseline Experiment:** Compare and analyze differences between standard and large-scale results. **(5%)**
#### IV. Conclusion (8%)
- Provide an overall conclusion summarizing the impact of different methods and parameters. **(8%)**
- **Notes:** Must include at least: (1) effect of different representations, (2) effect of crossover/mutation, (3) effect of parameter adjustments, and (4) large-scale results.
#### V.  Requirement
- **Every experiment must be presented with figures:**
  - Convergence curves or comparison plots are required for all experiments.
- **Reports without figures will be considered incomplete.**
- **Notes:** Each figure must have a clear caption and be referenced in the analysis text.

---

## Submission Guidelines

Suppose your student ID is `114062500`.

If using **C/C++**, package your submission as `114062500_SCH.zip/rar`, containing:
```
Makefile          (ensure the executable is named "main")
report.pdf        (must be named exactly "report.pdf")
other source code (e.g., main.cpp, parser.h, parser.cpp, ...)
```

If using **Python**, package your submission as `114062500_SCH.zip/rar`, containing:
```
main.py           (must be named exactly "main.py")
report.pdf        (must be named exactly "report.pdf")
other source code (e.g., ga.py, parser.py, ...)
```

Please strictly follow these naming and structural requirements.
