# Component Tester PRO v3.0 — CYD Edition

<p align="center">
<img src="assets/hero.png" alt="Component Tester PRO v3.0" width="700">
</p>

<p align="center">
<a href="https://ESP32-2432S028R"><img src="https://img.shields.io/badge/Placa-ESP32--2432S028R-yellow?style=for-the-badge&logo=espressif" alt="Placa"></a>
<a href="https://arduino.cc"><img src="https://img.shields.io/badge/Framework-Arduino Framework-00979D?style=for-the-badge&logo=arduino" alt="Framework"></a>
<a href="https://platformio.org/"><img src="https://img.shields.io/badge/PlatformIO-L民主-FA7343?style=for-the-badge&logo=platformio" alt="PlatformIO"></a>
<a href="LICENSE"><img src="https://img.shields.io/badge/Licen├ºa-MIT-green?style=for-the-badge" alt="Licen├ºa"></a>
<a href="https://github.com/lelebrr/Component_Tester/releases"><img src="https://img.shields.io/badge/Vers├úo-3.0.0-blue?style=for-the-badge" alt="Vers├úo"></a>
</p>

---

## O que ├⌐ Este Projeto?

O **Component Tester PRO v3.0 ├⌐ uma ferramenta de diagn├│stico eletr├┬nico profissional**, constru├ªda sobre a placa **ESP32-2432S028R (Cheap Yellow Display)**. O dispositivo combina teste de componentes, mult├şmetro AC/DC True RMS e medi├º├úo de temperatura em uma ├║nica interface touchscreen.

> **Destaque:** Sa├¡mos do ATmega328P para o poder do ESP32 Dual Core 240MHz, oferecendo processamento paralelo para medi├º├úo True RMS, busca instant├ónea em banco de dados e interface gr├ífica fluida.

---

## Recursos Principais

| Recurso | Descri├º├úo |
|:---|:---|
| **Modo Auto-Detect** | Identifica automaticamente transistores, MOSFETs, diodos, capacitores, resistores e indutores |
| **Banco de Dados** | Arquivo COMPBD.CSV no MicroSD com milhares de componentes identificados |
| **Mult├şmetro AC** | Tens├úo de rede (110V/220V) via sensor ZMPT101B com precis├úo True RMS |
| **Mult├şmetro DC** | Tens├úo, corrente e pot├¬ncia via sensor INA219 (I2C) |
| **Sonda T├®rmica** | Temperatura via DS18B20 com alertas visuais |
| **Interface Touch** | Display TFT 2.8" com navega├º├úo t├útil sem bot├Áes f├¡sicos |
| **Datalogger** | Salva medi├º├Áes no cart├úo SD com timestamp |

---

## Especifica├º├Áes T├®cnicas

| Especifica├º├úo | Valor |
|:---|:---|
| **Microcontrolador** | ESP32-WROOM-32 Dual Core @ 240MHz |
| **Mem├│ria RAM** | 520KB |
| **Mem├│ria Flash** | 4MB |
| **Display** | TFT 2.8" ILI9341 320x240 |
| **Touchscreen** | XPT2046 Resistivo |
| **Slot MicroSD** | Integrado (at├® 32GB FAT32) |
| **Tens├úo AC M├íxima** | 250V RMS |
| **Tens├úo DC M├íxima** | 26V |
| **Corrente DC M├íxima** | ┬▒3.2A |
| **Faixa de Resistencia** | 0.1O a 10MO |
| **Faixa de Capacit├óncia** | 1pF a 10000uF |
| **Faixa de Indut├óncia** | 0.1mH a 1000mH |

---

## Sensores Externos Suportados

| Sensor | Fun├º├úo | Interface |
|:---|:---|:---|
| **ZMPT101B** | Tens├úo AC (True RMS) | GPIO 34 (ADC) |
| **INA219** | Tens├úo/Corrente DC | I2C (GPIO 27/22) |
| **DS18B20** | Temperatura | OneWire (GPIO 4) |

---

## Avisos de Seguran├ºa

> **PERIGO:** Este projeto envolve medi├º├Áes em tens├úo de rede el├®trica (110V/220V). Sempre desconecte a energia antes de manipular as conex├Áes dos probes.

> **ATENÇÃO:** Use luvas isolantes ao trabalhar com tens├Áes superiores a 50V. A seguran├ºa pessoal ├® responsabilidade do operador.

---

## Instala├º├úo R├ípida

### 1. Preparar o SD Card

1. Formate o cart├úo em **FAT32** (n├úo exFAT!)
2. Copie `sd_files/COMPBD.CSV` para a raiz do cart├úo

### 2. Compilar e Enviar

```bash
# Usando PlatformIO
pio run -e cyd
pio run -e cyd --target upload
```

### 3. Conectar Sensores

Siga o diagrama de pinagem em `docs/PINOUT.md` antes de energizar.

---

## Documenta├º├úo Completa

Para informasi detalhadas, consulte:

| Guia | Descri├º├úo |
|:---|:---|
| [docs/MANUAL.md](docs/MANUAL.md) | Manual do usu├ário completo |
| [docs/HARDWARE.md](docs/HARDWARE.md) | Esquemas e lista de materiais |
| [docs/PINOUT.md](docs/PINOUT.md) | Refer├¬ncia de pinagem |
| [docs/COMPONENTS.md](docs/COMPONENTS.md) | Componentes suportados |
| [docs/CONFIG.md](docs/CONFIG.md) | calibra├º├úo e configura├º├úo |
| [docs/FAQ.md](docs/FAQ.md) | Perguntas frequentes |

---

## Solu├º├úo de Problemas

| Problema | Solu├º├úo |
|:---|:---|
| SD Card n├úo detectado | Verifique formata├º├úo FAT32 e presence do COMPBD.CSV |
| Touchscreen n├úo responde | Use os bot├Áes f├¡sicos como fallback |
| Leitura AC incorreta | Recalibre o sensor ZMPT101B nos Ajustes |
| INA219 n├úo responde | Verifique conex├Áes I2C (SDA/SCL) |

---

## Licen├ºa

MIT License - Livre para uso, modifica├º├úo e distribui├º├úo.

---

<p align="center">
<b>Feito com ❤️ por Leandro</b> | Component Tester PRO Team
</p>