# Generate Predeclaration for all classes/structs

import os;
import re;
from typing import Dict, List, Tuple, Literal, Iterator

def analyzeFile(headerData: str):
    dictionary: Dict[str, str] = dict()
    stack: List[Tuple[str, str]] = list()

    def processBrackets(line: str):
        line = line.strip()

        for _ in range(line.count('{')):
            stack.append(('Unknown', 'unknown'))

        for _ in range(line.count('}')):
            stack.pop()

    def analyzeDeclarationType(identifier: str, line: str):
        line = line.strip()

        # print(f"Line stripped: \"{line}\"")

        def fullname(declName: str) -> str:
            if len(stack) == 0:
                return declName
            return "::".join([name for (name, _) in stack]) + f"::{declName}"

        if line.startswith(identifier):
            line = line.removeprefix(identifier)
            if line.find(';') != -1:
                # A forward declaration, add to dictionary only.
                line = line.replace(';', '')
                declName = line.strip()
                if not declName:
                    return False
                dictionary[fullname(declName)] = identifier
                return True
            elif line.find('{') != -1:
                # Normal declaration, add to stack.
                line = line.replace('{', '')
                declName = line.strip()
                if not declName:
                    return False
                dictionary[fullname(declName)] = identifier
                stack.append((declName, identifier))
                return True
        
        return False

    for line in headerData.split('\n'):
        if analyzeDeclarationType('namespace', line):
            continue
        if analyzeDeclarationType('class', line):
            continue
        if analyzeDeclarationType('struct', line):
            continue

        processBrackets(line)
    
    return dictionary

def analyzeDirectory(files: Iterator[Tuple[str, list[str], list[str]]]):
    dictionary: Dict[str, str] = dict()
    for (directory, _, fileList) in files:
        for fileName in fileList:
            if(fileName.endswith(".h")):
                directory = os.path.realpath(directory)
                filePath = f"{directory}/{fileName}"
                
                with open(filePath, 'r') as file:
                    data = file.read()
                    dictionary = dictionary | analyzeFile(data)

    return dictionary