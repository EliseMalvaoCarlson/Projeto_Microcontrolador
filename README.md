# 💡 Automação Inteligente de Iluminação e Conforto Ambiental

## 📌 Sobre o projeto

Este projeto extensionista tem como objetivo desenvolver um sistema de automação utilizando o microcontrolador **ESP32**, sensores e componentes eletrônicos para controlar a iluminação de três ambientes de uma clínica de oftalmologia.

A solução identifica a entrada e a saída de pessoas em cada ambiente, mantendo as luzes acesas enquanto houver ocupantes e desligando-as automaticamente quando o ambiente estiver vazio.

Além do controle de iluminação, o projeto prevê uma funcionalidade específica para o banheiro: o acionamento automático de um aromatizador assim que a iluminação for desligada, contribuindo para manter o ambiente agradável para a próxima pessoa que for utilizá-lo.

## 🎯 Problema identificado

O acionamento manual da iluminação pode ocasionar desperdício de energia elétrica, principalmente quando as luzes permanecem acesas em ambientes desocupados.

Além disso, em ambientes de uso compartilhado, como banheiros, é possível implementar recursos adicionais que contribuam para o conforto e a qualidade da experiência dos usuários.

A proposta busca solucionar essas questões por meio da automação, combinando eficiência energética, praticidade e conforto ambiental.

## ⚙️ Como funciona a solução

O ESP32 recebe as informações dos sensores instalados nos acessos dos ambientes e realiza a contagem de pessoas, identificando entradas e saídas.

A lógica de funcionamento é:

- **Entrada de pessoa:** a contagem é incrementada e a iluminação é acionada.
- **Saída de pessoa:** a contagem é decrementada.
- **Ambiente ocupado:** enquanto houver pelo menos uma pessoa, a iluminação permanece ligada.
- **Ambiente vazio:** quando a contagem chega a zero, a iluminação é desligada.
- **Funcionalidade do banheiro:** após o desligamento da iluminação, o sistema aciona o dispositivo responsável pela aromatização do ambiente, preparando-o para o próximo usuário.

Na simulação do protótipo, um buzzer representa o acionamento do aromatizador, permitindo demonstrar essa funcionalidade sem a necessidade de utilizar o equipamento real.

## 🔌 Tecnologias e componentes

- **ESP32:** microcontrolador responsável pelo processamento das informações e controle do sistema.
- **Sensores ultrassônicos HC-SR04:** utilizados para identificar a passagem de pessoas.
- **Módulo relé:** utilizado para representar o acionamento de dispositivos externos.
- **LEDs:** utilizados para simular a iluminação dos ambientes.
- **Buzzer:** utilizado na simulação para representar o acionamento do aromatizador do banheiro.
- **Display OLED:** utilizado para apresentar informações do funcionamento do sistema.

## 🛠️ Desenvolvimento e simulação

O desenvolvimento envolve a montagem do circuito eletrônico, programação do ESP32, integração dos sensores e implementação da lógica de controle.

A simulação do protótipo é realizada no **Cirkit Designer**, permitindo testar o comportamento dos componentes e validar a lógica de funcionamento antes de uma possível implementação física.

## 🌱 Benefícios e possibilidades de aplicação

- Redução do consumo desnecessário de energia elétrica.
- Automatização do controle de iluminação.
- Maior praticidade para os usuários.
- Melhoria do conforto e da experiência de utilização dos ambientes.
- Aromatização automática do banheiro após seu uso.
- Possibilidade de adaptação para residências, consultórios, clínicas, escritórios e outras empresas.

Embora o projeto tenha sido inicialmente pensado para uma clínica de oftalmologia, sua aplicação não se limita a esse ambiente. A solução pode ser expandida para diferentes espaços residenciais e comerciais, mediante pequenas adaptações na programação e na configuração dos componentes, conforme as necessidades de cada local.

## 👥 Equipe do projeto

Projeto desenvolvido por:

- Ailton
- Leonardo
- Elise Malvão Carlson
- Letícia
- Lucas
- Pedro

**Projeto Extensionista – Sistemas Microcontrolados**  
Curso de Ciência da Computação – Universidade Estácio de Sá.
