# Component Tester PRO v2.0

Dispositivo testador de componentes eletrônicos para Arduino Uno R3 com tela TFT colorida, navegação por botões físicos, sonda térmica DS18B20, logging em SD Card e EEPROM para configurações persistentes.

## Hardware

| Componente | Especificação |
|------------|---------------|
| Microcontrolador | Arduino Uno R3 (ATmega328P) |
| Display | TFT 2.8" ILI9341 SPI |
| Armazenamento | Módulo Micro SD SPI |
| Interface | 6 botões push (UP/DW/LF/RT/OK/BCK) |
| Feedback | Buzzer 5V, LED Verde, LED Vermelho |
| Sonda | DS18B20 waterproof |
| Persistência | EEPROM interna |

## Pinagem

| Pino | Função |
|------|--------|
| A0 | Probe 1 (+) |
| A1 | Probe 2 (-) |
| D2 | Botão UP |
| D3 | Botão DOWN |
| D4 | SD Card CS / DS18B20 |
| D5 | Botão LEFT / Freq Input |
| D6 | Botão RIGHT |
| D7 | Botão OK |
| D8 | Botão BACK |
| D9 | Buzzer / PWM Output |
| D10 | LED Verde |
| D11 | LED Vermelho |
| D12 | TFT CS |
| D13 | TFT DC |
| A2 | TFT RST |
| A3 | DS18B20 Bus |

## Estrutura do Projeto

```
src/
├── main.cpp          - Setup e loop principal, EEPROM, histórico
├── config.h          - Definições de pinos e constantes
├── globals.h         - Variáveis globais, structs Settings e History
├── buttons.cpp/h     - Sistema de botões com debounce
├── leds.cpp/h        - Controle de LEDs
├── buzzer.cpp/h      - Sistema de beep
├── menu.cpp/h        - Navegação, menus e funcionalidades
├── measurements.cpp/h - Funções de medição
├── thermal.cpp/h     - Sonda DS18B20 com alertas
├── drawings.cpp/h    - Ícones e telas
├── database.cpp/h    - Busca de componentes no SD
├── logger.cpp/h      - Logging SD Card
└── utils.cpp/h       - Funções utilitárias
```

## Funcionalidades

### Menu Principal

1. **Medir** - Menu de medições
2. **Termica** - Sonda de temperatura DS18B20
3. **Scanner** - Auto-detecção contínua
4. **Historico** - Últimas 10 medições
5. **Estatist** - Estatísticas de medições
6. **Config** - Configurações
7. **Sobre** - Informações do dispositivo

### Medições Disponíveis (13 modos)

| Função | Descrição | Histórico |
|--------|-----------|-----------|
| measure_capacitor | Capacitor + ESR via RC | Cap |
| measure_resistor | Resistência via divisor tensão | Res |
| measure_diode | Diodo/LED com polaridade | Diod |
| measure_transistor | Identifica BJT NPN | Trans |
| measure_inductor | Indutância via tempo resposta | Ind |
| measure_voltmeter_dc | Tensão DC 0-5V | Volt |
| measure_frequency_counter | Frequência 1Hz-1MHz (Pino 5) | Freq |
| generate_pwm | PWM 1kHz 50% (Pino 9) | PWM |
| test_optocoupler | Testa isolamento LED/Transistor | Opto |
| test_cable_continuity | Continuidade de cabos | Cable |
| test_bridge_rectifier | Testa 4 diodos | Bridge |
| auto_detect_component | Detecção automática | Auto |
| test_continuity_buzzer | Continuidade com beep | Cont |

### Sonda Térmica DS18B20

- Leitura contínua a cada 500ms
- **Beep progressivo por temperatura:**
  - 70-90°C: Beep curto a cada 1s + LED piscando
  - 90-110°C: Beep a cada 500ms + LED piscando rápido
  - >110°C: Beep rápido a cada 200ms + LED fixo

### Configurações (EEPROM)

1. **Calibrar Probes** - Calibração dos probes
2. **Modo Escuro** - Alterna cores do display
3. **Modo Silencioso** - Desabilita beeps
4. **Timeout** - Alterna 30s/60s
5. **Salvar Cfg** - Salva configurações na EEPROM

### Histórico de Medições

- Armazena últimas 10 medições
- Exibe nome, valor e status (verde/vermelho)
- Indicador de boas/ruins

### Estatísticas

- Total de medições realizadas
- Total de componentes defeituosos
- Taxa de falha percentual
- Temperatura atual

### Scanner Automático

- Pressiona OK para iniciar auto-detecção
- Executa medição a cada 2 segundos
- Pressiona OK novamente para parar
- BACK para sair

## Limites de Temperatura

| Threshold | Valor | Comportamento |
|-----------|-------|---------------|
| NORMAL | < 70°C | Sem alerta |
| HOT | 70-90°C | Beep 1s, LED piscando |
| DANGER | 90-110°C | Beep 500ms, LED rápido |
| CRITICAL | > 110°C | Beep 200ms, LED fixo |

## Compilação

```bash
pio run              # Compila
pio run --target upload    # Upload
pio run --target monitor   # Monitor serial
```

## Uso de Memória

- Flash: 96.1% (31,002 bytes)
- RAM: 73.9% (1,513 bytes)

## Novas Funções Exportadas

```cpp
// Histórico
void addToHistory(const char* name, float value, float temp, bool isGood);

// EEPROM
void loadSettings();
void saveSettings();

// Estatísticas
extern Settings deviceSettings;
extern MeasurementHistory measurementHistory[HISTORY_SIZE];
```

## Estruturas de Dados

```cpp
struct MeasurementHistory {
  char name[MAX_MEASUREMENT_NAME];  // "Cap", "Res", etc.
  float value;                      // Valor medido
  float temp;                        // Temperatura no momento
  bool isGood;                       // true=verde, false=vermelho
};

struct Settings {
  float offset1, offset2;           // Calibração
  bool darkMode, silentMode;        // Modos
  unsigned long timeoutDuration;    // Timeout em ms
  unsigned long totalMeasurements;  // Total de medições
  unsigned long faultyMeasurements; // Componentes ruins
};
```

## Autores

- **Leandro** - Desenvolvedor original
- **v2.0** - Versão completa com todas funcionalidades