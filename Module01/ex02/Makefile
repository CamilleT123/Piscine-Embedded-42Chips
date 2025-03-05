
PROG_NAME = Embedded
FILE_NAME = main
F_CPU = 16000000 #si 16MHz - verifier la frequence reelle
# SRC_DIR = src
# BUILD_DIR = build
SRC = main.c
BAUD = 115200

CFLAGS = -mmcu=atmega328p -Wall -Werror -Wextra -Os 
# -mmcu=atmega328p spécifie que la compilation doit être faite pour le microcontrôleur ATMega328p.
# -Os optimise pour la taille
			
# OBJ = $(SRC:%.c=%.elf)
BIN = $(SRC:%.c=%.bin)
HEX = $(BIN:%.bin=%.hex)

all: hex flash

hex:
	avr-gcc $(CFLAGS) -DF_CPU=$(F_CPU) -o $(BIN) $(SRC)
	avr-objcopy -O ihex $(BIN) $(HEX)
# avr-gcc -mmcu=atmega328p -DF_CPU=$(F_CPU) -Os -o $(BUILD_DIR)/$(PROGRAM_NAME).bin $(SRC_DIR)/$(PROGRAM_NAME).c
# avr-objcopy -O ihex $(BUILD_DIR)/$(PROGRAM_NAME).bin $(BUILD_DIR)/$(PROGRAM_NAME).hex
# -O specifie le format de sortie

flash:
	avrdude -c arduino -P /dev/ttyUSB0 -b $(BAUD) -p m328p -U flash:w:$(HEX):i
# -U est utilisé pour spécifier l'opération de programmation et les fichiers à utiliser.
# flash spécifie que vous voulez programmer la mémoire flash du microcontrôleur.
# w indique que vous voulez écrire des données (write).
# $(HEX) fait référence à la variable contenant le fichier .hex que vous voulez charger dans la mémoire flash du microcontrôleur
# i indique que le fichier .hex est au format Intel Hex, qui est un format standard pour le transfert de données dans les microcontrôleurs.	

	
# $(PROG_NAME): $(OBJ) 
# 	$(CC) $(CFLAGS) -o $(OBJ) $(SRC)
# 	avr-gcc
# 	avr-objcopy
# 	avrdude

clean:
	rm -f $(BIN) $(HEX)

# fclean: clean
# 	rm -f $(NAME)

# re : fclean
# 	$(MAKE) $(NAME)

# .PHONY: all clean fclean re all

