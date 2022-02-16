# IPK Projekt 1 
## Jakub Kuzník xkuzni04 Feb 16 2022


# IPK Projekt 1 

Minimalistický HTTP server v jazyce C. Který dokáže zpracovat jednoduché HTTP požadavky. Server dokáže zjistit základní údaje o sobě a poslat je pomocí http protokolu klientovi. 

## Spuštění 

Potřeba gcc. Nutný jeden argument programu, ten udavá číslo portu, na kterém bude server naslouchat.  


kompilace
```
$ make
```
Spuštění serveru, který bude naslouchat na portu 12345
```
$ ./hinfosvc 12345 & 
```

## Použití 

Použití ze strany klienta. například pomocí nástroju curl, nebo internet browseru.

Zjistíme network hostname serveru 
```
http://localhost:12345/hostname 
```
Pro zjištění modelu cpu, na kterém server běží.
```
http://localhost:12345/cpu-name
```
Zjištění vytížení cpu serveru.
```
http://localhost:12345/load
```


