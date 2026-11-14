# Ferramenta Amiga

Eduardo Pereira Costa
UniCesumar — Engenharia de Software — Turma B, noturno
Maringá — PR, 2026

Protótipo acadêmico de terminal em C99 para controlar um acervo comunitário de ferramentas.
Tema proposto a partir da estrutura do modelo fornecido; confirmar aderência ao desafio original e à primeira entrega do grupo, que não foram disponibilizados.

## Funcionalidades
1. Cadastrar ferramenta com código único, nome, categoria e total.
2. Listar acervo.
3. Pesquisar por código exato ou parte do nome (sem diferenciar caixa em texto ASCII).
4. Registrar empréstimo de unidades disponíveis.
5. Registrar devolução de unidades emprestadas.
6. Exibir resumo e percentual emprestado.
0. Encerrar.

## Compilar e executar
Requer compilador C99. O VS Code é o editor; precisa de compilador externo.

macOS / Linux, em um terminal na pasta deste arquivo:
```sh
cc -std=c99 -Wall -Wextra -Wpedantic ferramenta_amiga.c -o ferramenta_amiga
./ferramenta_amiga
```
Windows com GCC disponível:
```sh
gcc -std=c99 -Wall -Wextra -Wpedantic ferramenta_amiga.c -o ferramenta_amiga.exe
.\ferramenta_amiga.exe
```

Verificado em macOS x86_64 com Apple Clang 14.0.3: compilação C99 sem avisos e 14 testes aprovados. Resultados em `resultado_testes.txt`. A sequência completa do roteiro também foi executada, com saída em `execucao_demonstracao.txt`. O executável incluído é para este Mac Intel; em outra plataforma, recompile o código-fonte.

## Testes reproduzíveis
Com Python 3 e compilador no PATH:
```sh
python3 testar.py
```
O script compila o fonte em diretório temporário e realiza casos de aceitação. Se passar, imprime os resultados e salva `resultado_testes.txt`. Os 14 casos passaram neste ambiente. Repetir no ambiente do professor se ele for diferente.

## Exemplo de demonstração
- Cadastrar F001 / Furadeira / Eletrica / 4.
- Cadastrar F002 / Martelo / Manual / 6.
- Emprestar 2 unidades de F001.
- Tentar devolver 3 e, após rejeição, devolver 1.
- Resumo esperado: 2 tipos, 10 unidades totais, 1 emprestada, 9 disponíveis e 10,0% emprestado (na tela, 10.0%).

## Regras e limitações
- Até 100 tipos de ferramenta, cada um com 1 a 10.000 unidades.
- Disponíveis = total - emprestadas; empréstimo e devolução não alteram o total.
- Texto obrigatório e limitado; código de até 16 caracteres; nome/categoria até 80 bytes. Use ASCII para demonstração previsível.
- Código único sem diferenciar letras maiúsculas de minúsculas em ASCII.
- Dados somente em memória: fechar o programa elimina todos os registros.
- Não registra pessoas, prazos, multas, histórico, identificação de cada unidade ou manutenção.
- Devoluções são quantitativas, sem associação a empréstimos ou a usuários específicos.
- Não é um sistema de produção nem comprova implantação em comunidade real.

## Antes de entregar
Conferir a identificação preenchida e incluir outros integrantes, se houver; confirmar escopo com o professor, compilar e testar, substituir exemplos ilustrativos por capturas reais, publicar o repositório e inserir os links do GitHub e do vídeo. Não copiar nomes, matrículas ou links do modelo.

## VS Code no Mac
Abra esta pasta pelo menu Arquivo > Abrir Pasta. Depois use Terminal > Executar Tarefa > Executar Ferramenta Amiga. A tarefa compila com Clang e inicia o programa no terminal integrado.
