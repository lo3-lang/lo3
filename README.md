# lo3 programming language

![License](https://img.shields.io/github/license/lo3-lang/lo3-core)
![Release](https://img.shields.io/github/v/release/lo3-lang/lo3-core)
![Build](https://img.shields.io/github/actions/workflow/status/lo3-lang/lo3-core/release.yml)
![Language](https://img.shields.io/badge/written%20in-C-blue)
![Platform](https://img.shields.io/badge/platform-linux%20|%20windows-lightgrey)

> Currently the code is only available in linux, because in windows there is no getline ... 

## Run lo3 at your own machine

```bash
lo3 {FILENAME}
```
This will start the interpreter and run your code.

## Progress

| Feature | Status |
| :--- | :--- |
| ALU Operations | ✅ |
| Control Flow | ❌ |
| Func Calling | ❌ |
| Input/STDIN | ✅ |
| Output/STDOUT | ✅ |
| Operating with vars | ✅ |
| Operating with g[] | ❌ |

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
sudo cp ./build/lo3 /usr/local/bin/
```

Or if you dont:
```
cp ./build/lo3 ~/.local/bin/
export PATH="$HOME/.local/bin:$PATH"
```

> Then you can just write `lo3 ...`

### Windows:
Press `Win+R` input `sysdm.cpl`
In more (or any other name) > Environment Variables
In path add a new index and input there the path to this program.

For example:
`C:\Users\yourUserName\Desktop\lo3-core\build\`
