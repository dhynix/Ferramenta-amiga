"""Testes de aceitacao. Execucao depende de compilador C99 local."""
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

base = Path(__file__).resolve().parent
cc = os.environ.get('CC') or shutil.which('cc') or shutil.which('gcc') or shutil.which('clang')
if not cc:
    raise SystemExit('PENDENTE: nenhum compilador C encontrado. Nenhum teste foi aprovado.')

def cadastro(codigo='F001', nome='Furadeira', categoria='Eletrica', total='4'):
    return '\n'.join(['1', codigo, nome, categoria, total]) + '\n'

casos = [
    ('T01 Acervo vazio', '2\n3\n4\n5\n6\n0\n', ['Nenhuma ferramenta cadastrada.', 'acervo vazio'], []),
    ('T02 Cadastro e listagem', cadastro() + '2\n0\n', ['Ferramenta cadastrada.', 'Disponiveis: 4'], []),
    ('T03 Codigo duplicado', cadastro() + '1\nf001\nF002\nMartelo\nManual\n6\n0\n', ['Codigo duplicado.', 'Ferramenta cadastrada.'], []),
    ('T04 Entradas invalidas', 'x\n9\n\n1\nF001\n\nFuradeira\nEletrica\nabc\n-1\n0\n2.5\n4\n0\n', ['Campo obrigatorio.', 'Informe um inteiro entre 0 e 6.', 'Informe um inteiro entre 1 e 10000.', 'Ferramenta cadastrada.'], []),
    ('T05 Pesquisa', cadastro() + '3\n1\nf001\n3\n2\nRADE\n3\n1\nZZZ\n0\n', ['Resultados encontrados: 1', 'Resultados encontrados: 0'], []),
    ('T06 Emprestimo sem saldo', cadastro(total='1') + '4\nF001\n1\n4\nF001\n0\n', ['Emprestimo registrado.', 'Nao ha unidades disponiveis.'], []),
    ('T07 Registro inexistente', cadastro() + '4\nZZZ\n5\nZZZ\n0\n', ['Registro inexistente.'], ['Emprestimo registrado.', 'Devolucao registrada.']),
    ('T08 Devolucao sem emprestimo', cadastro() + '5\nF001\n0\n', ['Nao ha unidades emprestadas para devolver.'], []),
    ('T09 Limites e resumo', cadastro() + cadastro('F002', 'Martelo', 'Manual', '6') + '4\nF001\n5\n2\n5\nF001\n3\n1\n6\n0\n', ['Informe um inteiro entre 1 e 4.', 'Informe um inteiro entre 1 e 2.', 'Emprestadas: 1\nDisponiveis: 9', 'Percentual emprestado: 10.0%'], []),
    ('T10 Capacidade', ''.join(cadastro('F%03d' % i, 'Item', 'Manual', '1') for i in range(100)) + '1\n6\n0\n', ['Limite de 100 registros atingido.', 'Tipos cadastrados: 100'], []),
    ('T11 Texto longo', '1\n' + 'A' * 200 + '\nF001\nFuradeira\nEletrica\n4\n0\n', ['Entrada longa demais.', 'Ferramenta cadastrada.'], []),
    ('T12 EOF no cadastro', '1\nF001\n', ['Programa encerrado.'], ['Ferramenta cadastrada.']),
    ('T13 Limite inteiro e lixo', cadastro(total='10001\n99999999999999999999999999\n4x\n4') + '6\n0\n', ['Informe um inteiro entre 1 e 10000.', 'Unidades totais: 4'], []),
    ('T14 EOF no menu', '', ['Programa encerrado.'], []),
]
with tempfile.TemporaryDirectory() as temp:
    exe = Path(temp) / ('programa.exe' if os.name == 'nt' else 'programa')
    compilacao = subprocess.run([cc, '-std=c99', '-Wall', '-Wextra', '-Wpedantic', str(base/'ferramenta_amiga.c'), '-o', str(exe)], capture_output=True, text=True)
    if compilacao.returncode != 0:
        raise SystemExit('COMPILACAO NAO CONCLUIDA. Testes pendentes.\n' + compilacao.stderr)
    resultados = ['Compilador: ' + cc, 'Compilacao: aprovada', 'Diagnosticos: ' + (compilacao.stderr.strip() or 'nenhum')]
    falhas = 0
    for nome, entrada, obrigatorios, proibidos in casos:
        p = subprocess.run([str(exe)], input=entrada, capture_output=True, text=True, timeout=10)
        erros = [repr(t) + ' ausente' for t in obrigatorios if t not in p.stdout]
        erros += [repr(t) + ' indevido' for t in proibidos if t in p.stdout]
        if p.returncode != 0: erros.append('encerramento com erro')
        if nome.startswith('T10') and p.stdout.count('Ferramenta cadastrada.') != 100: erros.append('total incorreto de cadastros')
        status = 'FALHOU: ' + '; '.join(erros) if erros else 'APROVADO'
        falhas += bool(erros)
        resultados.append(nome + ': ' + status)
    texto = '\n'.join(resultados) + '\n'
    print(texto)
    (base/'resultado_testes.txt').write_text(texto, encoding='utf-8')
    if falhas: raise SystemExit(1)
