# Component Tester PRO v3.0 — CYD Edition

<p align="center">
  <img src="assets/hero.png" alt="Component Tester PRO v3.0 Hero" width="800">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32--2432S028R-yellow?style=for-the-badge&logo=espressif" alt="Platform">
  <img src="https://img.shields.io/badge/Framework-Arduino-00979D?style=for-the-badge&logo=arduino" alt="Framework">
  <img src="https://img.shields.io/badge/Version-3.0.0-blue?style=for-the-badge" alt="Version">
  <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge" alt="License">
</p>

---

## Visão Geral

O **Component Tester PRO v3.0** é a evolução definitiva em diagnóstico de componentes eletrônicos. Projetado para a plataforma **Cheap Yellow Display (CYD)**, este dispositivo transforma um ESP32 em uma ferramenta de bancada profissional, combinando medições precisas com uma interface visual "Cyber Dark" premium e controle total via touchscreen.

> **Por que v3.0?** Saímos da limitação do ATmega328P para o poder do Dual Core 240MHz. Agora temos processamento paralelo para medições True RMS, busca instantânea em banco de dados de milhares de componentes e uma interface gráfica fluida.

---

## Recursos de Elite

| Categoria | Funcionalidade | Vantagem |
|:---|:---|:---|
| **Medição** | 13 modos incluindo Auto-detect | Identifica Transistores, MOSFETs, Diodos e mais. |
| **Inteligência** | Database em tempo real | Identifica o Part Number exato (ex: BC547) via SD Card. |
| **Multímetro** | AC True RMS & DC Precision | Monitora rede elétrica e consumo de dispositivos DC. |
| **Térmico** | Sonda DS18B20 | Monitoramento de temperatura com alertas visuais RGB. |
| **Interface** | Touch UI 2.8" | Navegação intuitiva sem botões físicos necessários. |
| **Registro** | Datalogger SD | Salva histórico de medições para relatórios técnicos. |

---

## Hardware Suportado

### Placa Base: ESP32-2432S028R (Cheap Yellow Display)

- **Processador:** ESP32-WROOM-32 Dual Core 240MHz
- **RAM:** 520KB
- **Flash:** 4MB
- **Display:** TFT 2.8" ILI9341 320x240
- **Touchscreen:** XPT2046 Resistivo
- **Slot MicroSD:** Integrado
- **LED RGB:** Integrado (GPIO 4=R, 16=G, 17=B)
- **Speaker:** PAM8002 (GPIO 26)

### Sensores Externos Suportados

- **ZMPT101B:** Sensor de tensão AC (True RMS)
- **INA219:** Sensor de corrente/tensão DC (I2C)
- **DS18B20:** Sonda térmica (OneWire)

---

## Pinagem Obrigatória

```
╔══════════════════╦════════════════╦════════════════════════════╗
║   Função        ║     Pino       ║       Descrição          ║
╠══════════════════╬════════════════╬════════════════════════════╣
║ TFT_CS         ║ GPIO 15       ║ Chip Select do Display   ║
║ TFT_DC         ║ GPIO 2        ║ Data/Command do TFT    ║
║ TFT_SCK        ║ GPIO 14       ║ Clock SPI do Display   ║
║ TFT_MOSI       ║ GPIO 13       ║ MOSI do Display      ║
║ TFT_MISO      ║ GPIO 12       ║ MISO do Display      ║
║ TFT_Backlight ║ GPIO 21       ║ Backlight PWM        ║
╠══════════════════╬════════════════╬════════════════════════════╣
║ SD_CS          ║ GPIO 5        ║ Chip Select SD       ║
╠══════════════════╬��═══════════════╬════════════════════════════╣
║ Touch_CS       ║ GPIO 33       ║ Chip Select Touch    ║
║ Touch_SCK     ║ GPIO 25       ║ Clock Touch        ║
║ Touch_MOSI    ║ GPIO 32       ║ MOSI Touch         ║
║ Touch_MISO    ║ GPIO 39       ║ MISO Touch         ║
╠══════════════════╬════════════════╬════════════════════════════╣
║ Probe_1 (+)    ║ GPIO 35       ║ Probe positivo      ║
║ Probe_2 (-)    ║ GPIO 22       ║ Probe negativo     ║
╠══════════════════╬════════════════╬════════════════════════════╣
║ ZMPT101B       ║ GPIO 34       ║ Tensão AC          ║
╠══════════════════╬════════════════╬════════════════════════════╣
║ DS18B20       ║ GPIO 4        ║ OneWire           ║
╠══════════════════╬════════════════╬════════════════════════════╣
║ Buzzer/Speaker ║ GPIO 26       ║ Saída de áudio     ║
╠══════════════════╬════════════════╬════════════════════════════╣
║ LED Verde      ║ GPIO 16       ║ LED RGB           ║
║ LED Vermelho   ║ GPIO 17       ║ LED RGB           ║
╠══════════════════╬════════════════╬════════════════════════════╣
║ INA219_SDA   ║ GPIO 32       ║ I2C SDA          ║
║ INA219_SCL   ║ GPIO 33       ║ I2C SCL          ║
╚══════════════════╩════════════════╩════════════════════════════════════╝
```

> **Nota:** A CYD tem muitos pinos já destinados aos periféricos integrados. Os pinos livres disponíveis para expansão são mainly GPIO 22, 27 e 35 (input only).

---

## Instalação

### 1. Pré-requisitos

- [VS Code](https://code.visualstudio.com/)
- [PlatformIO](https://platformio.org/) (extensão)
- MicroSD formatado em FAT32

### 2. Preparação do SD Card

1. Formate o cartão em **FAT32** (não exFAT!)
2. Copie `sd_files/COMPBD.CSV` para a raiz do cartão

### 3. Compilação e Upload

```bash
# Clone o projeto
git clone https://github.com/lelebrr/Component_Tester.git

# Entre no diretório
cd Component_Tester

# Compile
pio run -e cyd

# Upload via USB
pio run -e cyd --target upload

# Ou upload via OTA (após o primeiro flash)
pio device monitor --onitor-speed 115200
```

### 4. Conexão dos Sensores

Siga o diagrama de pinagem acima. Os sensores externos (ZMPT101B, INA219, DS18B20) são opcionais - o tester funciona com as medições básicas mesmo sem eles.

---

## Uso

### Tela Inicial (Splash)

O dispositivo mostra uma tela de booting animada com:
- Versão do firmware
- Status do SD Card
- Carregamento do banco de dados
- Inicialização dos sensores

### Menu Principal

Navegação em grid 2x3:
- **Medir:** Teste de componentes (transistores, diodos, capacitores, etc.)
- **Térmica:** Temperatura via DS18B20
- **Scanner:** Auto-detecção de componentes
- **Multímetro:** AC/DC (ZMPT101B + INA219)
- **Histórico:** Últimas medições
- **Estatísticas:** Uso do dispositivo
- **Ajustes:** Configura��ões

### Multímetro AC/DC

O modo multímetro oferece:
1. **Tensão AC (True RMS):** Via ZMPT101B
2. **Tensão DC:** Via INA219
3. **Corrente DC:** Via INA219
4. **Potência DC:** Via INA219

---

## Documentação

- [Hardware & Montagem](docs/HARDWARE.md) - Esquemas e diagrama de ligação
- [Manual do Usuário](docs/MANUAL.md) - Como operar cada modo
- [Configuração](docs/CONFIG.md) - Personalização do firmware
- [FAQ & Suporte](docs/FAQ.md) - Resolução de problemas

---

## Troubleshooting

### Problemas Comuns

**"SD Card não encontrado"**
- Verifique se o cartão está formatado em FAT32
- Certifique-se que COMPBD.CSV está na raiz

**"INA219 N/C"**
- Verifique conexões I2C (SDA→GPIO 32, SCL→GPIO 33)
- Use pull-ups de 4.7kΩ se necessário

**"ZMPT101B sem resposta"**
- Verifique a calibração em Ajustes
- Ajuste o fator de escala conforme multímetro de referência

**Touchscreen não responde**
- Use os botões físicos como fallback
- Recalibre o touchscreen em Ajustes

---

## Licença

MIT License - Livre para uso, modificação e distribuição.

---

<p align="center">
Feito com ❤️ por <b>Leandro</b> | Component Tester PRO Team
</p>