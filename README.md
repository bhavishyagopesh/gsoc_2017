# Google Summer Of Code '17 [PSF](https://www.python.org/psf/)

## Goals
- To identify set of regressed benchmarks in the [performance suite](https://github.com/python/performance).
- To find reasons for this regression
- To fix the benchmark suite itself whereever possible
- To come-up with new benchmarks for modules that do not have benchmarks yet.


### **NOTE: For parallel reports on advances see [here](bhavishyagopesh.github.io)**
### [This](https://github.com/bhavishyagopesh/gsoc_2017) repository contains source files of the project but it is suggested to use the blog for navigation.

## Tools Used
- [cpython](https://github.com/python/cpython/)
- [performance](https://github.com/python/performance)
- [perf](https://perf.readthedocs.io/en/latest/)
- [Kcachegrind](http://kcachegrind.sourceforge.net/html/Home.html)

## Results

- NOTE: For system-specs see [here](https://bhavishyagopesh.github.io/Second-Post/)

### Regressed benchmarks:

```text
Regressed benchmarks:
Benchmark 	            py2 	    py3 	       Times-slow
python_startup_no_site 	9.42 ms 	26.0 ms 	2.76x slower (+176%)
python_startup 	        19.2 ms 	42.3 ms 	2.21x slower (+121%)
spectral_norm 	        194 ms 	       259 ms 	    2.20x slower (+120%)
sqlite_synth 	        6.70 us 	8.49 us 	1.27x slower (+27%)
crypto_pyaes 	        158 ms 	       199 ms 	    1.26x slower (+26%)
xml_etree_parse 	193 ms 	  242 ms 	    1.25x slower (+25%)
xml_etree_iterparse 	154 ms 	       179 ms 	    1.16x slower (+16%)
go 	                439 ms 	  493 ms 	    1.12x slower (+12%)
```

1. **Startup-time**
**Startup-time** is the most regressed benchmark.

```bash
+-------------------------+----------+-------------------------------+
| python_startup          | 19.2 ms  | 42.3 ms: 2.21x slower (+121%) |
+-------------------------+----------+-------------------------------+
| python_startup_no_site  | 9.42 ms  | 26.0 ms: 2.76x slower (+176%) |
+-------------------------+----------+-------------------------------+
```

- py2 uses **"read method of file object"** which is done away with in py3. It imports `io` module(the prime reason for it being slow) then `TextIOWrapper` uses encoding passed by constructor. So following were the suggested  solutions:

i)improving time for **'abc module'** areas like [these](https://github.com/python/cpython/blob/5ff7132313eb651107b179d20218dfe5d4e47f13/Lib/abc.py#L134-L143)) because `getattr(value, "__isabstractmethod__", False)` is called for *all*
class attributes of ABCs (including subclass of ABCs).
It's slow because:

* When the value is not abstractmethod, AttributeError is raised and
cleared internally.

* getattr uses method cache (via PyType_Lookup), but
`__isabstractmethod__` is mostly in
  instance __dict__.  So checking method cache is mostly useless efforts.


ii) avoiding import of whole **sysconfig** and only of the variables required.This is fixed in [this](http://bugs.python.org/issue29585) "bpo" thread.

iii) avoiding the import of uncommon modules.

iv) But if the module excluded from startup is very common ,The module will be imported while Python "application" startup anyways,So faster import time is better than avoiding importing for such common modules,Like **"functools, pathlib, os, enum, collections, re."**

- Idea of parallelizing marshalling :
If we could somehow paralleize `marshalling` and thus **"loading"(not "executing")** than it could speedup import and henceforth **"startup-time"**.But it **won't** improve things drastically as **loading is a small fraction of execution time**
Eg:-for complex module like "typings" -> "29x" greater but for smaller ones like "ABC"-> "4x" greater.

- NOTE: For comparison relating to `C` portion of code see [here](https://bhavishyagopesh.github.io/Fourteenth-Post/)
- Import time of specific modules:
```Text
* encodings + encodings.utf_8 + encodings.latin_1 took 2.5ms
* io + abc + _wakrefset took 1.2ms
* _collections_abc took 2.1ms
* sysconfig + _sysconfigdata took 0.9ms

```

- Next I tried writing a `C version` of `WeakSet` ([here](https://github.com/bhavishyagopesh/gsoc_2017/blob/master/python_startup_time/weakrefsetmodule.c)) but it wasn't approved by Raymond Hettinger as it would have been difficult to maintain.

- On Nick Coughlan's Suggestion I tried if we could:

 - Push "commonly-imported" modules to a separate zip archive
 - Seed `sys.modules` with contents of that archive
 - freeze the import of those modules

I wrote a `python-script` to create a `zip-archive` from common modules and ran the different versions of python inside `docker` containers.See [this](https://bhavishyagopesh.github.io/Seventeenth-Post/) blog entry for more details. But it was realised that this might **not** reap huge benefits **because in writing a custom-importer we are already using import of some common modules and also python by itself adds a .zip of library in sys.path.**

- **Lazy- Loading**
I used a custom lazy-loader/importer for import of modules during “startup”, so as to prevent import of module which are not necessary and explore the possibility of possible decrease in startup time.[Here's](https://bhavishyagopesh.github.io/Seventh-Post/) blog entry explaining the implementation and the code for `custom lazy-loader/importer.`But lazy-loading didn't decreased the startup-time and rather increased it slightly.

- Also there was a suggestion to write `cython modules` for common modules.

2. Optimizing "logging" benchmark
py3 showed *regression* on logging benchmark,

```text
|  logging_format          | 57.7 us  | 75.1 us: 1.30x slower (+30%)  |
+-------------------------+----------+-------------------------------+
| logging_silent           | 818 ns   | 1.00 us: 1.22x slower (+22%)  |
+-------------------------+----------+-------------------------------+
| logging_simple           | 46.2 us  | 70.0 us: 1.51x slower (+51%)  |
```

This was fixed by this [PR](https://github.com/python/performance/pull/27). See [this](https://bhavishyagopesh.github.io/Tenth-Post/) blog entry for details.


3. Other benchmarks `pickle`, `sqlite_synth`, `crypto_pyaes` :

- `Pickle/Unpickle` was realised of lower practical importance.[here](https://bhavishyagopesh.github.io/Eleventh-Post/)
- For `sqlite_synth` see [here](https://bhavishyagopesh.github.io/Twelfth-Post/)
- For `crypto_pyaes` see [here](https://bhavishyagopesh.github.io/Thirteenth-Post/)


### New benchmarks

The present [performance-suite](https://github.com/python/performance) lacks benchmark for many of the common library modules.

1. zlib

This benchmark tries to measure basic `Compression` and `Decompression` using `zlib`.**This showed significant regression as length of binary string increased.** The code and the details to reproduce are in [this](https://bhavishyagopesh.github.io/Eighteenth-Post/) blog entry.

2. math

This benchmark measures basic *math operations*. And here here `py2` comes out **faster**. [Here's](https://bhavishyagopesh.github.io/Nineteenth-Post/) blog entry for code and statistics.

3. smtplib
The smtplib module defines an SMTP client session object that can be used to send mail to any Internet machine with an SMTP or ESMTP listener daemon.This benchmark measure that performance. [Here's](https://bhavishyagopesh.github.io/Nineteenth-Post/) the entry for code.Again **py3 regresses.**

4. Concurrency benchmark and concurency primitives
There are primarily two concurency primitives offered by `python` that are `threading` and `multiprocessing`.This benchmark tries to measure a same `number-crunching` task when done concurrently by "threading" and "multiprocessing" separately. [Here's](https://bhavishyagopesh.github.io/Twentieth-Post/) the  benchmark that measures concurency and also the cost of creating `threading-objects`(*It is not of much use as-such*).

## Acknowledgements:
1. @Botanic
2. Victor Stinner
3. Inada Naoki
4. James Lopeman
5. And everyone in python community.
