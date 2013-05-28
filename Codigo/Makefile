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
#gtkmm = si

# Descomentar si se quiere ver como se invoca al compilador
#verbose = si

# Archivo log de Valgrind.
S_VGLOGFILE = valgrind_server.out
C_VGLOGFILE = valgrind_client.out

# Flags para el Valgrind
S_VGFLAGS = --tool=memcheck --trace-children=yes --track-fds=yes --time-stamp=yes --num-callers=20 --error-exitcode=42 --db-attach=no --leak-check=full --leak-resolution=med --log-file=$(S_VGLOGFILE)

C_VGFLAGS = --tool=memcheck --trace-children=yes --track-fds=yes --time-stamp=yes --num-callers=20 --error-exitcode=42 --db-attach=no --leak-check=full --leak-resolution=med --log-file=$(C_VGLOGFILE)



# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ en modo ansi.
CFLAGS = -Wall -Werror -pedantic -pedantic-errors

# Para que explote lo mas posible
#CFLAGS += -O3 -DNDEBUG

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
fuentes_client ?= $(wildcard client*.$(extension))
fuentes_server ?= $(wildcard server*.$(extension))
fuentes_common ?= $(wildcard common*.$(extension))


# REGLAS
#########

.PHONY: all clean

all: client server

o_common_files = $(patsubst %.$(extension),%.o,$(fuentes_common))
o_client_files = $(patsubst %.$(extension),%.o,$(fuentes_client)) $(o_common_files)
o_server_files = $(patsubst %.$(extension),%.o,$(fuentes_server)) $(o_common_files)

%.deps: %.$(extension)
	@set -e; rm -f $@; \
		$(old) -MM -MG -MP $(CXXFLAGS) $(CFLAGS) $< -MF $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

-include $(o_files:.o=.deps)

client: $(o_client_files)
	@if [ -z "$(o_client_files)" ]; \
	then \
		echo "No hay archivos de entrada para el cliente (archivos client*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_client_files) $(LOADLIBES) $(LDLIBS) -o client $(LDADD)

server: $(o_server_files)
	@if [ -z "$(o_server_files)" ]; \
	then \
		echo "No hay archivos de entrada para el servidor (archivos server*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_server_files) $(LOADLIBES) $(LDLIBS) -o server $(LDADD)

clean:
	@$(RM) -fv *.o client server

cpplint:
	python ./cpplint.py --filter=`cat filter_options` `find -regextype posix-egrep -regex '.*\.(h|hpp|c|cpp)'`
	

# PRUEBAS
###################################################################

p_client:
	./client localhost:8000	

ps_basic:
	./server 8000 tests/basic 3 5

ps_multiple:
	./server 8000 tests/multiple 3 5

ps_nosolution:
	./server 8000 tests/no_solution 3 5

s_valgrind_basic:
	valgrind $(S_VGFLAGS) ./server 8000 tests/basic 3 5

s_valgrind_multiple:
	valgrind $(S_VGFLAGS) ./server 8000 tests/multiple 3 5

s_valgrind_nosolution:
	valgrind $(S_VGFLAGS) ./server 8000 tests/no_solution 3 5

c_valgrind:
	valgrind $(C_VGFLAGS) ./client localhost:8000

p1_basic: 
	sudo chmod +x run_1.sh free_port_1.sh;./run_1.sh 127.0.0.1 $(./free_port_1.sh 10001) no-valgrind single tests/basic 3 5

p1_multiple:
	sudo chmod +x run_1.sh free_port_1.sh;./run_1.sh 127.0.0.1 $(./free_port_1.sh 10001) no-valgrind single tests/multiple 3 5

p1_nosolution:
	sudo chmod +x run_1.sh free_port_1.sh;./run_1.sh 127.0.0.1 $(./free_port_1.sh 10001) no-valgrind single tests/no_solution 3 5

p2_basic:
	sudo chmod +x run_2.sh free_port_2.sh;./run_2.sh 127.0.0.1 $(./free_port_2.sh 11001) valgrind single tests/basic 3 5

p2_multiple:
	sudo chmod +x run_2.sh free_port_2.sh;./run_2.sh 127.0.0.1 $(./free_port_2.sh 11001) valgrind single tests/multiple 3 5

p2_nosolution:
	sudo chmod +x run_2.sh free_port_2.sh;./run_2.sh 127.0.0.1 $(./free_port_2.sh 11001) valgrind single tests/no_solution 3 5

p3_basic:
	sudo chmod +x run_3.sh free_port_3.sh;./run_3.sh 127.0.0.1 $(./free_port_3.sh 12001) no-valgrind multiple tests/basic 3 5

p3_multiple:
	sudo chmod +x run_3.sh free_port_3.sh;./run_3.sh 127.0.0.1 $(./free_port_3.sh 12001) no-valgrind multiple tests/multiple 3 5

p3_nosolution:
	sudo chmod +x run_3.sh free_port_3.sh;./run_3.sh 127.0.0.1 $(./free_port_3.sh 12001) no-valgrind multiple tests/no_solution 3 5

p4_basic:
	sudo chmod +x run_4.sh free_port_4.sh;./run_4.sh 127.0.0.1 $(./free_port_4.sh 13001) valgrind multiple tests/basic 3 5

p4_multiple:
	sudo chmod +x run_4.sh free_port_4.sh;./run_4.sh 127.0.0.1 $(./free_port_4.sh 13001) valgrind multiple tests/multiple 3 5

p4_nosolution:
	sudo chmod +x run_4.sh free_port_4.sh;./run_4.sh 127.0.0.1 $(./free_port_4.sh 13001) valgrind multiple tests/no_solution 3 5

pruebas_sercom: p1_basic
