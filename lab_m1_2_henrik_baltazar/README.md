# Lab M1 parte 2
Aluno: Henrik Gomes Baltazar;<br>
Linguagem: C++;<br>
Dependências: OpenCV, cmake(4.4.2), ninja(1.13.2), g++(16.1.1 20260728);<br>

## Como preparar o ambiente
Todo projeto foi desenvolvido no CLion da JetBrains através da licença de estudante. Recomenda-se a utilização desta IDE devidamente licenciada.

## Como compilar
```shell
cmake -S . -B build/unix-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/unix-debug
```

## Como executar
*O binário será compilado no diretório 'build/unix-debug', execute o comando a seguir a partir deste diretório.*

```shell
./pdi_lab \
  --input <arquivo> \
  --output <arquivo-ou-diretorio> \
  --operation <operacao> \
  [opcoes]
```
