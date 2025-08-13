# Определяем компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -pedantic
# Флаги для OpenSSL
OPENSSL_LDFLAGS = -lssl -lcrypto
# Флаги для других библиотек можно добавлять сюда

# Имена команд
COMMANDS = hashit memstat expr

# Директории, где лежат исходники
DIR = src/

# Директория для установки исполняемых файлов
BIN_DIR = $(DESTDIR)/usr/local/bin

#---
# Правила сборки
#---

# Основное правило для сборки всех команд
all: $(COMMANDS)

# Правило для сборки hashit
hashit: $(DIR)/hashit.c
	$(CC) $(CFLAGS) $< -o $@ $(OPENSSL_LDFLAGS)

# Правило для сборки memstat
memstat: $(DIR)/memstat.c
	$(CC) $(CFLAGS) $< -o $@
	
expr: $(DIR)/expr.c
	$(CC) $(CFLAGS) $< -o $@

#---
# Правила установки и удаления
#---

# Установка всех команд в систему
install: all
	@echo "Installing commands to $(BIN_DIR)..."
	@mkdir -p $(BIN_DIR)
	@for cmd in $(COMMANDS); do cp $$cmd $(BIN_DIR); done

# Удаление всех команд из системы
uninstall:
	@echo "Uninstalling commands from $(BIN_DIR)..."
	@for cmd in $(COMMANDS); do rm -f $(BIN_DIR)/$$cmd; done

#---
# Правила для очистки
#---

# Очистка скомпилированных файлов
clean:
	@echo "Cleaning up..."
	@for cmd in $(COMMANDS); do rm -f $$cmd; done


#---
# Фальшивые цели, чтобы make не искал файлы с такими именами
#---

.PHONY: all install uninstall clean