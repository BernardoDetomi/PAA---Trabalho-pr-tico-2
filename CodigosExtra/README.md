# PAA---Trabalho-pr-tico-2

Agora, para adicionar uma heurística própria, o objetivo é otimizar a resolução utilizando técnicas como:  

- Ordenação de células por número de possibilidades (Minimum Remaining Values).  
- Escolha de valores baseada na frequência (Least Constraining Value).  

Estratégia:
    
- Adicionar uma função para priorizar células com menos possibilidades.  
- Modificar o algoritmo de backtracking para usar essa priorização.    

A heurística que será usada é MRV (Minimum Remaining Values), que escolhe a próxima célula a ser preenchida com base no menor número de opções possíveis, reduzindo o espaço de busca.  

Modificações no Algoritmo  

Adicionar a heurística MRV:  

- Criar uma função que calcula o número de valores possíveis para cada célula vazia.  
- Escolher a próxima célula com o menor número de possibilidades.  

Integração com o backtracking:  

- Modificar o fluxo de resolução para priorizar as células baseadas na heurística.  

Executar os programas...    

    gcc -o backtracking backtracking.c  
    
    gcc -o heuristica heuristica.c  

    ./backtracking -i entrada.txt -o saida.txt  
    
    ./heuristica -i entrada.txt -o saida2.txt  

Para comparar os tempos de execução entre os métodos de backtracking simples e com heurística MRV, podemos fazer algumas modificações no código. A ideia é medir os tempos de execução de cada método separadamente e registrar os resultados para análise.  

Modificações no Código  

Adicionar a função de backtracking simples ao programa:  

- Renomear a função de backtracking anterior como solve_sudoku_simple.  

Medir tempos separadamente:  

- Usar gettimeofday para medir o tempo gasto por cada método.  
- Adicionar uma flag para escolher o método pela linha de comando.  

Saída de resultados:  

- Registrar os tempos para comparação.  

        gcc -o sudoku sudoku.c
  
        ./sudoku -i entrada.txt -o saida_simples.txt -m 0
  
        ./sudoku -i entrada.txt -o saida_heuristica.txt -m 1  


Para os próximos passos, vamos implementar as funcionalidades propostas:  

Salvar os tempos de execução em um arquivo para análise posterior:  

- Registrar os tempos de ambos os métodos (força bruta e heurística MRV) em um arquivo adicional.  
- Esse arquivo conterá os tempos em um formato estruturado, como CSV.  

Testar o desempenho com diferentes níveis de dificuldade dos tabuleiros:  

- Adicionar suporte para processar múltiplos tabuleiros em sequência, com tempos registrados para cada método e tabuleiro.  

        gcc -o sudokus sudokus.c
  
        ./sudokus -d pastasudokus/  
