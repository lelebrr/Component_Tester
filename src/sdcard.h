// ============================================================================
// Sondvolt v3.0 — Sistema de SD Card (Header)
// Descricao: Wrapper para gerenciamento do cartao SD
// ============================================================================
#ifndef SDCARD_H
#define SDCARD_H

#include <stdint.h>
#include <Arduino.h>
#include <FS.h>
#include <SD.h>

// ============================================================================
// CONFIGURACOES
// ============================================================================

#define SD_MOUNT_POINT         "/sdcard"
#define SD_MAX_FILENAME_LEN    64
#define SD_MAX_FILES_OPEN      4

// ============================================================================
// ESTADO DO CARTAO
// ============================================================================

enum SDState {
    SD_STATE_NOT_MOUNTED = 0,
    SD_STATE_MOUNTED = 1,
    SD_STATE_ERROR = 2,
    SD_STATE_READ_ONLY = 3
};

// Informacoes do cartao
typedef struct {
    SDState state;
    uint32_t totalBytes;
    uint32_t usedBytes;
    uint8_t type;
    uint8_t speed;
    bool writeEnabled;
} SDCardInfo;

// ============================================================================
// VARIAVEIS EXTERNAS
// ============================================================================

extern bool sdCardPresent;
extern bool sdCardError;

// ============================================================================
// PROTOTIPOS
// ============================================================================

// Inicializacao
bool sd_init();
bool sd_mount();
void sd_unmount();

// Informacoes
SDCardInfo sd_get_info();
bool sd_is_ready();
uint32_t sd_free_space();

// Operacoes de arquivo
File sd_open_file(const char* path, const char* mode);
bool sd_close_file(File* file);
bool sd_file_exists(const char* path);
bool sd_delete_file(const char* path);
bool sd_rename_file(const char* oldPath, const char* newPath);

// Escrita
size_t sd_write_line(File* file, const char* line);
size_t sd_write_csv(File* file, const char* data);

// Listagem
bool sd_list_dir(const char* path);

// Utilitarios
const char* sd_get_error_string();
void sd_print_info();

#endif // SDCARD_H