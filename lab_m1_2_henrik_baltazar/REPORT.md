# Mini relatório

## Lab M1 Parte 02
### 1. Qual é a diferença entre resolução espacial e resolução radiométrica?
A resolução espacial é basicamente o tamanho da imagem, ou seja, as dimensões da matriz de pixels que formam a imagem, já a resolução radiométrica diz respeito aos tons possíveis por pixel, onde neste trabalho trabalhamos com uma resolução radiométrica de 256 tons.

### 2. Por que a média ponderada produz resultado diferente da média simples?
A média ponderada aumenta a intensidade de determinados canais e diminui a intensidade de outros, desta forma a escala de cinza resultante possui tons de cinza diferentes de acordo com os pesos utilizados para cada canal, por exemplo, atribuir pesos menores para o canal vermelho deixará todos os tons de vermelho mais próximos de preto, enquanto que atribuir pesos maiores para o canal azul, tornará os tons azuis mais claros na escala de cinza resultante. Enquanto que sem pesos todos os canais são compactados em escala de cinza igualmente.

### 3. O que ocorre visualmente quando a quantidade de níveis é reduzida?
A imagem se torna menos detalhada, os tons intermediários de 0 a 255 agrupados em menos grupos torna a quantidade de tons de cinza muito menor, fazendo com que o salto de tom de cinza de um pixel à outro seja muito mais brusco dada a ausência de muitos tons de cinza.

### 4. Em quais regiões da imagem a perda de informação fica mais evidente?
A imagem perde mais informação nas transições suavez de cinza, mais fortes ficam mais nítidas mas a diferença entre os tons intermediários se tornam cada vez menos perceptível conforme os níveis são reduzidos.

### 5. Como o tipo e o número de canais da imagem interferem na forma de acessar um pixel?
Um pixel pode possuir diferentes quantidades de canais e um diferente tipo para trabalhar com a quantidade certa de canais, se o tipo possuir menos capacidade de leitura do que a quantidade de canais de um pixel a leitura dos canais será interpretada incorretamente, resultando em canais extras sendo interpretados como novos pixels e inevitavelmente um overflow. Logo ao ler uma imagem com uma capacidade de tipo maior do que a quantidade de canais ocasionará em um underflow.