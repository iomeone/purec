# C backend for Purescript

## Status

### Things to come back to

* [ ] Bake `True` and `False` into the runtime to avoid mallocing them in guards
  over and over
* [ ] Do not use managed blocks (in `purs_any_t`) for lambdas that are generated
  and applied immediately with no arguments (ex. let bindings). We can avoid the
  malloc and GC registration for that block entirely.