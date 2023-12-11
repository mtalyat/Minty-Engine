import re

INPUT_PATH = "input.txt"
OUTPUT_PATH = "output.txt"
POSTFIX = " = "

# from chatgpt
def find_prefix(strings: list):
    if not strings:
        return ""
    
    strings = strings.copy()
    
    # Sort the list to easily compare the first and last strings
    strings.sort()
    
    # Compare the first and last strings character by character
    for i, char in enumerate(strings[0]):
        if char != strings[-1][i]:
            return strings[0][:i]
    
    # If all strings are identical, return any one of them
    return strings[0]

def main():
    # get file contents
    lines = []
    with open(INPUT_PATH, 'r') as file:
        lines = file.readlines()

    if len(lines) == 0:
        print("No text in input.txt.")

    # PREFIX = re.sub(r'(?<!^)(?=[A-Z])', '_', lines[0]).upper() + '_'
    # PREFIX = lines[0][:-1] # ignore the '\n' at the end of the line

    # get enum name
    pattern = r'enum (.*?) {'
    name = re.findall(pattern, lines[0], re.DOTALL)[0]
    name2 = re.sub(r'(?<!^)(?=[A-Z])', '_', name).lower()

    # get all enum values
    pattern = fr'(.*?){POSTFIX}'
    values = []
    for line in lines[1:-1]: # ignore first and last lines with the { and }
        result = re.findall(pattern, line.strip(), re.DOTALL)
        if len(result) > 0:
            values.append(result[0])
    
    PREFIX = find_prefix(values)
    pattern = fr'{PREFIX}(.*?)$'
    values = [re.findall(pattern, value, re.DOTALL)[0] for value in values]

    # print found values
    print('name:', name)
    print('name2:', name2)
    print('prefix:', PREFIX)
    print('values:')
    for value in values:
        print(value)
    

    # create C++ code
    with open(OUTPUT_PATH, 'w') as file:
        # .h file
        file.write(f'std::string to_string({name} const value);\n')
        file.write(f'{name} from_string_{name2}(std::string const& value);\n\n\n')

        # .cpp file
        # to_string
        # header
        file.write(f'std::string minty::vk::to_string({name} const value)\n{{\n\tswitch (value)\n\t{{\n')
        # body
        for value in values:
            file.write(f'\tcase {name}::{PREFIX}{value}: return "{value}";\n')

        # footer
        file.write(f'\tdefault: return "";\n\t}}\n}}\n')

        file.write('\n')

        # from_string
        # header
        file.write(f'{name} minty::vk::from_string_{name2}(std::string const& value)\n{{\n\tstd::string value2 = minty::string::to_upper(value);\n')

        # body
        i = 0
        for value in values:
            file.write(f'\tif (value2 == "{value}") return {name}::{PREFIX}{value};\n')

        # footer
        file.write(f'\n\treturn static_cast<{name}>(0);\n}}')

main()