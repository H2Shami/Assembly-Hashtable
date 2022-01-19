a.out: main.cc you.s
	@#Uncomment this if you want to test the C++ code with ASAN enabled
	@#g++ -O3 -std=c++17 -fsanitize=address -static-libasan main.cc you.s
	g++ -std=c++17 main.cc you.s

clean:
	rm -f a.out core *.o
