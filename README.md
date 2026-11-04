# 💡 Automação Inteligente da Iluminação – Clínica de Oftalmologia

## 📌 Sobre o projeto

Este projeto extensionista tem como objetivo desenvolver uma solução de automação para o controle da iluminação de três salas de uma clínica de oftalmologia, utilizando o microcontrolador **ESP32 e sensores de presença por detecção de passagem**.

A proposta consiste em monitorar a entrada e a saída de pessoas em cada ambiente, mantendo a iluminação ligada enquanto houver alguém na sala e desligando-a automaticamente quando não houver mais ocupantes.

## 🎯 Problema identificado

A iluminação de ambientes que não estão sendo utilizados pode gerar desperdício de energia elétrica, especialmente quando o acionamento e o desligamento das lâmpadas dependem exclusivamente da intervenção humana.

A solução proposta busca reduzir esse desperdício, proporcionando maior eficiência energética, praticidade e automatização da rotina da clínica.

## ⚙️ Como funciona a solução

O ESP32 recebe as informações dos sensores instalados nos acessos dos ambientes e identifica a sequência de passagem das pessoas.

A partir dessa identificação, o sistema realiza a contagem de entradas e saídas, atualizando a quantidade de pessoas presentes em cada sala.

- **Entrada:** a contagem de pessoas é incrementada e a iluminação é acionada.
- **Saída:** a contagem é decrementada.
- **Ambiente vazio:** quando a contagem chega a zero, a iluminação é desligada automaticamente.
- **Ambiente ocupado:** enquanto houver pelo menos uma pessoa, a iluminação permanece ligada.

## 🔌 Tecnologias e componentes

- **ESP32:** microcontrolador responsável pelo processamento das informações e controle do sistema.
- **Sensores ultrassônicos HC-SR04:** utilizados para identificar a sequência de passagem das pessoas.
- **Módulo relé:** representa o acionamento do circuito de iluminação.
- **LED:** utilizado para simular a lâmpada do ambiente.
- **Display OLED:** permite apresentar informações do funcionamento do sistema.
- **Buzzer:** componente auxiliar para sinalização.

## 🛠️ Desenvolvimento

O projeto envolve a elaboração do circuito eletrônico, programação do ESP32, integração dos sensores e desenvolvimento da lógica de controle da iluminação.

A montagem e os testes do protótipo são realizados em ambiente de simulação, utilizando o **Cirkit Designer**, permitindo avaliar o funcionamento dos componentes e realizar ajustes antes de uma possível implementação física.

## 🌱 Benefícios esperados

- Redução do consumo desnecessário de energia elétrica.
- Automatização do controle de iluminação.
- Maior praticidade para os profissionais da clínica.
- Melhor aproveitamento dos recursos disponíveis.
- Possibilidade de expansão da solução para outros ambientes.

## 👥 Equipe do projeto

Projeto desenvolvido por:

- Ailton
- Leonardo
- Elise Malvão Carlson
- Letícia
- Lucas
- Pedro

**Projeto Extensionista – Sistemas Microcontrolados**  
Curso de Ciência da Computação – Universidade Estácio de Sá - 2026.2
