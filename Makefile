funftp: ftplib.o ftp.o hdu_log.o main.o
	gcc -o funftp main.o ftplib.o ftp.o hdu_log.o

ftplib.o: ftplib.c ftplib.h
	cc -c ftplib.c

ftp.o: ftp.c ftp.h
	cc -c ftp.c

hdu_log.o: hdu_log.c hdu_log.h
	cc -c hdu_log.c

main.o: main.c ftplib.h ftp.h hdu_log.h
	cc -c main.c

clean:
	rm main.o ftplib.o ftp.o hdu_log.o 
	rm funftp
