# Component Tester PRO v2.0

![Component Tester](https://img.shields.io/badge/Component_Tester-PRO_v2.0-blue)
![Arduino](https://img.shields.io/badge/Arduino-Uno_R3-green)
![Display](https://img.shields.io/badge/TFT-2.8%22_ILI9341-orange)

## O Que É Este Dispositivo?

O **Component Tester PRO v2.0** é um dispositivo eletrônico projetado para testar e identificar componentes eletrônicos diversos. Ele é construído usando uma placa Arduino Uno R3 e possui uma tela TFT colorida de 2.8 polegadas para interface com o usuário.

### Para Que Serve?

Este testador permite que você:
- ✅ Medir capacitores (incluindo ESR)
- ✅ Medir resistores
- ✅ Testar diodos e LEDs (incluindo identificação de polaridade)
- ✅ Identificar transistores BJT NPN
- ✅ Medir indutores
- ✅ Medir tensão DC (0-5V)
- ✅ Contar frequência (1Hz - 1MHz)
- ✅ Gerar sinal PWM (1kHz 50%)
- ✅ Testar optocouplers
- ✅ Testar continuidade de cabos
- ✅ Testar retificadores em ponte
- ✅ Detecção automática de componentes
- ✅ Teste de continuidade com beep
- ✅ Monitor de temperatura (sonda DS18B20)

---

## Quick Start (Início Rápido)

### Monte o Hardware

Consulte o diagrama de conexões em [HARDWARE.md](HARDWARE.md) e [PINOUT.md](PINOUT.md).

### Carregue o Firmware

```bash
# Conecte o Arduino ao PC via USB
pio run --target upload
```

### Como Usar

1. **Ligue o dispositivo** - A splash screen aparecerá
2. **Use UP/DW** para navegar no menu principal
3. **Pressione OK** para selecionar
4. **Pressione BCK** para voltar

Consulte o [MANUAL.md](MANUAL.md) para instruções detalhadas.

---

## Estrutura da Documentação

| Arquivo | Descrição |
|--------|-----------|
| [README.md](README.md) | Este arquivo - Visão geral |
| [MANUAL.md](MANUAL.md) | Manual do usuário completo |
| [GUIDES.md](GUIDES.md) | Guias passo a passo para cada função |
| [HARDWARE.md](HARDWARE.md) | Especificações de hardware |
| [PINOUT.md](PINOUT.md) | Pinagem detalhada com diagramas |
| [MENUS.md](MENUS.md) | Descrição completos dos menus |
| [COMPONENTS.md](COMPONENTS.md) | Como testar cada componente |
| [CONFIG.md](CONFIG.md) | Configurações do dispositivo |
| [TROUBLESHOOTING.md](TROUBLESHOOTING.md) | Solução de problemas |
| [FAQ.md](FAQ.md) | Perguntas frequentes |
| [DEVELOP.md](DEVELOP.md) | Informações para desenvolvedores |

---

## Menu Principal

```
┌─────────────────────────┐
│ ► Medir                 │
│   Termica               │
│   Scanner               │
│   Historico            │
│   Estatist             │
│   Config               │
│   Sobre                │
└─────────────────────────┘
      UP/DW | OK | BCK
```

### Funções do Menu

| Opção | Função |
|-------|--------|
| **Medir** | Menu com 13 tipos de medições |
| **Termica** | Monitor de temperatura com sonda DS18B20 |
| **Scanner** | Auto-detecção contínua a cada 2 segundos |
| **Historico** | Últimas 10 medições |
| **Estatist** | Estatísticas (total, ruins, taxa falha) |
| **Config** | Calibração e configurações |
| **Sobre** | Informações do dispositivo |

---

## Especificações Técnicas

| Especificação | Valor |
|---------------|-------|
| Microcontrolador | Arduino Uno R3 (ATmega328P) |
| Display | TFT 2.8" ILI9341 SPI |
| Resolução | 320x240 pixels |
| Cores | 65k cores |
| Memória Flash | 31,002 bytes (96.1%) |
| Memória RAM | 1,513 bytes (73.9%) |
| Sonda Térmica | DS18B20 waterproof |
| Armazenamento | SD Card + EEPROM |

---

## Modos de Medição (Badge)

|Badge|Descrição|
|-----|---------|
|![Cap](https://img.shields.io/badge/Capacitor-uF-blue)|Capacitor + ESR|
|![Res](https://img.shields.io/badge/Resistor-Ohm-green)|Resistor|
|![Diod](https://img.shields.io/badge/Diodo-LED-yellow)|Diodo/LED|
|![Trans](https://img.shields.io/badge/Transistor-NPN-red)|Transistor BJT|
|![Ind](https://img.shields.io/badge/Indutor-mH-orange)|Indutor|
|![Volt](https://img.shields.io/badge/Tensao-DC-purple)|Voltímetro DC|
|![Freq](https://img.shields.io/badge/Frequencia-Hz-cyan)|Frequencímetro|
|![PWM](https://img.shields.io/badge/PWM-1kHz-pink)|Gerador PWM|
|![Opto](https://img.shields.io/badge/Optocoupler-teal)|Optocoupler|
|![Cable](https://img.shields.io/badge/Cabo-Continuidade-lime)|Continuidade|
|![Bridge](https://img.shields.io/badge/Bridge-Red-green)|Ponte de Diodos|
|![Auto](https://img.shields.io/badge/Auto-Detectar-gray)|Detecção Auto|

---

## Alertas de Temperatura

| Status | Temperatura | Comportamento |
|-------|-------------|--------------|
| 🔵 Normal | < 70°C | Sem alerta |
| 🟡 HOT | 70-90°C | Beep 1s, LED piscando |
| 🟠 DANGER | 90-110°C | Beep 500ms, LED rápido |
| 🔴 CRITICAL | > 110°C | Beep 200ms, LED fixo |

---

## Limites de Temperatura

| Threshold | Valor | Comportamento |
|-----------|-------|---------------|
| NORMAL | < 70°C | Sem alerta |
| HOT | 70-90°C | Beep 1s, LED piscando |
| DANGER | 90-110°C | Beep 500ms, LED rápido |
| CRITICAL | > 110°C | Beep 200ms, LED fixo |

---

## Compilação e Upload

```bash
# Compilar
pio run

# Upload
pio run --target upload

# Monitor Serial
pio run --target monitor
```

---

## Autor

- **Leandro** - Desenvolvedor original
- **v2.0** - Versão completa com todas funcionalidades

---

## Licença

Este projeto é de código aberto para fins educativos.

---

## Links Úteis

- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [PlatformIO](https://platformio.org/)
- [Biblioteca Adafruit ILI9341](https://github.com/adafruit/Adafruit_ILI9341)
- [OneWire Library](https://www.pjrc.com/teensy/td_libs_OneWire.html)