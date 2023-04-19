SRV_KEY=server.key
SRV_PEM=server.pem
depends:
	openssl req -subj '/CN=dranger.zone/O=dranger/C=FR' -new -newkey rsa:4096 -days 3650 -nodes -x509 -keyout ${SRV_KEY} -out ${SRV_PEM}
	cat ${SRV_KEY} >> ${SRV_PEM}
mingw:
	x86_64-w64-mingw32-gcc reverse.c -o shell.exe -lws2_32