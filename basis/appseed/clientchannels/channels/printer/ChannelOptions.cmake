
set(OPTION_DEFAULT OFF)
set(OPTION_CLIENT_DEFAULT ON)
set(OPTION_SERVER_DEFAULT OFF)

if(WIN32)
	set(OPTION_CLIENT_DEFAULT ON)
	set(OPTION_SERVER_DEFAULT OFF)
elseif(WITH_CUPS)
	set(OPTION_CLIENT_DEFAULT ON)
	set(OPTION_SERVER_DEFAULT OFF)
else()
	set(OPTION_CLIENT_DEFAULT OFF)
	set(OPTION_SERVER_DEFAULT OFF)
endif()

define_channel_options(NAME "printer" TYPE "device"
	DESCRIPTION "Print Virtual Channel Extension"
	SPECIFICATIONS "[MS-RDPEPC]"
	DEFAULT ${OPTION_DEFAULT})

define_channel_client_options(${OPTION_CLIENT_DEFAULT})
define_channel_server_options(${OPTION_SERVER_DEFAULT})

