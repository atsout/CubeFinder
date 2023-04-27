# Makefile for CubeFinder

.PHONY: all clean

SOURCE=src
OUT=build
INCLUDE=include

OBJ=${OUT}/parse.o ${OUT}/filter.o ${OUT}/main.o ${OUT}/contour.o ${OUT}/convert.o ${OUT}/export.o

CVPATH=/usr/include/opencv4
CVLIB=-lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_photo
XLSXLIB=-lxlsxwriter -lz

all: ${OUT}/CubeFinder

# rule to link object files and libraries 
${OUT}/CubeFinder: ${OBJ}
	g++ -I${CVPATH} ${OBJ} -o ${OUT}/CubeFinder ${CVLIB} ${XLSXLIB}

# rule to build source files
${OUT}/%.o: ${SOURCE}/%.cpp ${INCLUDE}/%.hpp
	g++ -c -I${CVPATH} -I${INCLUDE} $< -o $@ ${CVLIB} ${XLSXLIB}

clean:
	rm ${OUT}/* 


