static PyObject*
_IterationGuard_new(PyObject *arg)
{
  _IterationGuard *self;
  self = PyObject_New(_IterationGuard, &_IterationGuard_Type);
  if (self == NULL)
      return NULL;
  self->weakcontainer = PyWeakref_NewRef(arg);
  return self;
}

static PyObject *
_IterationGuard_enter(PyObject *self)
{
  PyObject *w = *(self->weakcontainer);
  if (w)
  {
    PySet_Add(w->_iterating, self);
  }
  return self;
}

static PyObject *                    /*Doubtful of parameters Type,Value,Trace*/
_IterationGuard_exit(PyObject *self)
{
  PyObject *w = *(self->weakcontainer);
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
  Py_DECREF(s);
  Py_DECREF(w);
}
