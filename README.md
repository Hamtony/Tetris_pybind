# To execute:
1. run: 
```pip install pybind11 pygame```
2. run: 
```git clone https://github.com/pybind/pybind11.git```
3. adapt this to your compiler (in my case is clang) and set DPYTHON_EXECUTABLE with your path to your python executable:
```mkdir build; cd build; cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DPYTHON_EXECUTABLE="path/to/python.exe" ..; make; cd ..```
4. run: 
```python game.py```