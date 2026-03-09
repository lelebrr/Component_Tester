#include "logger.h"
#include "globals.h"
#include "utils.h"
#include <Arduino.h>
#include <SD.h>

// Inicializa o logging (já feito no setup do main, mas a função pode ser usada
// para re-inicializar)
void log_init() {
  // O SD.begin(SD_CS_PIN) já é chamado no setup do main.cpp
  // Aqui poderíamos adicionar lógica para verificar a existência do arquivo
  // LOG.TXT ou criar cabeçalhos se necessário.

  // Verifica se o SD Card está disponível
  if (!SD.begin(SD_CS_PIN)) {
    LOG_SERIAL("SD Err!");
    return;
  }

  // Cria arquivo LOG.TXT se não existir
  File logFile = SD.open(F("LOG.TXT"), FILE_WRITE);
  if (logFile) {
    if (logFile.size() == 0) {
      // Escreve cabeçalho se arquivo estiver vazio
      logFile.println(F("CT PRO v2.0 Log"));
      logFile.println(F("ms,Type,Val,Temp,Judg"));
    }
    logFile.close();
  } else {
    LOG_SERIAL("Log Err");
  }
}

// Registra uma medição no arquivo LOG.TXT
void log_measurement(const char *type, float value, float temp,
                     const char *judgment) {
  File logFile = SD.open(F("LOG.TXT"), FILE_WRITE);
  if (logFile) {
    logFile.print(millis());
    logFile.print(',');
    logFile.print(type);
    logFile.print(',');
    fprint(logFile, value, 2);
    logFile.print(',');
    fprint(logFile, temp, 1);
    logFile.print(',');
    logFile.println(judgment);
    logFile.close();
  } else {
    LOG_SERIAL("SD Wrt Err");
  }
}
