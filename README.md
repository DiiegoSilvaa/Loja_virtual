# Trabalho-de-quarta

Programa em C, executado no terminal, que cria um arquivo de números, ordena os valores com o método escolhido pelo usuário e permite alterar um valor já salvo.

## Integrantes

| Nome | RGM |
|------|-----|
| Cauã Hernandes Honorato | 47863617 |
| Diego Silva | 47855533 |
| Miguel Galego Meira | 47963662 |
| Nicolas Dias da Silva | 47609125 |
| Victor Hugo Gonzaga | 47300701 |

## Funcionalidades

O programa exibe um menu com quatro opções:

1. **Criar arquivo**: o usuário informa a quantidade de números (de 1 a 100) e digita cada um deles. Os valores são salvos em `dados.txt`.
2. **Ordenar números**: lê o `dados.txt`, pergunta o método de ordenação, mostra o resultado na tela e salva em `ordenado.txt`.
3. **Alterar valor**: o usuário escolhe uma posição (de 1 até a quantidade de números salvos) e informa o novo valor. O `dados.txt` é atualizado e é preciso ordenar novamente pela opção 2.
4. **Sair**: encerra o programa.

### Métodos de ordenação disponíveis

- Bubble Sort
- Insertion Sort
- Selection Sort
- Merge Sort (versão iterativa)

## Arquivos do repositório

| Arquivo | Descrição |
|---------|-----------|
| `loja virtual` | Código-fonte do programa |
| `LICENSE` | Licença BSD-3-Clause |
| `README.md` | Este arquivo |
| `slide da apresentação` | Apresentação em slide |

Os arquivos `dados.txt` e `ordenado.txt` são gerados automaticamente na pasta onde o programa é executado.

## Como compilar e executar

É necessário ter um compilador C instalado, como o GBD online. [[GDB Online](https://www.onlinegdb.com/)]

## Exemplo de uso

```
1 - Criar arquivo
2 - Ordenar numeros
3 - Alterar valor
4 - Sair
Opcao: 1
Quantidade de numeros (maximo 100): 3
Digite um numero: 8
Digite um numero: 2
Digite um numero: 5
Arquivo criado!

Opcao: 2
Metodo de ordenacao:
1 - Bubble Sort
2 - Insertion Sort
3 - Selection Sort
4 - Merge Sort
Escolha: 1
2 5 8
Ordenado e salvo!
```

## Observações

- O limite é de 100 números por arquivo.
- Se o `dados.txt` não existir, as opções 2 e 3 pedem para criar o arquivo primeiro.
- Criar um novo arquivo (opção 1) sobrescreve o `dados.txt` anterior.

## Evolução do trabalho

O projeto foi desenvolvido em etapas, e cada uma acrescentou uma funcionalidade ao programa:

1. **Base do programa**: criamos o menu principal em laço (`do...while`) e a leitura dos números digitados pelo usuário em um vetor de até 100 posições.
2. **Persistência em arquivo**: passamos a gravar os números em `dados.txt`, para que os dados continuassem disponíveis depois que o programa fosse encerrado.
3. **Primeiros métodos de ordenação**: implementamos o Bubble Sort e o Insertion Sort, comparando o funcionamento de cada um.
4. **Ampliação dos métodos**: incluímos o Selection Sort e o Merge Sort (em versão iterativa), deixando o usuário escolher o método pelo menu.
5. **Resultado salvo**: o vetor ordenado passou a ser exibido na tela e gravado em `ordenado.txt`.
6. **Alteração de valores**: acrescentamos a opção de trocar um número por posição, atualizando o `dados.txt`.
7. **Validações e versão final**: adicionamos tratamento de erros (quantidade inválida, arquivo inexistente, posição inválida, método inválido) e consolidamos tudo no arquivo `Trabalho atualizado C.c`.

### Próximos passos possíveis

- Permitir mais de 100 números com alocação dinâmica de memória.
- Exibir o tempo de execução de cada método para comparar o desempenho.
- Separar o código em funções para facilitar a leitura e a manutenção.
