

test: neural_net
		./neural_net -a 2 -b 2 -c 250 -s 4 -t 2000000

view:
	gnuplot readed_data.gp
	gnuplot result.gp
	gnuplot result_error.gp
	open readed_data.png result.png result_error.png

load:
	curl -u ensyu:ensyu -O  http://www.me.cs.scitec.kobe-u.ac.jp/~takigu/class/media/work/class1.dat
	curl -u ensyu:ensyu -O  http://www.me.cs.scitec.kobe-u.ac.jp/~takigu/class/media/work/class2.dat
	curl -u ensyu:ensyu -O  http://www.me.cs.scitec.kobe-u.ac.jp/~takigu/class/media/work/test.dat

mkdata:make_data
	./make_data

mkdata_nonlinear:make_data_nonlinear
	./make_data_nonlinear

mkdata_circle:make_data_circle
	./make_data_circle

make_data:make_data.c
	gcc -Wall make_data.c -o make_data

make_data_nonlinear:make_data_nonlinear.c
	gcc -Wall make_data_nonlinear.c -o make_data_nonlinear

make_data_circle:make_data_circle.c
	gcc -Wall make_data_circle.c -o make_data_circle

rmdata:
	rm -f class1.dat class2.dat test.dat

neural_net: initialize_weight.o read_data.o neural_net.o make.o
		gcc -Wall initialize_weight.o read_data.o neural_net.o main.o -o neural_net

initialize_weight.o: initialize_weight.c
	gcc -Wall -c initialize_weight.c

read_data.o: read_data.c
	gcc -Wall -c read_data.c

neural_net.o: neural_net.c
	gcc -Wall -c neural_net.c

make.o: main.c
	gcc -Wall -c main.c

clean:
	rm -f initialize_weight.o read_data.o neural_net.o main.o neural_net make_data make_data_nonlinear make_data_circle
	rm -f result.png result_error.png readed_data.png result_error.png
	rm -f make_data
	rm -f readed_data.dat result.dat result_error.dat
