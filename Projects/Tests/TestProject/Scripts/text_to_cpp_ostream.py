TEXT = """#include "init.h"

using namespace minty;

int init(Runtime &runtime)
{
    return 0;
}

int destroy(Runtime &runtime)
{
    return 0;
}"""

for line in TEXT.split('\n'):
    fixedLine = line.replace('"', '\\"')
    fixedLine = f'<< "{fixedLine}" << std::endl'
    print(fixedLine)