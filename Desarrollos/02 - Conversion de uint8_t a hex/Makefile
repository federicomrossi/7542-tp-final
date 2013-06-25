# Makefile de ejemplo para C++
#
# Creado: jue abr 15 15:34:19 ART 2004
#
# Copyleft 2004 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]
#

# CONFIGURACION
################

# Nombre del ejecutable.
target = tp

# Extensión de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp

# Archivos con el código fuente que componen el ejecutable. Si no se especifica,
# toma todos los archivos con la extensión mencionada. Para especificar hay que
# descomentar la línea (quitarle el '#' del principio).
# NOTA: No poner cabeceras (.h).
#fuentes = entrada.cpp

# Si usa funciones de math.h, descomentá (quitale el '#' a) la siguiente línea.
#math = si

# Si usa pthreads, descomentá (quitale el '#' a) la siguiente línea.
#pthreads = si

# Si usa gthreads, descomentá (quitale el '#' a) la siguiente línea.
#gthreads = si

# Si es un programa GTK+, descomentá (quitale el '#' a) la siguiente línea.
#gtk = si

# Si es un programa gtkmm, descomentá (quitale el '#' a) la siguiente línea.
#gtkmm = si

# Descomentar si se quiere ver como se invoca al compilador
#verbose = si


# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ en modo ansi.
CFLAGS = -Wall -Werror -pedantic -pedantic-errors

# Para que explote lo mas posible
CFLAGS += -O3 -DNDEBUG

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++98

# Si se quiere compilar estaticamente, descomentar la siguiente linea
#static = si

# Archivo log de Valgrind.
VGLOGFILE = valgrind.out

# Flags para el Valgrind
VGFLAGS = --tool=memcheck --trace-children=yes --track-fds=yes --time-stamp=yes --num-callers=20 --error-exitcode=42 --db-attach=no --leak-check=full --leak-resolution=med --log-file=$(VGLOGFILE)



# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtk
CFLAGS += $(shell pkg-config --cflags gtk+-2.0)
LDFLAGS += $(shell pkg-config --libs gtk+-2.0)
endif

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtkmm
CFLAGS += $(shell pkg-config --cflags gtkmm-2.4)
LDFLAGS += $(shell pkg-config --libs gtkmm-2.4)
endif

# Agrego flags y libs de gthreads de ser necesario.
ifdef gthreads
CFLAGS += $(shell pkg-config --cflags gthread-2.0)
LDFLAGS += $(shell pkg-config --libs gthread-2.0)
endif

# Linkeo con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

# Linkeo con pthreads de ser necesario.
ifdef pthreads
LDFLAGS += -lpthread -pthread
endif

ifdef static
LDFLAGS += -static
endif

# Pongo flags de C para C++ también
CXXFLAGS += $(CFLAGS)

# Uso enlazador de c++ si es código no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
else
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)
endif

# Si no especifica archivos, tomo todos.
fuentes ?= $(wildcard *.$(extension))

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif


# REGLAS
#########

.PHONY: all clean

all: $(target)

o_files = $(patsubst %.$(extension),%.o,$(fuentes))

%.deps: %.$(extension)
	@set -e; rm -f $@; \
		$(old) -MM -MG -MP $(CXXFLAGS) $(CFLAGS) $< -MF $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

-include $(o_files:.o=.deps)

$(target): $(o_files)
	@if [ -z "$(o_files)" ]; \
	then \
		echo "No hay archivos de entrada, recuerde que la extensión debe ser '.$(extension)'."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_files) $(LOADLIBS) $(LDLIBS) -o $(target)

clean:
	$(RM) $(o_files) $(o_files:.o=.deps) $(VGLOGFILE) $(target)

run:
	./$(target)

valgrind1:
	if [ -a 01/_out.txt ] ; \
	then \
	     rm 01/_out.txt ; \
	fi;	
	valgrind $(VGFLAGS) ./$(target) 01/reglas.txt 01/in.txt 01/_out.txt

valgrind2: 
	if [ -a 02/_out.txt ] ; \
	then \
	     rm 02/_out.txt ; \
	fi;	
	valgrind $(VGFLAGS) ./$(target) 02/reglas.txt 02/in.txt 02/_out.txt

valgrind3:
	if [ -a 03/_out.txt ] ; \
	then \
	     rm 03/_out.txt ; \
	fi;	
	valgrind $(VGFLAGS) ./$(target) 03/reglas.txt 03/in.txt 03/_out.txt

valgrind4: 
	if [ -a 04/_out.txt ] ; \
	then \
	     rm 04/_out.txt ; \
	fi;	
	valgrind $(VGFLAGS) ./$(target) 04/reglas.txt 04/in.txt 04/_out.txt

valgrind5: 
	if [ -a 05/_out.txt ] ; \
	then \
	     rm 05/_out.txt ; \
	fi;	
	valgrind $(VGFLAGS) ./$(target) 05/reglas.txt 05/in.txt 05/_out.txt

valgrind6: 
	if [ -a 06/_out.txt ] ; \
	then \
	     rm 06/_out.txt ; \
	fi;	
	valgrind $(VGFLAGS) ./$(target) 06/reglas.txt 06/in.txt 06/_out.txt

p1:
	./tp 01/reglas.txt 01/in.txt -

p2:
	./tp 02/reglas.txt 02/in.txt -

p3:
	./tp 03/reglas.txt 03/in.txt -

p4:
	./tp 04/reglas.txt 04/in.txt -

p5:
	./tp 05/reglas.txt 05/in.txt -

p6:
	./tp 06/reglas.txt 06/in.txt -

p1s:	
	if [ -a 01/_out.txt ] ; \
	then \
	     rm 01/_out.txt ; \
	fi;	
	./tp 01/reglas.txt 01/in.txt 01/_out.txt

p2s:
	if [ -a 02/_out.txt ] ; \
	then \
	     rm 02/_out.txt ; \
	fi;
	./tp 02/reglas.txt 02/in.txt 02/_out.txt

p3s:
	if [ -a 03/_out.txt ] ; \
	then 
	     rm 03/_out.txt ; \
	fi;
	./tp 03/reglas.txt 03/in.txt 03/_out.txt

p4s:
	if [ -a 04/_out.txt ] ; \
	then \
	     rm 04/_out.txt ; \
	fi;
	./tp 04/reglas.txt 04/in.txt 04/_out.txt

p5s:
	if [ -a 05/_out.txt ] ; \
	then \
	     rm 05/_out.txt ; \
	fi;
	./tp 05/reglas.txt 05/in.txt 05/_out.txt

p6s:
	if [ -a 06/_out.txt ] ; \
	then \
	     rm 06/_out.txt ; \
	fi;
	./tp 06/reglas.txt 06/in.txt 06/_out.txt


cpplint:
	python ./cpplint.py --filter=`cat filter_options` `find -regextype posix-egrep -regex '.*\.(h|hpp|c|cpp)'`
