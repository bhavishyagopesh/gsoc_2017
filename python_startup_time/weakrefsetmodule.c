#include "Python.h"
static PyObject *WeakSet_error = NULL; //global error

typedef struct {
  PyObject_HEAD
  PyObject              *weakcontainer  //Weak-reference
}_IterationGuard;

static PyTypeObject _IterationGuard;

#define _IterationGuard_Check(v)      (Py_TYPE(v) == &_IterationGuard_Type)

static PyObject*
_IterationGuard_new(PyObject *arg)
{
  _IterationGuard *self;
  self = PyObject_New(_IterationGuard,&_IterationGuard_Type);
  if (self == NULL)
      return NULL;
  self->weakcontainer = PyWeakref_NewRef(arg);
  return self;
}

/* This is not be required now
static int
_IterationGuard_setattr(_IterationGuard *self,PyObject *weakcontainer)
{
  self->weakcontainer = PyWeakref_NewRef(weakcontainer);
}
*/

static PyObject *
_IterationGuard_enter(PyObject *self)
{
  PyObject *w = *(self->weakcontainer);
  if (w)
  {
    PySet_Add(w->_iterating,self);
  }
  return self;
}

static PyObject *                    //Doubtful of parameters e,t,b
_IterationGuard_exit(PyObject *self)
{
  w = *(self->weakcontainer);
  PyObject *s = PySet_New();
  if(w)
  {
    s = w->_iterating ;
    PySet_Discard(s, self);

    if(!s)
    {
      *(w->_commit_removals);
    }
  }
}


typedef struct {
    PyObject_HEAD
    PyObject            *data;      //set  /* Attributes dictionary */
    PyObject            *_pending_removals //list
    PyObject            *_iterating  //set
    PyObject            *_remove     //function
} WeakSet;

static PyTypeObject WeakSet;

#define WeakSet_Check(v)      (Py_TYPE(v) == &WeakSet_Type)

static PyObject *
WeakSet_new(PyObject *arg)
{
    WeakSet *self
    self = PyObject_New(WeakSet, &WeakSet_Type);
    if (self == NULL)
        return NULL;
    self->data = PySet_New();
    if(arg)
    self->data = arg;
    self->_pending_removals = PyList_New();
    self->_iterating = PySet_New();
    self->_remove = _remove;
    return self;
}


static void (*_remove)(PyObject *item,WeakSet *self)
{
  self = *(PyWeakref_NewRef(self));

  if(self!=NULL)
    {
      if (self->_iterating != NULL)
        PyList_Append(self->_pending_removals, item);
      else
        PySet_Discard(self->data, item);
    }
}

static void
WeakSet_dealloc(WeakSet *self)
{
    Py_XDECREF(self->data);
    PyObject_Del(self);
}

static PyObject *
WeakSet_commit_removals(WeakSet *self)
{
  PyListObject *l;
  l = self->_pending_removals;

  while (l)
  {
    PySet_Discard(self->data, PyList_GetItem(l, PyList_Size(l)-1);
  }
}

static PyObject *
WeakSet_iter(WeakSet *self)
{
  _IterationGuard *w,*l;
  w = _IterationGuard_new(self);
  l = _IterationGuard_enter(w);

  PyObject *itemref = PyObject_GetIter(l->data);
  PyObject *item ;

if (itemref == NULL) {
    return NULL;
}

while (item = PyIter_Next(itemref)) {

    if(item)
    {
      PyObject *it1 = PyObject_GetIter(*item);
      PyObject *it2;
      while(it2 = PyIter_Next(it1))
      {
        return *(it2);
        Py_DECREF(it2);
      }
      Py_DECREF(it1);
    }

    Py_DECREF(item);
}

Py_DECREF(itemref);

if (PyErr_Occurred()) {
  return NULL;
}

_IterationGuard_exit(l);
}

static PyObject *
WeakSet_len(WeakSet *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ":len"))
        return NULL;
    return PySet_Size(self->data) - PyList_Size(self->_pending_removals);
}

static PyObject *
WeakSet_contains(WeakSet *self, PyObject *args)
{
  int wrefcount;
  PyObject *wr,*item;
  if (!PyArg_ParseTuple(args, ":contains",&item))
      return NULL;
  if(!(wr = PyWeakref_NewRef(item)))
  {
    return Py_False;
  }

  else
  return PySet_Contains(self->data,wr);
}

static PyObject*
WeakSet_reduce(WeakSet *self)
{
  return PyTuple_Pack(3,Py_Type(self),PyTuple_Pack(1,self),PyModule_GetDict(self)) //Doubtful
}

static PyObject *
WeakSet_add(WeakSet *self, PyObject *args)
{
  PyObject *item;
  if (!PyArg_ParseTuple(args, ":add",&item))
      return NULL;
  if(_pending_removals)
    WeakSet_commit_removals(self);
  PySet_Add(self->data,PyWeakref_NewRef(item,self->_remove));
}

void
WeakSet_clear(WeakSet *self, PyObject *args)
{
  if(_pending_removals)
    WeakSet_commit_removals(self);
  PySet_Clear(self->data);
}

/*Non-standard and probably incorrect implementation*/
static PyObject *
WeakSet_copy(WeakSet *self)
{
  WeakSet *l;
  *(l->data) = *(self->data);
  *(l->_pending_removals) = *(self->_pending_removals);
  *(l->_commit_removals) = *(self->_commit_removals);
  *(l->_return) = *(self->_return);
  return l
}

static PyObject *
WeakSet_pop(WeakSet *self)
{
  if(_pending_removals!=NULL)
    WeakSet_commit_removals(self);
  while(1)
  {
    PySetObject *itemref;
    PyObject *item;
    itemref = PySet_Pop(self->data);
    if(itemref!=NULL)
      PyErr_SetString(PyExc_KeyError,"pop from empty WeakSet");

      item = *itemref
      if(item)
      return item;
  }
}

void
WeakSet_remove(WeakSet *self, PyObject *args)
{
  PySetObject *item;
  if (!PyArg_ParseTuple(args, ":remove",&item))
      return NULL;
  if(_pending_removals!=NULL)
    WeakSet_commit_removals(self);
  if(PySet_Contains(self->data,PyWeakref_NewRef(item) ))
  PySet_Discard(self->data, PyWeakref_NewRef(item));
}

void
WeakSet_discard(WeakSet *self, PyObject *args)
{
  PySetObject *item;
  if (!PyArg_ParseTuple(args, ":discard",&item))
      return NULL;
  if(_pending_removals!=NULL)
    WeakSet_commit_removals(self);
  PySet_Discard(self->data, PyWeakref_NewRef(item));
}


static PyObject *
WeakSet_update(WeakSet *self, PyObject *args)
{
  WeakSet *other;
  if (!PyArg_ParseTuple(args, ":update",&other))
      return NULL;
  if(_pending_removals!=NULL)
      WeakSet_commit_removals(self);

    PyObject *iterator = PyObject_GetIter(other);
    PyObject *element;

    if (iterator == NULL) {
        return NULL;
    }

    while (element= PyIter_Next(iterator)) {
        WeakSet_add(self,element)
        Py_DECREF(item);
    }

    Py_DECREF(iterator);

    if (PyErr_Occurred()) {
        return NULL;
    }
}

static PyObject *
WeakSet_ior(WeakSet *self, PyObject *args)
{
  WeakSet *other;
  if (!PyArg_ParseTuple(args, ":__ior__",&other))
    return NULL;
  WeakSet_update(self,other);
  return self;
}

static PyObject *
WeakSet_difference(WeakSet *self, PyObject *args)
{
  WeakSet *newset,*other;
  if (!PyArg_ParseTuple(args, ":difference",&other))
    return NULL;
  newest = WeakSet_copy(self);
  WeakSet_difference_update(newest,other);
  return newest;
}

static PyObject *
WeakSet_difference_update(WeakSet *self, PyObject *args)
{
  WeakSet *other;
  if (!PyArg_ParseTuple(args, ":difference_update",&other))
    return NULL;
  WeakSet_isub(self,other);
}

static PyObject *
WeakSet_isub(WeakSet *self, PyObject *args)
{
  WeakSet *other;
  if (!PyArg_ParseTuple(args, ":__isub__",&other))
    return NULL;
  if(_pending_removals!=NULL)
    WeakSet_commit_removals(self);
  if(self==other)
    PySet_Clear(self->data);
  else
    {
          PyObject *iterator = PyObject_GetIter(other);
          PyObject *item;

          if (iterator == NULL) {
          return NULL;
        }

        while (item = PyIter_Next(iterator)) {
          PyNumber_Subtract(self->data,  PyWeakref_NewRef(item))
          Py_DECREF(item);
        }

        Py_DECREF(iterator);

        if (PyErr_Occurred()) {
        return NULL;
        }

    }
  return self;
}

static PyObject *
WeakSet_intersection(PyObject *self,PyObject *other)
{
  WeakSet *l;
  if (!PyArg_ParseTuple(args, ":intersection",&l))
    return NULL;
    self->data = PyNumber_And(self->data, l->data);
    return self;
}

static PyObject *
WeakSet_intersection_update(PyObject *self,PyObject *other)
{
  WeakSet *l;
  if (!PyArg_ParseTuple(args, ":intersection_update",&l))
    return NULL;
  WeakSet_iand(self,l);
}

static PyObject *
WeakSet_iand(PyObject *self,PyObject *other)
{
  if(self->_pending_removals)
  {
    *(self->_commit_removals);
  }

  PyObject *itemref = PyObject_GetIter(other);
  PyObject *item ;

if (itemref == NULL) {
    return NULL;
}

while (item = PyIter_Next(itemref)) {

    if(item)
    {
      PyObject *it1 = PyObject_GetIter(PyWeakref_NewRef(item));
      while(it2 = PyIter_Next(it1))
      {
        PyNumber_And(self->data, it2);
        Py_DECREF(it2);
      }
      Py_DECREF(it1);
    }

    Py_DECREF(item);
}

Py_DECREF(itemref);

if (PyErr_Occurred()) {
  return NULL;
}

else
{
  return self;
}

}

static PyObject *
WeakSet_issubset(PyObject *self,PyObject *other)
{
  PySet_Type *l;
  PyObject *itemref = PyObject_GetIter(other);
  PyObject *item ;

if (itemref == NULL) {
    return NULL;
}

while (item = PyIter_Next(itemref)) {

    if(item)
    {
      PySet_Add(l,PyWeakref_NewRef(item));
    }

    Py_DECREF(item);
}

Py_DECREF(itemref);

if (PyErr_Occurred()) {
  return NULL;
}
else
return (PyNumber_And(self->data, l)==*l);

}

static PyObject *
WeakSet_lt(PyObject *self,PyObject *other)
{
  PyObject *itemref = PyObject_GetIter(other);
  PyObject *item ;
  PySet_Type *l;

  if (itemref == NULL) {
      return NULL;
  }

  while (item = PyIter_Next(itemref)) {

    PySet_Add(l,item);
    Py_DECREF(item);
  }

  Py_DECREF(itemref);

  if (PyErr_Occurred()) {
    return NULL;
  }

  else
  {
    return (PySet_Size(self->data)<PySet_Size(l));
  }
}

static PyObject *
WeakSet_issuperset(PyObject *self,PyObject *other)
{
  PySet_Type *l;
  PyObject *itemref = PyObject_GetIter(other);
  PyObject *item ;

if (itemref == NULL) {
    return NULL;
}

while (item = PyIter_Next(itemref)) {

    if(item)
    {
      PySet_Add(l,PyWeakref_NewRef(item));
    }

    Py_DECREF(item);
}

Py_DECREF(itemref);

if (PyErr_Occurred()) {
  return NULL;
}
else
return (PyNumber_Add(self->data, l)==*self->data);

}

static PyObject *
WeakSet_gt(PyObject *self,PyObject *other)
{
  PyObject *itemref = PyObject_GetIter(other);
  PyObject *item ;
  PySet_Type *l;

  if (itemref == NULL) {
      return NULL;
  }

  while (item = PyIter_Next(itemref)) {

    PySet_Add(l,item);
    Py_DECREF(item);
  }

  Py_DECREF(itemref);

  if (PyErr_Occurred()) {
    return NULL;
  }

  else
  {
    return (PySet_Size(self->data)>PySet_Size(l));
  }
}

static PyObject *
WeakSet_eq(PyObject *self,PyObject *other)
{

  if(!WeakSet_Check(other))
  {
    Py_RETURN_NOTIMPLEMENTED;
  }


  PyObject *itemref = PyObject_GetIter(other);
  PyObject *item ;
  PySet_Type *l;

  if (itemref == NULL) {
      return NULL;
  }

  while (item = PyIter_Next(itemref)) {

    PySet_Add(l,item);
    Py_DECREF(item);
  }

  Py_DECREF(itemref);

  if (PyErr_Occurred()) {
    return NULL;
  }

  else
  {
    return (PySet_Size(self->data)==PySet_Size(l));
  }
}

static PyObject *
WeakSet_symmetric_difference(PyObject *self,PyObject *other)
{
  WeakSet *newset;
  newset = WeakSet_copy(self);
  newset = WeakSet_symmetric_difference_update(newset,other);
  return newset;
}

static PyObject *
WeakSet_symmetric_difference_update(PyObject *self,PyObject *other)
{
  WeakSet_ixor(self,other);
}

staic PyObject *
WeakSet_ixor(PyObject *self,PyObject *other)
{
  if(self->_pending_removals)
    *(self->_commit_removals);
  if(self==other)
  {
    PySet_Clear(self->data);
  }
  else
  {
    PyObject *itemref = PyObject_GetIter(other);
    PyObject *item ;

    if (itemref == NULL) {
        return NULL;
    }

    while (item = PyIter_Next(itemref)) {

      PyNumber_Subtract(self->data,PyWeakref_NewRef(item,self->_remove));
      Py_DECREF(item);
    }

    Py_DECREF(itemref);

    if (PyErr_Occurred()) {
      return NULL;
    }

    else
    return self;
  }
}

static PyObject *
WeakSet_union(PyObject *self,PyObject *other)
{
  WeakSet *l;
  if (!PyArg_ParseTuple(args, ":union",&l))
    return NULL;
  self->data = PyNumber_Or(self->data, l->data);
  return self;
}

static PyObject *
WeakSet_isdisjoint(PyObject *self,PyObject *other)
{
  return(WeakSet_len(WeakSet_intersection(self,other))==0);
}

static PyTypeObject WeakSet_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "weakrefset",             /*tp_name*/
    sizeof(WeakSet),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)WeakSet_dealloc,    /*tp_dealloc*/
    0,                          /*tp_print*/
    (getattrfunc)0,             /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_reserved*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash*/
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0, /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,         /*tp_flags*/
    0,                          /*tp_doc*/
    0,                          /*tp_traverse*/
    0,                          /*tp_clear*/
    0,                          /*tp_richcompare*/
    0,                          /*tp_weaklistoffset*/
    WeakSet_iter,                          /*tp_iter*/
    0,                          /*tp_iternext*/
    WeakSet_methods,                /*tp_methods*/
    0,                          /*tp_members*/
    0,                          /*tp_getset*/
    0,                          /*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    0,                          /*tp_init*/
    0,                          /*tp_alloc*/
    WeakSet_new,                /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};


/* List of functions defined in the module */

static PyMethodDef WeakSet_methods[] = {
    {"_commit_removals",    WeakSet_commit_removals,         METH_VARARGS,
        PyDoc_STR("_commit_removals(a,b) -> None")},
    {"__iter__",          WeakSet_iter,         METH_VARARGS,
        PyDoc_STR("__iter__(a,b) -> iterator")},
    {"__len__",            WeakSet_new,         METH_VARARGS,
        PyDoc_STR("__len__ -> length")},
    {"__contains__",           WeakSet_contains,         METH_VARARGS,
        PyDoc_STR("__contains__ -> boolean")},
    {"__reduce__",           WeakSet_reduce,         METH_VARARGS,
        PyDoc_STR("__reduce__ -> None")},
    {"add",           WeakSet_add,         METH_VARARGS,
        PyDoc_STR("add(a) -> None")},
    {"clear",           WeakSet_clear,         METH_VARARGS,
        PyDoc_STR("clear(a) -> None")},
    {"copy",           WeakSet_copy,         METH_VARARGS,
        PyDoc_STR("copy(a) -> copy of WeakSet Object")},
    {"pop",           WeakSet_pop,         METH_VARARGS,
        PyDoc_STR("pop(a) -> Weakreference of set element")},
    {"remove",           WeakSet_remove,         METH_VARARGS,
        PyDoc_STR("remove(a) -> remove Weakreference of set element")},
    {"discard",           WeakSet_discard,         METH_VARARGS,
        PyDoc_STR("discard(a,b) -> discard Weakreference of set element")},
    {"update",           WeakSet_update,         METH_VARARGS,
        PyDoc_STR("update(a,b) -> updates Weakreference of set element")},
    {"__ior__",           WeakSet_ior,         METH_VARARGS,
        PyDoc_STR("ior(a,b) -> Weakset")},
    {"__sub__",           WeakSet_difference,         METH_VARARGS,
        PyDoc_STR("sub(a,b) -> Weakset after differnce update")},
    {"difference_update",           WeakSet_difference_up,         METH_VARARGS,
        PyDoc_STR("differnce_update(a,b) -> Weakset after differnce update")},
    {"__and__",           WeakSet_intersection,         METH_VARARGS,
        PyDoc_STR("and(a,b) -> Weakset after intersection")},
    {"intersection_update",           WeakSet_intersection_update,         METH_VARARGS,
        PyDoc_STR("intersection_update(a,b) -> Weakset after differnce update")},
    {"__le__",           WeakSet_issubset,         METH_VARARGS,
        PyDoc_STR("le(a,b) -> boolean")},
    {"__lt__",           WeakSet_lt,         METH_VARARGS,
        PyDoc_STR("lt(a,b) -> boolean")},
    {"__ge__",           WeakSet_issuperset,         METH_VARARGS,
        PyDoc_STR("ge(a,b) -> boolean")},
    {"__gt__",           WeakSet_gt,         METH_VARARGS,
        PyDoc_STR("gt(a,b) -> boolean")},
    {"__eq__",           WeakSet_eq,         METH_VARARGS,
        PyDoc_STR("eq(a,b) -> boolean")},
    {"symmetric_difference",           WeakSet_symmetric_difference,         METH_VARARGS,
        PyDoc_STR("symmetric_difference(a,b) -> symmetric_difference of Weaksets")},
    {"__xor__",           WeakSet_xor,         METH_VARARGS,
        PyDoc_STR("xor(a,b) -> symmetric_difference of Weaksets")},
    {"symmetric_difference_update",           WeakSet_symmetric_difference_update,         METH_VARARGS,
        PyDoc_STR("symmetric_difference_update(a,b) -> symmetric_difference of Weaksets")},
    {"__ixor__",           WeakSet_ixor,         METH_VARARGS,
        PyDoc_STR("ixor(a,b) -> symmetric_difference of Weaksets")},
    {"symmetric_difference_update",           WeakSet_symmetric_difference_update,         METH_VARARGS,
        PyDoc_STR("symmetric_difference_update(a,b) -> symmetric_difference of Weaksets")},
    {"union|__or__",           WeakSet_union,         METH_VARARGS,
        PyDoc_STR("union(a,b) -> union of Weaksets")},
    {"isdisjoint",           WeakSet_isdisjoint,         METH_VARARGS,
        PyDoc_STR("isdisjoint(a,b) -> boolean")},
    {NULL,              NULL}           /* sentinel */
};


PyMODINIT_FUNC
PyInit_WeakSet(void)
{
    PyObject *module = PyModule_Create(&weakrefsetmodule);
    if(!module)
      return NULL;
    if(PyType_Ready(&PyWeakSet_Type)<0)
    return NULL;

    Py_INCREF((PyObject*)&PyWeakSet_Type);
    PyModule_AddObject(module,"weakrefset",(PyObject*)&PyWeakSet_Type);
    return module;
}
