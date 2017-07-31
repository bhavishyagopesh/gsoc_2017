## This applies the idea of inmemory import of a zip-archive for python2.

I tried inserting a custom importer in `sys.meta_path` that loads zipfile in memory(uses python2 and inspired from a stackoverflow answer.) And than added a startup script.It is coming out faster when compared with usual py2 (but with a small margin mainly because a lot of import is already done in writing the ZipImporter).

- To reproduce do this:

```bash
python common_module_archive.py
export PYTHONSTARTUP=startup.py
python bm_python_startup_time.py

```

```bash
python bm_python_startup_time.py

```

- NOTE: This might **not** be the perfect thing to do because in writing a custom-importer we are already using import of **some common modules** and also python by itself adds a .zip of library in `sys.path`.
