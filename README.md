# lo3 programming language

![License](https://img.shields.io/github/license/lo3-lang/lo3-core)
![Release](https://img.shields.io/github/v/release/lo3-lang/lo3-core)
![Build](https://img.shields.io/github/actions/workflow/status/lo3-lang/lo3-core/release.yml)
![Language](https://img.shields.io/badge/written%20in-C-blue)
![Platform](https://img.shields.io/badge/platform-linux%20|%20windows-lightgrey)
![GitHub Issues or Pull Requests](https://img.shields.io/github/issues/lo3-lang/lo3-core)


> Currently the code is only available on linux, because in windows there is no getline ...
> NOTE: Claude Code is not used for the core func of this repo. Only for bug fixes (human in a loop - version)

## What is this language about?
lo3 is a token based interpreted programming language written in C.

## Website
See [Main Website](https://lo3-lang.github.io)

See [learn-the-syntax](https://lo3-lang.github.io/learn-the-syntax/) for examples and documentation.

## Run lo3 at your own machine

```bash
lo3 {FILENAME}
```
This will start the interpreter and run your code.

## Progress

| Feature | Status |
| :--- | :--- |
| ALU Operations | ✅ |
| Control Flow | ✅ |
| Func Calling | ❌ |
| Input/STDIN | ✅ |
| Output/STDOUT | ✅ |
| Operating with vars | ✅ |
| Operating with g[] | ⚠️ |
| Manually exiting with 0 or 1 | ✅ |
| Auto exiting | ✅ |

## Compile it yourself

### Linux:
```bash
cmake -B build
cmake --build build
./bin/lo3 test.lo3
```

### Win:
```powershell
cmake -B build
cmake --build build
.\bin\lo3.exe test.lo3
```

## Add it to the PATH

### Linux:

If you are root / have rights to sudo:
```
sudo cp ./bin/lo3 /usr/local/bin/
```

Or if you dont:
```
cp ./bin/lo3 ~/.local/bin/
export PATH="$HOME/.local/bin:$PATH"
```

> Then you can just write `lo3 ...`

### Windows:
Press `Win+R` input `sysdm.cpl`
In more (or any other name) > Environment Variables
In path add a new index and input there the path to this program.

For example:
`C:\Users\yourUserName\Desktop\lo3-core\build\`
