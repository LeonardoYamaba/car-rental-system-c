<div align="center">

# 🚗 Locadora de Carros em C

**Um sistema de aluguel de carros feito no terminal, com menu, struct e dados salvos em arquivo.**

![Linguagem](https://img.shields.io/badge/linguagem-C-blue?style=for-the-badge&logo=c)
![Tipo](https://img.shields.io/badge/tipo-projeto%20acad%C3%AAmico-green?style=for-the-badge)
![Interface](https://img.shields.io/badge/interface-terminal-lightgrey?style=for-the-badge)

</div>

---

## 💡 A ideia

Uma locadora simples: o programa tem uma lista de carros e o cliente pode **ver os disponíveis, alugar e devolver**. Também dá para **cadastrar novos carros**, e tudo fica **salvo em arquivo**, então os dados não se perdem quando o programa fecha.

O objetivo é praticar os conceitos de lógica de programação em C, de um jeito organizado e fácil de entender.

---

## ✨ Funcionalidades

| Opção | O que faz |
|:-----:|-----------|
| **1** | Mostra apenas os carros disponíveis |
| **2** | Mostra todos os carros (com quem alugou e por quantos dias) |
| **3** | Aluga um carro: pede ID, nome do cliente e dias |
| **4** | Devolve um carro, calcula multa se houve atraso e mostra o total |
| **5** | Cadastra um novo carro (modelo e valor da diária) |
| **0** | Sai do programa |

> 🏷️ **Desconto:** aluguéis de **7 dias ou mais** ganham **10% de desconto**.
>
> ⏰ **Multa:** cada dia de atraso custa a diária **+ 50%**.
>
> 💾 **Dados salvos:** tudo é gravado no arquivo `carros.dat`, criado automaticamente na primeira execução.

---

## 🖥️ Como fica no terminal

```text
=============================
      LOCADORA DE CARROS
=============================
 1 - Ver carros disponiveis
 2 - Ver todos os carros
 3 - Alugar um carro
 4 - Devolver um carro
 5 - Cadastrar novo carro
 0 - Sair
-----------------------------
Escolha uma opcao: 2

 ID   Modelo             Valor/dia   Status
 1    Fiat Mobi          R$ 90.00    Disponivel
 2    Chevrolet Onix     R$ 110.00   Alugado (Maria Silva, 5 dias)
 3    Hyundai HB20       R$ 115.00   Disponivel
 ...
```

---

## 🧠 Conceitos praticados

| Conceito | Onde aparece |
|----------|--------------|
| **Entrada e saída** | `printf`, `scanf` e `fgets` |
| **Condicionais** | `if / else` e `switch` (menu e validações) |
| **Laços** | `do-while` (menu) e `for` (percorrer os carros) |
| **Struct** | `Carro` agrupa modelo, diária, status, cliente e dias |
| **Vetores** | vetor de `Carro` com até 50 carros |
| **Funções** | `contarDisponiveis()`, `obterIndice()` e `carregarDados()` retornam valores |
| **Procedimentos** | `exibirMenu()`, `listarCarros()` e `salvarDados()` apenas executam ações |
| **Ponteiros** | `*total`, `*multa`, `*destino` e `Carro *c` (acesso com `->`) |
| **Arquivos** | `fopen`, `fread`, `fwrite` e `fclose` (arquivo binário) |
| **Strings** | `strcpy`, `strlen` e `strcspn` |

---

## 📁 Estrutura

```text
locadora-c/
├── main.c       # código do projeto
├── README.md    # este arquivo
└── carros.dat   # criado sozinho ao rodar o programa
```

---

## ▶️ Como rodar

Você precisa do compilador **GCC** instalado.

```bash
# 1. Compile
gcc main.c -o locadora

# 2. Execute
./locadora          # Linux / Mac
locadora.exe        # Windows
```

> 🔄 Para voltar ao estado inicial (carros de exemplo), apague o arquivo `carros.dat`.

---


<div align="center">

Feito por **Leonardo Yamaba Baeta** 💻

</div>
