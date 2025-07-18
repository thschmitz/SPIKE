#!/bin/bash

if [ -z "$1" ]; then
  echo "Uso: ./run_spike.sh arquivo.elf"
  exit 1
fi

ELF="$1"
ESTATS_FILE="estatisticas.txt"
LOG_FILE="log.txt"

rm -f "$ESTATS_FILE" "$LOG_FILE"

# Executa o spike, separando apenas as mensagens do seu programa para o log
spike -l pk "$ELF" 2>&1 | tee >(grep -v "^core" > "$LOG_FILE") | awk '
  /###START###/ { contar = 1; next }
  /###END###/   { contar = 0; next }

  contar && /^core/ {
    total_instr++;
    if ($0 ~ /(lb|lh|lw|lbu|lhu)/) total_load++;
    if ($0 ~ /(sb|sh|sw)/) total_store++;
  }

  END {
    print "=== Estatísticas entre START e END ===" > "'"$ESTATS_FILE"'";
    print "Instruções executadas: " total_instr >> "'"$ESTATS_FILE"'";
    print "Acessos de leitura (load): " total_load >> "'"$ESTATS_FILE"'";
    print "Acessos de escrita (store): " total_store >> "'"$ESTATS_FILE"'";
  }
'

