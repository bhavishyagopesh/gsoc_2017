PyNumber_And
#include "Python.h"
#include "clinic/_weakref.c.h"
static PyObject *WeakSet_error = NULL; //global error

typedef struct {
  PyObject_HEAD
  PyObject              *weakcontainer  //Weak-reference

}

static PyTypeObject _IterationGuard;  //Lacks inheritance

#define _IterationGuard_Check(v)      (Py_TYPE(v) == &_IterationGuard_Type)

static PyObject*
_IterationGuard_new(PyTypeObject *type,PyObject *args)
{
  _IterationGuard *self,*weakcontainer;
  self = PyObject_New(_IterationGuard,&_IterationGuard_Type);
  if (self == NULL)
      return NULL;
  self->weakcontainer = NULL;
  return self;
}

static int
_IterationGuard_setattr(_IterationGuard *self,PyObject *weakcontainer)
{
  self->weakcontainer = PyWeakref_NewRef(weakcontainer);
}

static PyObject *
_IterationGuard_enter(_IterationGuard *self)
{
  PYObject *w = *(self->weakcontainer);
  if (w!=Py_None)
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
  if(w!=Py_None)
  {
    s = w->_iterating ;
    WeakSet_remove(self,s);

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
WeakSet_new(PyObject *args)
{
    WeakSet *self
    self = PyObject_New(WeakSet, &WeakSet_Type);
    if (self == NULL)
        return NULL;
    self->data = NULL;
    self->_pending_removals = NULL;
    self->_iterating = NULL;
    self->_remove = NULL;
    return self;
}

static int
WeakSet_setattr(WeakSet *self, PyObject *v)
{
    if (self->data == NULL) {
        self->data = PySet_New();
        if (self->data == NULL)
            return -1;
    }
    if (v == NULL) {
        int rv = PySet_Contains(self->data, v);
        if (rv < 0)
            PyErr_SetString(PyExc_AttributeError,
                "delete non-existing data attribute");
        return rv;
    }
    else
        return PySet_Add(self->data, v);

    if(self->_pending_removals == NULL){
        self->_pending_removals = PyList_New();
        if(self->_pending_removals == NULL)
            return -1;
    }

    if (self->_iterating == NULL) {
        self->_iterating = PySet_New();
        if (self->_iterating == NULL)
            return -1;
    }

    if (self->_remove == NULL){
      self->_remove = _remove;
      if (self->_remove == NULL)
        return -1;
    }

    if(self->data != NULL){
      WeakSet_update(self->data);
    }

}

static void (*_remove)(PyObject *item,WeakSet *self)
{
  self = *(self);

  if(self!=NULL)
    {
      if (self->_iterating != NULL)
        PyList_Append(self->_pending_removals, item);
      else
        PySet_Discard(self->data, item);
    }
}

static PyObject *
WeakSet_commit_removals(WeakSet *self)
{
  PyListObject *l;
  l = self->_pending_removals;
  PyObject *discard;
  discard = WeakSer_discard;

  while (l)
  {
    discard()//TO Do implement discard
  }
}

static PyObject *
WeakSet_iter(WeakSet *self)
{
  _IterationGuard *w,*l;
  w = _IterationGuard_new(self);
  l = _IterationGuard_enter(w);

  PyObject *itemref = PyObject_GetIter(self->data);
  PyObject *item = *(itemref);

if (itemref == NULL) {
    return NULL;
}

while (item = PyIter_Next(itemref)) {

    if(item)
    {
      PyObject *it1 = PyObject_GetIter(*item);
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
  wr = PyWeakref_NewRef(item);
  return PySet_Contains(self->data,wr);
}

static PyObject*
WeakSet_reduce(WeakSet *self)
{
  char *dict =
  return PyTuple_Pack(3,Py_Type(self),PyTuple_Pack(1,self),PyModule_GetDict(self)) //Doubtful
}

static PyObject *
WeakSet_add(WeakSet *self, PyObject *args)
{
  PyObject *item;
  if (!PyArg_ParseTuple(args, ":add",&item))
      return NULL;
  if(_pending_removals!=NULL)
    WeakSet_commit_removals(self); /*To------------------------Do*/
  PySet_Add(self->data,PyWeakref_NewRef(item));
}

void
WeakSet_clear(WeakSet *self, PyObject *args)
{
  if(_pending_removals!=NULL)
    WeakSet_commit_removals(self) /*To------------------------Do*/
  PySet_Clear(self->data);
}

static PyObject *
WeakSet_copy(WeakSet *self)
{
  return PyInstanceMethod_Function(self);
}

static PyObject *
WeakSet_pop(WeakSet *self)
{
  if(_pending_removals!=NULL)
    WeakSet_commit_removals(self);
  while(1)
  {
    PySetObject *itemref;
    itemref = PySet_Pop(self->data);
    if(itemref!=NULL)
      return itemref;
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
  int l = Py_Size(other);
  while(l)
  {
    WeakSet_add(self,*(other+l-1));  //very-very hackish
    l--;
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
  WeakSet___isub__(other);
}

static PyObject *
WeakSet_isub_(WeakSet *self, PyObject *args)
{
  WeakSet *other;
  if (!PyArg_ParseTuple(args, ":__isub__",&other))
    return NULL;
  if(_pending_removals!=NULL)
    WeakSet_commit_removals(self);
  if(self==other) //check how to Implement "is"
    PySet_Clear(self->data);
  else
    {
      int l = Py_Size(other);
      while(l)
      {
        WeakSet_difference_update(PyWeakref_NewRef(*(other+l-1))) //again hack
        l--;
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

static PyTypeObject Xxo_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "xxmodule.Xxo",             /*tp_name*/
    sizeof(XxoObject),          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    (destructor)Xxo_dealloc,    /*tp_dealloc*/
    0,                          /*tp_print*/
    (getattrfunc)0,             /*tp_getattr*/
    (setattrfunc)Xxo_setattr,   /*tp_setattr*/
    0,                          /*tp_reserved*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash*/
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    (getattrofunc)Xxo_getattro, /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,         /*tp_flags*/
    0,                          /*tp_doc*/
    0,                          /*tp_traverse*/
    0,                          /*tp_clear*/
    0,                          /*tp_richcompare*/
    0,                          /*tp_weaklistoffset*/
    0,                          /*tp_iter*/
    0,                          /*tp_iternext*/
    Xxo_methods,                /*tp_methods*/
    0,                          /*tp_members*/
    0,                          /*tp_getset*/
    0,                          /*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    0,                          /*tp_init*/
    0,                          /*tp_alloc*/
    0,                          /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};
/* --------------------------------------------------------------------- */

/* Function of two integers returning integer */

PyDoc_STRVAR(xx_foo_doc,
"foo(i,j)\n\
\n\
Return the sum of i and j.");

static PyObject *
xx_foo(PyObject *self, PyObject *args)
{
    long i, j;
    long res;
    if (!PyArg_ParseTuple(args, "ll:foo", &i, &j))
        return NULL;
    res = i+j; /* XXX Do something here */
    return PyLong_FromLong(res);
}


/* Function of no arguments returning new Xxo object */

static PyObject *
xx_new(PyObject *self, PyObject *args)
{
    XxoObject *rv;

    if (!PyArg_ParseTuple(args, ":new"))
        return NULL;
    rv = newXxoObject(args);
    if (rv == NULL)
        return NULL;
    return (PyObject *)rv;
}

/* Example with subtle bug from extensions manual ("Thin Ice"). */

static PyObject *
xx_bug(PyObject *self, PyObject *args)
{
    PyObject *list, *item;

    if (!PyArg_ParseTuple(args, "O:bug", &list))
        return NULL;

    item = PyList_GetItem(list, 0);
    /* Py_INCREF(item); */
    PyList_SetItem(list, 1, PyLong_FromLong(0L));
    PyObject_Print(item, stdout, 0);
    printf("\n");
    /* Py_DECREF(item); */

    Py_INCREF(Py_None);
    return Py_None;
}

/* Test bad format character */

static PyObject *
xx_roj(PyObject *self, PyObject *args)
{
    PyObject *a;
    long b;
    if (!PyArg_ParseTuple(args, "O#:roj", &a, &b))
        return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}


/* ---------- */

static PyTypeObject Str_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "xxmodule.Str",             /*tp_name*/
    0,                          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    0,                          /*tp_dealloc*/
    0,                          /*tp_print*/
    0,                          /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_reserved*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash*/
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    0,                          /*tp_doc*/
    0,                          /*tp_traverse*/
    0,                          /*tp_clear*/
    0,                          /*tp_richcompare*/
    0,                          /*tp_weaklistoffset*/
    0,                          /*tp_iter*/
    0,                          /*tp_iternext*/
    0,                          /*tp_methods*/
    0,                          /*tp_members*/
    0,                          /*tp_getset*/
    0, /* see PyInit_xx */      /*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    0,                          /*tp_init*/
    0,                          /*tp_alloc*/
    0,                          /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};

/* ---------- */

static PyObject *
null_richcompare(PyObject *self, PyObject *other, int op)
{
    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}

static PyTypeObject Null_Type = {
    /* The ob_type field must be initialized in the module init function
     * to be portable to Windows without using C++. */
    PyVarObject_HEAD_INIT(NULL, 0)
    "xxmodule.Null",            /*tp_name*/
    0,                          /*tp_basicsize*/
    0,                          /*tp_itemsize*/
    /* methods */
    0,                          /*tp_dealloc*/
    0,                          /*tp_print*/
    0,                          /*tp_getattr*/
    0,                          /*tp_setattr*/
    0,                          /*tp_reserved*/
    0,                          /*tp_repr*/
    0,                          /*tp_as_number*/
    0,                          /*tp_as_sequence*/
    0,                          /*tp_as_mapping*/
    0,                          /*tp_hash*/
    0,                          /*tp_call*/
    0,                          /*tp_str*/
    0,                          /*tp_getattro*/
    0,                          /*tp_setattro*/
    0,                          /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    0,                          /*tp_doc*/
    0,                          /*tp_traverse*/
    0,                          /*tp_clear*/
    null_richcompare,           /*tp_richcompare*/
    0,                          /*tp_weaklistoffset*/
    0,                          /*tp_iter*/
    0,                          /*tp_iternext*/
    0,                          /*tp_methods*/
    0,                          /*tp_members*/
    0,                          /*tp_getset*/
    0, /* see PyInit_xx */      /*tp_base*/
    0,                          /*tp_dict*/
    0,                          /*tp_descr_get*/
    0,                          /*tp_descr_set*/
    0,                          /*tp_dictoffset*/
    0,                          /*tp_init*/
    0,                          /*tp_alloc*/
    0, /* see PyInit_xx */      /*tp_new*/
    0,                          /*tp_free*/
    0,                          /*tp_is_gc*/
};


/* ---------- */


/* List of functions defined in the module */

static PyMethodDef xx_methods[] = {
    {"roj",             xx_roj,         METH_VARARGS,
        PyDoc_STR("roj(a,b) -> None")},
    {"foo",             xx_foo,         METH_VARARGS,
        xx_foo_doc},
    {"new",             xx_new,         METH_VARARGS,
        PyDoc_STR("new() -> new Xx object")},
    {"bug",             xx_bug,         METH_VARARGS,
        PyDoc_STR("bug(o) -> None")},
    {NULL,              NULL}           /* sentinel */
};

PyDoc_STRVAR(module_doc,
"This is a template module just for instruction.");


static int
xx_exec(PyObject *m)
{
    /* Due to cross platform compiler issues the slots must be filled
     * here. It's required for portability to Windows without requiring
     * C++. */
    Null_Type.tp_base = &PyBaseObject_Type;
    Null_Type.tp_new = PyType_GenericNew;
    Str_Type.tp_base = &PyUnicode_Type;

    /* Finalize the type object including setting type of the new type
     * object; doing it here is required for portability, too. */
    if (PyType_Ready(&Xxo_Type) < 0)
        goto fail;

    /* Add some symbolic constants to the module */
    if (ErrorObject == NULL) {
        ErrorObject = PyErr_NewException("xx.error", NULL, NULL);
        if (ErrorObject == NULL)
            goto fail;
    }
    Py_INCREF(ErrorObject);
    PyModule_AddObject(m, "error", ErrorObject);

    /* Add Str */
    if (PyType_Ready(&Str_Type) < 0)
        goto fail;
    PyModule_AddObject(m, "Str", (PyObject *)&Str_Type);

    /* Add Null */
    if (PyType_Ready(&Null_Type) < 0)
        goto fail;
    PyModule_AddObject(m, "Null", (PyObject *)&Null_Type);
    return 0;
 fail:
    Py_XDECREF(m);
    return -1;
}

static struct PyModuleDef_Slot xx_slots[] = {
    {Py_mod_exec, xx_exec},
    {0, NULL},
};

static struct PyModuleDef xxmodule = {
    PyModuleDef_HEAD_INIT,
    "xx",
    module_doc,
    0,
    xx_methods,
    xx_slots,
    NULL,
    NULL,
    NULL
};

/* Export function for the module (*must* be called PyInit_xx) */

PyMODINIT_FUNC
PyInit_xx(void)
{
    return PyModuleDef_Init(&xxmodule);
}
