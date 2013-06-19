#
#	MAKEFILE DE COMPILACION DE AU
#
#	Se permite compilar las aplicaciones servidor y cliente.
#



# DIRECTIVAS DE COMPILACION
# #########################

.PHONY: all clean

all: server monitor client 

server:
	@ make --no-print-directory -s -f Makefile_server

monitor:
	@ make --no-print-directory -s -f Makefile_monitor

client:
	@ make --no-print-directory -s -f Makefile_client

clean:
	@ make --no-print-directory -s -f Makefile_server clean
	@ make --no-print-directory -s -f Makefile_monitor clean
	@ make --no-print-directory -s -f Makefile_client clean



# DIRECTIVAS DE EJECUCION
# #######################

run_server:
	@ make --no-print-directory -s -f Makefile_server p_server

run_monitor:
	@ make --no-print-directory -s -f Makefile_monitor p_monitor

run_client:
	@ make --no-print-directory -s -f Makefile_client p_client


# DIRECTIVAS DE DEBUG
# ###################

run_server_valgrind:
	@ make --no-print-directory -s -f Makefile_server s_valgrind

run_monitor_valgrind:
	@ make --no-print-directory -s -f Makefile_monitor m_valgrind

run_client_valgrind:
	@ make --no-print-directory -s -f Makefile_client c_valgrind