# Sondvolt — Component Tester Pro

<p align="center">
  <img src="assets/hero.png" alt="Sondvolt Component Tester" width="600">
</p>

<p align="center">
  <b>Teste. Meça. Confirme.</b><br>
  <i>Sistema profissional de teste de componentes eletrônicos e multímetro AC/DC True RMS</i>
</p>

---

## O Que é o Sondvolt?

O **Sondvolt** é um testador de componentes eletrônicos profissional e multímetro AC/DC True RMS, desenvolvido para makers, técnicos e entusiastas da eletrônica. Construído sobre a plataforma ESP32-2432S028R (Cheap Yellow Display), oferece uma solução completa e de baixo custo para identificação e teste de componentes.

---

## Características Principais

| Categoria | Recursos |
|:----------|:---------|
| **Teste de Componentes** | Resistor, capacitor, diodo, LED, transistor, MOSFET, indutor, cristal, fusível, varistor, potenciômetro, optoacoplador |
| **Multímetro** | Tensão DC (0-26V), Tensão AC True RMS (110V/220V), Corrente DC (até 3.2A), Resistência, Capacitância |
| **Segurança** | Bloqueio automático 220V, alertas sonoros e visuais, confirmação obrigatória para medições de alta tensão |
| **Armazenamento** | Banco de dados em MicroSD (COMPBD.CSV), histórico de medições com data/hora |
| **Interface** | Display TFT 320×240 touchscreen, design profissional com ícones vetoriais, animação de splash screen |

---

## Especificações Técnicas

| Parâmetro | Valor |
|:----------|:------|
| **Microcontrolador** | ESP32-WROOM-32 Dual Core @ 240MHz |
| **Display** | TFT 2.8" (320×240) ILI9341 com Touch Resistivo XPT2046 |
| **Memória** | 520KB SRAM / 4MB Flash SPI |
| **Tensão DC Máx** | 26V |
| **Tensão AC Máx** | 250V RMS |
| **Corrente DC Máx** | ±3.2A (INA219) |
| **Resistência** | 0.1Ω — 10MΩ |
| **Capacitância** | 1pF — 100μF |
| **Precisão** | ±2% (após calibração) |
| **Sensor AC** | ZMPT101B (Transformador de Tensão Isolado) |
| **Sensor DC** | INA219 (I2C Bi-Direcional) |

---

## Hardware

### Placa Base
- **ESP32-2432S028R** (Cheap Yellow Display)
- Display ILI9341 320×240 pixels
- Touchscreen XPT2046

### Sensores
- **ZMPT101B**: Transformador de tensão AC para medição 110V/220V
- **INA219**: Sensor de corrente DC via I2C
- ADC ESP32 para medições de resistência/capacitância

### Periféricos
- LEDs indicadores (verde/vermelho)
- Buzzer piezoelétrico
- Slot para cartão MicroSD

---

## Pinagem

| Função | Pino ESP32 | Descrição |
|:-------|:-----------|:----------|
| TFT MOSI | 13 | VSPI dados |
| TFT SCLK | 14 | VSPI clock |
| TFT CS | 15 | Chip select |
| TFT DC | 2 | Data/Command |
| TFT BL | 21 | Backlight |
| Touch CS | 33 | Touchscreen |
| SD CS | 5 | Cartão SD |
| ADC Resistor | 35 | Sonda 1 |
| ADC Capacitor | 34 | Sonda 2 |
| ZMPT AC | 36 | Sensor AC |
| LED Verde | 16 | Indicador OK |
| LED Vermelho | 17 | Alerta |
| Buzzer | 26 | Som |

---

## Instalação

### Pré-requisitos

```bash
# PlatformIO
pip install platformio
```

### Biblioteca TFT_eSPI

```bash
cd ~/Arduino/libraries
git clone https://github.com/Bodmer/TFT_eSPI.git
```

### Configuração User_Setup.h

- HSPI/Touch: CLK=25, MOSI=32, MISO=39, CS=33
- VSPI/TFT: CLK=14, MOSI=13, CS=15, DC=2
- SD Card: HSPI CLK=18, MOSI=23, MISO=19, CS=5
- Backlight: GPIO 21 (PWM)

### Upload

```bash
cd Sondvolt
pio run --upload
```

---

## Uso

### Menu Principal

1. **Componentes**: Testar componentes passivos e ativos
2. **Multímetro**: Medir tensão AC/DC, corrente, resistência
3. **Histórico**: Ver medições anteriores
4. **Calibração**: Calibrar pontas de prova
5. **Configurações**: Ajustes do sistema
6. **Sobre**: Informações do projeto

### Modo Componentes

Conecte o componente entre as pontas de prova e o sistema identificará automaticamente o tipo, exibindo valor, tolerância e condições de uso.

### Modo Multímetro

| Função | Faixa |
|:-------|:------|
| **VCC** | Tensão DC (0-26V) |
| **VAC** | Tensão AC True RMS (0-250V) |
| **A** | Corrente DC (±3.2A) |
| **Ω** | Resistência (0.1Ω - 10MΩ) |

### Segurança Elétrica

O sistema alertará e bloqueará medições se detectar:
- Tensões AC acima de 50V (alerta amarelo)
- Tensões AC acima de 180V (bloqueio vermelho)

> ⚠️ **CONFIRMAÇÃO OBRIGATÓRIA** para medir 220V AC — requer fusível e varistor instalados.

---

## Estrutura do Projeto

```
src/
├── pins.h           # Pinagem completa
├── config.h         # Configurações gerais
├── graphics.h/cpp   # Ícones e UI
├── multimeter.h/cpp # Multímetro AC/DC
├── safety.h/cpp     # Segurança elétrica
├── ui.h/cpp         # Telas e menus
├── globals.h        # Variáveis globais
├── database.h/cpp   # Banco de dados CSV
├── splash.cpp       # Splash screen
├── buzzer.h/cpp     # Som
├── main.cpp         # Programa principal
```

---

## Banco de Dados (COMPBD.CSV)

```csv
Código,Nome,Valor,Mín,Máx,Tolerância
R100,Resistor 100Ω,100,95,105,5%
R1K,Resistor 1KΩ,1000,950,1050,5%
R10K,Resistor 10KΩ,10000,9500,10500,5%
C100n,Capacitor 100nF,100,90,110,10%
```

---

## Histórico de Medições (HISTORY.CSV)

```csv
Timestamp;Data;Hora;Componente;Valor;Unidade;Status
1714051200;24/04/2025;14:00:00;Resistor 1K;987;ohm;OK
```

---

## Calibração

1. Acesse o modo **Calibração** no menu
2. Curto-circuite as pontas de prova (junte as pontas)
3. Aguarde o processo de calibração automática finalizar
4. O sistema salvara automaticamente os valores de offset

---

## Limitações

- ❌ Não mede componentes em circuito (PCB)
- ❌ Não mede corrente AC diretamente
- ⚠️ Requer proteção externa para medições em 220V

---

## Documentação

| Guia | Descrição |
|:-----|:----------|
| [Manual do Usuário](docs/MANUAL.md) | Instruções completas de operação |
| [Guia de Hardware](docs/HARDWARE.md) | Esquemas e diagramas de ligação |
| [Pinagem](docs/PINOUT.md) | Referência completa de GPIOs |
| [Componentes](docs/COMPONENTS.md) | Lista de componentes suportados |
| [Menus](docs/MENUS.md) | Estrutura de interface |
| [Configuração](docs/CONFIG.md) | Ajustes de firmware |
| [Desenvolvimento](docs/DEVELOP.md) | Arquitetura do código |

---

## Agradecimentos

- **Bodmer** — Biblioteca TFT_eSPI
- ** Comunidade ESP32** — Suporte e documentação
- ** Makers DIY** — Projetos inspiradores

---

## Licença

MIT License — Livre para uso, modificação e distribuição.

---

<p align="center">
<b>SONDVOLT</b> — Teste. Meça. Confirme.<br>
<i>Professional Electronics Testing</i>
</p>