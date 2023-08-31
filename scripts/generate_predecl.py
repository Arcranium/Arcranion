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

    for name, t in dictionary.items():
        namespace = name.split("::")
        declName = namespace.pop()


        currentDict = namespaceDict

        if t == "namespace":
            continue

        for i in range(len(namespace)):
            if not currentDict.get(namespace[i]):
                currentDict[namespace[i]] = dict()
            currentDict = currentDict[namespace[i]]

        currentDict[declName] = dict()
        currentDict[declName]["$$__type__$$"] = t

    headerContent = """
// This file is auto-generated by python script
// DO NOT MODIFY, CHANGES WILL BE LOST

#ifndef ARCRANION_GENERATED_PREDECLS_H
#define ARCRANION_GENERATED_PREDECLS_H

"""
    
    def searchNamespace(dictionary: Dict, depth: int = 0):
        global headerContent

        tabs = ""
        for _ in range(depth):
            tabs += "\t"

        for k, v in dictionary.items():
            if (type(v) is dict) and ((len(v.items()) > 1) or v["$$__type__$$"] == "namespace"):
                # namespace
                headerContent += f"{tabs}namespace {k} {'{'}\n"
                searchNamespace(v, depth+1)
                headerContent += f"{tabs}{'}'} // namespace {k}\n"
            else:
                # struct / class
                if type(v) is not dict:
                    continue
                if v["$$__type__$$"] == "class":
                    headerContent += f"{tabs}class {k};\n"
                elif v["$$__type__$$"] == "struct":
                    headerContent += f"{tabs}struct {k};\n"
    searchNamespace(namespaceDict)

    headerContent += "\n#endif"

    with open(outputFile, 'w') as file:
        file.write(headerContent)