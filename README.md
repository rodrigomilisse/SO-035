## SO-035 – Projeto de Sistemas Operativos

### **Compilação (na raiz do projeto)**
1. **`make`**: Compila o projeto.
2. **`make re`**: Recompila o projeto.
3. **`make run`**: Recompila e executa o programa com argumentos predefinidos no `Makefile`.
4. **`make prof`**: Executa a versão fornecida pelos professores.

### **Dúvidas**
1. **Função `end_execution(struct info_container *info, struct buffers *buffs)`**: O parâmetro `buffs` é explicitamente ignorado, pois não está a ser utilizado na função.
2. **Valor inicial de `tx_counter`**: O `tx_counter` não começa com o valor `-1` como esperado. Deve ser corrigido.

### **Limitações**
1. **Formatação do Saldo e Transações Assinadas no comando `stat`**: A formatação pode ficar distorcida quando o **Saldo** ou o número de **Transações Assinadas** for muito alto. Considerar ajustes na formatação para suportar grandes valores de forma legível.
