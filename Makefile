CFLAGS=-Wall 
LIBS=-O2 -L/usr/X11R6/lib -lm -lpthread -lX11
EXE=SCALP

all:
	mkdir -p res
	g++ $(CFLAGS) SCALP.cpp -o $(EXE) $(LIBS)

test:
	./SCALP -i ./data/test_img.jpg -k 450 -m 0.075 -outm test_img_labelmap.png -outb test_img_border.png -c ./data/contour_test_img.png

test_list:
	./scripts/test_list.sh ./data/list_file.txt ./data/ 450 0.075 
