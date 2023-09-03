import argparse
import os
import termcolor

def createParser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser()
    
    parser.add_argument("-c", "--glslc", dest="glslcPath", action="store")
    parser.add_argument("-s", "--source", dest="sourceDirectory", action="store")
    parser.add_argument("-o", "--output", dest="outputDirectory", action="store")

    return parser

if __name__ == "__main__":
    args = createParser().parse_args()

    print("Compiling shaders")
    print(f"-Source: \"{args.sourceDirectory}\"")
    print(f"-Output: \"{args.outputDirectory}\"\n")

    for (dir, dirs, files) in os.walk(args.sourceDirectory):
        for file in files:
            sourceRelDir = os.path.relpath(dir, args.sourceDirectory)
            outputDir = os.path.join(args.outputDirectory, sourceRelDir)

            sourcePath = os.path.abspath(f"{dir}/{file}")
            outputPath = os.path.abspath(f"{outputDir}/{file}.spv")

            print(f"\n{termcolor.colored('Compiling:', 'green')} \"{termcolor.colored(os.path.relpath(f'{dir}/{file}'), 'blue')}\" → \"{termcolor.colored(os.path.join(os.path.relpath(outputDir), file), 'cyan')}\"")

            os.system(f"{args.glslcPath} {sourcePath} -o {outputPath}")
    pass