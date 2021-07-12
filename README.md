# envoy

build evented, multicast notification chains

## Dynamic Linking

Linking to `lib.envoy`:

```bash
# 1) include and use lib.envoy in your project
# 2) generate object file for your project
gcc -I ../path/to/libenvoy -c main.c -o main.o
# 3) generate shared object file
make
# 4) link your project to lib.envoy
gcc -o main main.o -L../path/to/libenvoy -llibenvoy
# you may need to add the lib location to your PATH
```

Linking to `lib.envoy` on Windows:

```bash
# 1) include and use lib.envoy in your project
# 2) generate object file for your project
gcc -I ../path/to/libenvoy -c main.c -o main.o
# 3) generate shared object file
make win
# 3) link your project to lib.envoy
gcc -o main.exe main.o -L /path/to/lib.envoy -llib_envoy.dll
# you may need to add the lib location to your PATH
```
