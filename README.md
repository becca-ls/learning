# projetoIP
Projeto da disciplina de introdução a programação de 2019.2.
A proposta do projeto é fazer um jogo que dê para ser jogado de multiplos computadores ao mesmo tempo (um jogo multiplayer). Sendo assim será utilizada uma estrutura cliente/servidor para seu desenvolvimento.

## Jogo e objetivo
Por votação da equipe (e não havendo coerção social nenhuma) o jogo escolhido foi "O jogo do óleo" *(pf vamo achar um título mais legal)*.
A ideia veio perante tudo que está acontecendo no litoral do nosso nordeste. O jogo representa os voluntários colocando sua saúde em risco para que haja um bem estar ambiental diante de um total desinteresse governamental. Assim, o jogo conectará dois jogadores e esses vão trabalhar juntos (ou não) para que seja coletado a maior quantidade óleo possível num determinado período de tempo. O jogador que tiver retirado a maior quantidade de litros de óleo da praia será o "vencedor". **NÃO EXISTE VENCEDORES NA LUTA PELA SALVAÇÃO DO MEIO AMBIENTE** 
Ps: Viva o agronegócio. 

### Funcionamento

1. Do jogo
    - Haverá uma determinada quantidade de tempo.
    - O jogo continua podendo ser jogado enquanto essa quantidade de tempo não é atingida ou enquanto ainda existe óleo.
    - Existem equipamentos de proteção para auxílio dos jogadores.
    - Existem equipamentos que atrapalham os jogadores.

2. Do jogador
    - Jogadores terão uma quantidade de saúde.
    - A saúde pode ser diminuida devido a armadilhas do outro jogador ou por pisar em cima do oleo (prejudicando-a).
    - Jogadores podem plantar armadilhar para atingir outros jogadores. *ATENÇÃO: o jogador que colocou a armadilha também pode ser atingida pela mesma*.
    - Jogadores podem adquirir equipamentos de proteção e assim reduzir os danos durante a coleta.

### O que será usado para o desenvolvimento

Como é proposto pelo projeto serão utilizadas as tecnologias:
    - Biblioteca Allegro:
        Biblioteca de interface gráfica c/c++ para desenvolvimento de jogos. tutoriais de auxílio podem ser encontrados nos sites a seguir.
            1. [Rafael](https://rafaeltoledo.net/tutoriais-allegro-5/)
            2. [IFSul](https://aprendendoallegro.tk/)
            3. [Video Aulas](https://www.youtube.com/watch?v=IZ2krJ8Ls2A&list=PL6B459AAE1642C8B4)
    
    - Cliente e servidor:
        Para o cliente servidor utilizaremos as bibliotecas de conexão socket disponibilizadas e, desenvolvidas, especialmente para o desenvolvimento desse projeto de introdução a programação. Sua documentação pode ser encontrada em
            1. [server.h](https://github.com/luucasv/projetoIP/blob/master/server-doc.md)
            2. [client.h](https://github.com/luucasv/projetoIP/blob/master/client-doc.md)
            
            
    