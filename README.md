# 🍄 CoguRVRP
CoguRVRP is a small game developed with <a href="https://github.com/raysan5/raylib">Raylib</a> by a small group of friends. The main language is C++ and the goal was to study how to make games with frameworks and how to work together. 
## 🛠️ How to play
To play CoguRVRP, you will need:
- G++ compiler.
- Cmake.
- Git


1. Clone the repository into the machine:
```bash
git clone https://github.com/Renan-G-projec/CoguRVRP.git
cd CoguRVRP
```

2. and run those commands at the folder:
```sh
mkdir lib
cd lib
git clone https://github.com/raysan5/raylib.git
cd ..
mkdir build && cd build
cmake .. && make
```

3. The game is ready to play! Just run at the build folder this command:
```bash
./cogurvrp
```