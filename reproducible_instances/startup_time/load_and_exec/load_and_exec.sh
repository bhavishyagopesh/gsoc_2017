
printf "Firstly a larger module like typings:"

printf "a full reload cycle \n"
python3 -m perf timeit -s "import typing; from importlib import reload" "reload(typing)"

printf "cost of unmarshalling the code object \n"
python3 -m perf timeit -s "import typing; from marshal import loads; from importlib.util import cache_from_source; cache = cache_from_source(typing.__file__); data = open(cache, 'rb').read()[12:]" "loads(data)"

printf "cost of module exec \n"
python3 -m perf timeit -s "import typing; loader_exec = typing.__spec__.loader.exec_module" "loader_exec(typing)"

printf "cost of module_specs \n"
python3 -m perf timeit -s "from importlib.util import find_spec" "find_spec('typing')"

printf "A smaller module ABC"

printf "-----------------------------------Analysis of typings ends here-----------------------------------------"

printf "Similar analysis for smaller modules \n"

printf "Example1: ABC"

printf "a full reload cycle \n"
python3 -m perf timeit -s "import abc; from importlib import reload" "reload(abc)"

printf "cost of unmarshalling the code object \n"
python3 -m perf timeit -s "import abc; from marshal import loads; from importlib.util import cache_from_source" "cache = cache_from_source(abc.__spec__.origin); data = open(cache,'rb').read()[12:]; loads(data)"

printf "cost of module exec \n"
python3 -m perf timeit -s "import abc; loader_exec = abc.__spec__.loader.exec_module" "loader_exec(abc)"


printf "-----------------------------------Analysis of ABCs ends here-----------------------------------------"

printf "Example1: _weakrefset"
printf "a full reload cycle \n"
python3 -m perf timeit -s "import _weakrefset; from importlib import reload" "reload(_weakrefset)"

printf "cost of unmarshalling the code object \n"
python3 -m perf timeit -s "import _weakrefset; from marshal import loads; from importlib.util import cache_from_source" "cache =cache_from_source(_weakrefset.__spec__.origin); data = open(cache,'rb').read()[12:]; loads(data)"

printf "cost of module exec \n"
python3 -m perf timeit -s "import _weakrefset; loader_exec = _weakrefset.__spec__.loader.exec_module" "loader_exec(_weakrefset)"

