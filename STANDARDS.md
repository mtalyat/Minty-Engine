# Standards for the Minty Game Engine

Notes:
ThisIsPascalCase
thisIsCamelCase
this_is_snake_case
This Is Title Case

## C++ Coding Standards
Standards for writing C++ code. If a rule is unknown, refer to the C++ coding standards.

### Classes
- Class names shall be pascal case.

### Comments
- Comments shall be in sentence case.
- Comments shall be written before any class or function, breifly explaining the functionality or purpose.
- Comments shall be in sentence form if written above a class, member, function, etc. (used for documentation).
- Comments shall refer the the class being commented in/on as "this Class" where applicable, where Class is the class being commented in/on.

### Filenames
- All file names shall be in pascal case.
- Minty header and source files shall be named a word representing the most significant thing within the file.
- Platform header and source files shall start with the name of their platform, followed by the above rull (Ex. "LinuxWindow.cpp").

### Functions
- Functions shall be snake case.

### Includes
- Includes shall first be ordered by "" includes, followed by <> includes.
- Includes shall second be ordered alphabetically.
- Includes shall be their full path (ex. `"Minty/Core/Application.h"`), with a few exceptions:
    - When in the corresponding source file. Ex. inside of `Application.cpp`, `#include "Application.h"` can be used.
    - When in an "all" header file. Ex. inside of `AllComponents.h`, `#include "NameComponent.h` can be used.

### Macros
- #ifdef statements must have a comment on the same line as their #endif that specifies the argument for the ifdef. Ex. `#ifdef MINTY_DEBUG ... #endif // MINTY_DEBUG`.

### Namespaces
- Namespace names shall be pascal case.

### Pragma

#### Region
- All `#pragma region`s shall use an appropriate name in title case.
- All `#pragma region`s within a class shall start with an access specifier: public, protected, private, etc.

## C# Coding Standards
Standards for writing C# scripting code.