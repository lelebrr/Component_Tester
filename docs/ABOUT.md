# Sobre o Sondvolt — História e Objetivo

---

## História do Projeto

O **Sondvolt** nasceu da necessidade de criar uma ferramenta de teste de componentes acessível, porém profissional, para makers e técnicos em eletrônica. O projeto começou como um protótipo simples utilizando o popular Cheap Yellow Display (ESP32-2432S028R), uma placa de desenvolvimento de baixo custo que combina um display TFT touchscreen com um microcontrolador ESP32 poderoso.

O nome "Sondvolt" surgiu da fusão de duas palavras essenciais no universo da eletrônica: **"Sond"** (do inglês *probe*, referring to test probes) e **"Volt"** (unidade de tensão). Este nome reflete a missão fundamental do equipamento: conectar-se aos componentes através das pontas de prova para revelar suas características elétricas.

### Evolução

| Versão | Marco |
|:-------|:------|
| v1.0 | Protótipo inicial com teste básico de resistores |
| v2.0 | Adição de capacitores, diodos e transistores |
| v2.5 | Integração do multímetro AC/DC |
| v3.0 | Sistema completo com segurança, MicroSD e interface profissional |
| **v3.1** | **Versão atual** — Otimizações de interface e estabilidade |

O projeto evoluiu de um testador simples para um verdadeiro laboratorio portátil de eletrônica, capable de testar mais de 12 tipos de componentes differentes e funcionar como multímetro True RMS completo.

---

## Objetivo do Equipamento

### Missão

> **"Proporcionar aos makers, técnicos e entusiastas uma ferramenta profissional de teste de componentes e medições elétricas, acessível e de código aberto."**

### Objetivos Específicos

1. **Identificação Automática**: Detectar e caracterizar componentes eletrônicos sem necessidade de conhecimento prévio das especificações

2. **Medições Precisas**: Fornecer leituras confiáveis de tensão, corrente, resistência e capacitância com precisão de ±2%

3. **Segurança em Primeiro Lugar**: Proteger o usuário e o equipamento através de bloqueios automáticos e alertas em medições de alta tensão

4. **Acessibilidade**: Manter o projetoopen source, com custos de hardware mínimos, para democratizar o acesso à ferramenta

5. **Portabilidade**: Oferecer um equipamento compacto e autossuficiente que funcione como laboratório de eletrônica portátil

---

## Diferenciais em Relação a Outros Testadores

### Comparativo com Opções do Mercado

| Característica | Sondvolt | LCR-T4/T7 | Multímetro Tradicional |
|:---------------|:---------|:----------|:----------------------|
| **Teste de componentes** | ✓ Sim (>12 tipos) | ✓ Sim | ✗ Não |
| **Multímetro True RMS** | ✓ Sim | ✗ Não | ✓ Sim (profissional) |
| **Display touchscreen** | ✓ Sim | ✓ Sim | ✗ Não |
| **Banco de dados personalizável** | ✓ CSV no MicroSD | ✗ Limitado | ✗ Não |
| **Histórico de medições** | ✓ Com data/hora | ✗ Não | ✗ Não |
| **Código aberto** | ✓ Sim | ✗ Não | ✗ Não |
| **Custo** | ~R$150-200 | ~R$200-300 | ~R$100-500 |
| **Medição 220V** | ✓ Com segurança | ✗ Não | ✓ Sim |

### Vantagens Competitivas

1. **Versatilidade Incomparável**
   - Único equipamento que combina testador de componentes E multímetro True RMS
   - Não há necessidade de dois equipamentos distintos

2. **Personalização Total**
   - Banco de dados em CSV permite adicionar componentes customizados
   - Código aberto para modificações e melhorias

3. **Interface Profissional**
   - Display touchscreen intuitivo
   - Visual moderno com ícones vetoriais e animações

4. **Registro de Dados**
   - Histórico automático com timestamp
   - Ideal para documentação e análise de medições

5. **Segurança Avançada**
   - Bloqueio automático para tensões perigosas
   - Confirmação obrigatória para medições de 220V
   - Alertas visuais e sonoros

---

## Especificações Diferenciadas

| Recurso | Detalhe |
|:--------|:--------|
| **True RMS Real** | Usa transformador ZMPT101B para medições AC precisas |
| **Corrente Bidirecional** | Sensor INA219 mede corrente em ambas as direções |
| **Calibração Automática** | Sistema compensa tolerâncias das pontas de prova |
| **Atualização de Banco de Dados** | COMPBD.CSV editável diretamente no MicroSD |
| **Log de Operações** | Cada medição é registrada com data, hora e status |

---

## Créditos e Agradecimentos

### Desenvolvedores

- **Equipe Sondvolt** — Idealização, desenvolvimento e manutenção do projeto

### Bibliotecas e Recursos

| Recurso | Contribuidor | Descrição |
|:--------|:-------------|:----------|
| **TFT_eSPI** | Bodmer | Biblioteca para display TFT no ESP32 |
| **ESP32 Arduino Core** | Espressif | Framework oficial do ESP32 |
| **Adafruit INA219** | Adafruit | Biblioteca para sensor de corrente |

### Inspiração e Referências

- **Thread do Cheap Yellow Display** no ESP32 Forum
- **Projetos de component testers** no Hackaday e GitHub
- **Comunidade Arduino/ESP32** pelo suporte contínuo

### Agradecimentos Especiais

A todos os makers que contribuíram com:
- Relatórios de bugs e sugestões
- Testes de hardware e validação
- Documentação e tutoriais
- Compartilhamento de experiências

---

## Contribua com o Projeto

O Sondvolt é um projeto **open source**. Você pode contribuir de várias formas:

- 🐛 Reportando bugs
- 💡 Sugerindo novas funcionalidades
- 📝 Melhorando a documentação
- 🔧 Enviando Pull Requests
- 📢 Compartilhando seu projeto

Visite o repositório para mais informações sobre como contribuir.

---

## Versão

**Sondvolt v3.1 (Component Tester Pro)**

- Firmware: v3.1
- Hardware: ESP32-2432S028R (Cheap Yellow Display)
- Última atualização: Abril/2026

---

<p align="center">
<i>"Teste. Meça. Confirme."</i>
</p>