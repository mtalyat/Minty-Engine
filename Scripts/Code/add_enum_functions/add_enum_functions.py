import os
import re

FILE_INPUT = 'add_enum_functions/input.txt'
FILE_OUTPUT = 'add_enum_functions/output.txt'

PATTERN_EMPTY = re.compile(r'^\s*$')
PATTERN_ENUM_NAME = re.compile(r'^\s*enum (class )?([a-zA-Z]\w*)\s*{?\s*$')
PATTERN_VALUE_NAME = re.compile(r'^\s*(\w+)\s*(=\s*[\dbBxX]+\s*)?,?\s*$')
PATTERN_PASCAL_TO_SNAKE = re.compile(r'(?<!^)(?=[A-Z])')

def add_enum(lines):
    # parse out name
    nameText = lines[0]
    nameMatch = re.search(PATTERN_ENUM_NAME, nameText)
    if nameMatch is None: 
        print(f'Name not found in line "{nameText}".')
        return
    name = nameMatch.group(2)
    name2 = re.sub(PATTERN_PASCAL_TO_SNAKE, '_', name).lower()

    # parse out the values
    values = []
    for line in lines[1:]:
        valueMatch = re.search(PATTERN_VALUE_NAME, line)
        if valueMatch:
            values.append(valueMatch.group(1))

    # create output
    output = ''

    # helpers
    def loop_with_values(formattedString):
        result = ''
        for value in values:
            result += formattedString.format(value)
        return result
    
    def header(name):
        return f'====================\n\t{name}\n====================\n\n'
    
    #   ToString
    output += header('ToString.h:')
    output += f'''String to_string({name} const obj);

template<>
inline String to_string({name} const& obj)
{{
    return to_string(obj);
}}'''
    
    output += header('ToString.cpp')
    temp = loop_with_values(f'\t\tcase {name}::{{0}}: return "{{0}}";\n')
    output += f'''String Minty::to_string({name} const obj)
{{
    switch (obj)
	{{
{temp}
		default: return "";
	}}
}}'''
    
    #   Parse
    output += header('Parse.h')
    output += f'''{name} to_{name2}(String const& string);
Bool try_{name2}(String const& string, {name}& value);

template<>
inline void parse_to(String const& string, {name}& value)
{{
    value = to_{name2}(string);
}}

template<>
inline Bool parse_try(String const& string, {name}& value)
{{
    return try_{name2}(string, value);
}}

'''
    
    output += header('Parse.cpp')
    temp = loop_with_values(f'\tif (string == "{{0}}") return {name}::{{0}};\n')
    output += f'''{name} Minty::Parse::to_{name2}(String const& string)
{{
{temp}
	return {name}();
}}

Bool Minty::Parse::try_{name2}(String const& string, {name}& value)
{{
	value = to_{name2}(string);
	return value != {name}();
}}

'''
    
    # output
    with open(FILE_OUTPUT, 'w') as file:
        file.write(output)

def main():
    # load text
    with open(FILE_INPUT, 'r') as file:
        lines = file.readlines()

    # split up by empty lines
    groups = []
    current = []
    for line in lines:
        if re.search(PATTERN_EMPTY, line):
            if len(current) > 0:
                groups.append(current)
                current = []
        else:
            current.append(line)
    if len(current) > 0:
        groups.append(current)

    # do each group separately
    for group in groups:
        add_enum(group)


if __name__ == '__main__':
    main()