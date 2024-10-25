# Standards for the Minty Game Engine

Notes:
ThisIsPascalCase
thisIsCamelCase
this_is_snake_case
This Is Title Case

## C++ Coding Standards
Standards for writing C++ code. If a rule is unknown, refer to the C++ coding standards.

### Classes
1. Class names shall be pascal case.

### Comments
1. Comments shall be in sentence case.
2. Comments shall be written before any class or function, breifly explaining the functionality or purpose.
3. Comments shall be in sentence form if written above a class, member, function, etc. (used for documentation).
4. Comments shall refer to all classes, structs, etc. with their proper name (Ex. "ApplicationBuilder").
5. Comments shall refer the the class being commented in/on as "this Class" where applicable, where Class is the class being commented in/on.

### Filenames
1. All file names shall be in pascal case.
2. Minty header and source files shall be named a word representing the most significant thing within the file.
3. Platform header and source files shall start with the name of their platform, followed by the above rull (Ex. "LinuxWindow.cpp").

### Functions
1. Functions shall be snake case.

### Includes
1. Includes shall first be ordered by "" includes, followed by <> includes.
2. Includes shall second be ordered alphabetically.
3. Includes shall be their full path (ex. `"Minty/Core/Application.h"`), with a few exceptions:
    3.1. When in the corresponding source file. Ex. inside of `Application.cpp`, `#include "Application.h"` can be used.
    3.2. When in an "all" header file. Ex. inside of `AllComponents.h`, `#include "NameComponent.h` can be used.

### Macros
1. #ifdef statements must have a comment on the same line as their #endif that specifies the argument for the ifdef. Ex. `#ifdef MINTY_DEBUG ... #endif // MINTY_DEBUG`.

### Namespaces
1. Namespace names shall be pascal case.

### Pragma

#### Region
1. All `#pragma region`s shall use an appropriate name in title case.
1. All `#pragma region`s within a class shall start with an access specifier: public, protected, private, etc.

## C# Coding Standards
Standards for writing C# scripting code.