# Makefile de ejemplo para programa cliente-servidor en C/C++
# 
# Creado: vie abr 27 00:37:46 ART 2007
#
# Copyleft 2007 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]
#

# CONFIGURACION
################

# Extensión de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp

# Si usa funciones de math.h, descomentá (quitale el '#' a) la siguiente línea.
#math = si

# Si usa pthreads, descomentá (quitale el '#' a) la siguiente línea.
pthreads = si

# Si usa gthreads, descomentá (quitale el '#' a) la siguiente línea.
#gthreads = si

# Si es un programa GTK+, descomentá (quitale el '#' a) la siguiente línea.
#gtk = si

# Si es un programa gtkmm, descomentá (quitale el '#' a) la siguiente línea.
gtkmm = si

# Descomentar si se quiere ver como se invoca al compilador
#verbose = si

# Archivo log de Valgrind.
S_VGLOGFILE = valgrind_server.out

# Flags para el Valgrind
S_VGFLAGS = --tool=memcheck --trace-children=yes --track-fds=yes --time-stamp=yes --num-callers=20 --error-exitcode=42 --db-attach=no --leak-check=full --leak-resolution=med --leak-check=full --show-reachable=yes --log-file=$(S_VGLOGFILE)



# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ en modo ansi.
CFLAGS = -Wall -Werror -pedantic -pedantic-errors -Wno-long-long -ggdb

# Para que explote lo mas posible
CFLAGS += -O3 -DNDEBUG

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
LDFLAGS =

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++98

# Si se quiere compilar estaticamente, descomentar la siguiente linea
#static = si


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtk
CFLAGS += $(shell pkg-config --cflags gtk+-2.0)
LDFLAGS += $(shell pkg-config --libs gtk+-2.0)
endif

# Agrego flags y libs de gtkmm de ser necesario.
ifdef gtkmm
CFLAGS += $(shell pkg-config --cflags gtkmm-3.0)
LDFLAGS += $(shell pkg-config --libs gtkmm-3.0)
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

# Si no especifica archivos, tomo todos.
fuentes_server ?= $(wildcard server*.$(extension))
fuentes_common ?= $(wildcard common*.$(extension))


# REGLAS
#########

.PHONY: all clean

all: server

o_common_files = $(patsubst %.$(extension),%.o,$(fuentes_common))
o_server_files = $(patsubst %.$(extension),%.o,$(fuentes_server)) $(o_common_files)

%.deps: %.$(extension)
	@set -e; rm -f $@; \
		$(old) -MM -MG -MP $(CXXFLAGS) $(CFLAGS) $< -MF $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

-include $(o_files:.o=.deps)

server: $(o_server_files)
	@if [ -z "$(o_server_files)" ]; \
	then \
		echo "No hay archivos de entrada para el servidor (archivos server*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(o_server_files) $(LDFLAGS) $(LOADLIBES) $(LDLIBS) -o server $(LDADD)

clean:
	@ $(RM) -fv *.o server $(S_VGLOGFILE)

cpplint:
	python ./cpplint.py --filter=`cat filter_options` `find -regextype posix-egrep -regex '.*\.(h|hpp|c|cpp)'`
	

# PRUEBAS
###################################################################

run:
	./server

p_server:
	./server

s_valgrind:
	valgrind $(S_VGFLAGS) ./server
