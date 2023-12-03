# 定义

A **definition** actually implements (for functions or types) or instantiates (for variables) the identifier.

----

A definition is needed to satisfy the *linker*. If you use an identifier without providing a definition, the linker will error.



## ODR

The **one definition rule** (or ODR for short) is a well-known rule in C++. The ODR has three parts:

1. Within a given *file*, a function, variable, type, or template can only have one definition.
2. Within a given *program*, a variable or normal function can only have one definition. This distinction is made because programs can have more than one file (we’ll cover this in the next lesson).
3. Types, templates, inline functions, and inline variables are allowed to have identical definitions in different files. 

> Violating part 1 of the ODR will cause the compiler to issue a redefinition error. 		Violating ODR part 2 will likely cause the linker to issue a redefinition error. 		Violating ODR part 3 will cause undefined behavior