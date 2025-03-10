
PROG_NAME = Embedded
FILE_NAME = main
F_CPU = 16000000 # frequence 16MHz - sur devkit
SRC = main.c
BAUD = 115200

GCCFLAGS = -mmcu=atmega328p -DF_CPU=$(F_CPU) -Wall -Os -g
# -mmcu=atmega328p spécifie que la compilation doit être faite pour le microcontrôleur ATMega328p.
# -Os optimise pour la taille
			
BIN = $(SRC:%.c=%.bin)
HEX = $(BIN:%.bin=%.hex)

all: hex flash

hex:
	avr-gcc $(GCCFLAGS)  -o $(BIN) $(SRC)
# avr-gcc compile le code source pour le transformer en binaire
	avr-objcopy -O ihex $(BIN) $(HEX)
# avr-objcopy pour convertir fichier bin en hex
# -O specifie le format de sortie

flash:
	avrdude -c arduino -P /dev/ttyUSB0 -b $(BAUD) -p m328p -U flash:w:$(HEX):i
#avrdude pour charger le programme sur le microcontroller
# -c arduino pour preciser le programmateur
# -P /dev/ttyUSB0 specifie le port de connection du programmateur
# -b $(BAUD) definie la vitesse de communication
# -p m328p specifie le modele du microcontroller
# -U est utilisé pour spécifier l'opération de programmation et les fichiers à utiliser.
# flash spécifie qu'on veut programmer la mémoire flash du microcontrôleur.
# w indique qu'on veut écrire des données (write)
# $(HEX) fait référence au fichier .hex a charger dans le microcontrôleur
# i indique que le fichier .hex est au format Intel Hex

clean:
	rm -f $(BIN) $(HEX)

.PHONY: all hex flash clean 
# les termes indiques sont des actions et non des fichiers - eviter les conflits

# quand plusieurs fichiers
# SRC_DIR = src
# BUILD_DIR = build
# avr-gcc -mmcu=atmega328p -DF_CPU=$(F_CPU) -Os -o $(BUILD_DIR)/$(PROGRAM_NAME).bin $(SRC_DIR)/$(PROGRAM_NAME).c
# avr-objcopy -O ihex $(BUILD_DIR)/$(PROGRAM_NAME).bin $(BUILD_DIR)/$(PROGRAM_NAME).hex