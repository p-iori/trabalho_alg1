---
description: >-
  O projeto oferece diversas estruturas de dados que podem ser escolhidas pelo
  usuário e que são independentes do dataset utilizado. As estruturas de dados
  que podem ser utilizadas são:
icon: database
---

# Estruturas de dados

* Pilha (`pilha)`
* Fila (`fila`)
* Lista sequencial (`lista_seq`)
* Lista encadeada (`lista_enc`)
* Lista encabeçada (`lista_cabeca`)
* Lista ordenada (`lista_ord`)
* Lista generalizada (`lista_gen`)
* Lista cruzada (`lista_cruz`)

Cada uma possui funções próprias que auxiliam no gerenciamento dos dados na memória.

{% hint style="success" %}
**Todas as estruturas de dados são independentes entre si e seguem os princípios dos TADs.**
{% endhint %}

Todas as estruturas são construídas para armazenar structs do tipo ITEM e possuem, em geral, as seguintes funcionalidades:

<table data-search="false"><thead><tr><th>Função</th><th>Explicação</th></tr></thead><tbody><tr><td><code>criar</code></td><td>Cria uma struct referente à estrutura de dados escolhida na memória</td></tr><tr><td><code>inserir</code></td><td>Insere um elemento na estrutura, obedecendo as regras de inserção impostas por ela</td></tr><tr><td><code>remover</code></td><td>Remove um elemento da estrutura, obedecendo as regras impostas por ela.</td></tr><tr><td><code>estrutura_apagar</code></td><td>Apaga a estrutura de dados da memória, com o cliente podendo escolher se os itens contidos nela devem ser apagados ou não </td></tr><tr><td><code>estrutura_vazia</code></td><td>Determina se uma estrutura de dados já criada está vazia ou não</td></tr><tr><td><code>estrutura_tamanho</code></td><td>Retorna um inteiro igual ao número de itens contidos na estrutura de dados</td></tr><tr><td><code>estrutura_percorrer</code></td><td>Função auxiliar que percorre todos os elementos contidos numa estrutura de dados</td></tr></tbody></table>

O cliente é responsável por apagar as estruturas da memória após o uso.

As **listas generalizadas,** **listas cruzadas e listas ordenadas** possuem funções auxiliares adicionais que são responsáveis por configurar seus parâmetros próprios que as outras funções não possuem. Elas são chamadas pelo próprio reader utilizado e usam os parâmetros fornecidos no arquivo de consulta.

## Veja mais

{% content-ref url="../../guides/custom-domains.md" %}
[custom-domains.md](../../guides/custom-domains.md)
{% endcontent-ref %}

{% content-ref url="../../core-concepts/permissions.md" %}
[permissions.md](../../core-concepts/permissions.md)
{% endcontent-ref %}

{% content-ref url="../../guides/automations.md" %}
[automations.md](../../guides/automations.md)
{% endcontent-ref %}
