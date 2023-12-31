import os
import analyze_decl
from typing import Dict, Any

if __name__ == "__main__":
    scriptDir = os.path.dirname(os.path.realpath(__file__))
    outputDir = f"{scriptDir}/../main/src/arcranion/generated"
    outputFile = f"{outputDir}/predecls.h"

    if not os.path.exists(outputDir):
        os.makedirs(outputDir, exist_ok=True)

    walkResult = list(os.walk(f'{scriptDir}/../main/src'))
    for dir, dirs, files in walkResult:
        for file in files:
            filePath = os.path.realpath(f"{dir}/{file}")
            if os.path.abspath(filePath) == os.path.abspath(outputFile):
                walkResult.remove((dir, dirs, files))
    dictionary = analyze_decl.analyzeDirectory(walkResult)

    namespaceDict: Dict[str, Any] = dict()

    headerContent = """
// This file is auto-generated by python script
// DO NOT MODIFY, CHANGES WILL BE LOST

#ifndef ARCRANION_GENERATED_PREDECLS_H
#define ARCRANION_GENERATED_PREDECLS_H

"""

    def searchDecls(dictionary: Dict, name: str, depth: int = 0) -> str:
        global headerContent

        tabs = ""
        for _ in range(depth):
            tabs += "\t"

        if dictionary["$$__type__$$"] == "class":
            headerContent += f"{tabs}class {name};\n"
        elif dictionary["$$__type__$$"] == "struct":
            headerContent += f"{tabs}struct {name};\n"

        for k, v in dictionary.items():
            if type(v) is str:
                continue
            
            searchDecls(v, f"{name}::{k}", depth+1)
        return
    
    def searchNamespace(dictionary: Dict, depth: int = 0):
        global headerContent

        tabs = ""
        for _ in range(depth):
            tabs += "\t"

        for k, v in dictionary.items():
            if type(v) is str:
                continue

            if v["$$__type__$$"] == "namespace":
                if len(v.items()) > 1:
                    headerContent += f"{tabs}namespace {k} {'{'}\n"
                    searchNamespace(v, depth+1)
                    headerContent += f"{tabs}{'};'}\n"
                else:
                    headerContent += f"{tabs}namespace {k} {'{}'}\n"
            elif v["$$__type__$$"] == "class" or v["$$__type__$$"] == "struct":
                searchDecls(v, k, depth+1)
    searchNamespace(dictionary)

    headerContent += "\n#endif"

    with open(outputFile, 'w') as file:
        file.write(headerContent)