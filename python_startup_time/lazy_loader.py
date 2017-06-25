import importlib.util
from importlib.util import LazyLoader
import sys

def lazy_import_module(name, package=None):
    """An implementation of lazy-import."""
    absolute_name = importlib.util.resolve_name(name, package)
    try:
        return sys.modules[absolute_name]
    except KeyError:
        pass

    path = None
    if '.' in absolute_name:
        parent_name, _, child_name = absolute_name.rpartition('.')
        parent_module = lazy_import_module(parent_name)
        path = parent_module.spec.submodule_search_locations
    for finder in sys.meta_path:
        spec = finder.find_spec(absolute_name, path)
        if spec is not None:
            break
    else:
        raise ImportError(f'No module named {absolute_name!r}')
    module = importlib.util.module_from_spec(spec)
    lazy = LazyLoader(spec.loader)
    lazy.exec_module(module)
    sys.modules[absolute_name] = module
    if path is not None:
        setattr(parent_module, child_name, module)
    return module

#For making a comparision 
for _ in range(1000):
    lazy_import_module('pprint','pprint')
    del sys.modules['pprint']
