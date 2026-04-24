# Changelog — Component Tester PRO v3.0

Este documento apresenta o históricos de todas as versões do Component Tester PRO, incluindo novas funcionalidades, correções de bugs e outras mudanças importantes.

---

## Formato das Entradas

Cada entrada segue o formato:

```markdown
## [Versão] — Data

### Adicionado
### Alterado
### Corrigido
### Removido
### BREAKING CHANGES
```

---

## [3.0.0] — Abril 2026

### Adicionado
- **Interface CYD completa** — Primeira versão estável para ESP32-2432S028R (Cheap Yellow Display)
- **13 modos de medição** — Suporte para teste de componentes:
  - Resistor
  - Capacitor
  - Diodo
  - LED
  - Zener
  - Transistor NPN
  - Transistor PNP
  - MOSFET N-Channel
  - MOSFET P-Channel
  - Tiristor (SCR)
  - Triac
  - DIAC
  - Indutor
- **Modo Auto-detecção** —Scanner automático de componentes
- **Multímetro AC/DC** — Medição de tensão AC (via ZMPT101B) e DC (via INA219)
- **Sensor de temperatura** — Suporte para sonda DS18B20 via OneWire
- **Interface touchscreen** —Display TFT 2.8" ILI9341 com touch resistivo XPT2046
- **Banco de dados SQLite-like** — Arquivo COMPBD.CSV no cartão SD
- **Datalogger** — Registro de medições com timestamp no SD Card
- **Visualização de histórico** — Últimas medições salvas
- **Sistema de estatísticas** — Uso do dispositivo
- **Calibração** — Offset e ganho ajusteis para cada modo
- **Interface Cyber Dark** — Tema visual premium
- **Suporte a múltiplos probes** — Probe positivo e negativo

### Alterado
- **Migração de ATmega328P para ESP32** — Grande refatoração da base de código
- **Novo sistema de menus** — Navegação grid 2x3via touchscreen
- **Display maior** — TFT 2.8" (320x240) substituindo display 16x2 ou 20x4
- **Armazenamento** — SD Card substituindo EEPROM interna
- **Processamento** — Dual Core 240MHz vs 16MHz single core

### Corrigido
- Limitações de memória do ATmega328P
- Precisão limitada de medição
- Interface de usuário limitada
- Banco de dados pequeno

### BREAKING CHANGES
- Pinagem completamente alterada para ESP32
- Banco de dados agora em CSV no SD Card (não mais hardcoded)
- Require formulário SD Card para operação completa
- Sensores externos (ZMPT101B, INA219, DS18B20) são opcionais

---

## [2.0.x] — Versões Anteriores (Base ATmega328P)

*Versões anteriores baseadas em ATmega328P (Uno/Nano). Histórico completo disponível no branch `legacy`.*

---

## [1.0.0] — Versão Original

*Versão inicial do projeto com funcionalidades básicas de teste de componentes.*

---

## Roapmap de Versões Futuras

Consulte o arquivo [ROADMAP.md](ROADMAP.md) para informações sobre versões futuras.

---

## Como Ler o Changelog

### Convenções de Semântica

| Símbolo | Significado |
|:---|:---|
| `[+]` | Nova funcionalidade |
| `[~]` | Alteração existente |
| `[-]` | Funcionalidade removida |
| `[!]` | Mudança incompatível (Breaking Change) |
| `[x]` | Correção de bug |

### Tipos de Mudanças

- **Adicionado**: Novas funcionalidades
- **Alterado**: Funcionalidades existentes modificadas
- **Corrigido**: Bugs e problemas resolvidos
- **Removido**: Funcionalidades removidas
- **BREAKING CHANGES**: Mudanças que quebram compatibilidade versões anteriores

---

## Contribuindo para o Changelog

Para adicionar uma entrada ao changelog:

1. Abra uma Issue com a tag `changelog`
2. Descreva a mudança realizada
3. Maintainers adicionarão à próxima versão

Ou envie um Pull Request diretamente editando este arquivo.

---

## Histórico

| Versão | Data | Status |
|:---|:---|:---|
| 3.0.0 | Abril 2026 | ✅ Estável |
| 2.x | Antes 2026 | ⚠️ Legado |
| 1.0.0 | Início | ⚠️ Legado |

---

<p align="center">
<i>Última atualização: Abril de 2026 — Component Tester PRO Team</i>
</p>