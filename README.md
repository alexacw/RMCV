# CV_Tutorial
Code for internal tutorial.

Compile & run the code:
```
cmake .
make
./main
```

CMakeLists.txt should work fine on linux system, on Mac it may raise error:
```
ld: library not found for -lrt
```
It can be solved by replacing the last line in CMakeLists.txt to:
```
target_link_libraries( main -lpthread ${OpenCV_LIBS})
```

If you still have problems, remeber google is your best teacher.

You can directly use CMakeLists.txt in your own CV project.
